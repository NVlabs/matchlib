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

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <mc_scverify.h>
#include <axi/testbench/Master.h>
#include <axi/testbench/Slave.h>
#include "AxiArbSplitTop.h"
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {
 public:
  enum { numMasters = 2, numSlaves = 2 };

  struct master0Cfg {
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
  struct master1Cfg {
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

  typedef AxiArbSplitTop::axi_ axi_;

  Master<axi::cfg::standard, master0Cfg> master0;
  Master<axi::cfg::standard, master1Cfg> master1;
  nvhls::nv_array<Slave<axi::cfg::standard>, numSlaves> slave;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  nvhls::nv_array<sc_signal<bool>, numSlaves> done;

  nvhls::nv_array<typename axi_::read::template chan<>, numSlaves>
      axi_read_m_tb;
  nvhls::nv_array<typename axi_::write::template chan<>, numSlaves>
      axi_write_m_tb;

  CCS_DESIGN(AxiArbSplitTop) axi_arbsplit;

  nvhls::nv_array<typename axi_::read::template chan<>, numSlaves>
      axi_read_s_tb;
  nvhls::nv_array<typename axi_::write::template chan<>, numSlaves>
      axi_write_s_tb;

  SC_CTOR(testbench)
      : master0("master0"),
        master1("master1"),
        slave("slave"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read_m_tb("axi_read_m_tb"),
        axi_write_m_tb("axi_write_m_tb"),
        axi_arbsplit("axi_arbsplit"),
        axi_read_s_tb("axi_read_s_tb"),
        axi_write_s_tb("axi_write_s_tb")
  {
    Connections::set_sim_clk(&clk);

    master0.clk(clk);
    master1.clk(clk);
    axi_arbsplit.clk(clk);

    master0.reset_bar(reset_bar);
    master1.reset_bar(reset_bar);
    axi_arbsplit.reset_bar(reset_bar);

    master0.if_rd(axi_read_m_tb[0]);
    master0.if_wr(axi_write_m_tb[0]);
    master0.done(done[0]);
    master1.if_rd(axi_read_m_tb[1]);
    master1.if_wr(axi_write_m_tb[1]);
    master1.done(done[1]);

    for (int i = 0; i < numMasters; i++) {
      axi_arbsplit.axi_rd_m_ar[i](axi_read_m_tb[i].ar);
      axi_arbsplit.axi_rd_m_r[i](axi_read_m_tb[i].r);
      axi_arbsplit.axi_wr_m_aw[i](axi_write_m_tb[i].aw);
      axi_arbsplit.axi_wr_m_w[i](axi_write_m_tb[i].w);
      axi_arbsplit.axi_wr_m_b[i](axi_write_m_tb[i].b);
    }

    for (int i = 0; i < numSlaves; i++) {
      slave[i].clk(clk);
      slave[i].reset_bar(reset_bar);
      slave[i].if_rd(axi_read_s_tb[i]);
      slave[i].if_wr(axi_write_s_tb[i]);
      axi_arbsplit.axi_rd_s_ar[i](axi_read_s_tb[i].ar);
      axi_arbsplit.axi_rd_s_r[i](axi_read_s_tb[i].r);
      axi_arbsplit.axi_wr_s_aw[i](axi_write_s_tb[i].aw);
      axi_arbsplit.axi_wr_s_w[i](axi_write_s_tb[i].w);
      axi_arbsplit.axi_wr_s_b[i](axi_write_s_tb[i].b);
    }

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
      for (int i = 0; i < numSlaves; i++) {
        if (done[i])
          doneCount++;
      }
      if (doneCount == numSlaves) {
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
