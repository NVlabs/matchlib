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
// nvhls_connections_buffered_ports.h
//========================================================================

#ifndef NVHLS_CONNECTIONS_BUFFERED_PORTS_H_
#define NVHLS_CONNECTIONS_BUFFERED_PORTS_H_

#include <systemc.h>
#include <nvhls_assert.h>
#include <nvhls_marshaller.h>
#include <nvhls_module.h>
#include <fifo.h>
#include <ccs_p2p.h>

namespace Connections {

template <typename Message, int BufferSize = 1, connections_port_t port_marshall_type = AUTO_PORT>
class InBuffered : public InBlocking<Message, port_marshall_type> {
  FIFO<Message, BufferSize> fifo;

 public:
   InBuffered() : InBlocking<Message, port_marshall_type>(), fifo() {}

  explicit InBuffered(const char* name) : InBlocking<Message, port_marshall_type>(name), fifo() {}

  void Reset() {
    InBlocking<Message,port_marshall_type>::Reset();
    fifo.reset();
  }

  // Empty
  bool Empty() { return fifo.isEmpty(); }

  Message Pop() { return fifo.pop(); }

  void IncrHead() { fifo.incrHead(); }

  Message Peek() { return fifo.peek(); }

  void TransferNB() {

    if (!fifo.isFull()) {
      Message msg;
      if (this->PopNB(msg)) {
        fifo.push(msg);
      }
    }
  }
};

template <typename Message, int BufferSize = 1, connections_port_t port_marshall_type = AUTO_PORT>
class OutBuffered : public OutBlocking<Message, port_marshall_type> {
  FIFO<Message, BufferSize> fifo;
  typedef NVUINTW(nvhls::index_width<BufferSize+1>::val) AddressPlusOne;
 public:
  OutBuffered() : OutBlocking<Message, port_marshall_type>(), fifo() {}

  explicit OutBuffered(const char* name) : OutBlocking<Message, port_marshall_type>(name), fifo() {}

  void Reset() {
    OutBlocking<Message,port_marshall_type>::Reset();
    fifo.reset();
  }

  // Full
  bool Full() { return fifo.isFull(); }
  // Empty
  bool Empty() { return fifo.isEmpty(); }

  AddressPlusOne NumAvailable() { return fifo.NumAvailable(); }

  void Push(const Message& msg) { fifo.push(msg); }

  void TransferNB() {
    if (!fifo.isEmpty()) {
      Message msg = fifo.peek();
      if (this->PushNB(msg)) {
        fifo.pop();
      }
    }
  }
};


//------------------------------------------------------------------------
// Helper class for Bypass and Pipeline
//------------------------------------------------------------------------
 
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class StateSignal;

template <typename Message>
class StateSignal<Message, SYN_PORT> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_signal<MsgBits> msg;
  
 StateSignal() : msg(sc_gen_unique_name("msg")) {}

 StateSignal(sc_module_name name) : msg(CONNECTIONS_CONCAT(name, "_msg")) { }
  
  void reset_state() {
    msg.write(0);
  }
};
 
template <typename Message>
class StateSignal<Message, MARSHALL_PORT> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_signal<MsgBits> msg;
  
 StateSignal() : msg(sc_gen_unique_name("msg")) {}

 StateSignal(sc_module_name name) : msg(CONNECTIONS_CONCAT(name, "_msg")) { }
  
  void reset_state() {
    msg.write(0);
  }
};

template <typename Message>
class StateSignal<Message, DIRECT_PORT> {
 public:
  // Interface
  sc_signal<Message> msg;
  
 StateSignal() : msg(sc_gen_unique_name("msg")) {}

 StateSignal(sc_module_name name) : msg(CONNECTIONS_CONCAT(name, "_msg")) { }

  void reset_state() {
    Message dc;
    msg.write(dc);
  }
};

// Because of ports not existing in TLM_PORT and the code depending on it,
// we remap to DIRECT_PORT here.
template <typename Message>
class StateSignal<Message, TLM_PORT> : public StateSignal<Message, DIRECT_PORT>
{
 public:
 StateSignal() : StateSignal<Message, DIRECT_PORT>() {}
 StateSignal(sc_module_name name) : StateSignal<Message, DIRECT_PORT>(name) {}
};
 
//------------------------------------------------------------------------
// Bypass
//------------------------------------------------------------------------

template <typename Message, connections_port_t port_marshall_type>
class Bypass : public sc_module {
  SC_HAS_PROCESS(Bypass);

