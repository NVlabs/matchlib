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
// TestNetwork.cpp
//========================================================================

#include <vector>
#include <systemc.h>
#include "TestSource.h"
#include "TestSink.h"
#include <nvhls_connections.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>


//------------------------------------------------------------------------
// TestHarnessNetworkBlocking
//------------------------------------------------------------------------

template< typename T >
class TestHarnessNetworkBlocking : public sc_module {
  SC_HAS_PROCESS(TestHarnessNetworkBlocking);

 public:
  // Module Interface
  sc_clock              clk;
  sc_signal< bool >     rst;
  TestSourceBlocking<T> src;
  TestSinkBlocking<T>   sink;

  Connections::OutNetwork<T,4,1,2> enq_net;
  Connections::InNetwork<T,4,1,2>  deq_net;

  Connections::Combinational<T> enq_chan;
  Connections::Combinational<T> deq_chan;

  Connections::Combinational< Packet<Wrapped<T>::width,4,1,2> > net_chan;

  Connections::Out< sc_lv< 4*1 > > route;
  Connections::Out< sc_lv< 2 > >   id;

  Connections::Combinational< sc_lv< 4*1 > > route_chan;
  Connections::Combinational< sc_lv< 2 > >   id_chan;

  TestHarnessNetworkBlocking(sc_module_name name,
                             std::vector<T>& src_msgs,
                             std::vector<T>& sink_msgs)
    : sc_module(name),
      clk("clk", 1, SC_NS, 0.5, 0, SC_NS, true),
      rst("rst"),
      src("src", Pacer(0.3, 0.7), src_msgs),
      sink("sink", Pacer(0.5, 0.7), sink_msgs),
      enq_net("enq_net"),
      deq_net("deq_net"),
      enq_chan("enq_chan"),
      deq_chan("deq_chan"),
      net_chan("net_chan"),
      route("route"),
      id("id"),
      route_chan("route_chan"),
      id_chan("id_chan"),
      cycle(0)
    {
      src.clk(clk);
      src.rst(rst);

      sink.clk(clk);
      sink.rst(rst);

      enq_net.clk(clk);
      enq_net.rst(rst);

      deq_net.clk(clk);
      deq_net.rst(rst);

      route(route_chan);
      enq_net.route(route_chan);
      id(id_chan);
      enq_net.id(id_chan);

      src.out(enq_chan);
      enq_net.enq(enq_chan);
      enq_net.deq(net_chan);
      deq_net.enq(net_chan);
      deq_net.deq(deq_chan);
      sink.in_(deq_chan);

      SC_THREAD(reset);

      SC_METHOD(line_trace);
      sensitive << clk.posedge_event();
    }

    void line_trace() {
      if (rst.read()) {
        std::cout << std::dec << "[" << std::setw(3) << cycle++ << "] ";
        src.line_trace();
        enq_net.line_trace();
        deq_net.line_trace();
        sink.line_trace();
        std::cout << std::endl;
      }
    }

    void reset() {
      id.Reset();
      route.Reset();
      route_chan.ResetWrite();
      id_chan.ResetWrite();
      std::cout << "@" << sc_time_stamp() <<" Asserting reset" << std::endl;
      rst.write(false);
      wait( 10, SC_NS );
      rst.write(true);
      std::cout << "@" << sc_time_stamp() <<" De-Asserting reset" << std::endl;

      cycle = 0;

      src.Go();
      sink.Go();
      sc_lv<2> set_id = 1;
      sc_lv<4> set_route = 3;

      id.Push(set_id);
      route.Push(set_route);

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
  static const unsigned int MAX_COUNT = 100;

  // Generate source and sink messages
  std::vector<Bits> src_msgs;
  std::vector<Bits> sink_msgs;
  for (unsigned int i = 0; i < MAX_COUNT; ++i ) {
    src_msgs.push_back( i );
    sink_msgs.push_back( i );
  }

  TestHarnessNetworkBlocking<Bits> test("test_net_blocking", src_msgs, sink_msgs);
  sc_start();
  return 0;
}
