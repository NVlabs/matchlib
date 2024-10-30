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
#ifndef AXI_ARB_SPLIT_TOP_H
#define AXI_ARB_SPLIT_TOP_H

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <axi/AxiSplitter.h>
#include <axi/AxiArbiter.h>

class AxiArbSplitTop : public sc_module {
 public:
  enum { numSubordinates = 2, numManagers = 2, numAddrBitsToInspect = 20 };

  static const int kDebugLevel = 4;
  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typedef axi::axi4<axi::cfg::standard> axi_;

  typedef typename axi_::read::template subordinate<>::ARPort axi_rd_subordinate_ar;
  typedef typename axi_::read::template subordinate<>::RPort axi_rd_subordinate_r;
  typedef typename axi_::write::template subordinate<>::AWPort axi_wr_subordinate_aw;
  typedef typename axi_::write::template subordinate<>::WPort axi_wr_subordinate_w;
  typedef typename axi_::write::template subordinate<>::BPort axi_wr_subordinate_b;
  
  typedef typename axi_::read::template manager<>::ARPort axi_rd_manager_ar;
  typedef typename axi_::read::template manager<>::RPort axi_rd_manager_r;
  typedef typename axi_::write::template manager<>::AWPort axi_wr_manager_aw;
  typedef typename axi_::write::template manager<>::WPort axi_wr_manager_w;
  typedef typename axi_::write::template manager<>::BPort axi_wr_manager_b;

  axi_rd_subordinate_ar axi_rd_m_ar[numManagers];
  axi_rd_subordinate_r axi_rd_m_r[numManagers];
  axi_wr_subordinate_aw axi_wr_m_aw[numManagers];
  axi_wr_subordinate_w axi_wr_m_w[numManagers];
  axi_wr_subordinate_b axi_wr_m_b[numManagers];

  AxiArbiter<axi::cfg::standard, numSubordinates, 16> axi_arbiter;

  typename axi_::read::template chan<> axi_read_int;
  typename axi_::write::template chan<> axi_write_int;

  AxiSplitter<axi::cfg::standard, numSubordinates, numAddrBitsToInspect> axi_splitter;

  axi_rd_manager_ar axi_rd_s_ar[numSubordinates];
  axi_rd_manager_r axi_rd_s_r[numSubordinates];
  axi_wr_manager_aw axi_wr_s_aw[numSubordinates];
  axi_wr_manager_w axi_wr_s_w[numSubordinates];
  axi_wr_manager_b axi_wr_s_b[numSubordinates];

  sc_signal<NVUINTW(numAddrBitsToInspect)> addrBound[numSubordinates][2];

  SC_HAS_PROCESS(AxiArbSplitTop);

  AxiArbSplitTop(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_arbiter("axi_arbiter"),
        axi_read_int("axi_read_int"),
        axi_write_int("axi_write_int"),
        axi_splitter("axi_splitter")
    {

    axi_arbiter.clk(clk);
    axi_splitter.clk(clk);

    axi_arbiter.reset_bar(reset_bar);
    axi_splitter.reset_bar(reset_bar);

    NVUINTW(numAddrBitsToInspect)
    addrBound_val[numSubordinates][2] = {{0x00, 0x7FFFF}, {0x80000, 0xFFFFF}};

    for (int i = 0; i < numManagers; i++) {
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

    for (int i = 0; i < numSubordinates; i++) {
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
