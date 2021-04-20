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
#include <axi/AxiSplitter.h>
#include <axi/AxiArbiter.h>
#include <testbench/nvhls_rand.h>

SC_MODULE(testbench) {
 public:
  enum { numSlaves = 2, numAddrBitsToInspect = 20 };

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

  Master<axi::cfg::standard, master0Cfg> master0;
  Master<axi::cfg::standard, master1Cfg> master1;
  nvhls::nv_array<Slave<axi::cfg::standard>, numSlaves> slave;

  sc_clock clk;
  sc_signal<bool> reset_bar;
  nvhls::nv_array<sc_signal<bool>, numSlaves> done;

  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::read::template chan<>, numSlaves>
      axi_read_m_tb;
  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::write::template chan<>, numSlaves>
      axi_write_m_tb;

  AxiArbiter<axi::cfg::standard, numSlaves, 16> axi_arbiter;

  typename axi::axi4<axi::cfg::standard>::read::template chan<> axi_read_tb_int;
  typename axi::axi4<axi::cfg::standard>::write::template chan<> axi_write_tb_int;

  AxiSplitter<axi::cfg::standard, numSlaves, numAddrBitsToInspect> axi_splitter;

  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::read::template chan<>, numSlaves>
      axi_read_s;
  nvhls::nv_array<typename axi::axi4<axi::cfg::standard>::write::template chan<>, numSlaves>
      axi_write_s;
  typename axi::axi4<axi::cfg::standard>::read::template chan<> axi_read_m;
  typename axi::axi4<axi::cfg::standard>::write::template chan<> axi_write_m;

  sc_signal<NVUINTW(numAddrBitsToInspect)> addrBound[numSlaves][2];

  SC_CTOR(testbench)
      : master0("master0"),
        master1("master1"),
        slave("slave"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        reset_bar("reset_bar"),
        axi_read_m_tb("axi_read_m_tb"),
        axi_write_m_tb("axi_write_m_tb"),
        axi_arbiter("axi_arbiter"),
        axi_read_tb_int("axi_read_tb_int"),
        axi_write_tb_int("axi_write_tb_int"),
        axi_splitter("axi_splitter"),
        axi_read_s("axi_read_s"),
        axi_write_s("axi_write_s"),
        axi_read_m("axi_read_m"),
        axi_write_m("axi_write_m") {
    Connections::set_sim_clk(&clk);

    master0.clk(clk);
    master1.clk(clk);
    axi_arbiter.clk(clk);
    axi_splitter.clk(clk);

    master0.reset_bar(reset_bar);
    master1.reset_bar(reset_bar);
    axi_arbiter.reset_bar(reset_bar);
    axi_splitter.reset_bar(reset_bar);

    NVUINTW(numAddrBitsToInspect)
    addrBound_val[numSlaves][2] = {{0x00, 0x7FFFF}, {0x80000, 0xFFFFF}};

    master0.if_rd(axi_read_m_tb[0]);
    master0.if_wr(axi_write_m_tb[0]);
    master0.done(done[0]);
    master1.if_rd(axi_read_m_tb[1]);
    master1.if_wr(axi_write_m_tb[1]);
    master1.done(done[1]);

    for (int i = 0; i < numSlaves; i++) {
      axi_arbiter.axi_rd_m_ar[i](axi_read_m_tb[i].ar);
      axi_arbiter.axi_rd_m_r[i](axi_read_m_tb[i].r);
      axi_arbiter.axi_wr_m_aw[i](axi_write_m_tb[i].aw);
      axi_arbiter.axi_wr_m_w[i](axi_write_m_tb[i].w);
      axi_arbiter.axi_wr_m_b[i](axi_write_m_tb[i].b);
      slave[i].clk(clk);
      slave[i].reset_bar(reset_bar);
      slave[i].if_rd(axi_read_s[i]);
      slave[i].if_wr(axi_write_s[i]);
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
