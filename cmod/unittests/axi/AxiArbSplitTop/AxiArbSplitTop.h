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
#ifndef AXI_ARB_SPLIT_TOP_H
#define AXI_ARB_SPLIT_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/AxiSplitter.h>
#include <axi/AxiArbiter.h>

class AxiArbSplitTop : public sc_module {
 public:
  enum { numSlaves = 2, numMasters = 2, numAddrBitsToInspect = 20 };

  static const int kDebugLevel = 4;
  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typedef axi::axi4<axi::cfg::standard> axi_;

  typedef typename axi_::read::template slave<>::ARPort axi_rd_slave_ar;
  typedef typename axi_::read::template slave<>::RPort axi_rd_slave_r;
  typedef typename axi_::write::template slave<>::AWPort axi_wr_slave_aw;
  typedef typename axi_::write::template slave<>::WPort axi_wr_slave_w;
  typedef typename axi_::write::template slave<>::BPort axi_wr_slave_b;
  
  typedef typename axi_::read::template master<>::ARPort axi_rd_master_ar;
  typedef typename axi_::read::template master<>::RPort axi_rd_master_r;
  typedef typename axi_::write::template master<>::AWPort axi_wr_master_aw;
  typedef typename axi_::write::template master<>::WPort axi_wr_master_w;
  typedef typename axi_::write::template master<>::BPort axi_wr_master_b;

  nvhls::nv_array<axi_rd_slave_ar, numMasters> axi_rd_m_ar;
  nvhls::nv_array<axi_rd_slave_r, numMasters> axi_rd_m_r;
  nvhls::nv_array<axi_wr_slave_aw, numMasters> axi_wr_m_aw;
  nvhls::nv_array<axi_wr_slave_w, numMasters> axi_wr_m_w;
  nvhls::nv_array<axi_wr_slave_b, numMasters> axi_wr_m_b;

  AxiArbiter<axi::cfg::standard, numSlaves, 16> axi_arbiter;

  typename axi_::read::template chan<> axi_read_int;
  typename axi_::write::template chan<> axi_write_int;

  AxiSplitter<axi::cfg::standard, numSlaves, numAddrBitsToInspect> axi_splitter;

  nvhls::nv_array<axi_rd_master_ar, numSlaves> axi_rd_s_ar;
  nvhls::nv_array<axi_rd_master_r, numSlaves> axi_rd_s_r;
  nvhls::nv_array<axi_wr_master_aw, numSlaves> axi_wr_s_aw;
  nvhls::nv_array<axi_wr_master_w, numSlaves> axi_wr_s_w;
  nvhls::nv_array<axi_wr_master_b, numSlaves> axi_wr_s_b;

  sc_signal<NVUINTW(numAddrBitsToInspect)> addrBound[numSlaves][2];

  SC_HAS_PROCESS(AxiArbSplitTop);

  AxiArbSplitTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_rd_m_ar("axi_rd_m_ar"),
        axi_rd_m_r("axi_rd_m_r"),
        axi_wr_m_aw("axi_wr_m_aw"),
        axi_wr_m_w("axi_wr_m_w"),
        axi_wr_m_b("axi_wr_m_b"),
        axi_arbiter("axi_arbiter"),
        axi_read_int("axi_read_int"),
        axi_write_int("axi_write_int"),
        axi_splitter("axi_splitter"),
        axi_rd_s_ar("axi_rd_s_ar"),
        axi_rd_s_r("axi_rd_s_r"),
        axi_wr_s_aw("axi_wr_s_aw"),
        axi_wr_s_w("axi_wr_s_w"),
        axi_wr_s_b("axi_wr_s_b")
    {

    axi_arbiter.clk(clk);
    axi_splitter.clk(clk);

    axi_arbiter.reset_bar(reset_bar);
    axi_splitter.reset_bar(reset_bar);

    NVUINTW(numAddrBitsToInspect)
    addrBound_val[numSlaves][2] = {{0x00, 0x7FFFF}, {0x80000, 0xFFFFF}};

    for (int i = 0; i < numMasters; i++) {
      axi_arbiter.axi_rd_m_ar[i](axi_rd_m_ar[i]);
      axi_arbiter.axi_rd_m_r[i](axi_rd_m_r[i]);
      axi_arbiter.axi_wr_m_aw[i](axi_wr_m_aw[i]);
      axi_arbiter.axi_wr_m_w[i](axi_wr_m_w[i]);
      axi_arbiter.axi_wr_m_b[i](axi_wr_m_b[i]);
    }

    axi_arbiter.axi_rd_s(axi_read_int);
    axi_arbiter.axi_wr_s(axi_write_int);
    axi_splitter.axi_rd_m(axi_read_int);
    axi_splitter.axi_wr_m(axi_write_int);

    for (int i = 0; i < numSlaves; i++) {
      axi_splitter.axi_rd_s_ar[i](axi_rd_s_ar[i]);
      axi_splitter.axi_rd_s_r[i](axi_rd_s_r[i]);
      axi_splitter.axi_wr_s_aw[i](axi_wr_s_aw[i]);
      axi_splitter.axi_wr_s_w[i](axi_wr_s_w[i]);
      axi_splitter.axi_wr_s_b[i](axi_wr_s_b[i]);
      for (int j = 0; j < 2; j++) {
        axi_splitter.addrBound[i][j](addrBound[i][j]);
        addrBound[i][j].write(addrBound_val[i][j]);
      }
    }
  }
};

#endif
