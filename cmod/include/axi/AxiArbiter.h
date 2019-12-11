/*
 * Copyright (c) 2018-2019, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef __AXI_ARBITER_H__
#define __AXI_ARBITER_H__

#include <systemc.h>
#include <nvhls_connections.h>
#include <nvhls_serdes.h>
#include <nvhls_packet.h>
#include <nvhls_int.h>
#include <nvhls_array.h>
#include <axi/axi4.h>
#include "Arbiter.h"
#include "TypeToBits.h"

/**
 * \brief An n-way arbiter that connects multiple AXI master ports to a single AXI slave port.
 * \ingroup AXI
 *
 * \tparam axiCfg                   A valid AXI config.
 * \tparam numMasters               The number of masters to arbitrate between.
 * \tparam maxOutstandingRequests   The number of oustanding read or write requests that can be tracked with internal state.
 *
 * \par Overview
 * AxiArbiter connects one or more AXI masters to a single AXI slave.  In the case of contention, a round-robin Arbiter selects the next request to pass through.
 * - The arbiter assumes that responses are returned in the order that requests are sent.  Downstream request reordering is currently not supported. 
 * - The AXI configs of all ports must be the same.
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/AxiArbiter/run_ar/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <typename axiCfg, int numMasters, int maxOutstandingRequests>
class AxiArbiter : public sc_module {
 public:
  static const int kDebugLevel = 5;
  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typedef typename axi::axi4<axiCfg> axi_;
  static const int numMasters_width = nvhls::log2_ceil<numMasters>::val;

  typedef typename axi_::read::template slave<>::ARPort axi_rd_slave_ar;
  typedef typename axi_::read::template slave<>::RPort axi_rd_slave_r;
  typedef typename axi_::write::template slave<>::AWPort axi_wr_slave_aw;
  typedef typename axi_::write::template slave<>::WPort axi_wr_slave_w;
  typedef typename axi_::write::template slave<>::BPort axi_wr_slave_b;

  // [ben] Unfortunately HLS cannot handle an nv_array of the master/slave wrapper classes.
  // It will work fine in C but die mysteriously in Catapult 10.1b when methods of the
  // bundled connections are accessed.
  nvhls::nv_array<axi_rd_slave_ar, numMasters> axi_rd_m_ar;
  nvhls::nv_array<axi_rd_slave_r, numMasters> axi_rd_m_r;
  nvhls::nv_array<axi_wr_slave_aw, numMasters> axi_wr_m_aw;
  nvhls::nv_array<axi_wr_slave_w, numMasters> axi_wr_m_w;
  nvhls::nv_array<axi_wr_slave_b, numMasters> axi_wr_m_b;
  typename axi_::read::template master<> axi_rd_s;
  typename axi_::write::template master<> axi_wr_s;

  typedef NVUINTW(numMasters_width) inFlight_t;
  FIFO<inFlight_t, maxOutstandingRequests> readQ;
  Connections::Combinational<inFlight_t> read_in_flight;
  FIFO<inFlight_t, maxOutstandingRequests> writeQ;
  Connections::Combinational<inFlight_t> write_in_flight;
  Connections::Combinational<inFlight_t> active_write_master;
  Connections::Combinational<NVUINT1> w_last; // true iff w item last bit is set

  SC_HAS_PROCESS(AxiArbiter);

  AxiArbiter(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_rd_m_ar("axi_rd_m_ar"),
        axi_rd_m_r("axi_rd_m_r"),
        axi_wr_m_aw("axi_wr_m_aw"),
        axi_wr_m_w("axi_wr_m_w"),
        axi_wr_m_b("axi_wr_m_b"),
        axi_rd_s("axi_rd_s"),
        axi_wr_s("axi_wr_s") {

    SC_THREAD(run_ar);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_r);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_w);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_aw);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_b);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

  void run_ar() {
    #pragma hls_unroll yes
    for (int i = 0; i < numMasters; i++) {
      axi_rd_m_ar[i].Reset();
    }
    axi_rd_s.ar.Reset();
    read_in_flight.ResetWrite();

    nvhls::nv_array<typename axi_::AddrPayload, numMasters> AR_reg;
    NVUINTW(numMasters) valid_mask = 0;
    NVUINTW(numMasters) select_mask = 0;
    Arbiter<numMasters> arb;

    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      for (int i = 0; i < numMasters; i++) {
        if (nvhls::get_slc<1>(valid_mask, i) == 0) {
          if (axi_rd_m_ar[i].PopNB(AR_reg[i])) {
            valid_mask = valid_mask | (1 << i);
          }
        }
      }

      select_mask = arb.pick(valid_mask);

      for (int i = 0; i < numMasters; i++) {
        if (nvhls::get_slc<1>(select_mask, i) == 1) {
          axi_rd_s.ar.Push(AR_reg[i]);
          select_mask = 0;
          valid_mask = ~(~valid_mask | (1 << i));
          CDCOUT(sc_time_stamp() << " " << name() << " Pushed read request:"
                        << " from_port=" << i
                        << " request=[" << AR_reg[i] << "]"
                        << endl, kDebugLevel);
          read_in_flight.Push(i);
        }
      }
    }
  }

  void run_r() {
    #pragma hls_unroll yes
    for (int i = 0; i < numMasters; i++) {
      axi_rd_m_r[i].Reset();
    }
    axi_rd_s.r.Reset();
    read_in_flight.ResetRead();
    readQ.reset();

    typename axi_::ReadPayload R_reg;
    inFlight_t inFlight_reg;
    inFlight_t inFlight_resp_reg;

    bool read_inProgress = 0;

    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      if (!readQ.isFull()) {
        if (read_in_flight.PopNB(inFlight_reg)) {
          readQ.push(inFlight_reg);
        }
      }

      if (!read_inProgress) {
        if (!readQ.isEmpty()) {
          inFlight_resp_reg = readQ.pop();
          read_inProgress = 1;
        }
      } else {
        if (axi_rd_s.r.PopNB(R_reg)) {
          axi_rd_m_r[inFlight_resp_reg].Push(R_reg);
          CDCOUT(sc_time_stamp() << " " << name() << " Pushed read response:"
                        << " to_port=" << inFlight_resp_reg
                        << " response=[" << R_reg << "]"
                        << endl, kDebugLevel);
          if (R_reg.last == 1)
            read_inProgress = 0;
        }
      }
    }
  }

  void run_aw() {
    #pragma hls_unroll yes
    for (int i = 0; i < numMasters; i++) {
      axi_wr_m_aw[i].Reset();
    }
    axi_wr_s.aw.Reset();
    write_in_flight.ResetWrite();
    active_write_master.ResetWrite();
    w_last.ResetRead();

    inFlight_t active_master;

    nvhls::nv_array<typename axi_::AddrPayload, numMasters> AW_reg;
    NVUINTW(numMasters) valid_mask = 0;
    NVUINTW(numMasters) select_mask = 0;
    Arbiter<numMasters> arb;

    while (1) {
      wait();

      #pragma hls_unroll yes
      for (int i = 0; i < numMasters; i++) {
        if (nvhls::get_slc<1>(valid_mask, i) == 0) {
          if (axi_wr_m_aw[i].PopNB(AW_reg[i])) {
            valid_mask = valid_mask | (1 << i);
          }
        }
      }

      select_mask = arb.pick(valid_mask);

      if (select_mask != 0) {
        #pragma hls_unroll yes
        for (int i = 0; i < numMasters; i++) {
          if (nvhls::get_slc<1>(select_mask, i) == 1) {
            active_master = i;
          }
        }

        axi_wr_s.aw.Push(AW_reg[active_master]);
        active_write_master.Push(active_master);
        write_in_flight.Push(active_master);

        #pragma hls_pipeline_init_interval 1
        #pragma pipeline_stall_mode flush
        while (w_last.Pop() == false) {
        }

        select_mask = 0;
        valid_mask = ~(~valid_mask | (1 << active_master));
      }
    }
  }

  void run_w() {
    typename axi_::WritePayload W_reg;
    active_write_master.ResetRead();
    w_last.ResetWrite();
    axi_wr_s.w.Reset();
    #pragma hls_unroll yes
    for (int i = 0; i < numMasters; i++) {
      axi_wr_m_w[i].Reset();
    }

    wait();

    while (1) {
      inFlight_t active_master = active_write_master.Pop();

      #pragma hls_pipeline_init_interval 1
      #pragma pipeline_stall_mode flush
      do {
        W_reg = axi_wr_m_w[active_master].Pop();
        axi_wr_s.w.Push(W_reg);
        w_last.Push(W_reg.last.to_uint64());
      } while (W_reg.last != 1);
    }
  }

  void run_b() {
    #pragma hls_unroll yes
    for (int i = 0; i < numMasters; i++) {
      axi_wr_m_b[i].Reset();
    }
    axi_wr_s.b.Reset();
    write_in_flight.ResetRead();
    writeQ.reset();

    typename axi_::WRespPayload B_reg;
    inFlight_t inFlight_reg;
    inFlight_t inFlight_resp_reg;
    bool write_inProgress = 0;

    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      if (!writeQ.isFull()) {
        if (write_in_flight.PopNB(inFlight_reg)) {
          writeQ.push(inFlight_reg);
        }
      }

      if (!write_inProgress) {
        if (!writeQ.isEmpty()) {
          inFlight_resp_reg = writeQ.pop();
          write_inProgress = 1;
        }
      } else {
        if (axiCfg::useWriteResponses) {
          if (axi_wr_s.b.PopNB(B_reg)) {
            axi_wr_m_b[inFlight_resp_reg].Push(B_reg);
            write_inProgress = 0;
          }
        } else {
          write_inProgress = 0;
        }
      }
    }
  }
};

#endif
