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

#ifndef __AXIMASTERGATETOP_H__
#define __AXIMASTERGATETOP_H__

#include <axi/AxiMasterGate.h>
#include <axi/axi4_configs.h>

SC_MODULE(AxiMasterGateTop) {

 private:
  AxiMasterGate<axi::cfg::standard> gate;

 public:
  typedef axi::axi4<axi::cfg::standard> axi4_;

  typename axi4_::read::template master<> if_rd;
  typename axi4_::write::template master<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;
  Connections::In<WrRequest<axi::cfg::standard> > wrRequestIn;
  Connections::Out<WrResp<axi::cfg::standard> > wrRespOut;
  Connections::In<RdRequest<axi::cfg::standard> > rdRequestIn;
  Connections::Out<RdResp<axi::cfg::standard> > rdRespOut;

  SC_CTOR(AxiMasterGateTop)
      : gate("gate"),
        if_rd("if_rd"),
        if_wr("if_wr"),
        reset_bar("reset_bar"),
        clk("clk") {
    gate.clk(clk);
    gate.reset_bar(reset_bar);

    gate.if_rd(if_rd);
    gate.if_wr(if_wr);

    gate.wrRequestIn(wrRequestIn);
    gate.wrRespOut(wrRespOut);
    gate.rdRequestIn(rdRequestIn);
    gate.rdRespOut(rdRespOut);
  }
};

#endif
