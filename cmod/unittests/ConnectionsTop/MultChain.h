/*
 * Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
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
//========================================================================
// MultChain.h
//========================================================================
// Chain of multiply oeprations to test the auto-pipelining with
// nvhls_connections. NOTE: The multiplier chain truncates the result to
// only the lower N bits.

#ifndef MULT_CHAIN_H
#define MULT_CHAIN_H

#include <systemc.h>
#include <nvhls_connections.h>

template< typename T, unsigned int N>
class MultChain : public sc_module {
  SC_HAS_PROCESS(MultChain);

 public:
  // Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::In<T>  in_;
  Connections::Out<T> out;

  MultChain(sc_module_name name) :
    sc_module(name),
    clk(nvhls_concat(name, "clk")),
    rst(nvhls_concat(name, "rst")),
    in_("in_"),
    out("out")
  {
    SC_CTHREAD(Tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

protected:
  void Tick() {
    in_.Reset();
    out.Reset();

    wait();
    
    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      T result = 1;
      T in_reg = in_.Pop();
      #pragma hls_pipeline_init_interval 1
      #pragma pipeline_stall_mode flush
      for (unsigned int i = 0; i < N; ++i) {
        result = result * in_reg;
      }
      out.Push(result);
    }
  }

#ifndef __SYNTHESIS__
public:
  void line_trace() {
#ifndef CONNECTIONS_FAST_SIM    
    if (rst.read()) {
      unsigned int width = (T().length()/4);
      // Input port
      if (in_.val.read() && in_.rdy.read()) {
        std::cout << std::hex << std::setw(width) << in_.msg.read();
      }
      else {
        std::cout << std::setw(width+1) << " ";
      }
      std::cout << " | ";

      // Output port
      if (out.val.read() && out.rdy.read()) {
        std::cout << std::hex << std::setw(width) << out.msg.read();
      }
      else {
        std::cout << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
#endif    
  }
#endif
};

#endif  // MULT_CHAIN_H
