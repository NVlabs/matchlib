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
#ifndef __AXIADDWRITERESPONSE_H__
#define __AXIADDWRITERESPONSE_H__

#include <systemc.h>
#include <nvhls_connections.h>
#include <nvhls_marshaller.h>
#include <axi/axi4.h>
#include <TypeToBits.h>

/**
 * \brief A simple shim that converts between two AXI configs by adding write responses.
 * \ingroup AXI
 *
 * \tparam CfgManager                A valid AXI config describing the manager port, with no write responses.
 * \tparam CfgSubordinate                 A valid AXI config describing the subordinate port, with write responses.
 *
 * \par Overview
 * This block converts between an AXI manager that does not use write responses and an AXI subordinate that does use write responses.  Most signals are simply passed through from manager to subordinate.  Write responses generated by the subordinate are received and discarded.
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
 * directive set /path/to/AxiAddWriteResponse/axi_read_ar/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 */
template <typename CfgManager, typename CfgSubordinate>
class AxiAddWriteResponse : public sc_module {
  SC_HAS_PROCESS(AxiAddWriteResponse);
  // Local typedefs and derived constants
  typedef axi::axi4<CfgManager> axiM;
  typedef axi::axi4<CfgSubordinate> axiS;

 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;

  typename axiM::read::template subordinate<> axiM_read;
  typename axiM::write::template subordinate<> axiM_write;
  typename axiS::read::template manager<> axiS_read;
  typename axiS::write::template manager<> axiS_write;

 private:
  // Local state and submodules
  typename axiM::AddrPayload AW;
  typename axiM::WritePayload W;
  typename axiM::AddrPayload AR;
  typename axiS::ReadPayload R;
  Connections::Sink<typename axiS::WRespPayload> B;
  // Ideally we'd remove the B field entirely, but a stub of it still exists,
  // so we need to connect it to something
  Connections::DummySource<typename axiM::WRespPayload> dummyB;

 public:
  // Constructor
  AxiAddWriteResponse(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        axiM_read("axiM_read"),
        axiM_write("axiM_write"),
        axiS_read("axiS_read"),
        axiS_write("axiS_write"),
        B("B"),
        dummyB("dummyB")
  {

    B.clk(clk);
    B.rst(rst);
    B.in(axiS_write.b);

    dummyB.clk(clk);
    dummyB.rst(rst);
    dummyB.out(axiM_write.b);

    SC_THREAD(axi_read_ar);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    SC_THREAD(axi_read_r);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    SC_THREAD(axi_write_aw);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
    
    SC_THREAD(axi_write_w);
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

  void axi_write_aw() {
    axiM_write.aw.Reset();
    axiS_write.aw.Reset();
    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while(1) {
      wait();
      if (axiM_write.aw.PopNB(AW)) {
        axiS_write.aw.Push(BitsToType<typename axiS::AddrPayload>(TypeToBits(AW)));
      }
    }
  }
  
  void axi_write_w() {
    axiM_write.w.Reset();
    axiS_write.w.Reset();
    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while(1) {
      wait();
      if (axiM_write.w.PopNB(W)) {
        axiS_write.w.Push(BitsToType<typename axiS::WritePayload>(TypeToBits(W)));
      }
    }
  }
};


#endif
