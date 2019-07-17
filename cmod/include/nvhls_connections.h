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
#include <nvhls_packet.h>
#include <nvhls_assert.h>
#include <nvhls_marshaller.h>
#include <nvhls_module.h>
#include <fifo.h>
#include <ccs_p2p.h>

#ifndef __SYNTHESIS__
#include <iomanip>
#endif

#if defined(CONNECTIONS_ACCURATE_SIM) && defined(CONNECTIONS_FAST_SIM)
#error "Both CONNECTIONS_ACCURATE_SIM and CONNECTIONS_FAST_SIM are defined. Define one or the other."
#endif

#if !defined(__SYNTHESIS__) && (defined(CONNECTIONS_ACCURATE_SIM) || defined(CONNECTIONS_FAST_SIM))
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
#include <tlm.h>
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

// Analogous to Mentor Catapult's abstraction settings, but also used to loosely and tightly define bit ordering of ports and channels
// Note: ConManager can't be a preprocessor thing, since we run into trouble along interface boundary and VCS wrappers.
//  SYN_PORT - Marshalled and never includes ConManager.
//  MARSHALL_PORT - Marshalled and uses ConManager if not synthesis.
//  DIRECT_PORT - Marshaller disabled and no type conversion needed at interfaces, uses ConManager.
//  TLM_PORT - Like DIRECT_PORT, but interchange is through tlm_fifo. Event-based, does not use ConManager.
//
//  AUTO_PORT - TLM for normal SystemC and cosimulation (except binding to wrappers). SYN during HLS to present correct bit order.
enum connections_port_t {SYN_PORT = 0, MARSHALL_PORT = 1, DIRECT_PORT = 2, TLM_PORT=3};


// Default mapping for AUTO_PORT
/**
 * \brief Sets simulation port type
 * \ingroup Connections
 *
 * \par Set this to one of four port simulation types. From slowest (most accurate) to fastest (least accurate):
 *   - SYN_PORT: Actual SystemC modulario code given to catapult. Many wait() statements leads to simulation
 *     timing inaccuracy.
 *   - MARSHALL_PORT: Like SYN_PORT, except when CONNECTIONS_SIM_ONLY is defined will use a cycle-based simulator
 *     to time the Push() and Pop() commands, which should approximate performance of real RTL when HLS'd with a
 *     pipeline init interval of 1. All input and output msg ports are "marshalled" into a sc_lv bitvectors.
 *   - DIRECT_PORT: Like MARSHALL_PORT, except without marshalling of msg ports into sc_lv bitvector, to save
 *     on simulation time.
 *   - TLM_PORT: rdy/val/msg ports do not exist, instead a shared tlm_fifo is used for each channel and simulation is
 *     event-based through an evaluate <-> update delta cycle loop. Best performance, but not cycle accurate for complex
 *     data dependencies and ports are not readibly viewable in a waveform viewer.
 *
 * All port types can bind to SYN_PORT, so that MARSHALL_PORT, DIRECT_PORT, and TLM_PORT can be used for the SystemC code during
 * RTL + SystemC co-simulation, while the co-simulation wrapper type maintains the RTL accurate SYN_PORT type.
 *
 * Default port types if no AUTO_PORT define is given are dependent on context. During HLS, SYN_PORT is always used (except
 * when overriden by FORCE_AUTO_PORT define). During SystemC simulation, TLM_PORT is used if CONNECTIONS_SIM_ONLY is set, otherwise
 * MARSHALL_PORT is used.
 *
 * \code
 *      #define AUTO_PORT MARSHALL_PORT
 *      #include <nvhls_connections.h>
 * \endcode
 * 
 * \par
 *
 * The FORCE_AUTO_PORT define can also be used, overrides all other AUTO_PORT settings, including forcing to a different port type
 * during HLS. Useful if a specific port type is desired for targetted unit tests. Example code:
 * 
 * \code
 *      #define FORCE_AUTO_PORT MARSHALL_PORT
 *      #include <nvhls_connections.h>
 * \endcode
 *
 * \par 
 * In practice, neither of these are defined in the SystemC code, but added to CFLAGS or Catapult's Input/CompilerFlags options on
 * a per simulation or HLS run basis.
 *
 * \par
 *
 */
#ifndef AUTO_PORT 

#if defined(__SYNTHESIS__)
#define AUTO_PORT SYN_PORT
#elif !defined(CONNECTIONS_SIM_ONLY)
#define AUTO_PORT MARSHALL_PORT
#else

// Switch if we are in CONNECTIONS_SIM_ONLY
#if defined(CONNECTIONS_FAST_SIM)
#define AUTO_PORT TLM_PORT
#else
#define AUTO_PORT MARSHALL_PORT
#endif
 
#endif // defined(__SYNTHESIS__)
 
#endif // ifndef AUTO_PORT


// If __SYNTHESIS__ always force SYN_PORT, even if
// manually defined in a Makefile or go_hls.tcl.
#if defined(__SYNTHESIS__)
#undef AUTO_PORT
#define AUTO_PORT SYN_PORT
#endif // defined(__SYNTHESIS__)

/**
 * \brief Forces simulation port type
 * \ingroup Connections
 *
 * \par See AUTO_PORT macro. This 
 * \par
 *
 */

// FORCE_AUTO_PORT has precedence, even over __SYNTHESIS__
#if defined(FORCE_AUTO_PORT)
#undef AUTO_PORT
#define AUTO_PORT FORCE_AUTO_PORT
#endif // defined(FORCE_AUTO_PORT)

 
 
// Forward declarations
// These represent what SystemC calls "Ports" (which are basically endpoints)
template <typename Message>
class InBlocking_abs;
template <typename Message>
class OutBlocking_abs;
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class InBlocking;
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class OutBlocking;
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class In;
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class Out;
// These represent what SystemC calls "Channels" (which are connections)
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class Combinational;
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class Bypass;
template <typename Message, connections_port_t port_marshall_type = AUTO_PORT>
class Pipeline;
template <typename Message, unsigned int NumEntries, connections_port_t port_marshall_type = AUTO_PORT>
class Buffer;

class Connections_BA_abs;
}

// Declarations used in the Marshaller
SpecialWrapperIfc(Connections::In);
SpecialWrapperIfc(Connections::Out);

namespace Connections {

#ifdef CONNECTIONS_SIM_ONLY

// Always enable __CONN_RAND_STALL_FEATURE, but stalling itself is
// disabled by default unless explicitly turned on by the variable
// rand_stall_enable() or by CONN_RAND_STALL
#define __CONN_RAND_STALL_FEATURE
  
#ifdef __CONN_RAND_STALL_FEATURE
#ifdef CONN_RAND_STALL
  bool rand_stall_enable = true;
#else
  bool rand_stall_enable = false;
#endif // ifdef CONN_RAND_STALL
#endif // ifdef __CONN_RAND_STALL_FEATURE

#ifdef __CONN_RAND_STALL_FEATURE
/**
 * \brief Enable global random stalling support.
 * \ingroup Connections
 *
 * Enable random stalling globally, valid in CONNECTIONS_ACCURATE_SIM and
 * CONNECTIONS_FAST_SIM modes. Random stalling is non engaged by default,
 * unless CONN_RAND_STALL is set.
 *
 * Random stalling is used to randomly stall In<> ports, creating random
 * back pressure in a design to assist catching latency-sentitive bugs.
 * 
 * Can also enable Connections::In<>.enable_local_rand_stall() to enable
 * on a per-port basis.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      int sc_main(int argc, char *argv[])
 *      {
 *      ...
 *      Connections::enable_global_rand_stall();
 *      ...
 *      }
 * \endcode
 * \par
 *
 */
  void enable_global_rand_stall() {
    rand_stall_enable = true;
  }

/**
 * \brief Disable global random stalling support.
 * \ingroup Connections
 *
 * Disable random stalling globally, valid in CONNECTIONS_ACCURATE_SIM and
 * CONNECTIONS_FAST_SIM modes. Random stalling is non engaged by default,
 * unless CONN_RAND_STALL is set.
 *
 * Random stalling is used to randomly stall In<> ports, creating random
 * back pressure in a design to assist catching latency-sentitive bugs.
 * 
 * Can also disable Connections::In<>.disable_local_rand_stall() to disable
 * on a per-port basis.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_connections.h>
 *
 *      int sc_main(int argc, char *argv[])
 *      {
 *      ...
 *      Connections::disable_global_rand_stall();
 *      ...
 *      }
 * \endcode
 * \par
 *
 */
  void disable_global_rand_stall() {
    rand_stall_enable = false;
  }
#endif
  
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
    std::vector<Connections_BA_abs*> tracked_annotate;
  
    void add(Blocking_abs* c)
    {
        tracked.push_back(c);
    }
    
    void add_annotate(Connections_BA_abs* c)
    {
        tracked_annotate.push_back(c);
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

        NVHLS_ASSERT(0);
    }

