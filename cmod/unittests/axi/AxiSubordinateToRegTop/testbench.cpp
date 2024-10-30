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
#include "AxiSubordinateToRegTop.h"
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {

  typedef AxiSubordinateToRegTop::axi_ axi_;
  enum {
    numControlReg = AxiSubordinateToRegTop::numControlReg,
    numStatusReg = AxiSubordinateToRegTop::numStatusReg,
    numAddrBitsToInspect = AxiSubordinateToRegTop::numAddrBitsToInspect
  };

  struct Mcfg {
    enum {
      numWrites = 200,
      numReads = 200,
      readDelay = 0,
      addrBoundLower = 0x100,
      addrBoundUpper = 0x4FF,
      seed = 0,
      useFile = false,
    };
  };

  Manager<axi::cfg::standard, Mcfg> manager;
  CCS_DESIGN(AxiSubordinateToRegTop) subordinate;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  sc_signal<bool> done;

  typename axi_::read::template chan<> axi_read;
  typename axi_::write::template chan<> axi_write;

  sc_signal<NVUINTW(axi::axi4<axi::cfg::standard>::DATA_WIDTH)> regOut[numControlReg];
#ifdef STATUS_REG
  sc_signal<NVUINTW(axi::axi4<axi::cfg::standard>::DATA_WIDTH)> regIn[numStatusReg];
#endif

  SC_CTOR(testbench)
      : manager("manager"),
        subordinate("subordinate"),
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
    manager.if_wr(axi_write);

    subordinate.axi_read(axi_read);
    subordinate.axi_write(axi_write);

    for (int i = 0; i < numControlReg; i++) {
      subordinate.regOut[i](regOut[i]);
    }

#ifdef STATUS_REG
    for (int i = 0; i < numStatusReg; i++) {
      subordinate.regIn[i](regIn[i]);
    }
#endif

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
  // Suppress mysterious time-zero X warnings in SCVerify
  sc_report_handler::set_actions(SC_ID_LOGIC_X_TO_BOOL_,SC_DO_NOTHING);
  sc_start();
  bool rc = (sc_report_handler::get_count(SC_ERROR) > 0);
  if (rc)
    DCOUT("TESTBENCH FAIL" << endl);
  else
    DCOUT("TESTBENCH PASS" << endl);
  return rc;
};
