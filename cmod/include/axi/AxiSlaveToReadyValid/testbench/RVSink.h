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

#ifndef __RV_SINK_H__
#define __RV_SINK_H__

#include <systemc.h>
#include <ac_int.h>
#include <axi/axi4.h>
#include <mem_array.h>
#include <fifo.h>
#include <axi/AxiSlaveToReadyValid.h>
#include <queue>
#include <map>

/**
 * \brief A testbench component to verify AxiSlaveToReadyValid.
 * \ingroup AXI
 *
 * \tparam rvCfg     A config for the ready-valid interface that is the output of AxiSlaveToReadyValid.
 *
 * \par Overview
 * 
 * This component connects to the ready-valid (non-AXI) interface of AxiSlaveToReadyValid.
 * It contains internal state and responds to read and write requests appropriately.
 *
 */
template <typename rvCfg>
SC_MODULE(RVSink) {
 public:
  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typedef NVUINTW(rvCfg::dataWidth) Data;
  typedef NVUINTW(rvCfg::addrWidth) Addr;
  typedef NVUINTW(rvCfg::wstrbWidth) Wstrb;
  typedef NVUINTW(1) RW;

/**
 * \brief The struct containing fields for the read and write request interface.
 */
  class Write : public nvhls_message {
   public:
    Data data;
    Addr addr;
    Wstrb wstrb;
    RW rw;  // 0=read, 1=write

    static const unsigned int width =
        rvCfg::dataWidth + rvCfg::addrWidth + rvCfg::wstrbWidth + 1;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& data;
      m& addr;
      m& wstrb;
      m& rw;
    }
  };

/**
 * \brief The struct containing the data field for the read response interface.
 */
  class Read : public nvhls_message {
   public:
    Data data;

    static const unsigned int width = rvCfg::dataWidth;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& data;
    }
  };

  static const int kDebugLevel = 4;

  Connections::Out<Read> rv_read;
  Connections::In<Write> rv_write;

  std::map<Addr, NVUINTW(8)> localMem;

  SC_HAS_PROCESS(RVSink);

  RVSink(sc_module_name name_)
      : sc_module(name_),
        clk("clk"),
        reset_bar("reset_bar"),
        rv_read("rv_read"),
        rv_write("rv_write") {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

  void run() {
    rv_read.Reset();
    rv_write.Reset();

    Write rv_req_reg;
    Read rv_resp_reg;

    while (1) {
      wait();
      if (rv_write.PopNB(rv_req_reg)) {
        CDCOUT(sc_time_stamp() << " " << name() << " RV request received:"
                               << " data=" << hex << rv_req_reg.data
                               << " addr=" << hex << rv_req_reg.addr.to_uint64()
                               << " wstrb=" << hex
                               << rv_req_reg.wstrb.to_uint64()
                               << " isWrite=" << rv_req_reg.rw << endl,
               kDebugLevel);
        if (rv_req_reg.rw) {
          for (int i = 0; i < rvCfg::wstrbWidth; i++) {
            if (rv_req_reg.wstrb[i] == 1) {
              localMem[rv_req_reg.addr + i] =
                  nvhls::get_slc<8>(rv_req_reg.data, 8 * i);
            }
          }
        } else {
          for (int i = 0; i < rvCfg::wstrbWidth; i++) {
            rv_resp_reg.data = nvhls::set_slc(
                rv_resp_reg.data, localMem[rv_req_reg.addr + i], 8 * i);
          }
          rv_read.Push(rv_resp_reg);
          CDCOUT(sc_time_stamp() << " " << name() << " Sending RV read response"
                                 << " data=" << hex
                                 << rv_resp_reg.data << endl,
                 kDebugLevel);
        }
      }
    }
  }
};

#endif
