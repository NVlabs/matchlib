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
// nvhls_connections.h
//========================================================================

#ifndef NVHLS_CONNECTIONS_H_
#define NVHLS_CONNECTIONS_H_

#include <systemc.h>
#include <modular_io.h>
#include <nvhls_packet.h>
#include <nvhls_assert.h>
#include <nvhls_marshaller.h>
#include <fifo.h>
#include <ccs_p2p.h>

#ifndef __SYNTHESIS__
#include <iomanip>
#endif

#if !defined(__SYNTHESIS__) && defined(CONNECTIONS_ACCURATE_SIM)
#define CONNECTIONS_SIM_ONLY
#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#error \
    "Make sure SC_INCLUDE_DYNAMIC_PROCESSES is defined BEFORE first systemc.h include"
#endif
#endif

#ifdef CONNECTIONS_ASSERT_ON_QUERY
#define QUERY_CALL() \
  error "Empty/Peek/Full functions are currently not supported in HLS"
#else
#define QUERY_CALL()
#endif

#ifdef CONNECTIONS_SIM_ONLY
#include <vector>
#include <testbench/Pacer.h>
#endif

/**
 * \brief Connections is a latency-insensitive communications library for MatchLib.
 * \ingroup Connections
 *
 * \par Overview
 *
 *   MatchLib Connections library is designed to address several limitations of
 *   existing Catapult p2p library:
 *     -# p2p ports are channel specific: when the channel changes, ports and
 *     modules that include the ports also need to change;
 *     -# No support for network channels that packetize a message into and
 *     extract from a network;
 *     -# Lack of Peek() for input ports;
 *     -# Lack of a default constructor that calls sc_gen_unique_name();
 *     -# Cannot be used to implement latency insensitive channels;
 *
 *   MatchLib Connections have separate definitions of Ports and Channels, both of
 *   which are based on the Modular IO methodology.
 *     - Ports supported methods:
 *       - In<T>: Peek(), Pop(), Empty(), Reset()
 *       - Out<T> : Push(), Full(), Reset()
 *     - Channel types:
 *       - Combinational<T>: Combinationally connects ports;
 *       - Bypass<T>: Enables DEQ when empty;
 *       - Pipeline<T>: Enables ENQ when full;
 *       - Buffer<T>: FIFO channel;
 *       - InNetwork<T>, OutNetwork<T>: Network channels;
 *
 *     - Ports blocking and non-blocking:
 *       - In<T>:
 *         - value = in.Pop(); //Blocking read
 *         - if (!in.Empty()) value = in.Pop(); //non-blocking read
 *       - Out<T>:
 *         - out.Push(value); //Blocking write
 *         - if (!out.Full()) out.Push(value); //non-blocking write
 *
 *     Channels:
 *       - Combinational: basically wires;
 *       - Pipeline: Added an internal state inside the channel so that the
 * communication is pipelined with a depth of one.
 *       - Bypass: Can buffer one msg when outgoing port is not available but
 *       incoming msg has arrived.
 *       - Buffer: Added a FIFO inside a channel.
 */
namespace Connections {

// Forward declarations
// These represent what SystemC calls "Ports" (which are basically endpoints)
template <typename Message>
class InBlocking;
template <typename Message>
class OutBlocking;
template <typename Message>
class In;
template <typename Message>
class Out;
// These represent what SystemC calls "Channels" (which are connections)
template <typename Message>
class Combinational;
template <typename Message>
class Bypass;
template <typename Message>
class Pipeline;
template <typename Message, unsigned int NumEntries>
class Buffer;
}

// Declarations used in the Marshaller
SpecialWrapperIfc(Connections::In);
SpecialWrapperIfc(Connections::Out);

namespace Connections {
#ifdef CONNECTIONS_SIM_ONLY

class SimConnectionsClk
{
    public:
    SimConnectionsClk(): 
        clk("default_sim_clk", 1, SC_NS, 0.5, 0, SC_NS, true), clk_ptr(&clk)
    {
    };

    void set(sc_clock* clk_ptr_) { clk_ptr = clk_ptr_; }; // should we use a module that is binded instead?

    void pre_delay() const
    {
        wait(adjust_for_edge(get_period_delay()-epsilon).to_seconds(),SC_SEC);
    }

    void post_delay() const
    {
        wait(adjust_for_edge(epsilon).to_seconds(),SC_SEC);
    }

    inline void post2pre_delay() const
    {
        static double delay_sec((get_period_delay()-2*epsilon).to_seconds()); //caching it to optimize wall runtime 
        wait(delay_sec, SC_SEC);
    }

    inline void pre2post_delay() const
    {
        static double delay_sec((2*epsilon).to_seconds()); //caching it to optimize wall runtime 
        wait(delay_sec, SC_SEC);
    }

    inline void period_delay() const
    {
        static double delay_sec(get_period_delay().to_seconds()); //caching it to optimize wall runtime 
        wait(delay_sec, SC_SEC);
    }


    private:
    static const sc_time epsilon; 

    sc_clock  clk;
    sc_clock* clk_ptr;

    inline sc_time get_period_delay() const
    {
        return clk_ptr->period();
    }

    double get_duty_ratio() const
    {
        return clk_ptr->duty_cycle();
    }

    sc_time adjust_for_edge(sc_time t) const
    {
        if (!clk_ptr->posedge_first())
            return t + get_duty_ratio()*get_period_delay();
        
        return t;
    }
};

const sc_time SimConnectionsClk::epsilon = sc_time(0.01, SC_NS);
SimConnectionsClk sim_clk;
#endif

void set_sim_clk(sc_clock* clk_ptr)
{
#ifdef CONNECTIONS_SIM_ONLY
    sim_clk.set(clk_ptr);
#endif
}

#ifdef CONNECTIONS_SIM_ONLY
// this is an abstract class for both blocking connections
// it is used to allow a containter of pointers to any blocking connection
class Blocking_abs {
public:
    Blocking_abs() {};

