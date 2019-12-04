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
#ifndef __AXI_SPLITTER_H__
#define __AXI_SPLITTER_H__

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
 * \brief An n-way splitter that connects a single AXI master port to a multiple AXI slave ports.
 * \ingroup AXI
 *
 * \tparam axiCfg                   A valid AXI config.
 * \tparam numSlaves                The number of slaves to send traffic to.
 * \tparam numAddrBitsToInspect     The number of address bits to inspect when determining which slave to direct traffic to.  If this is less than the full address width, the routing determination will be made based on the number of address LSBs specified.  (Default: axiCfg::addrWidth)
 * \tparam default_output           If true, requests with addresses that do not fall in any of the specified address ranges will be directed to the highest-indexed slave port.  (Default: false)
 * \tparam translate_addr           If true, requests are re-addressed relative to the base address of the receiving slave when they are passed through the splitter.  (Default: false)
 *
 * \par Overview
 * AxiSplitter connects one or more AXI slaves to a single AXI master.  Requests from the master are routed by address to the appropriate slave.
 * - The address ranges for each slave must be contiguous (except for the highest-indexed slave if default_output is true).  Address bounds for each slave are set by writing to a (numSlaves x 2) array of sc_in.
 * - Only a single outstanding request to all slaves is allowed; AxiSplitter blocks further requests until the response has been returned.
 * - As implemented, the splitter directs all writes from a burst to the destination indicated by the base address of the burst.  Guards against crossing address boundaries are not implemented.
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
 * directive set /path/to/AxiSplitter/run_r/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <typename axiCfg, int numSlaves, int numAddrBitsToInspect = axiCfg::addrWidth, bool default_output = false, bool translate_addr = false>
class AxiSplitter : public sc_module {
 public:
  static const int kDebugLevel = 5;
  sc_in<bool> clk;
  sc_in<bool> reset_bar;

  typedef typename axi::axi4<axiCfg> axi4_;

  typedef typename axi4_::read::template master<>::ARPort axi_rd_master_ar;
  typedef typename axi4_::read::template master<>::RPort axi_rd_master_r;
  typedef typename axi4_::write::template master<>::AWPort axi_wr_master_aw;
  typedef typename axi4_::write::template master<>::WPort axi_wr_master_w;
  typedef typename axi4_::write::template master<>::BPort axi_wr_master_b;

  static const unsigned int log_numSlaves = nvhls::log2_ceil<numSlaves>::val + 1;

  // [ben] Unfortunately HLS cannot handle an nv_array of the master/slave wrapper classes.
  // It will work fine in C but die mysteriously in Catapult 10.1b when methods of the
  // bundled connections are accessed.
  nvhls::nv_array<axi_rd_master_ar, numSlaves> axi_rd_s_ar;
  nvhls::nv_array<axi_rd_master_r, numSlaves> axi_rd_s_r;
  nvhls::nv_array<axi_wr_master_aw, numSlaves> axi_wr_s_aw;
  nvhls::nv_array<axi_wr_master_w, numSlaves> axi_wr_s_w;
  nvhls::nv_array<axi_wr_master_b, numSlaves> axi_wr_s_b;
  typename axi4_::read::template slave<> axi_rd_m;
  typename axi4_::write::template slave<> axi_wr_m;

  sc_in<NVUINTW(numAddrBitsToInspect)> addrBound[numSlaves][2];

  SC_HAS_PROCESS(AxiSplitter);

