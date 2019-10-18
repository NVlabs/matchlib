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
//========================================================================
// TestCombinationalIntoChan.cc
//========================================================================

#include <vector>
#include <systemc.h>
#include <nvhls_connections.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>
#include "TestSource.h"
#include "TestSink.h"

//------------------------------------------------------------------------
// TestHarnessBlocking
//------------------------------------------------------------------------

template< typename T >
class TestHarnessBlocking : public sc_module {
  SC_HAS_PROCESS(TestHarnessBlocking);

 public:
  // Module Interface
  sc_clock              clk;
  sc_signal< bool >     rst;
  Pacer                 src_pacer;
  TestSinkBlocking<T>   sink;

  Connections::Combinational<T> chan;
  std::vector<T> src_msgs;

  TestHarnessBlocking(sc_module_name name, std::vector<T>& src_msgs_, std::vector<T>& sink_msgs)
    : sc_module(name),
      clk("clk", 1, SC_NS, 0.5, 0, SC_NS, true),
      rst("rst"),
      src_pacer(0.3, 0.7),
      sink("sink", Pacer(0.2, 0.5), sink_msgs),
      src_msgs(src_msgs_),
      cycle(0)
    {
      sink.clk(clk);
      sink.rst(rst);
      sink.in_(chan);

      SC_THREAD(reset);

      SC_METHOD(line_trace);
      sensitive << clk.posedge_event();

      SC_THREAD(src_tick);
      sensitive << clk.posedge_event();
      NVHLS_NEG_RESET_SIGNAL_IS(rst);
    }

    void line_trace() {
      if (rst.read()) {
        std::cout << std::dec << "[" << std::setw(3) << cycle++ << "] ";
        sink.line_trace();
        src_line_trace();
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
      src_Go();
      sink.Go();
    }

 private:
  unsigned int cycle;

  void src_tick() {
    chan.ResetWrite();
    bool done = false;
    src_go = false;
    typename std::vector<T>::iterator it = src_msgs.begin();
    while (!done) {
      if (src_go) {
        chan.Push(*it);
        //cout << "@" << sc_time_stamp() << "\t" << name() << " Push x: " << x << endl;
        ++it;
        if (it == src_msgs.end()) {
          done = true;
          src_go = false;
        }
        wait();
        while (src_pacer.tic()) {
          cout << "@" << sc_time_stamp() << "\t" << name() << " STALL" << endl;
          wait();
        }
      } else {
        wait();
      }
    }
  }

  void src_line_trace() {
#ifndef CONNECTIONS_SIM_ONLY    
    if (rst.read()) {
      unsigned int width = (T().length()/4);
      if (chan.val.read() && chan.rdy.read()) {
        std::cout << sc_time_stamp() << " " << std::hex << std::setw(width) << chan.msg.read();
      }
      else {
        std::cout << sc_time_stamp() << " " << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
#endif    
  }

  void src_Go() {
    src_go = true;
  }
 
  bool src_go;
};

//------------------------------------------------------------------------
// sc_main
//------------------------------------------------------------------------

int sc_main(int argc, char* argv[]) {
  nvhls::set_random_seed();
  typedef sc_lv<32> Bits;
  static const unsigned int MAX_COUNT = 100;

  std::vector<Bits> src_msgs;
  std::vector<Bits> sink_msgs;
  for (unsigned i = 0; i < MAX_COUNT; ++i) {
    src_msgs.push_back(i);
    sink_msgs.push_back(i);
  }

  TestHarnessBlocking<Bits> test("test_blocking", src_msgs, sink_msgs);
  sc_start();
  return 0;
}
