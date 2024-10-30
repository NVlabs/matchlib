/*
 * Copyright (c) 2017-2024, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef __AXI_T_MANAGER_FROM_FILE__
#define __AXI_T_MANAGER_FROM_FILE__

#include <nvhls_int.h>
#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/testbench/CSVFileReader.h>
#include <nvhls_connections.h>
#include <hls_globals.h>

#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <boost/assert.hpp>

/**
 * \brief An AXI manager that generates traffic according to a file for use in testbenches.
 * \ingroup AXI
 *
 * \tparam axiCfg                   A valid AXI config.
 * \tparam enable_interrupts        Set true to enable support for interrupt instructions (default false)
 *
 * \par Overview
 * AxiManagerFromFile reads write and read requests from a CSV and issues them as an AXI manager.  Read responses are checked agains the expected values provided in the file.  If enable_interrupts is true, the file can also specify a wait-for-interrupt mode in which the interrupt input must go high before further instructions are processed. The format of the CSV is as follows:
 * - Writes: delay_from_previous_request,W,address_in_hex,data_in_hex,burst_len
 * - Reads: delay_from_previous_request,R,address_in_hex,expected_response_data_in_hex,burst_len
 * - Interrupts: delay_from_previous_request,Q,arbitrary,arbitrary,arbitrary
 * 
 *  For reads, it's best to specify the full DATA_WIDTH of expected response data.
 *
 *  Use burst_len=0 for a single-beat transaction (no burst). For burst_len>0,
 *  use one line for each beat in the burst. All beats in a burst must have the same
 *  burst_len, be listed sequentially with appropriately incrementing addresses,
 *  and be grouped together (no intervening transactions). The delay field on beats
 *  in a burst after the first is ignored (all subsequent beats are sent as soon
 *  as possible).
 *
 */

// Allow an sc_in to be present only if a template parameter is enabled
template <typename T, bool enable> class sc_in_conditional : public sc_signal<T> {};
template <typename T> class sc_in_conditional <T,1> : public sc_in<T> {};