  AxiSplitter(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        reset_bar("reset_bar"),
        axi_rd_s_ar("axi_rd_s_ar"),
        axi_rd_s_r("axi_rd_s_r"),
        axi_wr_s_aw("axi_wr_s_aw"),
        axi_wr_s_w("axi_wr_s_w"),
        axi_wr_s_b("axi_wr_s_b"),
        axi_rd_m("axi_rd_m"),
        axi_wr_m("axi_wr_m")
    {
    
    SC_THREAD(run_r);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
    
    SC_THREAD(run_w);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

  // As an optimization, responses with different IDs could be allowed
  // through, since it is safe to return them out of order.

  void run_r() {
#pragma hls_unroll yes
    for (int i=0; i<numSlaves; i++) {
      axi_rd_s_ar[i].Reset();
      axi_rd_s_r[i].Reset();
    }
    axi_rd_m.ar.Reset();
    axi_rd_m.r.Reset();

    typename axi4_::AddrPayload AR_reg;
    typename axi4_::ReadPayload R_reg;

    bool read_inFlight = 0;
    NVUINTW(log_numSlaves) pushedTo = numSlaves;
    
      #pragma hls_pipeline_init_interval 1
      #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      switch (read_inFlight) {
        case false:
          if (axi_rd_m.ar.PopNB(AR_reg)) {
            NVUINTW(numAddrBitsToInspect)
                addr(static_cast<sc_uint<numAddrBitsToInspect> >(AR_reg.addr)); // Cast larger to smaller
            pushedTo = numSlaves;
            // TODO - refactor this so it can be unrolled
            for (int i=0; i<numSlaves; i++) {
              if (addr >= addrBound[i][0].read() && addr <= addrBound[i][1].read() && pushedTo == numSlaves) {
                pushedTo = i;
              }
            }
            if (default_output && pushedTo == numSlaves) {
              pushedTo = numSlaves-1;
            }
            // If the address did not fall in any valid range, that's bad
            NVHLS_ASSERT_MSG(pushedTo != numSlaves, "Read_address_did_not_fall_into_any_output_address_range,_and_default_output_is_not_set");

            if (translate_addr)
              AR_reg.addr -= addrBound[pushedTo][0].read();

            axi_rd_s_ar[pushedTo].Push(AR_reg);
            read_inFlight = 1;
          }
          break;
        case true:
          if (axi_rd_s_r[pushedTo].PopNB(R_reg)) {
            axi_rd_m.r.Push(R_reg);
            if (R_reg.last == 1) read_inFlight = 0;
          }
          break;
      }
    }
  }

  void run_w() {
#pragma hls_unroll yes
    for (int i=0; i<numSlaves; i++) {
      axi_wr_s_aw[i].Reset();
      axi_wr_s_w[i].Reset();
      axi_wr_s_b[i].Reset();
    }
    axi_wr_m.aw.Reset();
    axi_wr_m.w.Reset();
    axi_wr_m.b.Reset();

    typename axi4_::AddrPayload AW_reg;
    typename axi4_::WritePayload W_reg;
    typename axi4_::WRespPayload B_reg;
    
    NVUINTW(log_numSlaves) pushedTo = numSlaves;
    enum {
      IDLE = 0,
      WRITE_INFLIGHT = 1,
      RESP_INFLIGHT = 2,
    };
    NVUINT2 s = IDLE;
    
      #pragma hls_pipeline_init_interval 1
      #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      switch (s) {
        case IDLE:
          if (axi_wr_m.aw.PopNB(AW_reg)) {
            NVUINTW(numAddrBitsToInspect)
                addr(static_cast<sc_uint<numAddrBitsToInspect> >(AW_reg.addr)); // Cast larger to smaller
            pushedTo = numSlaves;
            // TODO - refactor this so it can be unrolled
            for (int i=0; i<numSlaves; i++) {
              if (addr >= addrBound[i][0].read() && addr <= addrBound[i][1].read() && pushedTo == numSlaves) {
                pushedTo = i;
              }
            }
            if (default_output && pushedTo == numSlaves) {
              pushedTo = numSlaves-1;
            }
            NVHLS_ASSERT_MSG(pushedTo != numSlaves, "Write_address_did_not_fall_into_any_output_address_range,_and_default_output_is_not_set");
            if (translate_addr)
              AW_reg.addr -= addrBound[pushedTo][0].read();

            axi_wr_s_aw[pushedTo].Push(AW_reg);
            s = WRITE_INFLIGHT;
          }
          break;
        case WRITE_INFLIGHT:
          NVHLS_ASSERT_MSG(pushedTo != numSlaves, "Write_address_did_not_fall_into_any_output_address_range,_and_default_output_is_not_set");
          if (axi_wr_m.w.PopNB(W_reg)) {
            axi_wr_s_w[pushedTo].Push(W_reg);
            if (W_reg.last == 1) {
              if (axiCfg::useWriteResponses) {
                s = RESP_INFLIGHT;
              } else {
                s = IDLE;
              }
            }
          }
          break;
        case RESP_INFLIGHT:
          if (axiCfg::useWriteResponses) {
            if (axi_wr_s_b[pushedTo].PopNB(B_reg)) {
              axi_wr_m.b.Push(B_reg);
              s = IDLE;
            }
          } 
          else {
            NVHLS_ASSERT_MSG(0, "Should_never_reach_this_state_if_write_responses_are_disabled");
          }
          break;
      }
    }
  }
};

#endif
