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
#include "AxiSlaveToRegTop.h"
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {

  typedef AxiSlaveToRegTop::axi_ axi_;
  enum {
    numReg = AxiSlaveToRegTop::numReg,
    numAddrBitsToInspect = AxiSlaveToRegTop::numAddrBitsToInspect
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

  Master<axi::cfg::standard, Mcfg> master;
  CCS_DESIGN(AxiSlaveToRegTop) slave;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  sc_signal<bool> done;

  typename axi_::read::template chan<> axi_read;
  typename axi_::write::template chan<> axi_write;

  sc_signal<NVUINTW(axi::axi4<axi::cfg::standard>::DATA_WIDTH)> regOut[numReg];

  SC_CTOR(testbench)
      : master("master"),
        slave("slave"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write") {

    Connections::set_sim_clk(&clk);

    slave.clk(clk);
    master.clk(clk);

    slave.reset_bar(reset_bar);
    master.reset_bar(reset_bar);

    master.if_rd(axi_read);
    master.if_wr(axi_write);

    slave.axi_read(axi_read);
    slave.axi_write(axi_write);

    for (int i = 0; i < numReg; i++) {
      slave.regOut[i](regOut[i]);
    }

    master.done(done);
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