    virtual bool Post() {return false;};
    virtual bool Pre()  {return false;};
};


class ConManager {
public:
    ConManager()
    {
        sc_spawn(sc_bind(&ConManager::run, this, true), "connection_manager_run");
    }

    std::vector<Blocking_abs*> tracked;
  
    void add(Blocking_abs* c)
    {
        tracked.push_back(c);
    }

    void remove(Blocking_abs* c)
    {
        for (std::vector<Blocking_abs*>::iterator it = tracked.begin(); it!=tracked.end(); ++it)
        {
            if (*it == c) 
            {
                tracked.erase(it);
                return;
            }
        }

        assert(0);
    }

    void run(bool value) {
        sim_clk.post_delay();  // align to occur just after the cycle

        while (1) {
            //Post();
            for (std::vector<Blocking_abs*>::iterator it=tracked.begin(); it!=tracked.end(); )
                if ((*it)->Post()) ++it;
                    else tracked.erase(it);
            sim_clk.post2pre_delay();
            //Pre();
            for (std::vector<Blocking_abs*>::iterator it=tracked.begin(); it!=tracked.end(); )
                if ((*it)->Pre()) ++it;
                    else tracked.erase(it);
            sim_clk.pre2post_delay();      
        }
    }
};

ConManager conManager;
#endif

//------------------------------------------------------------------------
// InBlocking
//------------------------------------------------------------------------

template <typename Message>
#ifdef CONNECTIONS_SIM_ONLY
class InBlocking : public Blocking_abs {
#else
class InBlocking {
#endif
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in<MsgBits> msg;
  sc_in<bool> val;
  sc_out<bool> rdy;

  // Default constructor
  InBlocking()
      : msg(sc_gen_unique_name("in_msg")),
        val(sc_gen_unique_name("in_val")),
        rdy(sc_gen_unique_name("in_rdy")) {
#ifdef CONNECTIONS_SIM_ONLY
    Init_SIM(sc_gen_unique_name("in"));
#endif
  }

  // Constructor
  explicit InBlocking(const char* name)
      : msg(ccs_concat(name, "msg")),
        val(ccs_concat(name, "val")),
        rdy(ccs_concat(name, "rdy")) {
#ifdef CONNECTIONS_SIM_ONLY
    Init_SIM(name);
#endif
  }

  // Reset read
  virtual void Reset() {
#ifdef CONNECTIONS_SIM_ONLY
    Reset_SIM();
#else
    rdy.write(false);
#endif
  }

  // Bind to InBlocking
  void Bind(InBlocking<Message>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    msg(rhs.msg);
    val(rhs.val);
    rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message>& rhs) {
    msg(rhs.msg);
    val(rhs.val);
    rdy(rhs.rdy);
#ifdef CONNECTIONS_SIM_ONLY
    rhs.in_bound = true;
#endif
  }

  // Bind to p2p<>::in
  void Bind(p2p<SYN>::in<Message>& rhs) {
    msg(rhs.i_dat);
    val(rhs.i_vld);
    rdy(rhs.o_rdy);
  }

  // Bind to p2p<>::chan
  void Bind(p2p<SYN>::chan<Message>& rhs) {
    msg(rhs.dat);
    val(rhs.vld);
    rdy(rhs.rdy);
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
#ifdef CONNECTIONS_SIM_ONLY
    return Pop_SIM();
#else
    do {
      rdy.write(true);
      wait();
    } while (val.read() != true);
    rdy.write(false);
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    return result.val;
#endif
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    QUERY_CALL();
    while (!val.read()) {
      wait();
    }
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    return result.val;
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
#ifdef CONNECTIONS_SIM_ONLY
    if (Empty_SIM()) {
      Message m;
      data = m;
      return false;
    } else {
      data = ConsumeBuf_SIM();
      return true;
    }
#else
    rdy.write(true);
    if (do_wait) {
      wait();
      rdy.write(false);
    }
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    data = result.val;
    return val.read();
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
  void disable_spawn()
  {
    conManager.remove(this);
  } 

 protected:
  Message data_buf;
  bool data_val;
  bool rdy_set_by_api;
#ifdef CONN_RAND_STALL
  Pacer *post_pacer;
  bool pacer_stall;
#endif

  void Init_SIM(const char* name) {
    data_val = false;
    rdy_set_by_api = false;
    conManager.add(this);
#ifdef CONN_RAND_STALL
    double x = rand()%100;
    double y = rand()%100;
    post_pacer = new Pacer(x/100, y/100);
    pacer_stall = false;
#endif
  }

  void Reset_SIM() {
    data_val = false;
  }

// Although this code is being used only for simulation now, it could be
// synthesized if pre/post are used instead of spawned threads.
// Keep mudulario pragmas here to preserve that option

#pragma design modulario < out >
  void receive(const bool& stall) {
    if (stall) {
      rdy.write(false);
      rdy_set_by_api = false;
    } else {
      rdy.write(true);
      rdy_set_by_api = true;
    }
  }

#pragma design modulario < in >
  bool received(Message& data) {
    if (val.read())
    {
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    data = result.val;
    return true;
    }

    // not valid
    // randomize data
    //memset(&data, random(), sizeof(data));
    //if (!data_val)
    //    memset(&data, 0, sizeof(data));
    return false;
  }

  bool Pre() {
#ifdef CONN_RAND_STALL
    if (pacer_stall) return true;
#endif
    if (rdy_set_by_api != rdy.read())
    {
        // something has changed the value of the signal not through API
        // killing spawned threads;
        return false;
    }
    if (!data_val) {
      if (received(data_buf)) {
        data_val = true;
      }
    }
    return true;
  }

#ifdef CONN_RAND_STALL
  bool Post() {
    if (post_pacer->tic()) 
    {
        pacer_stall=true;
    } else {
        pacer_stall=false;
    }
    receive(data_val || pacer_stall); 
    return true;
  }
#else
  bool Post() {
    receive(data_val); 
    return true;
  }
#endif

  bool Empty_SIM() { return !data_val; }

  Message& ConsumeBuf_SIM() {
    assert(data_val);
    data_val = false;
    return data_buf;
  }

  Message& Pop_SIM() {
    while (Empty_SIM()) {
      wait();
    }
    return ConsumeBuf_SIM();
  }
#endif
};

//------------------------------------------------------------------------
// In
//------------------------------------------------------------------------
/**
 * \brief Connections IN port 
 * \ingroup Connections
 *
 * \tparam Message      DataType of message 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      ...
 *      Connections::In< T > in;
 *      ...
 *      in.Reset();
 *      wait();
 *      ...
 *      T msg = in.Pop(); // Blocking Pop from channel
 *      ...
 *      T reg;
 *      if (in.PopNB(reg)) { // Non-Blocking Pop from channel
 *      ...
 *      }
 * \endcode
 * \par
 *
 */
template <typename Message>
class In : public InBlocking<Message> {
 public:
  In() {}

