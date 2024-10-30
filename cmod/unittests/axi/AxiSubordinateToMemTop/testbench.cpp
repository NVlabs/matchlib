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

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/testbench/Manager.h>
#include <mc_scverify.h>
#include <testbench/nvhls_rand.h>
#include "AxiSubordinateToMemTop.h"

SC_MODULE(testbench) {
  typedef typename axi::axi4<axi::cfg::no_wstrb> axi_;

  struct memManagerCfg {
    enum {
      numWrites = 200,
      numReads = 100,
      readDelay = 0,
      addrBoundLower = 0,
      addrBoundUpper = 8 * 256 - 1,
      seed = 0,
    };
  };

  CCS_DESIGN(AxiSubordinateToMemTop) subordinate;
  Manager<axi::cfg::no_wstrb, memManagerCfg> manager;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  sc_signal<bool> done;

  axi_::read::template chan<> axi_read;
  axi_::write::template chan<> axi_write;

  SC_CTOR(testbench)
      : subordinate("subordinate"),
        manager("manager"),

        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write") {
    subordinate.clk(clk);
    manager.clk(clk);

    subordinate.reset_bar(reset_bar);
    manager.reset_bar(reset_bar);

    manager.if_rd(axi_read);
    subordinate.axi_read(axi_read);

    manager.if_wr(axi_write);
    subordinate.axi_write(axi_write);

    manager.done(done);
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
      if (done) {
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
