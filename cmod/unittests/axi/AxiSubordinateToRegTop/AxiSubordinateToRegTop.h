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
#ifndef AXI_SUBORDINATE_TO_REG_TOP_H
#define AXI_SUBORDINATE_TO_REG_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/AxiSubordinateToReg.h>

class AxiSubordinateToRegTop : public sc_module {
 public:
  static const int kDebugLevel = 4;

  typedef axi::axi4<axi::cfg::standard> axi_;
  enum { numControlReg = 128, numStatusReg = 16, baseAddress = 0x100, numAddrBitsToInspect = 16 };

  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typename axi_::read::template subordinate<> axi_read;
  typename axi_::write::template subordinate<> axi_write;

#ifdef STATUS_REG
  AxiSubordinateToCSReg<axi::cfg::standard, numControlReg, numStatusReg, numAddrBitsToInspect> subordinate;
#else
  AxiSubordinateToReg<axi::cfg::standard, numControlReg, numAddrBitsToInspect> subordinate;
#endif

  sc_signal<NVUINTW(numAddrBitsToInspect)> baseAddr;
  sc_out<NVUINTW(axi_::DATA_WIDTH)> regOut[numControlReg];
#ifdef STATUS_REG
  sc_in<NVUINTW(axi_::DATA_WIDTH)> regIn[numStatusReg];
#endif

  SC_HAS_PROCESS(AxiSubordinateToRegTop);

  AxiSubordinateToRegTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write"),
        subordinate("subordinate")
  {
    subordinate.clk(clk);
    subordinate.reset_bar(reset_bar);

    subordinate.if_axi_rd(axi_read);
    subordinate.if_axi_wr(axi_write);

    subordinate.baseAddr(baseAddr);
    baseAddr.write(baseAddress);

#pragma hls_unroll yes
    for (int i = 0; i < numControlReg; i++) {
      subordinate.regOut[i](regOut[i]);
    }

#ifdef STATUS_REG
#pragma hls_unroll yes
    for (int i = 0; i < numStatusReg; i++) {
      subordinate.regIn[i](regIn[i]);
    }
#endif

  }
};

#endif