template <typename axiCfg, bool enable_interrupts = false> class ManagerFromFile : public sc_module {
 public:
  static const int kDebugLevel = 1;
  typedef axi::axi4<axiCfg> axi4_;

  typename axi4_::read::template manager<> if_rd;
  typename axi4_::write::template manager<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  sc_in_conditional<bool,enable_interrupts> interrupt;

  static const int bytesPerBeat = axi4_::DATA_WIDTH >> 3;
  static const int bytesPerWord = axi4_::DATA_WIDTH >> 3;
  static const int axiAddrBitsPerWord = nvhls::log2_ceil<bytesPerWord>::val;

  std::queue< int > delay_q;
  std::queue< int > req_q;
  std::queue< typename axi4_::AddrPayload > raddr_q;
  std::queue< typename axi4_::AddrPayload > waddr_q;
  std::queue< typename axi4_::WritePayload > wdata_q;
  std::queue<typename axi4_::Data> rresp_q;
    
  typename axi4_::AddrPayload addr_pld;
  typename axi4_::WritePayload wr_data_pld;
  typename axi4_::ReadPayload data_pld;
  typename axi4_::AddrPayload wr_addr_pld;
  typename axi4_::WRespPayload wr_resp_pld;

  int burst_inflight = 0;
  sc_out<bool> done;

  SC_HAS_PROCESS(ManagerFromFile);

  ManagerFromFile(sc_module_name name_, std::string filename="requests.csv")
      : sc_module(name_), if_rd("if_rd"), if_wr("if_wr"), reset_bar("reset_bar"), clk("clk") {

    CDCOUT("Reading file: " << filename << endl, kDebugLevel);
    CSVFileReader reader(filename);
    std::vector< std::vector<std::string> > dataList = reader.readCSV();
    for (unsigned int i=0; i < dataList.size(); i++) {
      std::vector<std::string> vec = dataList[i];
      CMOD_ASSERT_MSG(vec.size() == 5, "Each request must have five elements");
      if (!burst_inflight) delay_q.push(atoi(vec[0].c_str()));
      if (vec[1] == "R") {
        if (!burst_inflight) {
          req_q.push(0);
          std::stringstream ss;
          sc_uint<axi4_::ADDR_WIDTH> addr;
          ss << hex << vec[2];
          ss >> addr;
          std::stringstream ss_len;
          addr_pld.addr = static_cast<typename axi4_::Addr>(addr);
          sc_uint<axi4_::ALEN_WIDTH> len;
          ss_len << hex << vec[4];
          ss_len >> len;
          if (len) CMOD_ASSERT_MSG(axiCfg::useBurst, "A burst transaction was requested but the AXI config does not support bursts");
          CMOD_ASSERT_MSG(axiCfg::maxBurstSize >= len, "A burst transaction was requested that is longer than the maximum allowed by the AXI config");
          addr_pld.len = 0;
          addr_pld.len = static_cast<typename axi4_::BeatNum>(len);
          raddr_q.push(addr_pld);
          burst_inflight = int(len);
        } else {
          burst_inflight--;
        }
        std::stringstream ss_data;
        sc_biguint<axi4_::DATA_WIDTH> data;
        ss_data << hex << vec[3];
        ss_data >> data;
        rresp_q.push(TypeToNVUINT(data));
      } else if (vec[1] == "W") {
        if (!burst_inflight) {
          req_q.push(1);
          std::stringstream ss;
          sc_uint<axi4_::ADDR_WIDTH> addr;
          ss << hex << vec[2];
          ss >> addr;
          std::stringstream ss_len;
          addr_pld.addr = static_cast<typename axi4_::Addr>(addr);
          sc_uint<axi4_::ALEN_WIDTH> len;
          ss_len << hex << vec[4];
          ss_len >> len;
          if (len) CMOD_ASSERT_MSG(axiCfg::useBurst, "A burst transaction was requested but the AXI config does not support bursts");
          CMOD_ASSERT_MSG(axiCfg::maxBurstSize >= len, "A burst transaction was requested that is longer than the maximum allowed by the AXI config");
          addr_pld.len = static_cast<typename axi4_::BeatNum>(len);
          waddr_q.push(addr_pld);
          burst_inflight = int(len);
        } else {
          burst_inflight--;
        }
        std::stringstream ss_data;
        sc_biguint<axi4_::DATA_WIDTH> data;
        ss_data << hex << vec[3];
        ss_data >> data;
        wr_data_pld.data = TypeToNVUINT(data);
        wr_data_pld.wstrb = ~0;
        if (!burst_inflight) {
          wr_data_pld.last = 1;
        } else {
          wr_data_pld.last = 0;
        }
        wdata_q.push(wr_data_pld);
      } else if (vec[1] == "Q") {
        CMOD_ASSERT_MSG(enable_interrupts, "Interrupt command read, but interrupts are not enabled");
        req_q.push(2);
      } else {
        CMOD_ASSERT_MSG(1, "Requests must be R or W or Q");
      }
    }

    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  void run() {

    done = 0;

    if_rd.reset();
    if_wr.reset();

    wait(20);

    while (!delay_q.empty()) {
      int delay = delay_q.front();
      if (delay > 0) wait(delay);
      delay_q.pop();
      if (req_q.front() == 2) {
        CMOD_ASSERT_MSG(enable_interrupts,"Interrupt command found, but interrupts are not enabled");
        CDCOUT(sc_time_stamp() << " " << name() << " Beginning wait for interrupt"
                      << endl, kDebugLevel);
        while (interrupt.read() == 0) wait();
        CDCOUT(sc_time_stamp() << " " << name() << " Interrupt received"
                      << endl, kDebugLevel);
      } else if (req_q.front() == 1) {
        addr_pld = waddr_q.front();
        if_wr.aw.Push(addr_pld);
        waddr_q.pop();
        CDCOUT(sc_time_stamp() << " " << name() << " Sent write request:"
                      << " addr=[" << addr_pld << "]"
                      << endl, kDebugLevel);
        do {
          wr_data_pld = wdata_q.front();
          if_wr.w.Push(wr_data_pld);
          wdata_q.pop();
          CDCOUT(sc_time_stamp() << " " << name() << " Sent write data:"
                        << " data=[" << wr_data_pld << "]"
                        << endl, kDebugLevel);
        } while (!wr_data_pld.last);
        if_wr.b.Pop();
      } else if (req_q.front() == 0) {
        addr_pld = raddr_q.front();
        if_rd.ar.Push(addr_pld);
        raddr_q.pop();
        CDCOUT(sc_time_stamp() << " " << name() << " Sent read request: "
                      << " addr=[" << addr_pld << "]"
                      << endl, kDebugLevel);
        do {
          data_pld = if_rd.r.Pop();
          CDCOUT(sc_time_stamp() << " " << name() << " Received read response: ["
                        << data_pld << "]"
                        << endl, kDebugLevel);
          CMOD_ASSERT_MSG(data_pld.data == rresp_q.front(),"Read response did not match expected value");
          rresp_q.pop();
        } while (!data_pld.last);
      } else {
        CMOD_ASSERT_MSG(0,"Unexpected value in req_q");
      }
      req_q.pop();
    }
    done = 1;
  }
};

#endif
