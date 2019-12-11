/*
 * Copyright (c) 2017-2019, NVIDIA CORPORATION.  All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __AXI_T_SLAVE__
#define __AXI_T_SLAVE__

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <nvhls_connections.h>
#include <hls_globals.h>

#include <queue>
#include <map>
#include <boost/assert.hpp>
#include <algorithm>

/**
 * \brief An AXI slave for use in a testbench.
 * \ingroup AXI
 *
 * \tparam axiCfg                   A valid AXI config.
 *
 * \par Overview
 * This test structure acts as an AXI slave memory, responding to read and write requests and generating responses.  An internal memory, initially unpopulated, stores the values of AXI write requests and is accessed for read requests.  The block enforces the following by assertion:
 * - Read requests are only valid if the address has previously been written to.
 * - Write data beats must each assert at least one strobe bit.
 *
 */
template <typename axiCfg>
class Slave : public sc_module {
 public:
  static const int kDebugLevel = 0;
  typedef axi::axi4<axiCfg> axi4_;

  typename axi4_::read::template slave<> if_rd;
  typename axi4_::write::template slave<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  std::queue <typename axi4_::ReadPayload> rd_resp;
  std::queue <typename axi4_::Addr> rd_resp_addr;
  std::queue <typename axi4_::AddrPayload> wr_addr;
  std::queue <typename axi4_::WritePayload> wr_data;
  std::queue <typename axi4_::WRespPayload> wr_resp;

  std::map<typename axi4_::Addr, typename axi4_::Data> localMem;
  std::map<typename axi4_::Addr, NVUINT8 > localMem_wstrb;
  std::vector<typename axi4_::Addr> validReadAddresses;

  static const int bytesPerBeat = axi4_::DATA_WIDTH >> 3;

  SC_CTOR(Slave)
      : if_rd("if_rd"), if_wr("if_wr"), reset_bar("reset_bar"), clk("clk") {
    SC_THREAD(run_rd);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_wr);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  void run_rd() {
    if_rd.reset();
    unsigned int rdBeatInFlight = 0;

    while (1) {
      wait();

      typename axi4_::AddrPayload rd_addr_pld;
      if (if_rd.nb_aread(rd_addr_pld)) {
        typename axi4_::Addr addr = rd_addr_pld.addr;
        NVHLS_ASSERT_MSG(addr % bytesPerBeat == 0, "Addresses_must_be_word_aligned");
        CDCOUT(sc_time_stamp() << " " << name() << " Received read request: ["
                      << rd_addr_pld << "]"
                      << endl, kDebugLevel);
        NVUINTW(axi4_::ALEN_WIDTH) len = (axiCfg::useBurst ? rd_addr_pld.len : NVUINTW(axi4_::ALEN_WIDTH)(0));
        for (unsigned int i=0; i<(len+1); i++) {
          std::ostringstream msg;
          msg << "\nError @" << sc_time_stamp() << " from " << name()
              << ": Received a read request from an address that has not yet been written to"
              << ", addr=" << hex << addr
              << endl;
          bool validAddr = false;
          for (unsigned int j=0; j<validReadAddresses.size(); j++) {
            if (validReadAddresses[j] == rd_addr_pld.addr) validAddr = true;
          }
          BOOST_ASSERT_MSG( validAddr, msg.str().c_str() );
          typename axi4_::ReadPayload data_pld;
          if (axiCfg::useWriteStrobes) {
            for (int k=0; k<axi4_::WSTRB_WIDTH; k++) {
              if (localMem_wstrb.find(addr+k) != localMem_wstrb.end()) {
                data_pld.data = nvhls::set_slc(data_pld.data, localMem_wstrb[addr+k], 8*k);
              } else {
                data_pld.data = nvhls::set_slc(data_pld.data, NVUINT8(0), 8*k);
              }
            }
          } else {
            data_pld.data = localMem[addr];
          }
          data_pld.resp = axi4_::Enc::XRESP::OKAY;
          data_pld.id = rd_addr_pld.id;
          data_pld.last = (i == len);
          rd_resp.push(data_pld);
          rd_resp_addr.push(addr);
          addr += bytesPerBeat;
        }
      }

      if (!rd_resp.empty()) {
        typename axi4_::ReadPayload data_pld;
        data_pld = rd_resp.front();
        typename axi4_::Addr addr = rd_resp_addr.front();
        if (if_rd.nb_rwrite(data_pld)) {
          CDCOUT(sc_time_stamp() << " " << name() << " Returned read data:"
                        << " data=[" << data_pld << "]"
                        << " addr=" << hex << addr.to_uint64()
                        << " beat=" << dec << (axiCfg::useBurst ? static_cast< sc_uint<32> >(rdBeatInFlight++) : "N/A")
                        << endl, kDebugLevel);
          if (data_pld.last == 1) {
            rdBeatInFlight = 0;
          }
          rd_resp.pop();
          rd_resp_addr.pop();
        }
      }
    }
  }

