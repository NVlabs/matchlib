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
#ifndef AXI_ADD_REMOVE_WRESP_TOP_H
#define AXI_ADD_REMOVE_WRESP_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/AxiAddWriteResponse.h>
#include <axi/AxiRemoveWriteResponse.h>

class AxiAddRemoveWRespTop : public sc_module {
 public:

  static const int kDebugLevel = 4;
  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typedef axi::axi4<axi::cfg::standard> axi_Wresp;
  typedef axi::axi4<axi::cfg::no_wresp> axi_noWresp;

  typename axi_Wresp::read::template slave<> axi_read_m;
  typename axi_Wresp::write::template slave<> axi_write_m;

  AxiRemoveWriteResponse<axi::cfg::standard, axi::cfg::no_wresp, 16> axi_remove_wresp;

  typename axi_noWresp::read::template chan<> axi_read_int;
  typename axi_noWresp::write::template chan<> axi_write_int;

  AxiAddWriteResponse<axi::cfg::no_wresp, axi::cfg::standard> axi_add_wresp;

  typename axi_Wresp::read::template master<> axi_read_s;
  typename axi_Wresp::write::template master<> axi_write_s;

  SC_HAS_PROCESS(AxiAddRemoveWRespTop);

  AxiAddRemoveWRespTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_read_m("axi_read_m"),
        axi_write_m("axi_write_m"),
        axi_remove_wresp("axi_remove_wresp"),
        axi_read_int("axi_read_int"),
        axi_write_int("axi_write_int"),
        axi_add_wresp("axi_add_wresp"),
        axi_read_s("axi_read_s"),
        axi_write_s("axi_write_s")
    {

    axi_remove_wresp.clk(clk);
    axi_add_wresp.clk(clk);

    axi_remove_wresp.rst(reset_bar);
    axi_add_wresp.rst(reset_bar);

    axi_remove_wresp.axiM_read(axi_read_m);
    axi_remove_wresp.axiM_write(axi_write_m);

    axi_remove_wresp.axiS_read(axi_read_int);
    axi_remove_wresp.axiS_write(axi_write_int);

    axi_add_wresp.axiM_read(axi_read_int);
    axi_add_wresp.axiM_write(axi_write_int);

    axi_add_wresp.axiS_read(axi_read_s);
    axi_add_wresp.axiS_write(axi_write_s);
  }
};

#endif
