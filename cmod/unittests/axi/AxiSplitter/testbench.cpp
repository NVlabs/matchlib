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

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <mc_scverify.h>
#include <axi/testbench/Manager.h>
#include <axi/testbench/Subordinate.h>
#include <axi/AxiSplitter.h>
#include <axi/AxiArbiter.h>
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {
 public:
  enum { numSubordinates = 2, numAddrBitsToInspect = 20 };

  struct manager0Cfg {
    enum {
      numWrites = 100,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0,
      addrBoundUpper = 0x7FFFF,
      seed = 0,
      useFile = false,
    };
  };
  struct manager1Cfg {
    enum {
      numWrites = 100,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0x80000,
      addrBoundUpper = 0xFFFFF,
      seed = 0,
      useFile = false,
    };
  };

  Manager<axi::cfg::standard, manager0Cfg> manager0;
  Manager<axi::cfg::standard, manager1Cfg> manager1;
  nvhls::nv_array<Subordinate<axi::cfg::standard>, numSubordinates> subordinate;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  nvhls::nv_array<sc_signal<bool>, numSubordinates> done;

  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::read::template chan<>, numSubordinates>
      axi_read_m_tb;
  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::write::template chan<>, numSubordinates>
      axi_write_m_tb;

  AxiArbiter<axi::cfg::standard, numSubordinates, 16> axi_arbiter;

  typename axi::axi4<axi::cfg::standard>::read::template chan<> axi_read_tb_int;
  typename axi::axi4<axi::cfg::standard>::write::template chan<> axi_write_tb_int;

  AxiSplitter<axi::cfg::standard, numSubordinates, numAddrBitsToInspect> axi_splitter;

  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::read::template chan<>, numSubordinates>
      axi_read_s;
  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::write::template chan<>, numSubordinates>
      axi_write_s;

  sc_signal<NVUINTW(numAddrBitsToInspect)> addrBound[numSubordinates][2];

  SC_CTOR(testbench)
      : manager0("manager0"),
        manager1("manager1"),
        subordinate("subordinate"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read_m_tb("axi_read_m_tb"),
        axi_write_m_tb("axi_write_m_tb"),
        axi_arbiter("axi_arbiter"),
        axi_read_tb_int("axi_read_tb_int"),
        axi_write_tb_int("axi_write_tb_int"),
        axi_splitter("axi_splitter"),
        axi_read_s("axi_read_s"),
        axi_write_s("axi_write_s") {
    Connections::set_sim_clk(&clk);

    manager0.clk(clk);
    manager1.clk(clk);
    axi_arbiter.clk(clk);
    axi_splitter.clk(clk);

    manager0.reset_bar(reset_bar);
    manager1.reset_bar(reset_bar);
    axi_arbiter.reset_bar(reset_bar);
    axi_splitter.reset_bar(reset_bar);

    NVUINTW(numAddrBitsToInspect)
    addrBound_val[numSubordinates][2] = {{0x00, 0x7FFFF}, {0x80000, 0xFFFFF}};

    manager0.if_rd(axi_read_m_tb[0]);
    manager0.if_wr(axi_write_m_tb[0]);
    manager0.done(done[0]);
    manager1.if_rd(axi_read_m_tb[1]);
    manager1.if_wr(axi_write_m_tb[1]);
    manager1.done(done[1]);

    for (int i = 0; i < numSubordinates; i++) {
      axi_arbiter.axi_rd_m_ar[i](axi_read_m_tb[i].ar);
      axi_arbiter.axi_rd_m_r[i](axi_read_m_tb[i].r);
      axi_arbiter.axi_wr_m_aw[i](axi_write_m_tb[i].aw);
      axi_arbiter.axi_wr_m_w[i](axi_write_m_tb[i].w);
      axi_arbiter.axi_wr_m_b[i](axi_write_m_tb[i].b);
      subordinate[i].clk(clk);
      subordinate[i].reset_bar(reset_bar);
      subordinate[i].if_rd(axi_read_s[i]);
      subordinate[i].if_wr(axi_write_s[i]);
      axi_splitter.axi_rd_s_ar[i](axi_read_s[i].ar);
      axi_splitter.axi_rd_s_r[i](axi_read_s[i].r);
      axi_splitter.axi_wr_s_aw[i](axi_write_s[i].aw);
      axi_splitter.axi_wr_s_w[i](axi_write_s[i].w);
      axi_splitter.axi_wr_s_b[i](axi_write_s[i].b);
      for (int j = 0; j < 2; j++) {
        axi_splitter.addrBound[i][j](addrBound[i][j]);
        addrBound[i][j].write(addrBound_val[i][j]);
      }
    }

    axi_arbiter.axi_rd_s(axi_read_tb_int);
    axi_arbiter.axi_wr_s(axi_write_tb_int);
    axi_splitter.axi_rd_m(axi_read_tb_int);
    axi_splitter.axi_wr_m(axi_write_tb_int);

    SC_THREAD(run);
  }

  void run() {
    reset_bar = 1;
    wait(2, SC_NS);
    reset_bar = 0;
    wait(2, SC_NS);
    reset_bar = 1;

    while (1) {
      wait(1, SC_NS);
      int doneCount = 0;
      for (int i = 0; i < numSubordinates; i++) {
        if (done[i])
          doneCount++;
      }
      if (doneCount == numSubordinates) {
        sc_stop();
      }
    }
  }
};

int sc_main(int argc, char *argv[]) {
  nvhls::set_random_seed();
  testbench tb("tb");
  sc_report_handler::set_actions(SC_ERROR, SC_DISPLAY);
  sc_start();
  bool rc = (sc_report_handler::get_count(SC_ERROR) > 0);
  if (rc)
    DCOUT("TESTBENCH FAIL" << endl);
  else
    DCOUT("TESTBENCH PASS" << endl);
  return rc;
};