  explicit In(const char* name) : InBlocking<Message>(name) {}

  // Empty
  bool Empty() {
#ifdef CONNECTIONS_SIM_ONLY
    return this->Empty_SIM();
#else
    QUERY_CALL();
    return !this->val.read();
#endif
  }
};

//------------------------------------------------------------------------
// OutBlocking
//------------------------------------------------------------------------

template <typename Message>
#ifdef CONNECTIONS_SIM_ONLY
class OutBlocking: public Blocking_abs {
#else 
class OutBlocking {
#endif
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_out<MsgBits> msg;
  sc_out<bool> val;
  sc_in<bool> rdy;

  // Default constructor
  OutBlocking()
      : msg(sc_gen_unique_name("out_msg")),
        val(sc_gen_unique_name("out_val")),
        rdy(sc_gen_unique_name("out_rdy")) {
#ifdef CONNECTIONS_SIM_ONLY
    Init_SIM(sc_gen_unique_name("out"));
#endif
  }

  // Constructor
  explicit OutBlocking(const char* name)
      : msg(ccs_concat(name, "msg")),
        val(ccs_concat(name, "val")),
        rdy(ccs_concat(name, "rdy")) {
#ifdef CONNECTIONS_SIM_ONLY
    Init_SIM(name);
#endif
  }

  // Reset write
  virtual void Reset() {
#ifdef CONNECTIONS_SIM_ONLY
    Reset_SIM();
#else
    val.write(false);
    msg.write(0);
#endif
  }

  // Bind to OutBlocking
  void Bind(OutBlocking<Message>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    msg(rhs.msg);
    val(rhs.val);
    rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message>& rhs) {
    msg(rhs.msg);
    val(rhs.val);
    rdy(rhs.rdy);
#ifdef CONNECTIONS_SIM_ONLY
    rhs.out_bound = true;
#endif
  }

  // Bind to p2p<>::out
  void Bind(p2p<SYN>::out<Message>& rhs) {
    msg(rhs.o_dat);
    val(rhs.o_vld);
    rdy(rhs.i_rdy);
  }

  // Bind to p2p<>::chan
  void Bind(p2p<SYN>::chan<Message>& rhs) {
    msg(rhs.dat);
    val(rhs.vld);
    rdy(rhs.rdy);
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    return Push_SIM(m);
#else
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    do {
      val.write(true);
      msg.write(bits);
      wait();
    } while (rdy.read() != true);
    val.write(false);
#endif
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
#ifdef CONNECTIONS_SIM_ONLY
    if (Full_SIM()) {
      return false;
    } else {
      FillBuf_SIM(m);
      return true;
    }
#else
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    val.write(true);
    msg.write(bits);
    if (do_wait) {
      wait();
      val.write(false);
    }
    return rdy.read();
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
  void disable_spawn()
  {
    conManager.remove(this);
  } 

 protected:
  bool data_val;
  Message data_buf;
  bool val_set_by_api;

  void Init_SIM(const char* name) {
    data_val = false;
    val_set_by_api = false;
    conManager.add(this);
  }

  void Reset_SIM() {
    msg.write(0);
    data_val = false;
  }

// Although this code is being used only for simulation now, it could be
// synthesized if pre/post are used instead of spawned threads.
// Keep mudulario pragmas here to preserve that option

#pragma design modulario < in >
  bool transmitted() { return rdy.read(); }

#pragma design modulario < out >
  void transmit_data(const Message& m) {
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msg.write(bits);
  }

#pragma design modulario < out >
  void transmit_val(const bool& vald) {
    if (vald) {
      val.write(true);
      val_set_by_api = true;
    } else {
      val.write(false);
      val_set_by_api = false;

      //corrupt and transmit data
      //memset(&data_buf, random(), sizeof(data_buf));
      //transmit_data(data_buf);
    }
  }

  bool Pre() {
    if (data_val) {
      if (transmitted()) {
        data_val = false;
      }
    }
    return true;
  }

  bool Post() { 
    if (val_set_by_api != val.read())
    {
        // something has changed the value of the signal not through API
        // killing spawned threads;
        return false;
    }
    transmit_val(data_val); 
    //val_set_by_api = val.read();
    return true;
  }

  void FillBuf_SIM(const Message& m) {
    assert(!data_val);
    data_val = true;
    transmit_data(m);
    data_buf = m;
  }

  bool Empty_SIM() { return !data_val; }

  bool Full_SIM() { return data_val; }

  void Push_SIM(const Message& m) {
    while (Full_SIM()) {
      wait();
    }
    FillBuf_SIM(m);
  }
#endif
};

//------------------------------------------------------------------------
// Out
//------------------------------------------------------------------------
/**
 * \brief Connections Out port 
 * \ingroup Connections
 *
 * \tparam Message      DataType of message 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      ...
 *      Connections::Out< T > out;
 *      ...
 *      out.Reset();
 *      wait();
 *      ...
 *
 *      out.Push(msg); // Blocking Push to channel
 *      ...
 *      if (out.PushNB(msg)) { // Non-Blocking Pop from channel
 *      ...
 *      }
 * \endcode
 * \par
 *
 */

template <typename Message>
class Out : public OutBlocking<Message> {
 public:
  Out() {}