  void run_wr() {
    if_wr.reset();

    typename axi4_::WRespPayload resp_pld;
    typename axi4_::AddrPayload wr_addr_pld;
    typename axi4_::WritePayload wr_data_pld;
    typename axi4_::AddrPayload wr_addr_pld_out;
    typename axi4_::WritePayload wr_data_pld_out;
    unsigned int wrBeatInFlight = 0;
    bool first_beat = 1;
    typename axi4_::Addr wresp_addr;

    while (1) {
      wait();

      // Send a write response out of the local queue
      if (axiCfg::useWriteResponses) {
        if (!wr_resp.empty()) {
          resp_pld = wr_resp.front();
          if (if_wr.nb_bwrite(resp_pld)) {
            wr_resp.pop();
            CDCOUT(sc_time_stamp() << " " << name() << " Sent write response: ["
                                   << resp_pld << "]"
                                   << endl, kDebugLevel);
          }
        }
      }

      // Grab a write request (addr) and put it in the local queue
      if (if_wr.aw.PopNB(wr_addr_pld)) {
        NVHLS_ASSERT_MSG(wr_addr_pld.addr.to_uint64() % bytesPerBeat == 0, "Addresses_must_be_word_aligned");
        wr_addr.push(wr_addr_pld);
        CDCOUT(sc_time_stamp() << " " << name() << " Received write request: ["
                      << wr_addr_pld << "]"
                      << endl, kDebugLevel);
      }

      // Grab a write request (data) and put it in the local queue
      if (if_wr.w.PopNB(wr_data_pld)) {
        wr_data.push(wr_data_pld);
        CDCOUT(sc_time_stamp() << " " << name() << " Received write data:"
                      << " data=[" << wr_data_pld << "]"
                      << " beat=" << dec << (axiCfg::useBurst ? static_cast< sc_uint<32> >(wrBeatInFlight++) : "N/A")
                      << endl, kDebugLevel);
        if (wr_data_pld.last == 1) {
          wrBeatInFlight = 0;
        }
      }

      // Handle a write request in the local queues
      if (!wr_addr.empty() & !wr_data.empty()) {
        if (first_beat) {
          wr_addr_pld_out = wr_addr.front();
          wresp_addr = wr_addr_pld_out.addr;
          first_beat = 0;
        }
        wr_data_pld_out = wr_data.front(); wr_data.pop();
        // Store the data
        if (axiCfg::useWriteStrobes) {
          std::ostringstream msg;
          msg << "\nError @" << sc_time_stamp() << " from " << name()
              << ": Wstrb cannot be all zeros" << endl;
          BOOST_ASSERT_MSG( wr_data_pld_out.wstrb != 0, msg.str().c_str() );
          for (int j=0; j<axi4_::WSTRB_WIDTH; j++) {
            if (wr_data_pld_out.wstrb[j] == 1) {
              localMem_wstrb[wresp_addr+j] = nvhls::get_slc<8>(wr_data_pld_out.data, 8*j);
            }
          }
        } else {
          localMem[wresp_addr] = wr_data_pld_out.data;
        }
        validReadAddresses.push_back(wresp_addr);
        wresp_addr += bytesPerBeat;
        if (wr_data_pld_out.last == 1) {
          wr_addr.pop();
          first_beat = 1;
          // Generate a response
          if (axiCfg::useWriteResponses) {
            resp_pld.resp = axi4_::Enc::XRESP::OKAY;
            resp_pld.id = wr_addr_pld_out.id;
            wr_resp.push(resp_pld);
          }
        }
      }
    }
  }
};

#endif
