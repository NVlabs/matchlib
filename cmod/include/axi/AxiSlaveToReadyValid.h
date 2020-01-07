/*
 * Copyright (c) 2017-2020, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef __AXISLAVETOREADYVALID_H__
#define __AXISLAVETOREADYVALID_H__

#include <systemc.h>
#include <ac_int.h>
#include <hls_globals.h>
#include <axi/axi4.h>
#include <mem_array.h>
#include <fifo.h>
#include <axi/AxiSlaveToReadyValid/testbench/RVSink.h>
#include "Arbiter.h"

/**
 * \brief An AXI slave that converts AXI requests into a simplified format.
 * \ingroup AXI
 *
 * \tparam axiCfg    A valid AXI config.
 * \tparam rvCfg     A config for the ready-valid interface that is the output of the slave. The data and strobe fields are assumed to match the widths of their AXI counterparts.
 *
 * \par Overview
 * This block converts AXI read and write requests into a simplified format consisting of a single ready-valid interface that has address, data, and write strobe fields, as well as a read/write indicator.  Read responses are returned to the block via a second ready-valid interface (there are no write responses expected).  AxiSlaveToReadyValid handles all of the AXI-specific protocol, generating write responses and packing/unpacking bursts as necessary.
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/AxiSlaveToReadyValid/run/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <typename axiCfg, typename rvCfg>
class AxiSlaveToReadyValid : public sc_module {
 public:
  static const int kDebugLevel = 5;
  
  typedef typename axi::axi4<axiCfg> axi4_;

  typename axi4_::read::template slave<> if_axi_rd;
  typename axi4_::write::template slave<> if_axi_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  enum {
    rvDataW = rvCfg::dataWidth,
    rvAddrW = rvCfg::addrWidth,
    rvWstrbW = rvCfg::wstrbWidth,
  };
 
  typedef typename RVSink<rvCfg>::Read Read;
  typedef typename RVSink<rvCfg>::Write Write;
  typedef typename RVSink<rvCfg>::Addr rvAddr;

  Connections::In<Read> if_rv_rd;
  Connections::Out<Write> if_rv_wr;

 public:
  SC_CTOR(AxiSlaveToReadyValid)
      : if_axi_rd("if_axi_rd"),
        if_axi_wr("if_axi_wr"),
        reset_bar("reset_bar"),
        clk("clk"),
        if_rv_rd("if_rv_rd"),
        if_rv_wr("if_rv_wr") {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  void run() {
    if_axi_rd.reset();
    if_axi_wr.reset();

    if_rv_rd.Reset();
    if_rv_wr.Reset();

    typename axi4_::AddrPayload axi_rd_req;
    typename axi4_::ReadPayload axi_rd_resp;
    typename axi4_::AddrPayload axi_wr_req_addr;
    typename axi4_::WritePayload axi_wr_req_data;
    typename axi4_::WRespPayload axi_wr_resp;

    static const int bytesPerBeat = rvDataW >> 3;

    Write rv_wr;
    Read rv_rd;
    rvAddr addr_temp;
    
    NVUINTW(axi4_::ADDR_WIDTH) read_addr;
    NVUINTW(axi4_::ALEN_WIDTH) axiRdLen;
    NVUINTW(rvAddrW) write_addr;

    bool read_arb_req = 0;
    bool write_arb_req = 0;
    NVUINTW(2) valid_mask = 0;
    NVUINTW(2) select_mask = 0;
    Arbiter<2> arb;

    bool read_inFlight = 0;
    bool arb_needs_update = 1;
    
    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      valid_mask = write_arb_req << 1 | read_arb_req;
      if (arb_needs_update) {
        select_mask = arb.pick(valid_mask);
        if (select_mask != 0) arb_needs_update = 0;
      }

      if (!read_arb_req) {
        if (if_axi_rd.ar.PopNB(axi_rd_req)) { 
          read_arb_req = 1;
          NVUINTW(axi4_::ADDR_WIDTH) addr_temp_cast(static_cast<sc_uint<axi4_::ADDR_WIDTH> >(axi_rd_req.addr));
          NVUINTW(axi4_::ALEN_WIDTH) len_temp(static_cast< sc_uint<axi4_::ALEN_WIDTH> >(axi_rd_req.len));
          read_addr = addr_temp_cast;
          axiRdLen = len_temp;
          axi_rd_resp.resp = axi4_::Enc::XRESP::OKAY;
          axi_rd_resp.id = axi_rd_req.id;
        }
      }

      if (!write_arb_req) {
        if (if_axi_wr.aw.PopNB(axi_wr_req_addr)) {
          write_arb_req = 1;
          NVUINTW(rvAddrW) addr_temp_cast(static_cast< sc_uint<rvAddrW> >(axi_wr_req_addr.addr));
          write_addr = addr_temp_cast;
        }
      }

      if (select_mask == 1) {
        if (!read_inFlight) {
          rv_wr.rw = 0;
          rv_wr.addr = nvhls::get_slc<rvAddrW>(read_addr,0);
          if_rv_wr.Push(rv_wr);
          CDCOUT(sc_time_stamp() << " " << name() << " RV read:"
                        << " addr=" << hex << rv_wr.addr.to_int64()
                        << endl, kDebugLevel);
          read_inFlight = 1;
        } else {
          if (if_rv_rd.PopNB(rv_rd)) {
            axi_rd_resp.data = rv_rd.data;
            if (axiRdLen == 0) {
              axi_rd_resp.last = 1;
              read_arb_req = 0;
              arb_needs_update = 1;
            } else {
              axi_rd_resp.last = 0;
              axiRdLen--;
              read_addr += bytesPerBeat;
            }
            if_axi_rd.r.Push(axi_rd_resp);
            CDCOUT(sc_time_stamp() << " " << name() << " RV read response:"
                          << axi_rd_resp
                          << endl, kDebugLevel);
            read_inFlight = 0;
          }
        }
      } else if (select_mask == 2) {
        if (if_axi_wr.w.PopNB(axi_wr_req_data)) {
          rv_wr.addr = write_addr;
          rv_wr.rw = 1;
          NVUINTW(axi4_::WSTRB_WIDTH) wstrb_temp_cast(static_cast<sc_uint<axi4_::WSTRB_WIDTH> >(axi_wr_req_data.wstrb));
          rv_wr.wstrb = wstrb_temp_cast;
          NVUINTW(axi4_::DATA_WIDTH) data_temp_cast(static_cast<typename axi4_::Data>(axi_wr_req_data.data));
          rv_wr.data = data_temp_cast;
          if_rv_wr.Push(rv_wr);
          CDCOUT(sc_time_stamp() << " " << name() << " RV write:"
                        << " data=" << hex << rv_wr.data
                        << " addr=" << hex << rv_wr.addr.to_uint64()
                        << " strb=" << hex << rv_wr.wstrb.to_uint64()
                        << endl, kDebugLevel);
          if (axi_wr_req_data.last == 1) {
            write_arb_req = 0;
            arb_needs_update = 1;
            if (axiCfg::useWriteResponses) {
              axi_wr_resp.resp = axi4_::Enc::XRESP::OKAY;
              axi_wr_resp.id = axi_wr_req_addr.id;
              if_axi_wr.bwrite(axi_wr_resp);
            }
          } else {
            write_addr += bytesPerBeat;
          }
        }
      }
    }
  }
};

#endif