  explicit Out(const char* name) : OutBlocking<Message>(name) {}

  // Full
  bool Full() {
#ifdef CONNECTIONS_SIM_ONLY
    return this->Full_SIM();
#else
    QUERY_CALL();
    return !this->rdy.read();
#endif
  }
};

//------------------------------------------------------------------------
// Combinational
//------------------------------------------------------------------------
/**
 * \brief Connections Combinational Channel 
 * \ingroup Connections
 *
 * \tparam Message      DataType of message 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      ...
 *      Connections::Combinational< T > channel;
 *      ...
 *      // Channel Binding
 *      src.out(chan);
 *      sink.in(chan);
 *      ...
 * \endcode
 * \par
 *
 */
template <typename Message>
class Combinational {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_signal<MsgBits> msg;
  sc_signal<bool> val;
  sc_signal<bool> rdy;

  // Default constructor
  Combinational()
      : msg(sc_gen_unique_name("comb_msg")),
        val(sc_gen_unique_name("comb_val")),
        rdy(sc_gen_unique_name("comb_rdy"))
#ifdef CONNECTIONS_SIM_ONLY
        ,out_bound(false), in_bound(false)
        ,dummyPortManager(sc_gen_unique_name("dummyPortManager"), sim_in, sim_out, *this)
#endif
{
}

  // Constructor
  explicit Combinational(const char* name)
      : msg(ccs_concat(name, "msg")),
        val(ccs_concat(name, "val")),
        rdy(ccs_concat(name, "rdy")) 
#ifdef CONNECTIONS_SIM_ONLY
        ,out_bound(false), in_bound(false)
        ,dummyPortManager(ccs_concat(name, "dummyPortManager"), sim_in, sim_out, *this)
#endif
{
}

  // Reset
  void ResetRead() { 
#ifdef CONNECTIONS_SIM_ONLY
    sim_in.Reset();
#else
    rdy.write(false); 
#endif
  }

  void ResetWrite() {
#ifdef CONNECTIONS_SIM_ONLY
    sim_out.Reset();
#else
    val.write(false);
    msg.write(0);
#endif
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
#ifdef CONNECTIONS_SIM_ONLY
    return sim_in.Pop();
#else
    do {
      rdy.write(true);
      wait();
    } while (val.read() != true);
    rdy.write(false);
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    return result.val;
#endif
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
#ifdef CONNECTIONS_SIM_ONLY
    return sim_in.Peek();
#else
    while (!val.read()) {
      wait();
    }
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    return result.val;
#endif
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
#ifdef CONNECTIONS_SIM_ONLY
    return sim_in.PopNB(data);
#else
    rdy.write(true);
    wait();
    rdy.write(false);
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    data = result.val;
    return val.read();
#endif
  }

// ekrimer: commenting these out, should really be EmptyWrite and EmptyRead, and in anycase will not work in verilog currently
  // Empty
//  bool Empty() { return !val.read(); }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    sim_out.Push(m);
#else
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    do {
      val.write(true);
      msg.write(bits);
      wait();
    } while (rdy.read() != true);
    val.write(false);
#endif
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    return sim_out.PushNB(m);
#else
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    val.write(true);
    msg.write(bits);
    wait();
    val.write(false);
    return rdy.read();
#endif
  }

// ekrimer: commenting out, similar to Empty
  // Full
//  bool Full() { return !rdy.read(); }

#ifdef CONNECTIONS_SIM_ONLY
  bool out_bound, in_bound;
protected:
    class DummyPortManager : public sc_module {
        SC_HAS_PROCESS(DummyPortManager);
    private:
    InBlocking<Message>& in;
    OutBlocking<Message>& out;
    Combinational<Message>& parent;

    public:
    DummyPortManager(sc_module_name name, InBlocking<Message>& in_, OutBlocking<Message>& out_, Combinational<Message>& parent_)
    : sc_module(name), in(in_), out(out_), parent(parent_)
    {}
    
    virtual void before_end_of_elaboration()
    {
        if (!parent.in_bound)
        {
            in(parent);
        } else {
            sc_signal<MsgBits>* dummy_in_msg = new sc_signal<MsgBits>;
            sc_signal<bool>* dummy_in_val = new sc_signal<bool>;
            sc_signal<bool>* dummy_in_rdy = new sc_signal<bool>;

            in.msg(*dummy_in_msg);
            in.val(*dummy_in_val);
            in.rdy(*dummy_in_rdy);

            in.disable_spawn();
        }
        if (!parent.out_bound)
        {
            out(parent);
        } else {
            sc_signal<MsgBits>* dummy_out_msg = new sc_signal<MsgBits>;
            sc_signal<bool>* dummy_out_val = new sc_signal<bool>;
            sc_signal<bool>* dummy_out_rdy = new sc_signal<bool>;

            out.msg(*dummy_out_msg);
            out.val(*dummy_out_val);
            out.rdy(*dummy_out_rdy);
            
            out.disable_spawn();
        }

    }

    } dummyPortManager;

  OutBlocking<Message> sim_out;
  InBlocking<Message> sim_in;

#endif
};

//------------------------------------------------------------------------
// Bypass
//------------------------------------------------------------------------

template <typename Message>
class Bypass : public sc_module {
  SC_HAS_PROCESS(Bypass);

 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message> enq;
  Out<Message> deq;

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
  sc_signal<MsgBits> state;

