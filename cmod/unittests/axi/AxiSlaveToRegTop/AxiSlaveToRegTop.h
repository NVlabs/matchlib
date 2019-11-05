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
#ifndef AXI_SLAVE_TO_REG_TOP_H
#define AXI_SLAVE_TO_REG_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/AxiSlaveToReg.h>

class AxiSlaveToRegTop : public sc_module {
 public:
  static const int kDebugLevel = 4;

  typedef axi::axi4<axi::cfg::standard> axi_;
  enum { numReg = 128, baseAddress = 0x100, numAddrBitsToInspect = 16 };

  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typename axi_::read::template slave<> axi_read;
  typename axi_::write::template slave<> axi_write;

  AxiSlaveToReg<axi::cfg::standard, numReg, numAddrBitsToInspect> slave;

  sc_signal<NVUINTW(numAddrBitsToInspect)> baseAddr;
  sc_out<NVUINTW(axi_::DATA_WIDTH)> regOut[numReg];

  SC_HAS_PROCESS(AxiSlaveToRegTop);

  AxiSlaveToRegTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write"),
        slave("slave")
  {
    slave.clk(clk);
    slave.reset_bar(reset_bar);

    slave.if_axi_rd(axi_read);
    slave.if_axi_wr(axi_write);

    slave.baseAddr(baseAddr);
    baseAddr.write(baseAddress);

    for (int i = 0; i < numReg; i++) {
      slave.regOut[i](regOut[i]);
    }
  }
};

#endif
