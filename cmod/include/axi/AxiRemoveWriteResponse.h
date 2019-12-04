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
#ifndef __AXIREMOVEWRITERESPONSE_H__
#define __AXIREMOVEWRITERESPONSE_H__

#include <systemc.h>
#include <nvhls_connections.h>
#include <nvhls_marshaller.h>
#include <nvhls_assert.h>
#include <axi/axi4.h>
#include <fifo.h>
#include <TypeToBits.h>

/**
 * \brief A simple shim that converts between two AXI configs by removing write responses.
 * \ingroup AXI
 *
 * \tparam CfgMaster                A valid AXI config describing the master port, with write responses.
 * \tparam CfgSlave                 A valid AXI config describing the slave port, with no write responses.
 *
 * \par Overview
 * This block converts between an AXI master that uses write responses and an AXI slave that does not use write responses.  Most signals are simply passed through from master to slave.  When a write request is received from the master, it is passed through to the slave, and a write response is also sent back to the master.
 * - Apart from support for write responses, the two AXI configs must otherwise be the same.
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/AxiRemoveWriteResponse/axi_read_ar/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <typename CfgMaster, typename CfgSlave, int maxInFlight>
class AxiRemoveWriteResponse : public sc_module {
  static const int kDebugLevel = 6;
  SC_HAS_PROCESS(AxiRemoveWriteResponse);
  // Local typedefs and derived constants
  typedef axi::axi4<CfgMaster> axiM;
  typedef axi::axi4<CfgSlave> axiS;

  enum { maxInFlight_width = nvhls::log2_ceil<maxInFlight>::val };

 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;

  typename axiM::read::template slave<> axiM_read;
  typename axiM::write::template slave<> axiM_write;
  typename axiS::read::template master<> axiS_read;
  typename axiS::write::template master<> axiS_write;

 private:
  // Local state and submodules
  typename axiM::AddrPayload AW;
  typename axiM::WritePayload W;
  typename axiM::AddrPayload AR;
  typename axiS::ReadPayload R;
  typename axiM::WRespPayload B;

  // Ideally we'd remove the B field entirely, but a stub of it still exists,
  // so we need to connect it to something
  Connections::DummySink<typename axiS::WRespPayload> dummyB;

  FIFO<typename axiM::BId, maxInFlight> wresp_id_q;
  FIFO<typename axiM::BId, maxInFlight> wresp_id_q_out;

 public:
  // Constructor
  AxiRemoveWriteResponse(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        axiM_read("axiM_read"),
        axiM_write("axiM_write"),
        axiS_read("axiS_read"),
        axiS_write("axiS_write"),
        dummyB("dummyB")
  {

    dummyB.clk(clk);
    dummyB.rst(rst);
    dummyB.in(axiS_write.b);

    SC_THREAD(axi_read_ar);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    SC_THREAD(axi_read_r);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    SC_THREAD(axi_write);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

 private:
  void axi_read_ar() {
    axiM_read.ar.Reset();
    axiS_read.ar.Reset();
    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while(1) {
      wait();
      if (axiM_read.ar.PopNB(AR)) {
        axiS_read.ar.Push(BitsToType<typename axiS::AddrPayload>(TypeToBits(AR)));
      }
    }
  }

  void axi_read_r() {
    axiM_read.r.Reset();
    axiS_read.r.Reset();
    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while(1) {
      wait();
      if (axiS_read.r.PopNB(R)) {
        axiM_read.r.Push(BitsToType<typename axiM::ReadPayload>(TypeToBits(R)));
      }
    }
  }

  void axi_write() {
    B.resp = axiM::Enc::XRESP::OKAY; // Never return an error response

    axiM_write.reset();
    axiS_write.w.Reset();
    axiS_write.aw.Reset();
    wresp_id_q.reset();
    wresp_id_q_out.reset();

    bool wresp_id_q_full = 0;
    bool wresp_id_q_empty = 0;
    bool wresp_id_q_out_full = 0;
    bool wresp_id_q_out_empty = 0;

    #pragma hls_pipeline_init_interval 2
    #pragma pipeline_stall_mode flush
    while(1) {
      wait();

      wresp_id_q_full = wresp_id_q.isFull();
      wresp_id_q_empty = wresp_id_q.isEmpty();
      wresp_id_q_out_full = wresp_id_q_out.isFull();
      wresp_id_q_out_empty = wresp_id_q_out.isEmpty();

      if (!wresp_id_q_full) {
        if (axiM_write.aw.PopNB(AW)) {
          axiS_write.aw.Push(BitsToType<typename axiS::AddrPayload>(TypeToBits(AW)));
          wresp_id_q.push(AW.id);
        }
      }

      if (!wresp_id_q_empty && !wresp_id_q_out_full) {
        if (axiM_write.w.PopNB(W)) {
          axiS_write.w.Push(BitsToType<typename axiS::WritePayload>(TypeToBits(W)));
          if (W.last == 1) wresp_id_q_out.push(wresp_id_q.pop());
        }
      }

      if (!wresp_id_q_out_empty) {
        B.id = wresp_id_q_out.pop();
        axiM_write.b.Push(B);
      }
    }
  }
};


#endif
