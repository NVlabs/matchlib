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
// TestMultChain.cpp
//========================================================================

#include <vector>
#include <systemc.h>
#include <mc_scverify.h>

#include "TestSource.h"
#include "TestSink.h"
#include <nvhls_connections.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>
#include "MultChainTop.h"

//------------------------------------------------------------------------
// MultChainRef
//------------------------------------------------------------------------

template< typename T>
T MultChainRef( T in, unsigned int size) {
  T result = 1;
  for (unsigned int i = 0; i < size; ++i) {
    result = result * in;
  }
  return result;
}

//------------------------------------------------------------------------
// TestHarness
//------------------------------------------------------------------------

template< typename T >
class TestHarness : public sc_module {
  SC_HAS_PROCESS(TestHarness);

 public:
  // Module Interface
  sc_clock                 clk;
  sc_signal< bool >        rst;
  TestSourceBlocking<T>    src;
  TestSinkBlocking<T>      sink;
  CCS_DESIGN(MultChainTop) dut;

  // Wires
  Connections::Combinational<T> src_wire;
  Connections::Combinational<T> sink_wire;

  TestHarness(sc_module_name name, std::vector<T>& src_msgs, std::vector<T>& sink_msgs)
    : sc_module(name),
      clk("clk", 1, SC_NS, 0.5, 0, SC_NS, true),
      rst("rst"),
      src("src", Pacer(0.3, 0.7), src_msgs),
      sink("sink", Pacer(0.5, 0.7), sink_msgs),
      dut("dut"),
      cycle(0)
    {
      src.clk(clk);
      src.rst(rst);

      sink.clk(clk);
      sink.rst(rst);

      dut.clk(clk);
      dut.rst(rst);

      src.out(src_wire);
      dut.in_(src_wire);
      dut.out(sink_wire);
      sink.in_(sink_wire);

      SC_THREAD(reset);

      SC_METHOD(line_trace);
      sensitive << clk.posedge_event();
    }

    void line_trace() {
      if (rst.read()) {
        std::cout << std::dec << "[" << std::setw(3) << cycle++ << "] ";
        src.line_trace();
        #ifndef __SYNTHESIS__
        dut.line_trace();
        #endif
        sink.line_trace();
        std::cout << std::endl;
      }
    }

    void reset() {
      std::cout << "@" << sc_time_stamp() <<" Asserting reset" << std::endl;
      rst.write(false);
      wait( 10, SC_NS );
      rst.write(true);
      std::cout << "@" << sc_time_stamp() <<" De-Asserting reset" << std::endl;
      cycle = 0;
      src.Go();
      sink.Go();
    }

 private:
  unsigned int cycle;
};

//------------------------------------------------------------------------
// sc_main
//------------------------------------------------------------------------

int sc_main(int argc, char* argv[]) {
  nvhls::set_random_seed();
  typedef sc_uint<32> Bits;
  static const unsigned int MAX_COUNT = 100;

  std::vector<Bits> src_msgs;
  std::vector<Bits> sink_msgs;
  for (unsigned i = 0; i < MAX_COUNT; ++i) {
    src_msgs.push_back(i);
    Bits res1 = MultChainRef(i, 2);
    Bits res2 = MultChainRef(res1,2);
    sink_msgs.push_back(res2);
  }

  TestHarness<Bits> test("test", src_msgs, sink_msgs);
  sc_start();
  return 0;
}
