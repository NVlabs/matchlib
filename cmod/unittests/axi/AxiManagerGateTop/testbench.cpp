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
#include <nvhls_connections.h>
#include <testbench/nvhls_rand.h>
#include "AxiManagerGateTop.h"
#include <axi/AxiManagerGate/testbench/Host.h>
#include <axi/testbench/Subordinate.h>
#include <mc_scverify.h>

typedef axi::axi4<axi::cfg::standard> axi4_;

SC_MODULE(testbench) {
  Subordinate<axi::cfg::standard> subordinate;
  CCS_DESIGN(AxiManagerGateTop) manager;
  Host<axi::cfg::standard> host;

  sc_clock clk;
  sc_signal<bool> reset_bar;

  sc_signal<bool> done_write, done_read;

  axi4_::read::template chan<> axi_read;
  axi4_::write::template chan<> axi_write;

  Connections::Combinational<WrRequest<axi::cfg::standard> > wrRequestChan;
  Connections::Combinational<WrResp<axi::cfg::standard> > wrRespChan;
  Connections::Combinational<RdRequest<axi::cfg::standard> > rdRequestChan;
  Connections::Combinational<RdResp<axi::cfg::standard> > rdRespChan;

  SC_CTOR(testbench)
      : subordinate("subordinate"),
        manager("manager"),
        host("host"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read("axi_read"),
        axi_write("axi_write") {
    subordinate.clk(clk);
    manager.clk(clk);
    host.clk(clk);

    subordinate.reset_bar(reset_bar);
    manager.reset_bar(reset_bar);
    host.reset_bar(reset_bar);

    manager.if_rd(axi_read);
    subordinate.if_rd(axi_read);

    manager.if_wr(axi_write);
    subordinate.if_wr(axi_write);

    manager.wrRequestIn(wrRequestChan);
    host.wrRequestOut(wrRequestChan);

    manager.wrRespOut(wrRespChan);
    host.wrRespIn(wrRespChan);

    manager.rdRequestIn(rdRequestChan);
    host.rdRequestOut(rdRequestChan);

    manager.rdRespOut(rdRespChan);
    host.rdRespIn(rdRespChan);

    host.done_write(done_write);
    host.done_read(done_read);

    SC_THREAD(run);
  }

  void run() {
    // reset
    reset_bar = 1;
    wait(20, SC_NS);
    reset_bar = 0;
    wait(2, SC_NS);
    reset_bar = 1;

    while (1) {
      wait(1, SC_NS);
      if (done_write && done_read) {
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
