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
// TestCombinational.cc
//========================================================================

#include <vector>
#include <systemc.h>
#include <nvhls_connections.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>
#include "TestSource.h"
#include "TestSink.h"

//------------------------------------------------------------------------
// TestHarnessBuffered
//------------------------------------------------------------------------

template< typename T, int W >
class TestHarnessBuffered : public sc_module {
  SC_HAS_PROCESS(TestHarnessBuffered);

 public:
  // Module Interface
  sc_clock              clk;
  sc_signal< bool >     rst;
  TestSourceBuffered<T, W> src;
  TestSinkBuffered<T, W>   sink;

  Connections::Combinational<T> chan;

  TestHarnessBuffered(sc_module_name name, std::vector<T>& src_msgs, std::vector<T>& sink_msgs)
    : sc_module(name),
      clk("clk", 1, SC_NS, 0.5, 0, SC_NS, true),
      rst("rst"),
      src("src", Pacer(0.3, 0.7), src_msgs),
      sink("sink", Pacer(0.2, 0.5), sink_msgs),
      cycle(0)
    {
      src.clk(clk);
      src.rst(rst);
      sink.clk(clk);
      sink.rst(rst);
      src.out(chan);
      sink.in_(chan);

      SC_THREAD(reset);

      SC_METHOD(line_trace);
      sensitive << clk.posedge_event();
    }

    void line_trace() {
      if (rst.read()) {
        std::cout << std::dec << "[" << std::setw(3) << cycle++ << "] ";
        src.line_trace();
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
  typedef sc_lv<32> Bits;
  static const int kBufferSize = 2;
  static const unsigned int MAX_COUNT = 100;

  std::vector<Bits> src_msgs;
  std::vector<Bits> sink_msgs;
  for (unsigned i = 0; i < MAX_COUNT; ++i) {
    src_msgs.push_back(i);
    sink_msgs.push_back(i);
  }

  TestHarnessBuffered<Bits, kBufferSize> test("test_blocking", src_msgs, sink_msgs);
  sc_start();
  return 0;
}