 public:
  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message, port_marshall_type> enq;
  Out<Message, port_marshall_type> deq;

  Bypass()
      : sc_module(sc_module_name(sc_gen_unique_name("byp"))),
        clk("clk"),
        rst("rst") {
    Init();
  }

  Bypass(sc_module_name name) : sc_module(name), clk("clk"), rst("rst") {
    Init();
  }

 protected:
  typedef bool Bit;

  // Internal wires
  sc_signal<Bit> wen;

  // Internal state
  sc_signal<Bit> full;
  StateSignal<Message, port_marshall_type> state;

  // Helper functions
  void Init() {
#ifdef CONNECTIONS_SIM_ONLY
    enq.disable_spawn();
    deq.disable_spawn();
#endif
    
    SC_METHOD(EnqRdy);
    sensitive << full;

    SC_METHOD(DeqVal);
    sensitive << enq.val << full;

    SC_METHOD(WriteEn);
    sensitive << enq.val << deq.rdy << full;

    SC_METHOD(BypassMux);
    sensitive << enq.msg << state.msg << full;

    SC_THREAD(Seq);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  // Combinational logic

  // Write enable
  void WriteEn() {
    wen.write(enq.val.read() && !deq.rdy.read() && !full.read());
  }

  // Enqueue ready
  void EnqRdy() { enq.rdy.write(!full.read()); }

  // Dequeue valid
  void DeqVal() {
    deq.val.write(full.read() || (!full.read() && enq.val.read()));
  }

  // Bypass mux
  void BypassMux() {
    if (full.read()) {
      deq.msg.write(state.msg.read());
    } else {
      deq.msg.write(enq.msg.read());
    }
  }

  // Sequential logic
  void Seq() {
    // Reset state
    full.write(0);
    state.msg.write(0);

    wait();

    while (1) {
      // Full update
      if (deq.rdy.read()) {
        full.write(false);
      } else if (enq.val.read() && !deq.rdy.read() && !full.read()) {
        full.write(true);
      }

      // State Update
      if (wen.read()) {
        state.msg.write(enq.msg.read());
      }
      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int width = (Message().length() / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << enq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }

      std::cout << " ( " << full.read() << " ) ";

      // Dequeue port
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << deq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};
 
// Because of ports not existing in TLM_PORT and the code depending on it,
// we remap to DIRECT_PORT here.
template <typename Message>
class Bypass<Message, TLM_PORT> : public Bypass<Message, DIRECT_PORT>
{
 public:
 Bypass() : Bypass<Message, DIRECT_PORT>() {}
 Bypass(sc_module_name name) : Bypass<Message, DIRECT_PORT>(name) {}
};
 
//------------------------------------------------------------------------
// Pipeline
//------------------------------------------------------------------------

template <typename Message, connections_port_t port_marshall_type>
class Pipeline : public sc_module {
  SC_HAS_PROCESS(Pipeline);

 public:
  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message, port_marshall_type> enq;
  Out<Message, port_marshall_type> deq;

  Pipeline()
      : sc_module(sc_module_name(sc_gen_unique_name("byp"))),
        clk("clk"),
        rst("rst") {
    Init();
  }

  Pipeline(sc_module_name name) : sc_module(name), clk("clk"), rst("rst") {
    Init();
  }

 protected:
  typedef bool Bit;

  // Internal wires
  sc_signal<Bit> wen;

  // Internal state
  sc_signal<Bit> full;
  StateSignal<Message, port_marshall_type> state;

  // Helper functions
  void Init() {
#ifdef CONNECTIONS_SIM_ONLY
    enq.disable_spawn();
    deq.disable_spawn();
#endif
    
    SC_METHOD(EnqRdy);
    sensitive << full << deq.rdy;

    SC_METHOD(DeqVal);
    sensitive << full;

    SC_METHOD(WriteEn);
    sensitive << enq.val << deq.rdy << full;

    SC_METHOD(DeqMsg);
    sensitive << state.msg;

    SC_THREAD(Seq);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  // Combinational logic

  // Internal state write enable: incoming msg is valid and (internal state is
  // not set or outgoing channel is ready.
  void WriteEn() {
    wen.write(enq.val.read() && (!full.read() || (full && deq.rdy.read())));
  }

  // Enqueue ready if either internal state is not set or outgoing channel is
  // ready.
  void EnqRdy() { enq.rdy.write(!full.read() || (full && deq.rdy.read())); }

  // Dequeue valid if the internal state is set.
  void DeqVal() { deq.val.write(full.read()); }

  // Dequeue Msg is from the internal state.
  void DeqMsg() { deq.msg.write(state.msg.read()); }

  // Sequential logic
  void Seq() {
    // Reset state
    full.write(0);
    state.reset_state();

    wait();

    while (1) {
      // Full update
      if (full.read() && deq.rdy.read() && !enq.val.read()) {
        full.write(false);
      } else if (enq.val.read() &&
                 (!full.read() || (full.read() && deq.rdy.read()))) {
        full.write(true);
      }

      // State Update
      if (wen.read()) {
        state.msg.write(enq.msg.read());
      }
      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int width = (Message().length() / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << enq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }

      std::cout << " ( " << full.read() << " ) ";

      // Dequeue port
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << deq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

// Because of ports not existing in TLM_PORT and the code depending on it,
// we remap to DIRECT_PORT here.
template <typename Message>
class Pipeline<Message, TLM_PORT> : public Pipeline<Message, DIRECT_PORT>
{
 public:
 Pipeline() : Pipeline<Message, DIRECT_PORT>() {}
 Pipeline(sc_module_name name) : Pipeline<Message, DIRECT_PORT>(name) {}
};

 
//
// NEW FEATURE: Buffered Bypass Channel.
// This is a BypassBuffered channel that can have depth > 1.
// W.r.t the Bypass it takes one more template parameter.
// TODO: It may also work with depth = 1, but it hasn't been tested.

//------------------------------------------------------------------------
// BypassBuffered
//------------------------------------------------------------------------

template <typename Message, unsigned int NumEntries, connections_port_t port_marshall_type = AUTO_PORT>
class BypassBuffered : public sc_module {
  SC_HAS_PROCESS(BypassBuffered);

 public:
  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message, port_marshall_type> enq;
  Out<Message, port_marshall_type> deq;

  BypassBuffered()
      : sc_module(sc_module_name(sc_gen_unique_name("byp"))),
        clk("clk"),
        rst("rst") {
    Init();
  }

  BypassBuffered(sc_module_name name) : sc_module(name), clk("clk"), rst("rst") {
    Init();
  }

 protected:
  typedef bool Bit;
  static const int AddrWidth = nvhls::nbits<NumEntries - 1>::val;
  typedef NVUINTW(AddrWidth) BuffIdx;

  // Internal wires
  sc_signal<Bit> full_next;
  sc_signal<BuffIdx> head_next;
  sc_signal<BuffIdx> tail_next;

  // Internal state
  sc_signal<Bit> full;
  sc_signal<BuffIdx> head;
  sc_signal<BuffIdx> tail;
  StateSignal<Message, port_marshall_type> buffer[NumEntries];

  // Helper functions
  void Init() {
#ifdef CONNECTIONS_SIM_ONLY
    enq.disable_spawn();
    deq.disable_spawn();
#endif
    
    SC_METHOD(EnqRdy);
    sensitive << full;

    SC_METHOD(DeqVal);
    sensitive << full << head << tail << enq.val;

    SC_METHOD(DeqMsg);
#ifndef __SYNTHESIS__
    sensitive << deq.rdy << full << head << tail << enq.val << enq.msg;
#else
    sensitive << full << head << tail << enq.msg;
#endif

    SC_METHOD(HeadNext);
    sensitive << enq.val << full << head << tail << deq.rdy;

    SC_METHOD(TailNext);
    sensitive << deq.rdy << full << head << tail;

    SC_METHOD(FullNext);
    sensitive << enq.val << deq.rdy << full << head << tail;

    SC_THREAD(Seq);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    // Needed so that DeqMsg always has a good tail value
    tail.write(0);
  }

  // Combinational logic

  // Enqueue ready
  void EnqRdy() { enq.rdy.write(!full.read()); }

  // Dequeue valid
  void DeqVal() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    deq.val.write(!empty || enq.val.read());
  }

  // Dequeue messsage
  void DeqMsg() {
    bool empty = (!full.read() && (head.read() == tail.read()));
#ifndef __SYNTHESIS__
    bool do_deq = !empty || enq.val.read();
    if (do_deq) {
#endif
      if (!empty) {
	deq.msg.write(buffer[tail.read()].msg.read());
      } else {
	deq.msg.write(enq.msg.read());
      }
#ifndef __SYNTHESIS__
    } else {
      deq.msg.write(0);
    }
#endif
  }

  // Head next calculations
  void HeadNext() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    bool do_enq = (enq.val.read() && !full.read() &&
		   !(empty && deq.rdy.read()));
    BuffIdx head_inc;
    if ((head.read() + 1) == NumEntries)
      head_inc = 0;
    else
      head_inc = head.read() + 1;

    if (do_enq)
      head_next.write(head_inc);
    else
      head_next.write(head.read());
  }

  // Tail next calculations
  void TailNext() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    bool do_deq = (deq.rdy.read() && !empty);
    BuffIdx tail_inc;
    if ((tail.read() + 1) == NumEntries)
      tail_inc = 0;
    else
      tail_inc = tail.read() + 1;

    if (do_deq)
      tail_next.write(tail_inc);
    else
      tail_next.write(tail.read());
  }

  // Full next calculations
  void FullNext() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    bool do_enq = (enq.val.read() && !full.read() &&
		   !(empty && deq.rdy.read()));
    bool do_deq = (deq.rdy.read() && !empty);

    BuffIdx head_inc;
    if ((head.read() + 1) == NumEntries)
      head_inc = 0;
    else
      head_inc = head.read() + 1;

    if (do_enq && !do_deq && (head_inc == tail.read()))
      full_next.write(1);
    else if (do_deq && full.read())
      full_next.write(0);
    else
      full_next.write(full.read());
  }

  // Sequential logic
  void Seq() {
    // Reset state
    full.write(0);
    head.write(0);
    tail.write(0);
#pragma hls_unroll yes
    for (unsigned int i = 0; i < NumEntries; ++i)
      buffer[i].msg.reset_state();

    wait();

    while (1) {

      // Head update
      head.write(head_next);

      // Tail update
      tail.write(tail_next);

      // Full update
      full.write(full_next);

      // Enqueue message
      bool empty = (!full.read() && (head.read() == tail.read()));

      if (enq.val.read() && !full.read() &&
	  !(empty && deq.rdy.read())) {
        buffer[head.read()].msg.write(enq.msg.read());
      }

      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int width = (Message().length() / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << enq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }

      std::cout << " ( " << full.read() << " ) ";

      // Dequeue port
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << deq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

// Because of ports not existing in TLM_PORT and the code depending on it,
// we remap to DIRECT_PORT here.
template <typename Message, unsigned int NumEntries>
class BypassBuffered<Message, NumEntries, TLM_PORT> : public BypassBuffered<Message, NumEntries, DIRECT_PORT>
{
 public:
 BypassBuffered() : BypassBuffered<Message, NumEntries, DIRECT_PORT>() {}
 BypassBuffered(sc_module_name name) : BypassBuffered<Message, NumEntries, DIRECT_PORT>(name) {}
};

 
//------------------------------------------------------------------------
// Buffer
//------------------------------------------------------------------------

template <typename Message, unsigned int NumEntries, connections_port_t port_marshall_type>
class Buffer : public sc_module {
  SC_HAS_PROCESS(Buffer);

 public:
  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message, port_marshall_type> enq;
  Out<Message, port_marshall_type> deq;

  Buffer()
      : sc_module(sc_module_name(sc_gen_unique_name("buffer"))),
        clk("clk"),
        rst("rst") {
    Init();
  }

  Buffer(sc_module_name name) : sc_module(name), clk("clk"), rst("rst") {
    Init();
  }

 protected:
  typedef bool Bit;
  static const int AddrWidth = nvhls::index_width<NumEntries>::val;
  typedef NVUINTW(AddrWidth) BuffIdx;

  // Internal wires
  sc_signal<Bit> full_next;
  sc_signal<BuffIdx> head_next;
  sc_signal<BuffIdx> tail_next;

  // Internal state
  sc_signal<Bit> full;
  sc_signal<BuffIdx> head;
  sc_signal<BuffIdx> tail;
  StateSignal<Message, port_marshall_type> buffer[NumEntries];

  // Helper functions
  void Init() {
#ifdef CONNECTIONS_SIM_ONLY
    enq.disable_spawn();
    deq.disable_spawn();
#endif
    
    SC_METHOD(EnqRdy);
    sensitive << full;

    SC_METHOD(DeqVal);
    sensitive << full << head << tail;

    SC_METHOD(DeqMsg);
#ifndef __SYNTHESIS__
    sensitive << deq.rdy << full << head << tail;
#else
    sensitive << tail;
#endif

    SC_METHOD(HeadNext);
    sensitive << enq.val << full << head;

    SC_METHOD(TailNext);
    sensitive << deq.rdy << full << head << tail;

    SC_METHOD(FullNext);
    sensitive << enq.val << deq.rdy << full << head << tail;

    SC_THREAD(Seq);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    // Needed so that DeqMsg always has a good tail value
    tail.write(0);
  }

  // Combinational logic

  // Enqueue ready
  void EnqRdy() { enq.rdy.write(!full.read()); }

  // Dequeue valid
  void DeqVal() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    deq.val.write(!empty);
  }

  // Dequeue messsage
  void DeqMsg() {
#ifndef __SYNTHESIS__
    bool empty = (!full.read() && (head.read() == tail.read()));
    bool do_deq = !empty;
    if (do_deq) {
#endif
      deq.msg.write(buffer[tail.read()].msg.read());
#ifndef __SYNTHESIS__
    } else {
      deq.msg.write(0);
    }
#endif
  }

  // Head next calculations
  void HeadNext() {
    bool do_enq = (enq.val.read() && !full.read());
    BuffIdx head_inc;
    if ((head.read() + 1) == NumEntries)
      head_inc = 0;
    else
      head_inc = head.read() + 1;

    if (do_enq)
      head_next.write(head_inc);
    else
      head_next.write(head.read());
  }

  // Tail next calculations
  void TailNext() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    bool do_deq = (deq.rdy.read() && !empty);
    BuffIdx tail_inc;
    if ((tail.read() + 1) == NumEntries)
      tail_inc = 0;
    else
      tail_inc = tail.read() + 1;

    if (do_deq)
      tail_next.write(tail_inc);
    else
      tail_next.write(tail.read());
  }

  // Full next calculations
  void FullNext() {
    bool empty = (!full.read() && (head.read() == tail.read()));
    bool do_enq = (enq.val.read() && !full.read());
    bool do_deq = (deq.rdy.read() && !empty);

    BuffIdx head_inc;
    if ((head.read() + 1) == NumEntries)
      head_inc = 0;
    else
      head_inc = head.read() + 1;

    if (do_enq && !do_deq && (head_inc == tail.read()))
      full_next.write(1);
    else if (do_deq && full.read())
      full_next.write(0);
    else
      full_next.write(full.read());
  }

  // Sequential logic
  void Seq() {
    // Reset state
    full.write(0);
    head.write(0);
    tail.write(0);
#pragma hls_unroll yes
    for (unsigned int i = 0; i < NumEntries; ++i)
      buffer[i].reset_state();

    wait();

    while (1) {
      // Head update
      head.write(head_next);

      // Tail update
      tail.write(tail_next);

      // Full update
      full.write(full_next);

      // Enqueue message
      if (enq.val.read() && !full.read()) {
        buffer[head.read()].msg.write(enq.msg.read());
      }

      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int width = (Message().length() / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << enq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }

      std::cout << " ( " << full.read() << " ) ";

      // Dequeue port
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(width) << deq.msg.read();
      } else {
        std::cout << std::setw(width + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

// Because of ports not existing in TLM_PORT and the code depending on it,
// we remap to DIRECT_PORT here.
template <typename Message, unsigned int NumEntries>
class Buffer<Message, NumEntries, TLM_PORT> : public Buffer<Message, NumEntries, DIRECT_PORT>
{
 public:
 Buffer() : Buffer<Message, NumEntries, DIRECT_PORT>() {}
 Buffer(sc_module_name name) : Buffer<Message, NumEntries, DIRECT_PORT>(name) {}
};

//////////////////////////////////////////////////////////////////////////////////
// Sink and Source
/////////////////////////////////////////////////////////////////////////////////
 
template <typename MessageType, connections_port_t port_marshall_type = AUTO_PORT>
class Sink : public sc_module {
  SC_HAS_PROCESS(Sink);
 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<MessageType, port_marshall_type> in;

  // Constructor
  Sink(sc_module_name nm) 
    : sc_module(nm), 
      clk("clk"),
      rst("rst"),
      in("in") {
    SC_THREAD(DoSink);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);    
  }
  static const unsigned int width = 0;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
  }

 protected:
  // Behavior
  void DoSink() {
    in.Reset();
    wait();
    while (1) {
      in.Pop();
    }
  }
  
  // Binding
  template <typename C>
  void operator()(C& rhs) {
    in.Bind(rhs);
  }
};

template <typename MessageType, connections_port_t port_marshall_type = AUTO_PORT>
class DummySink : public sc_module {
  SC_HAS_PROCESS(DummySink);
 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<MessageType, port_marshall_type> in;

  // Constructor
  DummySink(sc_module_name nm) 
    : sc_module(nm), 
      clk("clk"),
      rst("rst"),
      in("in") {
    SC_THREAD(DoSink);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);    
  }
  static const unsigned int width = 0;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
  }

 protected:
  // Behavior
  void DoSink() {
    in.Reset();
    wait();
    while (1) {
      wait();
    }
  }
  
  // Binding
  template <typename C>
  void operator()(C& rhs) {
    in.Bind(rhs);
  }
};

template <typename MessageType, connections_port_t port_marshall_type = AUTO_PORT>
class DummySource : public sc_module {
  SC_HAS_PROCESS(DummySource);
 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;
  Out<MessageType, port_marshall_type> out;

  // Constructor
  DummySource(sc_module_name nm) 
    : sc_module(nm), 
      clk("clk"),
      rst("rst"),
      out("out") {
    SC_THREAD(DoSource);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);    
  }
  static const unsigned int width = 0;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
  }

 protected:
  // Behavior
  void DoSource() {
    out.Reset();
    wait();
    while (1) {
      wait();
    }
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    out.Bind(rhs);
  }
};

//
// NEW FEATURE: ChannelBinder class
// This class binds together a channel to two ports.
// It works with any type of port and channel.
// TODO: add constructors that take clk and rst from the sender
// module. They take as argument the sender module instead of its output
// port.
//

template <typename Message, unsigned int NumEntries = 1>
class ChannelBinder {

 public:

  Combinational<Message> enq;
  Combinational<Message> deq;

  // Combinational binding
  ChannelBinder(InBlocking<Message>& in,
	       OutBlocking<Message>& out,
	       Combinational<Message>& chan)
   : enq(sc_gen_unique_name("bind_enq")),
    deq(sc_gen_unique_name("bind_deq")) {

   out(chan);
   in(chan);
  }

 // Combinational binding w/ extra clk and rst arguments
  ChannelBinder(InBlocking<Message>& in,
	       OutBlocking<Message>& out,
	       Combinational<Message>& chan,
	       sc_in_clk& clk, sc_in<bool>& rst)
   : enq(sc_gen_unique_name("bind_enq")),
    deq(sc_gen_unique_name("bind_deq")) {

    out(chan);
    in(chan);
  }

  // Bypass (depth 1) binding w/ clk and rst arguments.
  ChannelBinder(InBlocking<Message>& in,
	       OutBlocking<Message>& out,
	       Bypass<Message>& chan,
	       sc_in_clk& clk, sc_in<bool>& rst)
   : enq(sc_gen_unique_name("bind_enq")),
   deq(sc_gen_unique_name("bind_deq")) {

    out(enq);
    chan.clk(clk);
    chan.rst(rst);
    chan.enq(enq);
    chan.deq(deq);
    in(deq);
  }

 // BypassBuffered (any depth) binding w/ clk and rst arguments.
 ChannelBinder(InBlocking<Message>& in,
	       OutBlocking<Message>& out,
	       BypassBuffered<Message, NumEntries>& chan,
	       sc_in_clk& clk, sc_in<bool>& rst)
   : enq(sc_gen_unique_name("bind_enq")),
    deq(sc_gen_unique_name("bind_deq")) {

    out(enq);
    chan.clk(clk);
    chan.rst(rst);
    chan.enq(enq);
    chan.deq(deq);
    in(deq);

  }

 // Pipeline binding w/ clk and rst arguments.
 ChannelBinder(InBlocking<Message>& in,
	       OutBlocking<Message>& out,
	       Pipeline<Message>& chan,
	       sc_in_clk& clk, sc_in<bool>& rst)
   : enq(sc_gen_unique_name("bind_enq")),
    deq(sc_gen_unique_name("bind_deq")) {

    out(enq);
    chan.clk(clk);
    chan.rst(rst);
    chan.enq(enq);
    chan.deq(deq);
    in(deq);
  }

 // Buffer binding w/ clk and rst arguments.
 ChannelBinder(InBlocking<Message>& in,
	       OutBlocking<Message>& out,
	       Buffer<Message, NumEntries>& chan,
	       sc_in_clk& clk, sc_in<bool>& rst)
   : enq(sc_gen_unique_name("bind_enq")),
    deq(sc_gen_unique_name("bind_deq")) {

    out(enq);
    chan.clk(clk);
    chan.rst(rst);
    chan.enq(enq);
    chan.deq(deq);
    in(deq);

  }
};

	  
}  // namespace Connections


#endif  // NVHLS_CONNECTIONS_BUFFERED_PORTS_H_

