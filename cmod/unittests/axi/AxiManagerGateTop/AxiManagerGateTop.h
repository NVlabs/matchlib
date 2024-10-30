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

#ifndef __AXIMANAGERGATETOP_H__
#define __AXIMANAGERGATETOP_H__

#include <axi/AxiManagerGate.h>
#include <axi/axi4_configs.h>

SC_MODULE(AxiManagerGateTop) {

 private:
  AxiManagerGate<axi::cfg::standard> gate;

 public:
  typedef axi::axi4<axi::cfg::standard> axi4_;

  typename axi4_::read::template manager<> if_rd;
  typename axi4_::write::template manager<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;
  Connections::In<WrRequest<axi::cfg::standard> > wrRequestIn;
  Connections::Out<WrResp<axi::cfg::standard> > wrRespOut;
  Connections::In<RdRequest<axi::cfg::standard> > rdRequestIn;
  Connections::Out<RdResp<axi::cfg::standard> > rdRespOut;

  SC_CTOR(AxiManagerGateTop)
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
