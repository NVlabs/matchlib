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

#ifndef __AXIMG_T_HOST__
#define __AXIMG_T_HOST__

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <nvhls_connections.h>

#include <queue>
#include <deque>

/**
 * \brief A testbench component to verify AxiMasterGate.
 * \ingroup AXI
 *
 * \tparam Cfg     A valid AXI config.
 *
 * \par Overview
 *
 * This component connects to the request-response (non-AXI) interface of AXIMasterGate.
 * It launches read and write requests and checks for appropriate responses.
 */
template <typename Cfg>
SC_MODULE(Host) {
 public:
  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  Connections::Out<WrRequest<Cfg> > wrRequestOut;
  Connections::In<WrResp<Cfg> > wrRespIn;
  Connections::Out<RdRequest<Cfg> > rdRequestOut;
  Connections::In<RdResp<Cfg> > rdRespIn;

  static const int bytesPerBeat = Cfg::dataWidth >> 3;

  sc_out<bool> done_write;
  sc_out<bool> done_read;

  static const int write_count = 400;
  static const int read_count = 50;

  std::deque<unsigned int> read_ref;
  std::queue<typename axi::axi4<Cfg>::Data> dataQ;

  SC_CTOR(Host) : reset_bar("reset_bar"), clk("clk") {
    SC_THREAD(run_wr_source);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_wr_sink);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_rd_source);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_rd_sink);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

  struct Data { // don't really need this except to test out gen_random_payload
    static const int width = Cfg::dataWidth;
    NVUINTW(width) d;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& d;
    }
  };

 protected:
  void run_wr_source() {
    wrRequestOut.Reset();
    int addr = 0;
    int ctr = 0;

    wait(20); // If we start sending requests before resetting the ReorderBufs, bad things happen

    while (1) {
      wait();

      if (ctr < write_count) {
        WrRequest<Cfg> wrRequest;
        wrRequest.addr = addr;

        int len = rand() % 6;

        wrRequest.len = len;
        wrRequest.last = 0;

        for (int i = 0; i <= len; ++i) {
          if (i == len) {
            wrRequest.last = 1;
            ctr++;
          }
          wrRequest.data = nvhls::gen_random_payload<Data>().d;
          std::cout << "@" << sc_time_stamp()
                    << " write source initiated a request:"
                    << "\t addr = " << hex << wrRequest.addr
                    << "\t data = " << hex << wrRequest.data
                    << "\t len = " << dec << wrRequest.len
                    << std::endl;
          wrRequestOut.Push(wrRequest);
          dataQ.push(wrRequest.data);
          addr += bytesPerBeat;
        }
      }
    }
  }

  void run_wr_sink() {
    wrRespIn.Reset();
    done_write = 0;
    int ctr = 0;

    while (1) {
      wait();
      WrResp<Cfg> wrResp = wrRespIn.Pop();

      std::cout << "@" << sc_time_stamp() << " write sink received response:"
                << "\t resp = " << dec << wrResp.resp
                << std::endl;
      if (++ctr == write_count) done_write = 1;
    }
  }

  void run_rd_source() {
    rdRequestOut.Reset();
    int addr = 0;
    int ctr = 0;

    wait(2000); // Let writes run well ahead of reads

    while (1) {
      wait();

      if (ctr < read_count) {
        if (rand() % 5 == 0) { // Really need to pace ourselves here
          RdRequest<Cfg> rdRequest;
          rdRequest.addr = addr;
          int len = rand() % 3;

          rdRequest.len = len;

          std::cout << "@" << sc_time_stamp() << " read source initiated a request:"
                    << "\t addr = " << hex << rdRequest.addr
                    << "\t len = " << dec << rdRequest.len
                    << std::endl;
          rdRequestOut.Push(rdRequest);
          addr += (len+1)*bytesPerBeat;
          ctr++;
        }
      }
    }
  }

  void run_rd_sink() {
    rdRespIn.Reset();
    done_read = 0;
    int ctr = 0;

    while (1) {
      wait();
      RdResp<Cfg> rdResp = rdRespIn.Pop();
      typename axi::axi4<Cfg>::Data rd_data_expected = dataQ.front(); dataQ.pop();

      std::cout << "@" << sc_time_stamp() << " read sink received response:"
                << "\t last = " << rdResp.last
                << "\t data = " << hex << rdResp.data
                << "\t expected = " << hex << rd_data_expected
                << std::endl;
      NVHLS_ASSERT_MSG(rdResp.data == rd_data_expected, "Read_response_data_did_not_match_expected_value");

      if (rdResp.last == 1) ctr++;
      if (ctr == read_count) done_read = 1;
    }
  }
};

#endif
