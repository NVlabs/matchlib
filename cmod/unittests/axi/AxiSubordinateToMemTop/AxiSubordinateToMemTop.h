/*
 * Copyright (c) 2016-2024, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef AXI_SUBORDINATE_TO_MEM_TOP_H
#define AXI_SUBORDINATE_TO_MEM_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>
#include <axi/axi4_configs.h>

#include <axi/AxiSubordinateToMem.h>

class AxiSubordinateToMemTop : public sc_module {
 public:
  static const int kDebugLevel = 4;
  typedef typename axi::axi4<axi::cfg::no_wstrb> axi_;

  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typename axi_::read::template subordinate<> axi_read;
  typename axi_::write::template subordinate<> axi_write;

  AxiSubordinateToMem<axi::cfg::no_wstrb, 8 * 256> subordinate;

  SC_HAS_PROCESS(AxiSubordinateToMemTop);

  AxiSubordinateToMemTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write"),
        subordinate("subordinate")
  {
    subordinate.clk(clk);
    subordinate.reset_bar(reset_bar);

    subordinate.if_rd(axi_read);
    subordinate.if_wr(axi_write);
  }
};

#endif
