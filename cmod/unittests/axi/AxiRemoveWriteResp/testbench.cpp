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
#include <axi/AxiRemoveWriteResponse.h>
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {

  typedef axi::cfg::standard cfgWithWresp;
  typedef axi::cfg::no_wresp cfgNoWresp;
  struct Mcfg {
    enum {
      numWrites = 500,
      numReads = 500,
      readDelay = 2000,
      addrBoundLower = 0,
      addrBoundUpper = 0x7FFFFFFF,
      seed = 0,
      useFile = false,
    };
  };

  Subordinate<cfgNoWresp> subordinate;
  Manager<cfgWithWresp, Mcfg> manager;
  AxiRemoveWriteResponse<cfgWithWresp, cfgNoWresp, 16> removeWResp;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  sc_signal<bool> done;

  typename axi::axi4<cfgWithWresp>::read::template chan<> axi_read_m;
  typename axi::axi4<cfgWithWresp>::write::template chan<> axi_write_m;
  typename axi::axi4<cfgNoWresp>::read::template chan<> axi_read_s;
  typename axi::axi4<cfgNoWresp>::write::template chan<> axi_write_s;

  SC_CTOR(testbench)
      : subordinate("subordinate"),
        manager("manager"),
        removeWResp("removeWResp"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read_m("axi_read_m"),
        axi_write_m("axi_write_m"),
        axi_read_s("axi_read_s"),
        axi_write_s("axi_write_s") {

    Connections::set_sim_clk(&clk);

    subordinate.clk(clk);
    manager.clk(clk);
    removeWResp.clk(clk);

    subordinate.reset_bar(reset_bar);
    manager.reset_bar(reset_bar);
    removeWResp.rst(reset_bar);

    manager.if_rd(axi_read_m);
    removeWResp.axiM_read(axi_read_m);
    removeWResp.axiS_read(axi_read_s);
    subordinate.if_rd(axi_read_s);

    manager.if_wr(axi_write_m);
    removeWResp.axiM_write(axi_write_m);
    removeWResp.axiS_write(axi_write_s);
    subordinate.if_wr(axi_write_s);

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
