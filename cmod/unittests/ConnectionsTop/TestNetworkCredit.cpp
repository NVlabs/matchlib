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
// TestNetworkCredit.cc
//========================================================================

#include <vector>
#include <systemc.h>
#include <nvhls_connections.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>
#include "TestSource.h"
#include "TestSink.h"

//------------------------------------------------------------------------
// TestHarness
//------------------------------------------------------------------------

template< typename T >
class TestHarness: public sc_module {
  SC_HAS_PROCESS(TestHarness);

 public:
  // Module Interface
  sc_clock              clk;
  sc_signal< bool >     rst;
  TestSourceBlocking<T> src;
  TestSinkBlocking<T>   sink;

  // Constant parameters
  const static unsigned int DestWidthPerHop = 4;
  const static unsigned int MaxHops = 1;
  const static unsigned int PacketIdWidth = 2;
  const static unsigned int CreditWidth = 3;

  Connections::OutNetworkCredit<T,DestWidthPerHop,MaxHops,PacketIdWidth,CreditWidth> enq_net;
  Connections::InNetworkCredit<T,DestWidthPerHop,MaxHops,PacketIdWidth,CreditWidth> deq_net;

  Connections::Combinational<T> enq_chan;
  Connections::Combinational<T> deq_chan;

  Connections::Buffer<Packet<CreditWidth,DestWidthPerHop,MaxHops,PacketIdWidth>, 1> credit_buffer;
  Connections::Buffer<Packet<Wrapped<T>::width,DestWidthPerHop,MaxHops,PacketIdWidth>, 2> packet_buffer;

  Connections::Combinational<Packet<Wrapped<T>::width,DestWidthPerHop,MaxHops,PacketIdWidth> > net_chan_enq;
  Connections::Combinational<Packet<Wrapped<T>::width,DestWidthPerHop,MaxHops,PacketIdWidth> > net_chan_deq;
  Connections::Combinational<Packet<CreditWidth,DestWidthPerHop,MaxHops,PacketIdWidth> >   credit_chan_enq;
  Connections::Combinational<Packet<CreditWidth,DestWidthPerHop,MaxHops,PacketIdWidth> >   credit_chan_deq;

  Connections::Combinational<sc_lv<CreditWidth> >   init_credits;

  Connections::Combinational<sc_lv<DestWidthPerHop*MaxHops> > enq_route_chan;
  Connections::Combinational<sc_lv<PacketIdWidth> >   enq_id_chan;

  Connections::Combinational<sc_lv<DestWidthPerHop*MaxHops> > credit_route_chan;
  Connections::Combinational<sc_lv<PacketIdWidth> >   credit_id_chan;

  TestHarness(sc_module_name name,
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
      credit_buffer("credit_buffer"),
      packet_buffer("packet_buffer"),
      net_chan_enq("net_chan_enq"),
      net_chan_deq("net_chan_deq"),
      credit_chan_enq("credit_chan_enq"),
      credit_chan_deq("credit_chan_deq"),
      init_credits("init_credits"),
      enq_route_chan("enq_route_chan"),
      enq_id_chan("enq_id_chan"),
      credit_route_chan("credit_route_chan"),
      credit_id_chan("credit_id_chan"),
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

      credit_buffer.clk(clk);
      credit_buffer.rst(rst);

      packet_buffer.clk(clk);
      packet_buffer.rst(rst);

      enq_net.route(enq_route_chan);
      enq_net.id(enq_id_chan);

      deq_net.credit_route(credit_route_chan);
      deq_net.credit_id(credit_id_chan);
      deq_net.init_credits(init_credits);

      src.out(enq_chan);
      enq_net.enq(enq_chan);
      enq_net.deq(net_chan_enq);
      enq_net.credit(credit_chan_deq);

      packet_buffer.deq(net_chan_deq);
      packet_buffer.enq(net_chan_enq);

      credit_buffer.deq(credit_chan_deq);
      credit_buffer.enq(credit_chan_enq);

      deq_net.enq(net_chan_deq);
      deq_net.deq(deq_chan);
      deq_net.credit(credit_chan_enq);
      sink.in_(deq_chan);

      SC_CTHREAD(reset, clk);

      SC_METHOD(line_trace);
      sensitive << clk.posedge_event();
    }

    void line_trace() {
      if (rst.read()) {
        std::cout << std::dec << "[" << std::setw(3) << cycle++ << "] ";
        src.line_trace();
        enq_net.line_trace();
        deq_net.line_trace();
        std::cout << std::endl;
      }
    }

    void reset() {
      credit_id_chan.ResetWrite();
      credit_route_chan.ResetWrite();
      enq_id_chan.ResetWrite();
      enq_route_chan.ResetWrite();
      init_credits.ResetWrite();

      std::cout << "@" << sc_time_stamp() <<" Asserting reset" << std::endl;
      rst.write(false);
      wait(10);
      rst.write(true);
      std::cout << "@" << sc_time_stamp() <<" De-Asserting reset" << std::endl;

      cycle = 0;

      sc_lv<PacketIdWidth> set_id = 1;
      sc_lv<DestWidthPerHop*MaxHops> set_route = 3;

      enq_id_chan.Push(set_id);
      enq_route_chan.Push(set_route);

      set_id = 3;
      set_route = 2;
      credit_id_chan.Push(set_id);
      credit_route_chan.Push(set_route);

      init_credits.Push(4);
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
  static const unsigned int MAX_COUNT = 10;

  // Generate source and sink messages
  std::vector<Bits> src_msgs;
  std::vector<Bits> sink_msgs;
  for (unsigned int i = 0; i < MAX_COUNT; ++i ) {
    src_msgs.push_back( i );
    sink_msgs.push_back( i );
  }

  TestHarness<Bits> test("test", src_msgs, sink_msgs);
  sc_start();
  return 0;
}
