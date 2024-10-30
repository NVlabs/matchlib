/*
 * Copyright (c) 2020-2024, NVIDIA CORPORATION.  All rights reserved.
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
#include <axi/testbench/ManagerFromFile.h>
#include <axi/testbench/SubordinateFromFile.h>
#include <testbench/nvhls_rand.h>

// A simple AXI testbench example.  A Manager and Subordinate are wired together
// directly with no DUT in between.

SC_MODULE(testbench) {

  SubordinateFromFile<axi::cfg::standard> subordinate;
  ManagerFromFile<axi::cfg::standard,1> manager;

  sc_clock clk;
  sc_signal<bool> reset_bar;

  sc_signal<bool> done;
  sc_signal<bool> interrupt;

  typename axi::axi4<axi::cfg::standard>::read::template chan<> axi_read;
  typename axi::axi4<axi::cfg::standard>::write::template chan<> axi_write;

  SC_CTOR(testbench)
      : subordinate("subordinate", "mem.csv"),
        manager("manager", "requests_q.csv"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write") {

    Connections::set_sim_clk(&clk);

    subordinate.clk(clk);
    manager.clk(clk);

    subordinate.reset_bar(reset_bar);
    manager.reset_bar(reset_bar);

    manager.if_rd(axi_read);
    subordinate.if_rd(axi_read);

    manager.if_wr(axi_write);
    subordinate.if_wr(axi_write);

    manager.done(done);
    manager.interrupt(interrupt);

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
      interrupt.write(1);
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
