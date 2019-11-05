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
#ifndef AXI_SLAVE_TO_READY_VALID_TOP_H
#define AXI_SLAVE_TO_READY_VALID_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/AxiSlaveToReadyValid.h>

class AxiSlaveToReadyValidTop : public sc_module {
 public:
  static const int kDebugLevel = 4;
  
  typedef axi::axi4<axi::cfg::standard> axi_;
  struct rdyvldCfg {
    enum {
      dataWidth = 64,
      addrWidth = 20,
      wstrbWidth = 8,
    };
  };

  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typename axi_::read::template slave<> axi_read;
  typename axi_::write::template slave<> axi_write;

  AxiSlaveToReadyValid<axi::cfg::standard, rdyvldCfg> top;

  Connections::In<typename RVSink<rdyvldCfg>::Read> rv_read;
  Connections::Out<typename RVSink<rdyvldCfg>::Write> rv_write;

  SC_HAS_PROCESS(AxiSlaveToReadyValidTop);

  AxiSlaveToReadyValidTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write"),
        top("top")
  {
    top.clk(clk);
    top.reset_bar(reset_bar);

    top.if_axi_rd(axi_read);
    top.if_axi_wr(axi_write);

    top.if_rv_rd(rv_read);
    top.if_rv_wr(rv_write);
  }
};

#endif
