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
#include <axi/AxiArbiter.h>
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {

  enum {
    numManagers = 4,
    maxInFlight = 8,
  };

  struct manager0Cfg {
    enum {
      numWrites = 100,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0,
      addrBoundUpper = 0x1FFFFFFF,
      seed = 0,
      useFile = false,
    };
  };
  struct manager1Cfg {
    enum {
      numWrites = 100,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0x20000000,
      addrBoundUpper = 0x3FFFFFFF,
      seed = 1,
      useFile = false,
    };
  };
  struct manager2Cfg {
    enum {
      numWrites = 100,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0x40000000,
      addrBoundUpper = 0x5FFFFFFF,
      seed = 2,
      useFile = false,
    };
  };
  struct manager3Cfg {
    enum {
      numWrites = 100,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0x60000000,
      addrBoundUpper = 0x7FFFFFFF,
      seed = 3,
      useFile = false,
    };
  };

  Subordinate<axi::cfg::standard> subordinate;
  Manager<axi::cfg::standard, manager0Cfg> manager0;
  Manager<axi::cfg::standard, manager1Cfg> manager1;
  Manager<axi::cfg::standard, manager2Cfg> manager2;
  Manager<axi::cfg::standard, manager3Cfg> manager3;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  nvhls::nv_array<sc_signal<bool>, numManagers> done;

  AxiArbiter<axi::cfg::standard, numManagers, maxInFlight> axi_arbiter;

  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::read::template chan<>, numManagers>
      axi_read_m;
  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::write::template chan<>, numManagers>
      axi_write_m;
  typename axi::axi4<axi::cfg::standard>::read::template chan<> axi_read_s;
  typename axi::axi4<axi::cfg::standard>::write::template chan<> axi_write_s;

  SC_CTOR(testbench)
      : subordinate("subordinate"),
        manager0("manager0"),
        manager1("manager1"),
        manager2("manager2"),
        manager3("manager3"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_arbiter("axi_arbiter"),
        axi_read_m("axi_read_m"),
        axi_write_m("axi_write_m"),
        axi_read_s("axi_read_s"),
        axi_write_s("axi_write_s") {

    Connections::set_sim_clk(&clk);

    subordinate.clk(clk);
    axi_arbiter.clk(clk);

    subordinate.reset_bar(reset_bar);
    axi_arbiter.reset_bar(reset_bar);

    manager0.clk(clk);
    manager0.reset_bar(reset_bar);
    manager0.if_rd(axi_read_m[0]);
    manager0.if_wr(axi_write_m[0]);
    manager0.done(done[0]);
    manager1.clk(clk);
    manager1.reset_bar(reset_bar);
    manager1.if_rd(axi_read_m[1]);
    manager1.if_wr(axi_write_m[1]);
    manager1.done(done[1]);
    manager2.clk(clk);
    manager2.reset_bar(reset_bar);
    manager2.if_rd(axi_read_m[2]);
    manager2.if_wr(axi_write_m[2]);
    manager2.done(done[2]);
    manager3.clk(clk);
    manager3.reset_bar(reset_bar);
    manager3.if_rd(axi_read_m[3]);
    manager3.if_wr(axi_write_m[3]);
    manager3.done(done[3]);
    for (int i = 0; i < numManagers; i++) {
      axi_arbiter.axi_rd_m_ar[i](axi_read_m[i].ar);
      axi_arbiter.axi_rd_m_r[i](axi_read_m[i].r);
      axi_arbiter.axi_wr_m_aw[i](axi_write_m[i].aw);
      axi_arbiter.axi_wr_m_w[i](axi_write_m[i].w);
      axi_arbiter.axi_wr_m_b[i](axi_write_m[i].b);
    }

    axi_arbiter.axi_rd_s(axi_read_s);
    axi_arbiter.axi_wr_s(axi_write_s);

    subordinate.if_rd(axi_read_s);
    subordinate.if_wr(axi_write_s);

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
      for (int i = 0; i < numManagers; i++) {
        if (done[i])
          doneCount++;
      }
      if (doneCount == numManagers) {
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