    void remove_annotate(Connections_BA_abs* c)
    {
        for (std::vector<Connections_BA_abs*>::iterator it = tracked_annotate.begin(); it!=tracked_annotate.end(); ++it)
        {
            if (*it == c) 
            {
                tracked_annotate.erase(it);
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
// InBlocking MARSHALL_PORT
//------------------------------------------------------------------------

// For safety, disallow DIRECT_PORT <-> MARSHALL_PORT binding helpers during HLS.
#ifndef __SYNTHESIS__
 
template <typename Message>
SC_MODULE(MarshalledToDirectOutPort) {
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_signal<MsgBits> msgbits;
  sc_out<Message> msg;
  sc_in<bool> val;
  
  void do_marshalled2direct() {
    if(val) {
      MsgBits mbits = msgbits.read();
      Marshaller<WMessage::width> marshaller(mbits);
      WMessage result;
      result.Marshall(marshaller);
      msg.write(result.val);
    }
  }
  
  SC_CTOR(MarshalledToDirectOutPort) {
    SC_METHOD(do_marshalled2direct);
    sensitive << msgbits;
    sensitive << val;
  }
};

template <typename Message>
SC_MODULE(MarshalledToDirectInPort) {
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in<MsgBits> msgbits;
  sc_in<bool> val;
  sc_signal<Message> msg;

  void do_marshalled2direct() {
    if(val) {
      MsgBits mbits = msgbits.read();
      Marshaller<WMessage::width> marshaller(mbits);
      WMessage result;
      result.Marshall(marshaller);
      msg.write(result.val);
    }
  }
  
  SC_CTOR(MarshalledToDirectInPort) {
    SC_METHOD(do_marshalled2direct);
    sensitive << msgbits;
    sensitive << val;
  }
};
 
template <typename Message>
SC_MODULE(DirectToMarshalledInPort) {
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in<Message> msg;
  sc_signal<MsgBits> msgbits;

  void do_direct2marshalled() {
    Marshaller<WMessage::width> marshaller;
    //WMessage wm(msg);
    WMessage wm(msg.read());
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msgbits.write(bits);
  }
  
  SC_CTOR(DirectToMarshalledInPort) {
    SC_METHOD(do_direct2marshalled);
    sensitive << msg;
  }
};

template <typename Message>
SC_MODULE(DirectToMarshalledOutPort) {
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_signal<Message> msg;
  sc_out<MsgBits> msgbits;

  void do_direct2marshalled() {
    Marshaller<WMessage::width> marshaller;
    WMessage wm(msg);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msgbits.write(bits);
  }
  
  SC_CTOR(DirectToMarshalledOutPort) {
    SC_METHOD(do_direct2marshalled);
    sensitive << msg;
  }
};


// TLM
#if defined(CONNECTIONS_SIM_ONLY)

template <typename Message>
class TLMToDirectOutPort : public Blocking_abs {
public:
  sc_out<bool> val;
  sc_in<bool> rdy;
  sc_out<Message> msg;
  
  // Default constructor
  explicit TLMToDirectOutPort(tlm::tlm_fifo<Message> &fifo)
      : val(sc_gen_unique_name("out_val")),
        rdy(sc_gen_unique_name("out_rdy")),
        msg(sc_gen_unique_name("out_msg")) {
    
    Init_SIM(sc_gen_unique_name("out", fifo));
    
  }

  // Constructor
  explicit TLMToDirectOutPort(const char* name, tlm::tlm_fifo<Message> &fifo)
      : val(nvhls_concat(name, "val")),
        rdy(nvhls_concat(name, "rdy")),
	msg(nvhls_concat(name, "msg")) {
    Init_SIM(name, fifo);
  }

protected:
  // Protected member variables
  tlm::tlm_fifo<Message> *fifo;

  // Initializer
  void Init_SIM(const char* name, tlm::tlm_fifo<Message> &fifo) {
    this->fifo = &fifo;
    conManager.add(this);
  }

  // Blocking_abs functions
  bool Pre() {
    if (val.read() && rdy.read()) {
      NVHLS_ASSERT(fifo->nb_can_get());
      fifo->get(); // Discard, we've already peeked it. Just incrementing the head here.
    }
    return true;
  }

  bool Post() { 
    if(fifo->nb_can_peek()) {
      val.write(true);
      msg.write(fifo->peek());
    } else {
      Message blank_m;
      val.write(false);
      msg.write(blank_m);
    }
    return true;
  }
};

template <typename Message>
class DirectToTLMInPort : public Blocking_abs {
public:
  sc_in<bool> val;
  sc_out<bool> rdy;
  sc_in<Message> msg;
  
  // Default constructor
  explicit DirectToTLMInPort(tlm::tlm_fifo<Message> &fifo)
      : val(sc_gen_unique_name("in_val")),
        rdy(sc_gen_unique_name("in_rdy")),
        msg(sc_gen_unique_name("in_msg")) {
    
    Init_SIM(sc_gen_unique_name("in", fifo));
    
  }

  // Constructor
  explicit DirectToTLMInPort (const char* name, tlm::tlm_fifo<Message> &fifo)
      : val(nvhls_concat(name, "val")),
        rdy(nvhls_concat(name, "rdy")),
	msg(nvhls_concat(name, "msg")) {
    Init_SIM(name, fifo);
  }

protected:
  // Protected member variables
  tlm::tlm_fifo<Message> *fifo;

  // Initializer
  void Init_SIM(const char* name, tlm::tlm_fifo<Message> &fifo) {
    this->fifo = &fifo;
    conManager.add(this);
  }
  
  bool Pre() {
    if (rdy.read() && val.read()) {
      Message data;
      data = msg.read();
      NVHLS_ASSERT(fifo->nb_can_put());
      fifo->put(data);
    }
    return true;
  }

  bool Post() {
    if(fifo->nb_can_put()) {
      rdy.write(true);
    } else {
      rdy.write(false);
    }
    return true;
  }
};

#endif // defined(CONNECTIONS_SIM_ONLY)
 
#endif // __SYNTHESIS__
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Message>
#ifdef CONNECTIONS_SIM_ONLY
class InBlocking_abs : public Blocking_abs {
#else
class InBlocking_abs {
#endif
 public:

  // Protected because generic abstract class
 protected:
  
  // Default constructor
 InBlocking_abs()
#ifdef CONNECTONS_SIM_ONLY
   : Blocking_abs()
#endif
    {}

  // Constructor
  explicit InBlocking_abs(const char* name)
#ifdef CONNECTONS_SIM_ONLY
    : Blocking_abs()
#endif
    {}
  
 public:
  // Reset read
  virtual void Reset() {
    NVHLS_ASSERT(0);
  }

// Pop
#pragma design modulario < in >
  virtual Message Pop() {
    Message m;
    NVHLS_ASSERT(0);
    return m;
  }

// Peek
#pragma design modulario < in >
  virtual Message Peek() {
    Message m;
    NVHLS_ASSERT(0);
    return m;
  }

// PopNB
#pragma design modulario < in >
  virtual bool PopNB(Message& data, const bool& do_wait = true) {
    NVHLS_ASSERT(0);
    return false;
  }
};

 
template <typename Message>
class InBlocking_Ports_abs : public InBlocking_abs<Message> {
 public:
  // Interface
  sc_in<bool> val;
  sc_out<bool> rdy;

  // Protected because generic
 protected:
  // Default constructor
  InBlocking_Ports_abs()
    : InBlocking_abs<Message>(),
    val(sc_gen_unique_name("in_val")),
    rdy(sc_gen_unique_name("in_rdy"))
      {}

  // Constructor
  explicit InBlocking_Ports_abs(const char* name)
    : InBlocking_abs<Message>(name),
    val(nvhls_concat(name, "val")),
    rdy(nvhls_concat(name, "rdy"))
      {}

 public:
  // Reset read
  virtual void Reset() {
    rdy.write(false);
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    do {
      rdy.write(true);
      wait();
    } while (val.read() != true);
    rdy.write(false);

    Message m;
    read_msg(m);
    return m;
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    QUERY_CALL();
    while (!val.read()) {
      wait();
    }
    
    Message m;
    read_msg(m);
    return m;
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
    rdy.write(true);
    if (do_wait) {
      wait();
      rdy.write(false);
    }
    read_msg(data);
    return val.read();
  }
  
 protected:
  virtual void read_msg(Message &m) {
    NVHLS_ASSERT(0);
  }
};


template <typename Message>
class InBlocking_SimPorts_abs : public InBlocking_Ports_abs<Message> {
 public:

  // Protected because generic
 protected:
  // Default constructor
  InBlocking_SimPorts_abs()
    : InBlocking_Ports_abs<Message>()
    {
#ifdef CONNECTIONS_SIM_ONLY
      Init_SIM(sc_gen_unique_name("in"));
#endif
    }

  // Constructor
  explicit InBlocking_SimPorts_abs(const char* name)
      : InBlocking_Ports_abs<Message>(name)
    {
#ifdef CONNECTIONS_SIM_ONLY
      Init_SIM(name);
#endif
    }

 public:
  // Reset read
  void Reset() {
#ifdef CONNECTIONS_SIM_ONLY
    Reset_SIM();
#else
    InBlocking_Ports_abs<Message>::Reset();
#endif
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
#ifdef CONNECTIONS_SIM_ONLY
    return Pop_SIM();
#else
    return InBlocking_Ports_abs<Message>::Pop();
#endif
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    return InBlocking_Ports_abs<Message>::Peek();
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
    return InBlocking_Ports_abs<Message>::PopNB(data, do_wait);
#endif
  }
  
#ifdef CONNECTIONS_SIM_ONLY
  void disable_spawn()
  {
    conManager.remove(this);
  }

  
#ifdef __CONN_RAND_STALL_FEATURE
  /**
   * \brief Enable random stalling support on an input port.
   * \ingroup Connections
   *
   * Enable random stalling locally for an input port, valid in CONNECTIONS_ACCURATE_SIM
   * and CONNECTIONS_FAST_SIM modes. Random stalling is non engaged by default,
   * unless CONN_RAND_STALL is set.
   *
   * Random stalling is used to randomly stall In<> ports, creating random
   * back pressure in a design to assist catching latency-sentitive bugs.
   * 
   * Can also enable globally with enable_global_rand_stall(). This command takes
   * precendence over the global setting.
   *
   * \par A Simple Example
   * \code
   *      #include <nvhls_connections.h>
   *
   *      int sc_main(int argc, char *argv[])
   *      {
   *      ...
   *      my_testbench.dut.my_input_port.enable_local_rand_stall();
   *      ...
   *      }
   * \endcode
   * \par
   *
   */
  void enable_local_rand_stall() {
    local_rand_stall_override = true;
    local_rand_stall_enable = true;
  }

  /**
   * \brief Disable random stalling support on an input port.
   * \ingroup Connections
   *
   * Disable random stalling locally for an input port, valid in CONNECTIONS_ACCURATE_SIM
   * and CONNECTIONS_FAST_SIM modes. Random stalling is non engaged by default,
   * unless CONN_RAND_STALL is set.
   *
   * Random stalling is used to randomly stall In<> ports, creating random
   * back pressure in a design to assist catching latency-sentitive bugs.
   * 
   * Can also disable globally with disable_global_rand_stall(). This command takes
   * precendence over the global setting.
   *
   * \par A Simple Example
   * \code
   *      #include <nvhls_connections.h>
   *
   *      int sc_main(int argc, char *argv[])
   *      {
   *      ...
   *      enable_global_rand_stall();
   *      my_testbench.dut.my_input_port.disable_local_rand_stall();
   *      ...
   *      }
   * \endcode
   * \par
   *
   */
  void disable_local_rand_stall() {
    local_rand_stall_override = true;
    local_rand_stall_enable = false;
  }

  /**
   * \brief Remove a random stall setting on an input port.
   * \ingroup Connections
   *
   * Removes a random stall setting for an input port, valid in CONNECTIONS_ACCURATE_SIM
   * and CONNECTIONS_FAST_SIM modes. Effectively cancels enable_local_rand_stall() and
   * disable_local_rand_stall(), reverting to enable_global_rand_stall() or disable_global_rand_stall()
   * settings. 
   *
   * \par A Simple Example
   * \code
   *      #include <nvhls_connections.h>
   *
   *      int sc_main(int argc, char *argv[])
   *      {
   *      ...
   *      // globally enable rand stalling
   *      enable_global_rand_stall();
   *      // locally disable rand stalling
   *      my_testbench.dut.my_input_port.disable_local_rand_stall();
   *      ...
   *      // cancel local rand stall directive, re-enabling global setting
   *      // on this port
   *      my_testbench.dut.my_input_port.cancel_local_rand_stall();
   *      }
   * \endcode
   * \par
   *
   */
  void cancel_local_rand_stall() {
    local_rand_stall_override = false;
  }
#endif // __CONN_RAND_STALL_FEATURE

 protected:
  Message data_buf;
  bool data_val;
  bool rdy_set_by_api;
#ifdef __CONN_RAND_STALL_FEATURE
  Pacer *post_pacer;
  bool pacer_stall;
  bool local_rand_stall_override;
  bool local_rand_stall_enable;
#endif

  void Init_SIM(const char* name) {
    data_val = false;
    rdy_set_by_api = false;
    conManager.add(this);
#ifdef __CONN_RAND_STALL_FEATURE
    double x = rand()%100;
    double y = rand()%100;
    post_pacer = new Pacer(x/100, y/100);
    pacer_stall = false;
    local_rand_stall_override = false;
    local_rand_stall_enable = false;
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
      this->rdy.write(false);
      rdy_set_by_api = false;
    } else {
      this->rdy.write(true);
      rdy_set_by_api = true;
    }
  }

#pragma design modulario < in >
  bool received(Message& data) {
    if (this->val.read())
    {
    read_msg(data);
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
#ifdef __CONN_RAND_STALL_FEATURE
    if ((local_rand_stall_override ? local_rand_stall_enable : rand_stall_enable) && pacer_stall) return true;
#endif
    if (rdy_set_by_api != this->rdy.read())
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

#ifdef __CONN_RAND_STALL_FEATURE
  bool Post() {
    if((local_rand_stall_override ? local_rand_stall_enable : rand_stall_enable)) {
      if (post_pacer->tic()) 
	{
	  pacer_stall=true;
	} else {
        pacer_stall=false;
      }
    } else {
      pacer_stall = false;
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
    NVHLS_ASSERT(data_val);
    data_val = false;
    return data_buf;
  }

  Message& Pop_SIM() {
    while (Empty_SIM()) {
      wait();
    }
    return ConsumeBuf_SIM();
  }
#endif // CONNECTIONS_SIM_ONLY
  
 protected:
  virtual void read_msg(Message &m) {
    NVHLS_ASSERT(0);
  }
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Specializations of In port for marshall vs direct port
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Message>
class InBlocking <Message, SYN_PORT> : public InBlocking_Ports_abs<Message> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in<MsgBits> msg;

  InBlocking() : InBlocking_Ports_abs<Message>(),
                 msg(sc_gen_unique_name("in_msg")) {}
  
  explicit InBlocking(const char* name) : InBlocking_Ports_abs<Message>(name),
                                          msg(nvhls_concat(name, "msg")) {}

// Pop
#pragma design modulario < in >
  Message Pop() {
    return InBlocking_Ports_abs<Message>::Pop();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    return InBlocking_Ports_abs<Message>::Peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
    return InBlocking_Ports_abs<Message>::PopNB(data, do_wait);
  }

  // Bind to InBlocking
  void Bind(InBlocking<Message, SYN_PORT>& rhs) {
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message, SYN_PORT>& rhs) {
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to InBlocking
  void Bind(InBlocking<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    this->msg(rhs.out_msg);
    this->val(rhs.out_val);
    this->rdy(rhs.out_rdy);
    rhs.out_bound = true;
    rhs.out_ptr = this;
#else
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }
  
// Be safe: disallow DIRECT_PORT <-> SYN_PORT binding during HLS
#ifndef __SYNTHESIS__  
  void Bind(InBlocking<Message, DIRECT_PORT>& rhs) {
    DirectToMarshalledInPort<Message> *dynamic_d2mport;

    dynamic_d2mport = new DirectToMarshalledInPort<Message>(sc_gen_unique_name("dynamic_d2mport"));
    dynamic_d2mport->msg(rhs.msg);
    this->msg(dynamic_d2mport->msgbits);

#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(dynamic_d2mport->msgbits);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  void Bind(Combinational<Message, DIRECT_PORT>& rhs) {
    DirectToMarshalledInPort<Message> *dynamic_d2mport;

    dynamic_d2mport = new DirectToMarshalledInPort<Message>(sc_gen_unique_name("dynamic_d2mport"));
    this->msg(dynamic_d2mport->msgbits);

#ifdef CONNECTIONS_SIM_ONLY
    dynamic_d2mport->msg(rhs.out_msg);
    this->val(rhs.out_val);
    this->rdy(rhs.out_rdy);
    rhs.out_bound = true;
    rhs.out_ptr = this;
#else
    dynamic_d2mport->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
  void Bind(Combinational<Message, TLM_PORT>& rhs) {
    TLMToDirectOutPort<Message> *dynamic_tlm2d_port;
    Combinational<Message, DIRECT_PORT> *dynamic_comb;

    dynamic_tlm2d_port = new TLMToDirectOutPort<Message>(sc_gen_unique_name("dynamic_tlm2d_port"), rhs.fifo);
    dynamic_comb = new Combinational<Message, DIRECT_PORT>(sc_gen_unique_name("dynamic_comb"));

    // Bind the marshaller to combinational
    this->Bind(*dynamic_comb);
    dynamic_tlm2d_port->val(dynamic_comb->in_val);
    dynamic_tlm2d_port->rdy(dynamic_comb->in_rdy);
    dynamic_tlm2d_port->msg(dynamic_comb->in_msg);

    // Bound but no ptr indicates a cosim interface
    dynamic_comb->in_bound = true;
    dynamic_comb->in_ptr = 0;
  }
#endif // CONNECTIONS_SIM_ONLY

#endif // __SYNTHESIS__

  // Bind to p2p<>::in
  void Bind(p2p<SYN>::in<Message>& rhs) {
    this->msg(rhs.i_dat);
    this->val(rhs.i_vld);
    this->rdy(rhs.o_rdy);
  }

  // Bind to p2p<>::chan
  void Bind(p2p<SYN>::chan<Message>& rhs) {
    this->msg(rhs.dat);
    this->val(rhs.vld);
    this->rdy(rhs.rdy);
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }

 protected:
  void read_msg(Message &m) {
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    m = result.val;
  }
};  


template <typename Message>
class InBlocking <Message, MARSHALL_PORT> : public InBlocking_SimPorts_abs<Message> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_in<MsgBits> msg;

  InBlocking() : InBlocking_SimPorts_abs<Message>(),
    msg(sc_gen_unique_name("in_msg")) {}
  
  explicit InBlocking(const char* name) : InBlocking_SimPorts_abs<Message>(name),
    msg(nvhls_concat(name, "msg")) {}

  // Reset read
  void Reset() {
    InBlocking_SimPorts_abs<Message>::Reset();
  }
  
  // Pop
#pragma design modulario < in >
  Message Pop() {
    return InBlocking_SimPorts_abs<Message>::Pop();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    return InBlocking_SimPorts_abs<Message>::Peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
    return InBlocking_SimPorts_abs<Message>::PopNB(data, do_wait);
  }
  
  // Bind to InBlocking
  void Bind(InBlocking<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    this->msg(rhs.out_msg);
    this->val(rhs.out_val);
    this->rdy(rhs.out_rdy);
    rhs.out_bound = true;
    rhs.out_ptr = this;
#else
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

  // Bind to InBlocking
  void Bind(InBlocking<Message, SYN_PORT>& rhs) {
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message, SYN_PORT>& rhs) {
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }
  
// Be safe: disallow DIRECT_PORT <-> MARSHALL_PORT binding during HLS
#ifndef __SYNTHESIS__  
  void Bind(InBlocking<Message, DIRECT_PORT>& rhs) {
    DirectToMarshalledInPort<Message> *dynamic_d2mport;

    dynamic_d2mport = new DirectToMarshalledInPort<Message>(sc_gen_unique_name("dynamic_d2mport"));
    dynamic_d2mport->msg(rhs.msg);
    this->msg(dynamic_d2mport->msgbits);

#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(dynamic_d2mport->msgbits);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  void Bind(Combinational<Message, DIRECT_PORT>& rhs) {
    DirectToMarshalledInPort<Message> *dynamic_d2mport;

    dynamic_d2mport = new DirectToMarshalledInPort<Message>(sc_gen_unique_name("dynamic_d2mport"));
    this->msg(dynamic_d2mport->msgbits);

#ifdef CONNECTIONS_SIM_ONLY
    dynamic_d2mport->msg(rhs.out_msg);
    this->val(rhs.out_val);
    this->rdy(rhs.out_rdy);
    rhs.out_bound = true;
    rhs.out_ptr = this;
#else
    dynamic_d2mport->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
  void Bind(Combinational<Message, TLM_PORT>& rhs) {
    TLMToDirectOutPort<Message> *dynamic_tlm2d_port;
    Combinational<Message, DIRECT_PORT> *dynamic_comb;

    dynamic_tlm2d_port = new TLMToDirectOutPort<Message>(sc_gen_unique_name("dynamic_tlm2d_port"), rhs.fifo);
    dynamic_comb = new Combinational<Message, DIRECT_PORT>(sc_gen_unique_name("dynamic_comb"));

    // Bind the marshaller to combinational
    this->Bind(*dynamic_comb);
    dynamic_tlm2d_port->val(dynamic_comb->in_val);
    dynamic_tlm2d_port->rdy(dynamic_comb->in_rdy);
    dynamic_tlm2d_port->msg(dynamic_comb->in_msg);

    // Bound but no ptr indicates a cosim interface
    dynamic_comb->in_bound = true;
    dynamic_comb->in_ptr = 0;
  }
#endif // CONNECTIONS_SIM_ONLY

#endif // __SYNTHESIS__
  
  // Bind to p2p<>::in
  void Bind(p2p<SYN>::in<Message>& rhs) {
    this->msg(rhs.i_dat);
    this->val(rhs.i_vld);
    this->rdy(rhs.o_rdy);
  }

  // Bind to p2p<>::chan
  void Bind(p2p<SYN>::chan<Message>& rhs) {
    this->msg(rhs.dat);
    this->val(rhs.vld);
    this->rdy(rhs.rdy);
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }

 protected:
  void read_msg(Message &m) {
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    m = result.val;
  }
};  

template <typename Message>
class InBlocking <Message, DIRECT_PORT> : public InBlocking_SimPorts_abs<Message> {
 public:
  // Interface
  sc_in<Message> msg;

 InBlocking() : InBlocking_SimPorts_abs<Message>(),
    msg(sc_gen_unique_name("in_msg"))
      {}
  
  explicit InBlocking(const char* name) : InBlocking_SimPorts_abs<Message>(name),
    msg(nvhls_concat(name, "msg"))
      {}
  
  // Reset read
  void Reset() {
    InBlocking_SimPorts_abs<Message>::Reset();
  }
  
  // Pop
#pragma design modulario < in >
  Message Pop() {
    return InBlocking_SimPorts_abs<Message>::Pop();
  }

  // Peek
#pragma design modulario < in >
  Message Peek() {
    return InBlocking_SimPorts_abs<Message>::Peek();
  }

  // PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
    return InBlocking_SimPorts_abs<Message>::PopNB(data, do_wait);
  }
  
  // Bind to InBlocking
  void Bind(InBlocking<Message, DIRECT_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational
  void Bind(Combinational<Message, DIRECT_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    this->msg(rhs.out_msg);
    this->val(rhs.out_val);
    this->rdy(rhs.out_rdy);
    rhs.out_bound = true;
    rhs.out_ptr = this;
#else
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }
  
 protected:
  void read_msg(Message &m) {
    m = msg.read();
  }
};  


#ifdef CONNECTIONS_SIM_ONLY

template <typename Message>
class InBlocking <Message, TLM_PORT> : public InBlocking_abs<Message> {
 public:
  // Default constructor
 InBlocking() : InBlocking_abs<Message>(),
    i_fifo(sc_gen_unique_name("i_fifo"))
      {
#ifdef __CONN_RAND_STALL_FEATURE
	Init_SIM(sc_gen_unique_name("in"));
#endif	
      }
  
  explicit InBlocking(const char* name) : InBlocking_abs<Message>(name),
    i_fifo(nvhls_concat(name, "i_fifo"))
      {
#ifdef __CONN_RAND_STALL_FEATURE
	Init_SIM(name);
#endif
      }
  
  // Reset read
  void Reset() {
    Message temp;
    while (i_fifo->nb_get(temp));
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
#ifdef __CONN_RAND_STALL_FEATURE
    while((local_rand_stall_override ? local_rand_stall_enable : rand_stall_enable) && post_pacer->tic()) { wait(); }
#endif
    return i_fifo->get();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    return i_fifo->peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
#ifdef __CONN_RAND_STALL_FEATURE
    if((local_rand_stall_override ? local_rand_stall_enable : rand_stall_enable) && post_pacer->tic()) { return false; }
#endif
    return i_fifo->nb_get(data);
  }
  
  // Bind to InBlocking
  void Bind(InBlocking<Message, TLM_PORT>& rhs) {
    this->i_fifo(rhs.i_fifo);
  }

  // Bind to Combinational
  void Bind(Combinational<Message, TLM_PORT>& rhs) {
    this->i_fifo(rhs.fifo);
  }

  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }
  
#ifdef __CONN_RAND_STALL_FEATURE
  void enable_local_rand_stall() {
    local_rand_stall_override = true;
    local_rand_stall_enable = true;
  }

  void disable_local_rand_stall() {
    local_rand_stall_override = true;
    local_rand_stall_enable = false;
  }

  void cancel_local_rand_stall() {
    local_rand_stall_override = false;
  }
#endif // __CONN_RAND_STALL_FEATURE
  
 protected:
  sc_port<tlm::tlm_fifo_get_if<Message> > i_fifo;
  
#ifdef __CONN_RAND_STALL_FEATURE
  Pacer *post_pacer;
  bool local_rand_stall_override;
  bool local_rand_stall_enable;
  
  void Init_SIM(const char* name) {
    double x = rand()%100;
    double y = rand()%100;
    post_pacer = new Pacer(x/100, y/100);
    local_rand_stall_override = false;
    local_rand_stall_enable = false;
  }
#endif
  
 protected:
  void read_msg(Message &m) {
    m = i_fifo->peek();
  }
};

#endif
 
//------------------------------------------------------------------------
// In
//------------------------------------------------------------------------

template <typename Message>
class In<Message, SYN_PORT> : public InBlocking<Message, SYN_PORT> {
 public:
  In() {}

  explicit In(const char* name) : InBlocking<Message, SYN_PORT>(name) {}

  // Empty
  bool Empty() {
    QUERY_CALL();
    return !this->val.read();
  }
};

template <typename Message>
class In<Message, MARSHALL_PORT> : public InBlocking<Message, MARSHALL_PORT> {
 public:
  In() {}

  explicit In(const char* name) : InBlocking<Message, MARSHALL_PORT>(name) {}

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

template <typename Message>
class In<Message, DIRECT_PORT> : public InBlocking<Message, DIRECT_PORT> {
 public:
  In() {}

  explicit In(const char* name) : InBlocking<Message, DIRECT_PORT>(name) {}

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

#ifdef CONNECTIONS_SIM_ONLY
template <typename Message>
class In<Message, TLM_PORT> : public InBlocking<Message, TLM_PORT> {
 public:
  In() {}

  explicit In(const char* name) : InBlocking<Message, TLM_PORT>(name) {}

  // Empty
  bool Empty() {
    return ! this->i_fifo->nb_can_get();
  }
};
#endif
 
 
//------------------------------------------------------------------------
// OutBlocking_abs MARSHALL_PORT
//------------------------------------------------------------------------

template <typename Message>
#ifdef CONNECTIONS_SIM_ONLY
class OutBlocking_abs : public Blocking_abs {
#else 
class OutBlocking_abs {
#endif
 public:
  
  // Protected because abstract class
 protected:
  // Default constructor
 OutBlocking_abs()
#ifdef CONNECTIONS_SIM_ONLY
   : Blocking_abs()
#endif
    {}
  
  // Constructor
  explicit OutBlocking_abs(const char* name)
#ifdef CONNECTIONS_SIM_ONLY
    : Blocking_abs()
#endif
    {}
  
 public:

  // Reset write
  void Reset() {
    NVHLS_ASSERT(0);
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    NVHLS_ASSERT(0);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    NVHLS_ASSERT(0);
    return false;
  }
};


 
template <typename Message>
class OutBlocking_Ports_abs : public OutBlocking_abs<Message> {
 public:
  // Interface
  sc_out<bool> val;
  sc_in<bool> rdy;

  // Protected because abstract class
 protected:
  // Default constructor
 OutBlocking_Ports_abs()
   : OutBlocking_abs<Message>(),
    val(sc_gen_unique_name("out_val")),
    rdy(sc_gen_unique_name("out_rdy"))
      {}
  
  // Constructor
  explicit OutBlocking_Ports_abs(const char* name)
    : OutBlocking_abs<Message>(name),
    val(nvhls_concat(name, "val")),
    rdy(nvhls_concat(name, "rdy"))
      {}
  
 public:

  // Reset write
  void Reset() {
    val.write(false);
    reset_msg();
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    do {
      val.write(true);
      write_msg(m);
      wait();
    } while (rdy.read() != true);
    val.write(false);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    val.write(true);
    write_msg(m);
    //if (do_wait) {
      wait();
      val.write(false);
      //MsgBits dc_bits;
      //msg.write(dc_bits);
      invalidate_msg();
      //}
    return rdy.read();
  }

 protected:
    virtual void reset_msg() {
      NVHLS_ASSERT(0);
    }
    virtual void write_msg(const Message &m) {
      NVHLS_ASSERT(0);
    }
    virtual void invalidate_msg() {
      NVHLS_ASSERT(0);
    }
};


template <typename Message>
class OutBlocking_SimPorts_abs : public OutBlocking_Ports_abs<Message> {
 public:
  
  // Protected because abstract class
 protected:
  // Default constructor
 OutBlocking_SimPorts_abs()
   : OutBlocking_Ports_abs<Message>()
    {
#ifdef CONNECTIONS_SIM_ONLY
      Init_SIM(sc_gen_unique_name("out"));
#endif
    }

  // Constructor
  explicit OutBlocking_SimPorts_abs(const char* name)
    : OutBlocking_Ports_abs<Message>(name)
    {
#ifdef CONNECTIONS_SIM_ONLY
      Init_SIM(name);
#endif
    }

 public:

  // Reset write
  void Reset() {
#ifdef CONNECTIONS_SIM_ONLY
    Reset_SIM();
#else
    OutBlocking_Ports_abs<Message>::Reset();
#endif
  }

  // Push
#pragma design modulario < out >
  void Push(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    return Push_SIM(m);
#else
    OutBlocking_Ports_abs<Message>::Push(m);
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
    return OutBlocking_Ports_abs<Message>::PushNB(m, do_wait);
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
    this->reset_msg();
    data_val = false;
  }

// Although this code is being used only for simulation now, it could be
// synthesized if pre/post are used instead of spawned threads.
// Keep mudulario pragmas here to preserve that option

#pragma design modulario < in >
  bool transmitted() { return this->rdy.read(); }

#pragma design modulario < out >
  void transmit_data(const Message& m) {
    this->write_msg(m);
  }

#pragma design modulario < out >
  void transmit_val(const bool& vald) {
    if (vald) {
      this->val.write(true);
      val_set_by_api = true;
    } else {
      this->val.write(false);
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
    if (val_set_by_api != this->val.read())
    {
        // something has changed the value of the signal not through API
        // killing spawned threads;
        return false;
    }
    transmit_val(data_val); 
    return true;
  }

  void FillBuf_SIM(const Message& m) {
    NVHLS_ASSERT(!data_val);
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

 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Specializations of Out port for marshall vs direct port
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Message>
class OutBlocking <Message, SYN_PORT> : public OutBlocking_Ports_abs<Message> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_out<MsgBits> msg;

  OutBlocking() : OutBlocking_Ports_abs<Message>(),
                 msg(sc_gen_unique_name("out_msg")) {}
  
  explicit OutBlocking(const char* name) : OutBlocking_Ports_abs<Message>(name),
    msg(nvhls_concat(name, "msg")) {}

  // Reset write
  virtual void Reset() {
    OutBlocking_Ports_abs<Message>::Reset();
  }
  
  // Push
#pragma design modulario < out >
  void Push(const Message& m) {
    OutBlocking_Ports_abs<Message>::Push(m);
  }

  // PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    return OutBlocking_Ports_abs<Message>::PushNB(m,do_wait);
  }

  // Bind to OutBlocking
  void Bind(OutBlocking<Message, SYN_PORT>& rhs) {
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational Marshall Port
  void Bind(Combinational<Message, SYN_PORT>& rhs) {
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }
  
  // Bind to OutBlocking
  void Bind(OutBlocking<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational Marshall Port
  void Bind(Combinational<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    this->msg(rhs.in_msg);
    this->val(rhs.in_val);
    this->rdy(rhs.in_rdy);
    rhs.in_bound = true;
    rhs.in_ptr = this;
#else
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

  // For safety disallow DIRECT_PORT <-> SYN_PORT binding during HLS
#ifndef __SYNTHESIS__  
  void Bind(OutBlocking<Message, DIRECT_PORT>& rhs) {
    MarshalledToDirectOutPort<Message> *dynamic_m2dport;
    dynamic_m2dport = new MarshalledToDirectOutPort<Message>(sc_gen_unique_name("dynamic_m2dport"));

#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    
    this->msg(dynamic_m2dport->msgbits);
    dynamic_m2dport->msg(rhs.msg);
    dynamic_m2dport->val(rhs.val);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  void Bind(Combinational<Message, DIRECT_PORT>& rhs) {
    MarshalledToDirectOutPort<Message> *dynamic_m2dport;

    dynamic_m2dport = new MarshalledToDirectOutPort<Message>(sc_gen_unique_name("dynamic_m2dport"));
    this->msg(dynamic_m2dport->msgbits);
    
#ifdef CONNECTIONS_SIM_ONLY
    dynamic_m2dport->msg(rhs.in_msg);
    dynamic_m2dport->val(rhs.in_val);
    this->val(rhs.in_val);
    this->rdy(rhs.in_rdy);
    rhs.in_bound = true;
    rhs.in_ptr = this;
#else
    dynamic_m2dport->msg(rhs.msg);
    dynamic_m2dport->val(rhs.val);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
  void Bind(Combinational<Message, TLM_PORT>& rhs) {
    DirectToTLMInPort<Message> *dynamic_d2tlm_port;
    Combinational<Message, DIRECT_PORT> *dynamic_comb;
    
    dynamic_d2tlm_port = new DirectToTLMInPort<Message>(sc_gen_unique_name("dynamic_d2tlm_port"), rhs.fifo);
    dynamic_comb = new Combinational<Message, DIRECT_PORT>(sc_gen_unique_name("dynamic_comb"));

    // Bind the marshaller to combinational
    this->Bind(*dynamic_comb);
    dynamic_d2tlm_port->val(dynamic_comb->out_val);
    dynamic_d2tlm_port->rdy(dynamic_comb->out_rdy);
    dynamic_d2tlm_port->msg(dynamic_comb->out_msg);
    
    // Bound but no ptr indicates a cosim interface
    dynamic_comb->out_bound = true;
    dynamic_comb->out_ptr = 0;
  }

#endif // CONNECTIONS_SIM_ONLY

#endif // __SYNTHESIS__

  // Bind to p2p<>::out
  void Bind(p2p<SYN>::out<Message>& rhs) {
    this->msg(rhs.o_dat);
    this->val(rhs.o_vld);
    this->rdy(rhs.i_rdy);
  }

  // Bind to p2p<>::chan
  void Bind(p2p<SYN>::chan<Message>& rhs) {
    this->msg(rhs.dat);
    this->val(rhs.vld);
    this->rdy(rhs.rdy);
  }

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }
  
 protected:
  void reset_msg() {
    msg.write(0);
  }
  
  void write_msg(const Message &m) {
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msg.write(bits);
  }

  void invalidate_msg() {
      MsgBits dc_bits;
      msg.write(dc_bits);
  }
};  



template <typename Message>
class OutBlocking <Message, MARSHALL_PORT> : public OutBlocking_SimPorts_abs<Message> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_out<MsgBits> msg;

  OutBlocking() : OutBlocking_SimPorts_abs<Message>(),
    msg(sc_gen_unique_name("out_msg")) {}
  
  explicit OutBlocking(const char* name) : OutBlocking_SimPorts_abs<Message>(name),
    msg(nvhls_concat(name, "msg")) {}

  // Reset write
  void Reset() {
    OutBlocking_SimPorts_abs<Message>::Reset();
  }
  
// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    OutBlocking_SimPorts_abs<Message>::Push(m);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    return OutBlocking_SimPorts_abs<Message>::PushNB(m,do_wait);
  }
  
  // Bind to OutBlocking
  void Bind(OutBlocking<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  // Bind to Combinational Marshall Port
  void Bind(Combinational<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    this->msg(rhs.in_msg);
    this->val(rhs.in_val);
    this->rdy(rhs.in_rdy);
    rhs.in_bound = true;
    rhs.in_ptr = this;
#else
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

  // For safety disallow DIRECT_PORT <-> MARSHALL_PORT binding during HLS
#ifndef __SYNTHESIS__  
  void Bind(OutBlocking<Message, DIRECT_PORT>& rhs) {
    MarshalledToDirectOutPort<Message> *dynamic_m2dport;
    dynamic_m2dport = new MarshalledToDirectOutPort<Message>(sc_gen_unique_name("dynamic_m2dport"));

#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    
    this->msg(dynamic_m2dport->msgbits);
    dynamic_m2dport->msg(rhs.msg);
    dynamic_m2dport->val(rhs.val);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  void Bind(Combinational<Message, DIRECT_PORT>& rhs) {
    MarshalledToDirectOutPort<Message> *dynamic_m2dport;

    dynamic_m2dport = new MarshalledToDirectOutPort<Message>(sc_gen_unique_name("dynamic_m2dport"));
    this->msg(dynamic_m2dport->msgbits);

#ifdef CONNECTIONS_SIM_ONLY
    dynamic_m2dport->msg(rhs.in_msg);
    dynamic_m2dport->val(rhs.in_val);
    this->val(rhs.in_val);
    this->rdy(rhs.in_rdy);
    rhs.in_bound = true;
    rhs.in_ptr = this;
#else
    dynamic_m2dport->msg(rhs.msg);
    dynamic_m2dport->val(rhs.val);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
  void Bind(Combinational<Message, TLM_PORT>& rhs) {
    DirectToTLMInPort<Message> *dynamic_d2tlm_port;
    Combinational<Message, DIRECT_PORT> *dynamic_comb;
    
    dynamic_d2tlm_port = new DirectToTLMInPort<Message>(sc_gen_unique_name("dynamic_d2tlm_port"), rhs.fifo);
    dynamic_comb = new Combinational<Message, DIRECT_PORT>(sc_gen_unique_name("dynamic_comb") );

    // Bind the marshaller to combinational
    this->Bind(*dynamic_comb);
    dynamic_d2tlm_port->val(dynamic_comb->out_val);
    dynamic_d2tlm_port->rdy(dynamic_comb->out_rdy);
    dynamic_d2tlm_port->msg(dynamic_comb->out_msg);
    
    // Bound but no ptr indicates a cosim interface
    dynamic_comb->out_bound = true;
    dynamic_comb->out_ptr = 0;
  }

#endif // CONNECTIONS_SIM_ONLY

#endif // __SYNTHESIS__

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }
  
 protected:
  void reset_msg() {
    msg.write(0);
  }
  
  void write_msg(const Message &m) {
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msg.write(bits);
  }

  void invalidate_msg() {
      MsgBits dc_bits;
      msg.write(dc_bits);
  }
};  

template <typename Message>
class OutBlocking <Message, DIRECT_PORT> : public OutBlocking_SimPorts_abs<Message> {
 public:
  // Interface
  sc_out<Message> msg;

 OutBlocking() : OutBlocking_SimPorts_abs<Message>(),
    msg(sc_gen_unique_name("out_msg"))
      {}
  
  explicit OutBlocking(const char* name) : OutBlocking_SimPorts_abs<Message>(name),
    msg(nvhls_concat(name, "msg"))
      {}
  
  // Reset write
  void Reset() {
    OutBlocking_SimPorts_abs<Message>::Reset();
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    OutBlocking_SimPorts_abs<Message>::Push(m);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    return OutBlocking_SimPorts_abs<Message>::PushNB(m, do_wait);
  }
  
  // Bind to OutBlocking
  void Bind(OutBlocking<Message, DIRECT_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }
  
  // Bind to Combinational
  void Bind(Combinational<Message, DIRECT_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    this->msg(rhs.in_msg);
    this->val(rhs.in_val);
    this->rdy(rhs.in_rdy);
    rhs.in_bound = true;
    rhs.in_ptr = this;
#else
    this->msg(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }

  // For safety disallow DIRECT_PORT <-> MARSHALL_PORT binding during HLS
#ifndef __SYNTHESIS__  
  void Bind(OutBlocking<Message, MARSHALL_PORT>& rhs) {
    DirectToMarshalledOutPort<Message> *dynamic_d2mport;
    dynamic_d2mport = new DirectToMarshalledOutPort<Message>("dynamic_d2mport");

#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
#endif
    
    this->msg(dynamic_d2mport->msg);
    dynamic_d2mport->msgbits(rhs.msg);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
  }

  void Bind(Combinational<Message, MARSHALL_PORT>& rhs) {
    DirectToMarshalledOutPort<Message> *dynamic_d2mport;

    dynamic_d2mport = new DirectToMarshalledOutPort<Message>("dynamic_d2mport");
    this->msg(dynamic_d2mport->msg);

#ifdef CONNECTIONS_SIM_ONLY
    dynamic_d2mport->msg(rhs.in_msg);
    dynamic_d2mport->val(rhs.in_val);
    this->val(rhs.in_val);
    this->rdy(rhs.in_rdy);
    rhs.in_bound = true;
    rhs.in_ptr = this;
#else
    dynamic_d2mport->msg(rhs.msg);
    dynamic_d2mport->val(rhs.val);
    this->val(rhs.val);
    this->rdy(rhs.rdy);
#endif
  }
#endif // __SYNTHESIS__

  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }
  
 protected:
  void reset_msg() {
    Message dc;
    msg.write(dc);
  }
  
  void write_msg(const Message &m) {
    msg.write(m);
  }

  void invalidate_msg() {
      Message dc;
      msg.write(dc);
  }
};  


#ifdef CONNECTIONS_SIM_ONLY
template <typename Message>
class OutBlocking <Message, TLM_PORT> : public OutBlocking_abs<Message> {
 public:

 OutBlocking() : OutBlocking_abs<Message>(),
    o_fifo(sc_gen_unique_name("o_fifo"))
      {}
  
  explicit OutBlocking(const char* name) : OutBlocking_abs<Message>(name),
    o_fifo(nvhls_concat(name,"o_fifo"))
      {}
  
  // Reset write
  void Reset() {
    // TODO: Do we need to reset anything here?
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    o_fifo->put(m);
    wait(sc_core::SC_ZERO_TIME);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    return o_fifo->nb_put(m);
  }

  // Bind to OutBlocking
  void Bind(OutBlocking<Message, TLM_PORT>& rhs) {
    this->o_fifo(rhs.o_fifo);
  }
  
  // Bind to Combinational
  void Bind(Combinational<Message, TLM_PORT>& rhs) {
    this->o_fifo(rhs.fifo);
  }
  
  // Binding
  template <typename C>
  void operator()(C& rhs) {
    Bind(rhs);
  }
  
 protected:
  sc_port<tlm::tlm_fifo_put_if<Message> > o_fifo;
};
#endif

 
//------------------------------------------------------------------------
// Out
//------------------------------------------------------------------------

template <typename Message>
class Out<Message, SYN_PORT> : public OutBlocking<Message, SYN_PORT> {
 public:
  Out() {}

  explicit Out(const char* name) : OutBlocking<Message, SYN_PORT>(name) {}

  // Full
  bool Full() {
    QUERY_CALL();
    return !this->rdy.read();
  }
};

 
template <typename Message>
class Out<Message, MARSHALL_PORT> : public OutBlocking<Message, MARSHALL_PORT> {
 public:
  Out() {}

  explicit Out(const char* name) : OutBlocking<Message, MARSHALL_PORT>(name) {}

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


template <typename Message>
class Out<Message, DIRECT_PORT> : public OutBlocking<Message, DIRECT_PORT> {
 public:
  Out() {}

  explicit Out(const char* name) : OutBlocking<Message, DIRECT_PORT>(name) {}

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


#ifdef CONNECTIONS_SIM_ONLY
template <typename Message>
class Out<Message, TLM_PORT> : public OutBlocking<Message, TLM_PORT> {
 public:
  Out() {}

  explicit Out(const char* name) : OutBlocking<Message, TLM_PORT>(name) {}

  // Full
  bool Full() {
    return ! this->o_fifo->nb_can_put();
  }
};
#endif

 
//------------------------------------------------------------------------
// Combinational MARSHALL_PORT
//------------------------------------------------------------------------

#ifdef CONNECTIONS_SIM_ONLY
template <typename Message>
class BA_Message {
 public:
  Message m;
  unsigned long ready_cycle;
 };
 
 class Connections_BA_abs : public sc_module {
 public:
   
 Connections_BA_abs() : sc_module(sc_module_name(sc_gen_unique_name("chan_ba")))
     {}
   
   explicit Connections_BA_abs(const char* name) : sc_module(sc_module_name(name))
     {}

   virtual void annotate(unsigned long latency, unsigned int capacity) {
     NVHLS_ASSERT(0);
   }

   virtual const char *src_name() {
     NVHLS_ASSERT(0);
     return 0;
   }
   
   virtual const char *dest_name() {
     NVHLS_ASSERT(0);
     return 0;
   }
 };
#endif

template <typename Message>
class Combinational_abs {
  // Abstract class
 protected:
  // Default constructor
  Combinational_abs()
    {
    }

  // Constructor
  explicit Combinational_abs(const char* name)
  {
  }

 public:
  // Reset
  void ResetRead() {
    NVHLS_ASSERT(0);
  }

  void ResetWrite() {
    NVHLS_ASSERT(0);
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    Message m;
    NVHLS_ASSERT(0);
    return m;
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    Message m;
    NVHLS_ASSERT(0);
    return m;
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
    NVHLS_ASSERT(0);
    return false;
  }

// ekrimer: commenting these out, should really be EmptyWrite and EmptyRead, and in anycase will not work in verilog currently
  // Empty
//  bool Empty() { return !val.read(); }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    NVHLS_ASSERT(0);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
    NVHLS_ASSERT(0);
    return false;
  }

// ekrimer: commenting out, similar to Empty
  // Full
//  bool Full() { return !rdy.read(); }
};
 
template <typename Message>
class Combinational_Ports_abs : public Combinational_abs<Message> {
 public:
  // Interface
  sc_signal<bool> val;
  sc_signal<bool> rdy;

  // Abstract class
 protected:
  // Default constructor
  Combinational_Ports_abs()
    : Combinational_abs<Message>(),
    val(sc_gen_unique_name("comb_val")),
    rdy(sc_gen_unique_name("comb_rdy"))
      {}

  // Constructor
  explicit Combinational_Ports_abs(const char* name)
    : Combinational_abs<Message>(name),
    val(nvhls_concat(name, "val")),
    rdy(nvhls_concat(name, "rdy")) 
      {}

 public:
  // Reset
  void ResetRead() { 
    rdy.write(false); 
  }

  void ResetWrite() {
    val.write(false);
    reset_msg();
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    do {
      rdy.write(true);
      wait();
    } while (val.read() != true);
    rdy.write(false);
    Message m;
    read_msg(m);
    return m;
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    while (!val.read()) {
      wait();
    }
    Message m;
    read_msg(m);
    return m;
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
    rdy.write(true);
    wait();
    rdy.write(false);
    read_msg(data);
    return val.read();
  }

// ekrimer: commenting these out, should really be EmptyWrite and EmptyRead, and in anycase will not work in verilog currently
  // Empty
//  bool Empty() { return !val.read(); }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    do {
      val.write(true);
      write_msg(m);
      wait();
    } while (rdy.read() != true);
    val.write(false);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
    val.write(true);
    write_msg(m);
    wait();
    val.write(false);
    return rdy.read();
  }

// ekrimer: commenting out, similar to Empty
  // Full
//  bool Full() { return !rdy.read(); }
  
 protected:
    virtual void reset_msg() {
      NVHLS_ASSERT(0);
    }
    virtual void read_msg(Message &m) {
      NVHLS_ASSERT(0);
    }
    virtual void write_msg(const Message &m) {
      NVHLS_ASSERT(0);
    }
    virtual void invalidate_msg() {
      NVHLS_ASSERT(0);
    }
};


template <typename Message, connections_port_t port_marshall_type>
class Combinational_SimPorts_abs
#ifdef CONNECTIONS_SIM_ONLY
  : public Combinational_abs<Message>,
    public Connections_BA_abs,
    public Blocking_abs
#else
  : public Combinational_Ports_abs<Message>
#endif
{
#ifdef CONNECTIONS_SIM_ONLY
  SC_HAS_PROCESS(Combinational_SimPorts_abs);
#endif

  // Abstract class
 protected:
  // Default constructor
  Combinational_SimPorts_abs()
#ifdef CONNECTIONS_SIM_ONLY
    : Combinational_abs<Message>()

    , Connections_BA_abs(sc_gen_unique_name("comb_ba"))
    
    //, in_msg(sc_gen_unique_name("comb_in_msg"))
    , in_val(sc_gen_unique_name("comb_in_val"))
    , in_rdy(sc_gen_unique_name("comb_in_rdy"))
    //, out_msg(sc_gen_unique_name("comb_out_msg"))
    , out_val(sc_gen_unique_name("comb_out_val"))
    , out_rdy(sc_gen_unique_name("comb_out_rdy"))
    
    , current_cycle(0)
    , latency(0)
    
    , out_bound(false), in_bound(false)
    , sim_out(sc_gen_unique_name("sim_out")), sim_in(sc_gen_unique_name("sim_in"))
#else
    : Combinational_Ports_abs<Message>()
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      Init_SIM("comb");
#endif  
    }

  // Constructor
  explicit Combinational_SimPorts_abs(const char* name)
#ifdef CONNECTIONS_SIM_ONLY
    : Combinational_abs<Message>(name)
    
    , Connections_BA_abs(nvhls_concat(name, "comb_BA"))
    
    //, in_msg(nvhls_concat(name, "comb_in_msg"))
    , in_val(nvhls_concat(name, "comb_in_val"))
    , in_rdy(nvhls_concat(name, "comb_in_rdy"))
    //, out_msg(nvhls_concat(name, "comb_out_msg"))
    , out_val(nvhls_concat(name, "comb_out_val"))
    , out_rdy(nvhls_concat(name, "comb_out_rdy"))
    
    , current_cycle(0)
    , latency(0)
    
    , out_bound(false), in_bound(false)
    , sim_out(nvhls_concat(name,"sim_out")), sim_in(nvhls_concat(name, "sim_in"))
#else
    : Combinational_Ports_abs<Message>(name)
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      Init_SIM("comb");
#endif  
    }
  
 public:
  
#ifdef CONNECTIONS_SIM_ONLY
  //sc_signal<MsgBits> in_msg;
  sc_signal<bool>    in_val;
  sc_signal<bool>    in_rdy;
  //sc_signal<MsgBits> out_msg;
  sc_signal<bool>    out_val;
  sc_signal<bool>    out_rdy;
  unsigned long current_cycle;
  unsigned long latency;
  tlm::circular_buffer< BA_Message<Message> > b;
#endif
  
  // Reset
  void ResetRead() {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! out_bound); */
    
    sim_in.Reset();
    Reset_SIM();
#else
    Combinational_Ports_abs<Message>::ResetRead();
#endif
  }

  void ResetWrite() {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! in_bound); */
    
    sim_out.Reset();
    
    Reset_SIM();
#else
    Combinational_Ports_abs<Message>::ResetWrite();
#endif
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! out_bound); */
    
    return sim_in.Pop();
#else
    return Combinational_Ports_abs<Message>::Pop();
#endif
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! out_bound); */
    
    return sim_in.Peek();
#else
    return Combinational_Ports_abs<Message>::Peek();
#endif
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! out_bound); */
    
    return sim_in.PopNB(data);
#else
    return Combinational_Ports_abs<Message>::PopNB(data);
#endif
  }

// ekrimer: commenting these out, should really be EmptyWrite and EmptyRead, and in anycase will not work in verilog currently
  // Empty
//  bool Empty() { return !val.read(); }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! in_bound); */
    
    sim_out.Push(m);
#else
    Combinational_Ports_abs<Message>::Push(m);
#endif
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! in_bound); */
    
    return sim_out.PushNB(m);
#else
    return Combinational_Ports_abs<Message>::PushNB(m);
#endif
  }

// ekrimer: commenting out, similar to Empty
  // Full
//  bool Full() { return !rdy.read(); }

#ifdef CONNECTIONS_SIM_ONLY
 public:
  OutBlocking_Ports_abs<Message> *in_ptr;
  InBlocking_Ports_abs<Message> *out_ptr;
  
  bool out_bound, in_bound;
  
  inline bool is_bypass() {
    return (latency == 0);
  }
    
  void annotate(unsigned long latency, unsigned int capacity) {
    this->latency = latency;
    assert(! (latency > 0 && capacity == 0)); // latency > 0 but capacity == 0 is not supported.
    if(capacity > 0) {
      this->b.resize(capacity);
    } else {
      this->b.resize(1);
    }
  }
  
  const char *src_name() {
    if(in_bound) {
      if(in_ptr)
	return in_ptr->val.name();
      else
	return "TLM_INTERFACE";
    } else {
      return "UNBOUND";
    }
  }
  
  const char *dest_name() {
    if(out_bound) {
      if(out_ptr)
	return out_ptr->val.name();
      else
	return "TLM_INTERFACE";
    } else {
      return "UNBOUND";
    }
  }
  
 protected:
  OutBlocking<Message,port_marshall_type> sim_out;
  InBlocking<Message,port_marshall_type> sim_in;

  bool data_val;
  bool val_set_by_api;
  bool rdy_set_by_api;
  
  void Init_SIM(const char* name) {
    data_val = false;
    val_set_by_api = false;
    rdy_set_by_api = false;
    Reset_SIM();
    Connections::conManager.add(this);
    Connections::conManager.add_annotate(this);
  }
  
  void Reset_SIM() {
    current_cycle = 0;
    
    data_val = false;
    
    while(! b.is_empty()) { b.read(); }
  }
  
// Although this code is being used only for simulation now, it could be
// synthesized if pre/post are used instead of spawned threads.
// Keep mudulario pragmas here to preserve that option

#pragma design modulario < out >
  void receive(const bool& stall) {
    if (stall) {
      in_rdy.write(false);
      rdy_set_by_api = false;
    } else {
      in_rdy.write(true);
      rdy_set_by_api = true;
    }
  }

#pragma design modulario < in >
  bool received(Message& data) {
    if (in_val.read())
      {
	/* MsgBits mbits = in_msg.read(); */
	/* Marshaller<WMessage::width> marshaller(mbits); */
	/* WMessage result; */
	/* result.Marshall(marshaller); */
	/* data = result.val; */
	read_msg(data);
	return true;
    }

    // not valid
    // randomize data
    //memset(&data, random(), sizeof(data));
    //if (!data_val)
    //    memset(&data, 0, sizeof(data));
    return false;
  }
  
#pragma design modulario < in >
  bool transmitted() { 	return out_rdy.read(); }

#pragma design modulario < out >
  void transmit_data(const Message& m) {
    /* Marshaller<WMessage::width> marshaller; */
    /* WMessage wm(m); */
    /* wm.Marshall(marshaller); */
    /* MsgBits bits = marshaller.GetResult(); */
    /* out_msg.write(bits); */
    write_msg(m);
  }

#pragma design modulario < out >
  void transmit_val(const bool& vald) {
    if (vald) {
      out_val.write(true);
      val_set_by_api = true;
    } else {
      out_val.write(false);
      val_set_by_api = false;

      //corrupt and transmit data
      //memset(&data_buf, random(), sizeof(data_buf));
      //transmit_data(data_buf);
    }
  }

  bool Pre() {
    if(is_bypass()) return true; // TODO: return false to deregister.
    
    // Input
    if (rdy_set_by_api != in_rdy.read())
    {
        // something has changed the value of the signal not through API
        // killing spawned threads;
        return false;
    }
    
    if(!b.is_full()) {
      Message m;
      if(received(m)) {
	BA_Message<Message> bam;
	bam.m = m;
	assert(latency > 0);
	bam.ready_cycle = current_cycle + latency;
	b.write(bam);
      }
    }

    // Output
    if(!b.is_empty()) {
      if(transmitted() && val_set_by_api) {
	b.read(); // pop
      }
    }
    return true;
  }

  bool Post() {
    current_cycle++; // Increment to next cycle.
    
    if(is_bypass()) return true; // TODO: return false to deregister.
    
    // Input
    receive(b.is_full());

    // Output
    if (val_set_by_api != out_val.read())
    {
        // something has changed the value of the signal not through API
        // killing spawned threads;
        return false;
    }
    if(! b.is_empty() && (b.read_data().ready_cycle <= current_cycle)) {
      transmit_val(true);
      transmit_data(b.read_data().m); // peek
    } else {
      transmit_val(false);
    }
    return true;
  }

  void FillBuf_SIM(const Message& m) {
    BA_Message<Message> bam;
    bam.m = m;
    bam.ready_cycle = current_cycle + latency;
    assert(! b.is_full());
    b.write(bam);
  }

  bool Empty_SIM() {
    return b.is_empty();
  }

  bool Full_SIM() {
    return b.is_full();
  }

  void Push_SIM(const Message& m) {
    while (Full_SIM()) {
      wait();
    }
    FillBuf_SIM(m);
  }

  // If sim accurate mode is enabled, we use our own abstract declarations of these
  // since Combinational_Ports_abs includes sc_signal rdy/val signals, which
  // SimPorts may differentiate into in_rdy/in_val. There may be a better way to
  // factor this further, but SimPorts also takes over Push/Pop etc for sim accurate mode
  // already so we aren't strongly dependent on Combinational_Ports_abs.
  
 protected:
  virtual void reset_msg() {
    NVHLS_ASSERT(0);
  }
  virtual void read_msg(Message &m) {
    NVHLS_ASSERT(0);
  }
  virtual void write_msg(const Message &m) {
    NVHLS_ASSERT(0);
  }
  virtual void invalidate_msg() {
    NVHLS_ASSERT(0);
  }
#endif
};

 
template <typename Message>
class Combinational <Message, SYN_PORT> : public Combinational_Ports_abs<Message> {
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
  sc_signal<MsgBits> msg;

 Combinational() : Combinational_Ports_abs<Message>(),
                    msg(sc_gen_unique_name("comb_msg"))
    {}
  
  explicit Combinational(const char* name) : Combinational_Ports_abs<Message>(name),
                                             msg(nvhls_concat(name, "msg"))
    {}


  // Reset
  void ResetRead() { 
    return Combinational_Ports_abs<Message>::ResetRead();
  }

  void ResetWrite() {
    return Combinational_Ports_abs<Message>::ResetWrite();
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    return Combinational_Ports_abs<Message>::Pop();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    return Combinational_Ports_abs<Message>::Peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
    return Combinational_Ports_abs<Message>::PopNB(data);
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    Combinational_Ports_abs<Message>::Push(m);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
    return Combinational_Ports_abs<Message>::PushNB(m);
  }

  
 protected:
  void reset_msg() {
    msg.write(0);
  }
  
  void read_msg(Message &m) {
    MsgBits mbits = msg.read();
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    m = result.val;
  }
  
  void write_msg(const Message &m) {
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msg.write(bits);
  }

  void invalidate_msg() {
      MsgBits dc_bits;
      msg.write(dc_bits);
  }
};  

 
template <typename Message>
class Combinational <Message, MARSHALL_PORT> : public Combinational_SimPorts_abs<Message, MARSHALL_PORT> {
  #ifdef CONNECTIONS_SIM_ONLY
  SC_HAS_PROCESS(Combinational);
  #endif
  
 public:
  // Interface
  typedef Wrapped<Message> WMessage;
  static const unsigned int width = WMessage::width;
  typedef sc_lv<WMessage::width> MsgBits;
#ifdef CONNECTIONS_SIM_ONLY
  sc_signal<MsgBits> in_msg;
  sc_signal<MsgBits> out_msg;
#else
  sc_signal<MsgBits> msg;
#endif

 Combinational() : Combinational_SimPorts_abs<Message, MARSHALL_PORT>()
#ifdef CONNECTIONS_SIM_ONLY
    ,in_msg(sc_gen_unique_name("comb_in_msg"))
    ,out_msg(sc_gen_unique_name("comb_out_msg"))
    ,dummyPortManager(sc_gen_unique_name("dummyPortManager"), this->sim_in, this->sim_out, *this)
#else
    ,msg(sc_gen_unique_name("comb_msg"))
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      //SC_METHOD(do_bypass);
      declare_method_process(do_bypass_handle, sc_gen_unique_name("do_bypass"), SC_CURRENT_USER_MODULE, do_bypass);
      this->sensitive << in_msg << this->in_val << this->out_rdy;
#endif
    }
  
  explicit Combinational(const char* name) : Combinational_SimPorts_abs<Message, MARSHALL_PORT>(name)

#ifdef CONNECTIONS_SIM_ONLY
    ,in_msg(nvhls_concat(name, "comb_in_msg"))
    ,out_msg(nvhls_concat(name, "comb_out_msg"))
    ,dummyPortManager(nvhls_concat(name, "dummyPortManager"), this->sim_in, this->sim_out, *this)
#else
    ,msg(nvhls_concat(name, "msg"))
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      //SC_METHOD(do_bypass);
      declare_method_process(do_bypass_handle, sc_gen_unique_name("do_bypass"), SC_CURRENT_USER_MODULE, do_bypass);
      this->sensitive << in_msg << this->in_val << this->out_rdy;
#endif
    }

  //void do_bypass() { Combinational_SimPorts_abs<Message,MARSHALL_PORT>::do_bypass(); }

#ifdef CONNECTIONS_SIM_ONLY
  void do_bypass() {
    if(! this->is_bypass()) return;
    out_msg.write(in_msg.read());
    //write_msg(read_msg());
    this->out_val.write(this->in_val.read());
    this->in_rdy.write(this->out_rdy.read());
  }
#endif  
  
  // Parent functions, to get around Catapult virtual function bug.
  void ResetRead() { return Combinational_SimPorts_abs<Message,MARSHALL_PORT>::ResetRead(); }
  void ResetWrite() { return Combinational_SimPorts_abs<Message,MARSHALL_PORT>::ResetWrite(); }
#pragma design modulario < in >
  Message Pop() { return Combinational_SimPorts_abs<Message,MARSHALL_PORT>::Pop(); }
#pragma design modulario < in >
  Message Peek() { return Combinational_SimPorts_abs<Message,MARSHALL_PORT>::Peek(); }
#pragma design modulario < in >
  bool PopNB(Message& data) { return Combinational_SimPorts_abs<Message,MARSHALL_PORT>::PopNB(data); }
#pragma design modulario < out >
  void Push(const Message& m) { Combinational_SimPorts_abs<Message,MARSHALL_PORT>::Push(m); }
#pragma design modulario < out >
  bool PushNB(const Message& m) { return Combinational_SimPorts_abs<Message,MARSHALL_PORT>::PushNB(m);  }
  
 protected:
  void reset_msg() {
#ifdef CONNECTIONS_SIM_ONLY
    out_msg.write(0);
#else
    msg.write(0);
#endif
  }
  
  void read_msg(Message &m) {
#ifdef CONNECTIONS_SIM_ONLY
    MsgBits mbits = in_msg.read();
#else
    MsgBits mbits = msg.read();
#endif    
    Marshaller<WMessage::width> marshaller(mbits);
    WMessage result;
    result.Marshall(marshaller);
    m = result.val;
  }
  
  void write_msg(const Message &m) {
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
#ifdef CONNECTIONS_SIM_ONLY
    out_msg.write(bits);
#else
    msg.write(bits);
#endif
  }

  void invalidate_msg() {
      MsgBits dc_bits;
#ifdef CONNECTIONS_SIM_ONLY
      out_msg.write(dc_bits);
#else
      msg.write(dc_bits);
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
 protected:
    class DummyPortManager : public sc_module {
        SC_HAS_PROCESS(DummyPortManager);
    private:
    InBlocking<Message,MARSHALL_PORT>& in;
    OutBlocking<Message,MARSHALL_PORT>& out;
    Combinational<Message,MARSHALL_PORT>& parent;

    public:
    DummyPortManager(sc_module_name name, InBlocking<Message,MARSHALL_PORT>& in_, OutBlocking<Message,MARSHALL_PORT>& out_, Combinational<Message,MARSHALL_PORT>& parent_)
    : sc_module(name), in(in_), out(out_), parent(parent_)
    {}
    
    virtual void before_end_of_elaboration()
    {
        if (!parent.in_bound)
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
        if (!parent.out_bound)
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
    }

    } dummyPortManager;
#endif
};  

 
template <typename Message>
class Combinational <Message, DIRECT_PORT> : public Combinational_SimPorts_abs<Message, DIRECT_PORT> {
#ifdef CONNECTIONS_SIM_ONLY
  SC_HAS_PROCESS(Combinational);
#endif

 public:
  // Interface
#ifdef CONNECTIONS_SIM_ONLY
  sc_signal<Message> in_msg;
  sc_signal<Message> out_msg;
#else
  sc_signal<Message> msg;
#endif

 Combinational() : Combinational_SimPorts_abs<Message, DIRECT_PORT>()
#ifdef CONNECTIONS_SIM_ONLY
    ,in_msg(sc_gen_unique_name("comb_in_msg"))
    ,out_msg(sc_gen_unique_name("comb_out_msg"))
    ,dummyPortManager(sc_gen_unique_name("dummyPortManager"), this->sim_in, this->sim_out, *this)
#else
    ,msg(sc_gen_unique_name("comb_msg"))
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      //SC_METHOD(do_bypass);
      declare_method_process(do_bypass_handle, sc_gen_unique_name("do_bypass"), SC_CURRENT_USER_MODULE, do_bypass);
      this->sensitive << in_msg << this->in_val << this->out_rdy;
#endif
    }
  
  explicit Combinational(const char* name) : Combinational_SimPorts_abs<Message, DIRECT_PORT>(name)
#ifdef CONNECTIONS_SIM_ONLY
    ,in_msg(nvhls_concat(name, "comb_in_msg"))
    ,out_msg(nvhls_concat(name, "comb_out_msg"))
    ,dummyPortManager(nvhls_concat(name, "dummyPortManager"), this->sim_in, this->sim_out, *this)
#else
    ,msg(nvhls_concat(name, "msg"))
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      //SC_METHOD(do_bypass);
      declare_method_process(do_bypass_handle, sc_gen_unique_name("do_bypass"), SC_CURRENT_USER_MODULE, do_bypass);
      this->sensitive << in_msg << this->in_val << this->out_rdy;
#endif
    }

#ifdef CONNECTIONS_SIM_ONLY
  void do_bypass() {
    if(! this->is_bypass()) return;
    out_msg.write(in_msg.read());
    //write_msg(read_msg());
    this->out_val.write(this->in_val.read());
    this->in_rdy.write(this->out_rdy.read());
  }
#endif  

  // Parent functions, to get around Catapult virtual function bug.
  void ResetRead() { return Combinational_SimPorts_abs<Message,DIRECT_PORT>::ResetRead(); }
  void ResetWrite() { return Combinational_SimPorts_abs<Message,DIRECT_PORT>::ResetWrite(); }
#pragma design modulario < in >
  Message Pop() { return Combinational_SimPorts_abs<Message,DIRECT_PORT>::Pop(); }
#pragma design modulario < in >
  Message Peek() { return Combinational_SimPorts_abs<Message,DIRECT_PORT>::Peek(); }
#pragma design modulario < in >
  bool PopNB(Message& data) { return Combinational_SimPorts_abs<Message,DIRECT_PORT>::PopNB(data); }
#pragma design modulario < out >
  void Push(const Message& m) { Combinational_SimPorts_abs<Message,DIRECT_PORT>::Push(m); }
#pragma design modulario < out >
  bool PushNB(const Message& m) { return Combinational_SimPorts_abs<Message,DIRECT_PORT>::PushNB(m);  }
  
 protected:
  void reset_msg() {
    Message dc;
#ifdef CONNECTIONS_SIM_ONLY
    out_msg.write(dc);
#else
    msg.write(dc);
#endif
  }
  
  void read_msg(Message &m) {
#ifdef CONNECTIONS_SIM_ONLY
    m = in_msg.read();
#else
    m = msg.read();
#endif
  }
  
  void write_msg(const Message &m) {
#ifdef CONNECTIONS_SIM_ONLY
  out_msg.write(m);
#else
  msg.write(m);
#endif
  }

  void invalidate_msg() {
      Message dc;
#ifdef CONNECTIONS_SIM_ONLY
      out_msg.write(dc);
#else
      msg.write(dc);
#endif
  }

#ifdef CONNECTIONS_SIM_ONLY
 protected:

    class DummyPortManager : public sc_module {
        SC_HAS_PROCESS(DummyPortManager);
    private:
	InBlocking<Message,DIRECT_PORT>& in;
	OutBlocking<Message,DIRECT_PORT>& out;
	Combinational<Message,DIRECT_PORT>& parent;

    public:
   DummyPortManager(sc_module_name name, InBlocking<Message,DIRECT_PORT>& in_, OutBlocking<Message,DIRECT_PORT>& out_, Combinational<Message,DIRECT_PORT>& parent_)
    : sc_module(name), in(in_), out(out_), parent(parent_)
    {}
    
    virtual void before_end_of_elaboration()
    {
        if (!parent.in_bound)
        {
	  out(parent);
        } else {
            sc_signal<Message>* dummy_out_msg = new sc_signal<Message>;
            sc_signal<bool>* dummy_out_val = new sc_signal<bool>;
            sc_signal<bool>* dummy_out_rdy = new sc_signal<bool>;

            out.msg(*dummy_out_msg);
            out.val(*dummy_out_val);
            out.rdy(*dummy_out_rdy);
            
            out.disable_spawn();
        }
        if (!parent.out_bound)
        {
	  in(parent);
        } else {
            sc_signal<Message>* dummy_in_msg = new sc_signal<Message>;
            sc_signal<bool>* dummy_in_val = new sc_signal<bool>;
            sc_signal<bool>* dummy_in_rdy = new sc_signal<bool>;

            in.msg(*dummy_in_msg);
            in.val(*dummy_in_val);
            in.rdy(*dummy_in_rdy);

            in.disable_spawn();
        }
    }

    } dummyPortManager;
#endif
};  


#ifdef CONNECTIONS_SIM_ONLY
template <typename Message>
class Combinational <Message, TLM_PORT> : public Combinational_Ports_abs<Message> {
 public:

 Combinational() : Combinational_Ports_abs<Message>()
                    ,fifo(sc_gen_unique_name("fifo"), 2) {}

  
  explicit Combinational(const char* name) : Combinational_Ports_abs<Message>(name)
                    ,fifo(nvhls_concat(name, "fifo"), 1) {}

  // Reset
  void ResetRead() { 
    Message temp;
    while (fifo.nb_get(temp));
  }

  void ResetWrite() {
    // TODO: Do anything here?
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    return fifo.get();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    return fifo.peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
    return fifo.nb_get(data);
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    fifo.put(m);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
    return fifo.nb_put(m);
  }

 protected:
    void reset_msg() {
      NVHLS_ASSERT(0);
    }
    void read_msg(Message &m) {
      NVHLS_ASSERT(0);
    }
    void write_msg(const Message &m) {
      NVHLS_ASSERT(0);
    }
    void invalidate_msg() {
      NVHLS_ASSERT(0);
    }

 public:
  tlm::tlm_fifo<Message> fifo;
};
#endif
	  
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
  
 StateSignal() : msg("msg") {}

 StateSignal(sc_module_name name) : msg(nvhls_concat(name, "_msg")) { }
  
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
  
 StateSignal() : msg("msg") {}

 StateSignal(sc_module_name name) : msg(nvhls_concat(name, "_msg")) { }
  
  void reset_state() {
    msg.write(0);
  }
};

template <typename Message>
class StateSignal<Message, DIRECT_PORT> {
 public:
  // Interface
  sc_signal<Message> msg;
  
 StateSignal() : msg("msg") {}

 StateSignal(sc_module_name name) : msg(nvhls_concat(name, "_msg")) { }

  void reset_state() {
    Message dc;
    msg.write(dc);
  }
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

    SC_THREAD(SetState);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
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

    SC_THREAD(UpdateCredit);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
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

    SC_THREAD(UpdateCredit);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);

    SC_THREAD(SetState);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
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
#ifdef CONNECTIONS_SIM_ONLY
    chan.enq.disable_spawn();
    chan.deq.disable_spawn();
#endif
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
#ifdef CONNECTIONS_SIM_ONLY
    chan.enq.disable_spawn();
    chan.deq.disable_spawn();
#endif
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
#ifdef CONNECTIONS_SIM_ONLY
    chan.enq.disable_spawn();
    chan.deq.disable_spawn();
#endif
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
#ifdef CONNECTIONS_SIM_ONLY
    chan.enq.disable_spawn();
    chan.deq.disable_spawn();
#endif
    chan.clk(clk);
    chan.rst(rst);
    chan.enq(enq);
    chan.deq(deq);
    in(deq);

  }
};

}  // namespace Connections

#endif  // NVHLS_CONNECTIONS_H_