  // Helper functions
  void Init() {
    SC_METHOD(EnqRdy);
    sensitive << full;

    SC_METHOD(DeqVal);
    sensitive << enq.val << full;

    SC_METHOD(WriteEn);
    sensitive << enq.val << deq.rdy << full;

    SC_METHOD(BypassMux);
    sensitive << enq.msg << state << full;

    SC_CTHREAD(Seq, clk.pos());
    async_reset_signal_is(rst, false);
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
      deq.msg.write(state.read());
    } else {
      deq.msg.write(enq.msg.read());
    }
  }

  // Sequential logic
  void Seq() {
    // Reset state
    full.write(0);
    state.write(0);

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
        state.write(enq.msg.read());
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

//------------------------------------------------------------------------
// Pipeline
//------------------------------------------------------------------------

template <typename Message>
class Pipeline : public sc_module {
  SC_HAS_PROCESS(Pipeline);

 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message> enq;
  Out<Message> deq;

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
  sc_signal<MsgBits> state;

  // Helper functions
  void Init() {
    SC_METHOD(EnqRdy);
    sensitive << full << deq.rdy;

    SC_METHOD(DeqVal);
    sensitive << full;

    SC_METHOD(WriteEn);
    sensitive << enq.val << deq.rdy << full;

    SC_METHOD(DeqMsg);
    sensitive << state;

    SC_CTHREAD(Seq, clk.pos());
    async_reset_signal_is(rst, false);
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
  void DeqMsg() { deq.msg.write(state.read()); }

  // Sequential logic
  void Seq() {
    // Reset state
    full.write(0);
    state.write(0);

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
        state.write(enq.msg.read());
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

//------------------------------------------------------------------------
// Buffer
//------------------------------------------------------------------------

template <typename Message, unsigned int NumEntries>
class Buffer : public sc_module {
  SC_HAS_PROCESS(Buffer);

 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message> enq;
  Out<Message> deq;

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
  sc_signal<MsgBits> buffer[NumEntries];

  // Helper functions
  void Init() {
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

    SC_CTHREAD(Seq, clk.pos());
    async_reset_signal_is(rst, false);

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
      deq.msg.write(buffer[tail.read()].read());
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
      buffer[i].write(0);

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
        buffer[head.read()].write(enq.msg.read());
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

//------------------------------------------------------------------------
// InNetwork
//------------------------------------------------------------------------

template <typename Message, unsigned int DestWidthPerHop, unsigned int MaxHops,
          unsigned int PacketIdWidth>
class InNetwork : public sc_module {
  SC_HAS_PROCESS(InNetwork);

 public:
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  typedef Packet<WMessage::width, DestWidthPerHop, MaxHops, PacketIdWidth>
      Packet_t;

  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Packet_t> enq;
  Out<Message> deq;

  InNetwork()
      : sc_module(sc_module_name(sc_gen_unique_name("in_network"))),
        clk("clk"),
        rst("rst") {
    Init();
  }

  InNetwork(sc_module_name name) : sc_module(name), clk("clk"), rst("rst") {
    Init();
  }

 protected:
  void Init() {
    SC_METHOD(AssignMsg);
    sensitive << enq.msg << rst;

    SC_METHOD(AssignVal);
    sensitive << enq.val;

    SC_METHOD(AssignRdy);
    sensitive << deq.rdy;
  }

  void AssignMsg() {
    if (!rst.read()) {
      deq.msg.write(0);
    } else if (enq.val.read()) {
      sc_lv<Wrapped<Packet_t>::width> pbits = enq.msg.read();
      Marshaller<Wrapped<Packet_t>::width> pmarshaller(pbits);
      Packet_t packet;
      packet.Marshall(pmarshaller);
      MsgBits mbits(packet.data);
      deq.msg.write(mbits);
    }
  }

  void AssignVal() { deq.val.write(enq.val.read()); }
  void AssignRdy() { enq.rdy.write(deq.rdy.read()); }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int pwidth = (Packet_t::width / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(pwidth) << enq.msg.read();
      } else {
        std::cout << std::setw(pwidth + 1) << " ";
      }
      std::cout << " | ";

      // Dequeue port
      unsigned int mwidth = (Message().length() / 4);
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(mwidth) << deq.msg.read();
      } else {
        std::cout << std::setw(mwidth + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

//------------------------------------------------------------------------
// OutNetwork
//------------------------------------------------------------------------

template <typename Message, unsigned int DestWidthPerHop, unsigned int MaxHops,
          unsigned int PacketIdWidth>
class OutNetwork : public sc_module {
  SC_HAS_PROCESS(OutNetwork);

 public:
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  typedef Packet<WMessage::width, DestWidthPerHop, MaxHops, PacketIdWidth>
      Packet_t;

  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message> enq;
  Out<Packet_t> deq;
  In<sc_lv<DestWidthPerHop * MaxHops> > route;
  In<sc_lv<PacketIdWidth> > id;

  // Internal State
  sc_signal<sc_lv<DestWidthPerHop * MaxHops> > route_state;
  sc_signal<sc_lv<PacketIdWidth> > id_state;

  OutNetwork()
      : sc_module(sc_module_name(sc_gen_unique_name("out_network"))),
        clk("clk"),
        rst("rst") {
    Init();
  }

  OutNetwork(sc_module_name name) : sc_module(name), clk("clk"), rst("rst") {
    Init();
  }

 protected:
  void Init() {
    SC_METHOD(AssignMsg);
    sensitive << enq.msg << id_state << route_state << rst;

    SC_METHOD(AssignVal);
    sensitive << enq.val;

    SC_METHOD(AssignRdy);
    sensitive << deq.rdy;

    SC_METHOD(TieToHigh);
    sensitive << clk.pos();

    SC_CTHREAD(SetState, clk.pos());
    async_reset_signal_is(rst, false);
  }

  void AssignMsg() {
    if (!rst.read()) {
      deq.msg.write(0);
    } else if (enq.val.read()) {
      Packet_t packet;
      vector_to_type(id_state.read(), false, &packet.packet_id);
      vector_to_type(route_state.read(), false, &packet.dest);
      vector_to_type(enq.msg.read(), false, &packet.data);
      Marshaller<Wrapped<Packet_t>::width> pmarshaller;
      packet.Marshall(pmarshaller);
      deq.msg.write(pmarshaller.GetResult());
    }
  }

  void AssignVal() { deq.val.write(enq.val.read()); }
  void AssignRdy() { enq.rdy.write(deq.rdy.read()); }

  void TieToHigh() {
    route.rdy.write(1);
    id.rdy.write(1);
  }

  void SetState() {
    id_state.write(0);
    route_state.write(0);
    wait();
    while (1) {
      if (route.val.read()) {
        route_state.write(route.msg.read());
      }

      if (id.val.read()) {
        id_state.write(id.msg.read());
      }
      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int mwidth = (Message().length() / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(mwidth) << enq.msg.read();
      } else {
        std::cout << std::setw(mwidth + 1) << " ";
      }
      std::cout << " | ";

      // Dequeue port
      unsigned int pwidth = (Packet_t::width / 4);
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(pwidth) << deq.msg.read();
      } else {
        std::cout << std::setw(pwidth + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

//------------------------------------------------------------------------
// InNetworkCredit
//------------------------------------------------------------------------
// NOTE: Currently, the PacketIdWidth, DestWidthPerHop, MaxHops parameters
// are assumed to be the same for a packet and a credit_packet

template <typename Message, unsigned int DestWidthPerHop, unsigned int MaxHops,
          unsigned int PacketIdWidth, unsigned int CreditWidth>
class InNetworkCredit : public sc_module {
  SC_HAS_PROCESS(InNetworkCredit);

 public:
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  typedef Packet<WMessage::width, DestWidthPerHop, MaxHops, PacketIdWidth>
      Packet_t;
  typedef Packet<CreditWidth, DestWidthPerHop, MaxHops, PacketIdWidth>
      CreditPacket_t;
  typedef sc_lv<CreditWidth> Credit_t;

  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Packet_t> enq;
  Out<Message> deq;
  Out<CreditPacket_t> credit;
  In<Credit_t> init_credits;
  In<sc_lv<DestWidthPerHop * MaxHops> > credit_route;
  In<sc_lv<PacketIdWidth> > credit_id;

  // Sub Module
  OutNetwork<Credit_t, DestWidthPerHop, MaxHops, PacketIdWidth> credit_out;

  // Wire
  Combinational<Credit_t> credit_enq;

  // Internal State
  sc_signal<Credit_t> credits;
  sc_signal<Credit_t> credits_next;

  InNetworkCredit()
      : sc_module(sc_module_name(sc_gen_unique_name("in_nw_credit"))),
        clk("clk"),
        rst("rst"),
        credit_out("credit_out") {
    Init();
  }

  InNetworkCredit(sc_module_name name)
      : sc_module(name), clk("clk"), rst("rst"), credit_out("credit_out") {
    Init();
  }

 protected:
  void Init() {
    credit_out.clk(clk);
    credit_out.rst(rst);
    credit_out.route(credit_route);
    credit_out.id(credit_id);
    credit_out.enq(credit_enq);
    credit_out.deq(credit);

    SC_METHOD(AssignMsg);
    sensitive << enq.msg << rst;

    SC_METHOD(AssignVal);
    sensitive << enq.val;

    SC_METHOD(AssignRdy);
    sensitive << deq.rdy;

    SC_METHOD(TieToHigh);
    sensitive << clk.pos();

    SC_METHOD(AssignNextCredits);
    sensitive << deq.val << deq.rdy << credit_enq.val << credit_enq.rdy
              << credits;

    SC_METHOD(AssignCreditMsg);
    sensitive << deq.val << deq.rdy << credit_enq.val << credit_enq.rdy
              << credits;

    SC_METHOD(AssignCreditVal);
    sensitive << deq.val << deq.rdy << credits;

    SC_CTHREAD(UpdateCredit, clk.pos());
    async_reset_signal_is(rst, false);
  }

  void AssignMsg() {
    if (!rst.read()) {
      deq.msg.write(0);
    } else if (enq.val.read()) {
      sc_lv<Wrapped<Packet_t>::width> pbits = enq.msg.read();
      Marshaller<Wrapped<Packet_t>::width> pmarshaller(pbits);
      Packet_t packet;
      packet.Marshall(pmarshaller);
      MsgBits mbits(packet.data);
      deq.msg.write(mbits);
    }
  }

  void AssignVal() { deq.val.write(enq.val.read()); }
  void AssignRdy() { enq.rdy.write(deq.rdy.read()); }

  void TieToHigh() { init_credits.rdy.write(1); }

  void AssignNextCredits() {
    bool do_deq = deq.val.read() && deq.rdy.read();
    bool do_credit = credit_enq.val.read() && credit_enq.rdy.read();
    if (do_credit && !do_deq && (credits.read() != 0)) {
      credits_next.write(0);
    } else if (do_credit && do_deq) {
      credits_next.write(0);
    } else if (!do_credit && do_deq) {
      credits_next.write(credits.read().to_uint() + 1);
    } else {
      credits_next.write(credits.read());
    }
  }

  void AssignCreditMsg() {
    bool do_deq = deq.val.read() && deq.rdy.read();
    bool do_credit = credit_enq.val.read() && credit_enq.rdy.read();
    if (do_deq && do_credit && (credits.read() == 0)) {
      credit_enq.msg.write(1);
    } else if (do_deq && do_credit && (credits.read() != 0)) {
      credit_enq.msg.write(credits.read().to_uint() + 1);
    } else if (!do_deq && do_credit && (credits.read() != 0)) {
      credit_enq.msg.write(credits.read());
    } else {
      credit_enq.msg.write(0);
    }
  }

  void AssignCreditVal() {
    bool do_deq = deq.val.read() && deq.rdy.read();
    if (do_deq || (credits.read() != 0)) {
      credit_enq.val.write(1);
    } else {
      credit_enq.val.write(0);
    }
  }

  void UpdateCredit() {
    credits.write(0);
    wait();
    while (1) {
      if (init_credits.val.read()) {
        credits.write(init_credits.msg.read());
      } else {
        credits.write(credits_next.read());
      }
      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int pwidth = (Packet_t::width / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(pwidth) << enq.msg.read();
      } else {
        std::cout << std::setw(pwidth + 1) << " ";
      }
      std::cout << " | ";

      // Dequeue port
      unsigned int mwidth = (Message().length() / 4);
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(mwidth) << deq.msg.read();
      } else {
        std::cout << std::setw(mwidth + 1) << " ";
      }
      std::cout << " | ";

      // Pending credits
      std::cout << std::hex << " ( " << credits.read() << " ) ";

      // Credit
      unsigned int cwidth = (CreditPacket_t::width / 4);
      if (credit.val.read() && credit.rdy.read()) {
        std::cout << std::hex << std::setw(cwidth) << credit.msg.read();
      } else {
        std::cout << std::setw(cwidth + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

//------------------------------------------------------------------------
// OutNetworkCredit
//------------------------------------------------------------------------
// NOTE: Currently, the PacketIdWidth, DestWidthPerHop, MaxHops parameters
// are assumed to be the same for a packet and a credit_packet

template <typename Message, unsigned int DestWidthPerHop, unsigned int MaxHops,
          unsigned int PacketIdWidth, unsigned int CreditWidth>
class OutNetworkCredit : public sc_module {
  SC_HAS_PROCESS(OutNetworkCredit);

 public:
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  typedef Packet<WMessage::width, DestWidthPerHop, MaxHops, PacketIdWidth>
      Packet_t;
  typedef Packet<CreditWidth, DestWidthPerHop, MaxHops, PacketIdWidth>
      CreditPacket_t;
  typedef sc_lv<CreditWidth> Credit_t;

  // Interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<Message> enq;
  Out<Packet_t> deq;
  In<CreditPacket_t> credit;
  In<sc_lv<DestWidthPerHop * MaxHops> > route;
  In<sc_lv<PacketIdWidth> > id;

  // Sub Module
  InNetwork<Credit_t, DestWidthPerHop, MaxHops, PacketIdWidth> credit_in;

  // Wire
  Combinational<Credit_t> credit_deq;

  // Internal State
  sc_signal<sc_lv<DestWidthPerHop * MaxHops> > route_state;
  sc_signal<sc_lv<PacketIdWidth> > id_state;
  sc_signal<Credit_t> credits;
  sc_signal<Credit_t> credits_next;

  OutNetworkCredit()
      : sc_module(sc_module_name(sc_gen_unique_name("out_nw_credit"))),
        clk("clk"),
        rst("rst"),
        credit_in("credit_in") {
    Init();
  }

  OutNetworkCredit(sc_module_name name)
      : sc_module(name), clk("clk"), rst("rst"), credit_in("credit_in") {
    Init();
  }

 protected:
  void Init() {
    credit_in.clk(clk);
    credit_in.rst(rst);
    credit_in.enq(credit);
    credit_in.deq(credit_deq);

    SC_METHOD(AssignMsg);
    sensitive << enq.msg << id_state << route_state << rst;

    SC_METHOD(AssignVal);
    sensitive << enq.val << credits << credit_deq.val << credit_deq.rdy;

    SC_METHOD(AssignRdy);
    sensitive << deq.rdy;

    SC_METHOD(TieToHigh);
    sensitive << clk.pos();

    SC_METHOD(AssignNextCredits);
    sensitive << deq.val << deq.rdy << credit_deq.val << credit_deq.rdy
              << credits;

    SC_METHOD(AssignCreditRdy);
    sensitive << deq.val << deq.rdy << credits;

    SC_CTHREAD(UpdateCredit, clk.pos());
    async_reset_signal_is(rst, false);

    SC_CTHREAD(SetState, clk.pos());
    async_reset_signal_is(rst, false);
  }

  void AssignMsg() {
    if (!rst.read()) {
      deq.msg.write(0);
    } else if (enq.val.read()) {
      Packet_t packet;
      vector_to_type(id_state.read(), false, &packet.packet_id);
      vector_to_type(route_state.read(), false, &packet.dest);
      vector_to_type(enq.msg.read(), false, &packet.data);
      Marshaller<Wrapped<Packet_t>::width> pmarshaller;
      packet.Marshall(pmarshaller);
      deq.msg.write(pmarshaller.GetResult());
    }
  }

  void AssignVal() {
    bool do_credit = credit_deq.val.read() && credit_deq.rdy.read();
    if (enq.val.read() && (credits.read() != 0)) {
      deq.val.write(1);
    } else if (enq.val.read() && (credits.read() == 0) && do_credit) {
      deq.val.write(1);
    } else {
      deq.val.write(0);
    }
  }

  void AssignRdy() { enq.rdy.write(deq.rdy.read()); }

  void TieToHigh() {
    route.rdy.write(1);
    id.rdy.write(1);
  }

  void AssignNextCredits() {
    bool do_deq = deq.val.read() && deq.rdy.read();
    bool do_credit = credit_deq.val.read() && credit_deq.rdy.read();
    if (!do_credit && do_deq) {
      credits_next.write(credits.read().to_uint() - 1);
    } else if (do_credit && !do_deq) {
      credits_next.write(credits.read().to_uint() +
                         credit.msg.read().to_uint());
    } else if (do_credit && do_deq) {
      credits_next.write(credits.read().to_uint() +
                         credit.msg.read().to_uint() - 1);
    } else {
      credits_next.write(credits.read());
    }
  }

  void AssignCreditRdy() { credit_deq.rdy.write(1); }

  void UpdateCredit() {
    credits.write(0);
    wait();
    while (1) {
      credits.write(credits_next.read());
      wait();
    }
  }

  void SetState() {
    id_state.write(0);
    route_state.write(0);
    wait();
    while (1) {
      if (route.val.read()) {
        route_state.write(route.msg.read());
      }

      if (id.val.read()) {
        id_state.write(id.msg.read());
      }
      wait();
    }
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    if (rst.read()) {
      unsigned int mwidth = (Message().length() / 4);
      // Enqueue port
      if (enq.val.read() && enq.rdy.read()) {
        std::cout << std::hex << std::setw(mwidth) << enq.msg.read();
      } else {
        std::cout << std::setw(mwidth + 1) << " ";
      }
      std::cout << " | ";

      // Dequeue port
      unsigned int pwidth = (Packet_t::width / 4);
      if (deq.val.read() && deq.rdy.read()) {
        std::cout << std::hex << std::setw(pwidth) << deq.msg.read();
      } else {
        std::cout << std::setw(pwidth + 1) << " ";
      }
      std::cout << " | ";

      // Pending credits
      std::cout << std::hex << " ( " << credits.read() << " ) ";

      // Credit
      unsigned int cwidth = (CreditPacket_t::width / 4);
      if (credit.val.read() && credit.rdy.read()) {
        std::cout << std::hex << std::setw(cwidth) << credit.msg.read();
      } else {
        std::cout << std::setw(cwidth + 1) << " ";
      }
      std::cout << " | ";
    }
  }
#endif
};

template <typename Message>
class BufferedChannel : public sc_module {
 public:
  sc_in_clk clk;
  sc_in<bool> rst;
  Buffer<Message, 2> buf;
  Combinational<Message> enq_con;
  Combinational<Message> deq_con;
  BufferedChannel(sc_module_name nm)
      : sc_module(nm),
        clk("clk"),
        rst("rst"),
        buf("buf"),
        enq_con("enq_con"),
        deq_con("deq_con") {

    buf.clk(clk);
    buf.rst(rst);
    buf.enq(enq_con);
    buf.deq(deq_con);
  }

  void Connect(Out<Message>& out, In<Message>& in) {
    out(enq_con);
    in(deq_con);
  }
};
/**
 * \brief Connections Buffered IN port 
 * \ingroup Connections
 *
 * \tparam Message      DataType of message 
 * \tparam BufferSize   Default value = 1 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      ...
 *      Connections::InBuffered< T, 2 > in;
 *      ...
 *      in.Reset();
 *      wait();
 *      ...
 *      in.TransferNB();  // Non-blocking transfer from channel to FIFO
 *      ...
 *      if (!in.Empty()) { // Check if input buffer has data
 *      T msg1 = in.Pop(); // Pop from buffer
 *      T msg2 = in.Peek(); // Peek from buffer
 *      ...
 * \endcode
 * \par
 *
 */

template <typename Message, int BufferSize = 1>
class InBuffered : public InBlocking<Message> {
  FIFO<Message, BufferSize> fifo;

 public:
  InBuffered() : InBlocking<Message>(), fifo() {}

  explicit InBuffered(const char* name) : InBlocking<Message>(name), fifo() {}

  virtual void Reset() {
    InBlocking<Message>::Reset();
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
/**
 * \brief Connections Buffered Out port 
 * \ingroup Connections
 *
 * \tparam Message      DataType of message 
 * \tparam BufferSize   Default value = 1 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      ...
 *      Connections::OutBuffered< T, 2 > out;
 *      ...
 *      out.Reset();
 *      wait();
 *      ...
 *      out.TransferNB();  // Non-blocking transfer from FIFO to channel
 *      ...
 *      if (!out.Full()) { // Check if buffer is full or not
 *       out.Push(msg); // Push msg to Buffer
 *      ...
 * \endcode
 * \par
 *
 */

template <typename Message, int BufferSize = 1>
class OutBuffered : public OutBlocking<Message> {
  FIFO<Message, BufferSize> fifo;
  typedef NVUINTW(nvhls::index_width<BufferSize+1>::val) AddressPlusOne;
 public:
  OutBuffered() : OutBlocking<Message>(), fifo() {}

  explicit OutBuffered(const char* name) : OutBlocking<Message>(name), fifo() {}

  virtual void Reset() {
    OutBlocking<Message>::Reset();
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

template <typename MessageType>
class Sink : public sc_module {
  SC_HAS_PROCESS(Sink);
 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<MessageType> in;

  // Constructor
  Sink(sc_module_name nm) 
    : sc_module(nm), 
      clk("clk"),
      rst("rst"),
      in("in") {
    SC_CTHREAD(DoSink, clk.pos());
    async_reset_signal_is(rst, false);    
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

template <typename MessageType>
class DummySink : public sc_module {
  SC_HAS_PROCESS(DummySink);
 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;
  In<MessageType> in;

  // Constructor
  DummySink(sc_module_name nm) 
    : sc_module(nm), 
      clk("clk"),
      rst("rst"),
      in("in") {
    SC_CTHREAD(DoSink, clk.pos());
    async_reset_signal_is(rst, false);    
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

template <typename MessageType>
class DummySource : public sc_module {
  SC_HAS_PROCESS(DummySource);
 public:
  // External interface
  sc_in_clk clk;
  sc_in<bool> rst;
  Out<MessageType> out;

  // Constructor
  DummySource(sc_module_name nm) 
    : sc_module(nm), 
      clk("clk"),
      rst("rst"),
      out("out") {
    SC_CTHREAD(DoSource, clk.pos());
    async_reset_signal_is(rst, false);    
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

}  // namespace Connections

#endif  // NVHLS_CONNECTIONS_H_
