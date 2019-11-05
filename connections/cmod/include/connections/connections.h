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
// connections.h
//========================================================================

#ifndef __CONNECTIONS__CONNECTIONS_H__
#define __CONNECTIONS__CONNECTIONS_H__

#include <systemc.h>
#include "marshaller.h"
#include <ccs_p2p.h>

#include "connections_utils.h"

#ifndef __SYNTHESIS__
#include <iomanip>
#endif

#if defined(CONNECTIONS_ACCURATE_SIM) && defined(CONNECTIONS_FAST_SIM)
#error "Both CONNECTIONS_ACCURATE_SIM and CONNECTIONS_FAST_SIM are defined. Define one or the other."
#endif

#if defined(CONN_RAND_STALL) && !defined(__SYNTHESIS__) && !(defined(CONNECTIONS_ACCURATE_SIM) || defined(CONNECTIONS_FAST_SIM))
#warning "Warning: CONN_RAND_STALL only works in CONNECTIONS_ACCURATE_SIM and CONNECTIONS_FAST_SIM modes!"
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
#include "Pacer.h"
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
 *   - SYN_PORT: Actual SystemC modulario code given to catapult. Many wait() statements leads to 
 *     timing inaccuracy in SystemC simulation.
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
 *      #include <connections/connections.h>
 * \endcode
 * 
 * \par
 *
 * The FORCE_AUTO_PORT define can also be used, overrides all other AUTO_PORT settings, including forcing to a different port type
 * during HLS. Useful if a specific port type is desired for targetted unit tests. Example code:
 * 
 * \code
 *      #define FORCE_AUTO_PORT MARSHALL_PORT
 *      #include <connections/connections.h>
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
#define AUTO_PORT Connections::SYN_PORT
#elif !defined(CONNECTIONS_SIM_ONLY)
#define AUTO_PORT Connections::MARSHALL_PORT
#else

// Switch if we are in CONNECTIONS_SIM_ONLY
#if defined(CONNECTIONS_FAST_SIM)
#define AUTO_PORT Connections::TLM_PORT
#else
#define AUTO_PORT Connections::MARSHALL_PORT
#endif
 
#endif // defined(__SYNTHESIS__)
 
#endif // ifndef AUTO_PORT


// If __SYNTHESIS__ always force SYN_PORT, even if
// manually defined in a Makefile or go_hls.tcl.
#if defined(__SYNTHESIS__)
#undef AUTO_PORT
#define AUTO_PORT Connections::SYN_PORT
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
  
template <class Dummy>
struct clk_statics
{
    static const sc_time epsilon; 
};

template <class Dummy>
const sc_time clk_statics<Dummy>::epsilon = sc_time(0.01, SC_NS);

class SimConnectionsClk : public clk_statics<void>
{
    public:
    SimConnectionsClk()
    {
     clk_ptr = 0;
#ifndef NO_DEFAULT_CONNECTIONS_CLOCK
     clk_ptr = new sc_clock("default_sim_clk", 1, SC_NS, 0.5, 0, SC_NS, true);
#endif
    }

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


    sc_clock* clk_ptr;

    private:

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

SimConnectionsClk& get_sim_clk();
class ConManager;
ConManager& get_conManager();

#endif

class ResetChecker {
 protected:
  bool is_reset;
#ifndef __SYNTHESIS__
  const char *name;
  bool is_val_name;
#endif
  
 public:
 ResetChecker(const char *name_)
   : is_reset(false)
#ifndef __SYNTHESIS__
    ,name(name_)
    ,is_val_name(false)
#endif
  {}

  void reset() {
    is_reset = true;
  }
  
  void check() {
/*
#ifndef __SYNTHESIS__
    if(!is_reset) {
      // FIXME add warning here
      std::string name = this->name;
      if(is_val_name) {
	if(name.substr(name.length() - 4,4) == "_val") { name.erase(name.length() - 4,4); }
      } else {
	// Add in hierarchcy to name
	name = std::string(sc_core::sc_get_current_process_b()->get_parent_object()->name()) + "." + this->name;
      }
      SC_REPORT_ERROR("CONNECTIONS-101", ("Port or channel " + name + " wasn't reset! In thread or process '" \
				    + std::string(sc_core::sc_get_current_process_b()->basename()) \
				    + "'.").c_str());
      is_reset = true;
    }
#endif // ifndef __SYNTHESIS__
*/
  }

  void set_val_name(const char *name_) {
#ifndef __SYNTHESIS__
    name = name_;
    is_val_name = true;
#endif
  }
};
 

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

        CONNECTIONS_ASSERT_MSG(0, "Couldn't find port to remove from ConManager sim accurate tracking!");
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

        CONNECTIONS_ASSERT_MSG(0, "Couldn't find port to remove from ConManager back-annotation tracking!");
    }

    
    void run(bool value) {
        get_sim_clk().post_delay();  // align to occur just after the cycle

        while (1) {
            //Post();
            for (std::vector<Blocking_abs*>::iterator it=tracked.begin(); it!=tracked.end(); )
                if ((*it)->Post()) ++it;
                    else tracked.erase(it);
            get_sim_clk().post2pre_delay();
            //Pre();
            for (std::vector<Blocking_abs*>::iterator it=tracked.begin(); it!=tracked.end(); )
                if ((*it)->Pre()) ++it;
                    else tracked.erase(it);
            get_sim_clk().pre2post_delay();      
        }
    }
};

// See: https://stackoverflow.com/questions/18860895/how-to-initialize-static-members-in-the-header
template <class Dummy>
struct ConManager_statics
{
     static SimConnectionsClk sim_clk;
     static ConManager conManager;
     static bool rand_stall_enable;
     static bool rand_stall_print_debug_enable;
};

inline void set_sim_clk(sc_clock* clk_ptr)
{
#ifdef CONNECTIONS_SIM_ONLY
  ConManager_statics<void>::sim_clk.clk_ptr = clk_ptr;
#endif
}


#ifdef __CONN_RAND_STALL_FEATURE

#ifdef CONN_RAND_STALL
template <class Dummy>
bool ConManager_statics<Dummy>::rand_stall_enable = true;
#else
template <class Dummy>
bool ConManager_statics<Dummy>::rand_stall_enable = false;
#endif // ifdef CONN_RAND_STALL

#ifdef CONN_RAND_STALL_PRINT_DEBUG
template <class Dummy>
bool ConManager_statics<Dummy>::rand_stall_print_debug_enable = true;
#else
template <class Dummy>
bool ConManager_statics<Dummy>::rand_stall_print_debug_enable = false;
#endif // ifdef CONN_RAND_STALL_PRINT_DEBUG

#endif // ifdef __CONN_RAND_STALL_FEATURE
  
template <class Dummy>
SimConnectionsClk ConManager_statics<Dummy>::sim_clk;
template <class Dummy>
ConManager ConManager_statics<Dummy>::conManager;

inline SimConnectionsClk& get_sim_clk()
{
  CONNECTIONS_ASSERT_MSG(ConManager_statics<void>::sim_clk.clk_ptr, "You must call Connections::set_sim_clk(&clk) before sc_start()");
  return ConManager_statics<void>::sim_clk;
}

inline ConManager& get_conManager()
{
  return ConManager_statics<void>::conManager;
}

inline bool& get_rand_stall_enable()
{
  return ConManager_statics<void>::rand_stall_enable;
}

inline bool& get_rand_stall_print_debug_enable()
{
  return ConManager_statics<void>::rand_stall_print_debug_enable;
}
 
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
 * Can also use Connections::In<>.enable_local_rand_stall() to enable
 * on a per-port basis.
 *
 * \par A Simple Example
 * \code
 *      #include <connections/connections.h>
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
  inline void enable_global_rand_stall() {
    ConManager_statics<void>::rand_stall_enable = true;
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
 * Can also use Connections::In<>.disable_local_rand_stall() to disable
 * on a per-port basis.
 *
 * \par A Simple Example
 * \code
 *      #include <connections/connections.h>
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
  inline void disable_global_rand_stall() {
    ConManager_statics<void>::rand_stall_enable = false;
  }


/**
 * \brief Enable global random stalling debug print statements.
 * \ingroup Connections
 *
 * Enable random stalling print statements globally. Must already have
 * global random stalling enabled.
 *
 * Print statements will report when stall is entered, exited, and cycle
 * count in stall.
 *
 * Can also use Connections::In<>.enable_local_rand_stall_print_debug() to enable
 * on a per-port basis.
 *
 * \par A Simple Example
 * \code
 *      #include <connections/connections.h>
 *
 *      int sc_main(int argc, char *argv[])
 *      {
 *      ...
 *      Connections::enable_global_rand_stall();
 *      Connections::enable_global_rand_stall_print_debug();
 *      ...
 *      }
 * \endcode
 * \par
 *
 */
  inline void enable_global_rand_stall_print_debug() {
    ConManager_statics<void>::rand_stall_print_debug_enable = true;
  }

/**
 * \brief Disable global random stalling debug print statements.
 * \ingroup Connections
 *
 * Disable random stalling print statements globally. Must already have
 * global random stalling enabled.
 *
 * Can also use Connections::In<>.disable_local_rand_stall_print_debug() to enable
 * on a per-port basis.
 *
 * \par A Simple Example
 * \code
 *      #include <connections/connections.h>
 *
 *      int sc_main(int argc, char *argv[])
 *      {
 *      ...
 *      Connections::disable_global_rand_stall_print_debug();
 *      ...
 *      }
 * \endcode
 * \par
 *
 */
  inline void disable_global_rand_stall_print_debug() {
    ConManager_statics<void>::rand_stall_print_debug_enable = false;
  }

#endif

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
      : val(CONNECTIONS_CONCAT(name, "val")),
        rdy(CONNECTIONS_CONCAT(name, "rdy")),
	msg(CONNECTIONS_CONCAT(name, "msg")) {
    Init_SIM(name, fifo);
  }

protected:
  // Protected member variables
  tlm::tlm_fifo<Message> *fifo;

  // Initializer
  void Init_SIM(const char* name, tlm::tlm_fifo<Message> &fifo) {
    this->fifo = &fifo;
    get_conManager().add(this);
  }

  // Blocking_abs functions
  bool Pre() {
    if (val.read() && rdy.read()) {
      CONNECTIONS_ASSERT_MSG(fifo->nb_can_get(), "Val and rdy indicated data was available, but no data was available!");
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
      : val(CONNECTIONS_CONCAT(name, "val")),
        rdy(CONNECTIONS_CONCAT(name, "rdy")),
	msg(CONNECTIONS_CONCAT(name, "msg")) {
    Init_SIM(name, fifo);
  }

protected:
  // Protected member variables
  tlm::tlm_fifo<Message> *fifo;

  // Initializer
  void Init_SIM(const char* name, tlm::tlm_fifo<Message> &fifo) {
    this->fifo = &fifo;
    get_conManager().add(this);
  }
  
  bool Pre() {
    if (rdy.read() && val.read()) {
      Message data;
      data = msg.read();
      CONNECTIONS_ASSERT_MSG(fifo->nb_can_put(), "Val and rdy indicated data was available, but no data was available!");
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

class in_port_marker : public sc_object {
public:

  unsigned w;
  bool named;
  sc_in<bool>* val;
  sc_out<bool>* rdy;
  sc_port_base* msg;
  sc_object* bound_to;
  bool top_port;

  in_port_marker() {
    named = false;
    val = 0;
    rdy = 0;
    msg = 0;
    bound_to = 0;
    top_port = false;
  }

  in_port_marker(const char* name, unsigned _w, sc_in<bool>*_val, sc_out<bool>* _rdy, sc_port_base* _msg)
   : sc_object(name)
  {
    named = true;
    val = _val;
    rdy = _rdy;
    msg = _msg;
    bound_to = 0;
    top_port = false;
    
    w = _w;
  }

  void end_of_elaboration() {
   if (val)
    bound_to = dynamic_cast<sc_object*>(val->operator->());
  }
};

class out_port_marker : public sc_object {
public:

  unsigned w;
  bool named;
  sc_out<bool>* val;
  sc_in<bool>* rdy;
  sc_port_base* msg;
  sc_object* bound_to;
  bool top_port;

  out_port_marker() {
    named = false;
    val = 0;
    rdy = 0;
    msg = 0;
    bound_to = 0;
    top_port = false;
  }

  out_port_marker(const char* name, unsigned _w, sc_out<bool>*_val, sc_in<bool>* _rdy, sc_port_base* _msg)
   : sc_object(name)
  {
    named = true;
    val = _val;
    rdy = _rdy;
    msg = _msg;
    bound_to = 0;
    top_port = false;
    
    w = _w;
  }

  void end_of_elaboration() {
   if (val)
    bound_to = dynamic_cast<sc_object*>(val->operator->());
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

  ResetChecker read_reset_check;
  
  // Default constructor
 InBlocking_abs()
   :
#ifdef CONNECTONS_SIM_ONLY
   Blocking_abs(),
#endif
   read_reset_check("unnamed_in")
   {}

  // Constructor
  explicit InBlocking_abs(const char* name)
    :
#ifdef CONNECTONS_SIM_ONLY
    Blocking_abs(),
#endif
    read_reset_check(name)
    {}
  
 public:
  // Reset read
  virtual void Reset() {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }

// Pop
#pragma design modulario < in >
  virtual Message Pop() {
    Message m;
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    return m;
  }

// Peek
#pragma design modulario < in >
  virtual Message Peek() {
    Message m;
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    return m;
  }

// PopNB
#pragma design modulario < in >
  virtual bool PopNB(Message& data, const bool& do_wait = true) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
      {
#ifndef __SYNTHESIS__
	this->read_reset_check.set_val_name(val.name());
#endif
      }

  // Constructor
  explicit InBlocking_Ports_abs(const char* name)
    : InBlocking_abs<Message>(name),
    val(CONNECTIONS_CONCAT(name, "val")),
    rdy(CONNECTIONS_CONCAT(name, "rdy"))
      {
#ifndef __SYNTHESIS__
	this->read_reset_check.set_val_name(val.name());
#endif
      }

 public:
  // Reset read
  virtual void Reset() {
    this->read_reset_check.reset();
    rdy.write(false);
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    this->read_reset_check.check();
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
    this->read_reset_check.check();
    
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
    this->read_reset_check.check();
    
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
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
    this->read_reset_check.reset();
    Reset_SIM();
#else
    InBlocking_Ports_abs<Message>::Reset();
#endif
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
#ifdef CONNECTIONS_SIM_ONLY
    this->read_reset_check.check();
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
    this->read_reset_check.check();
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
    get_conManager().remove(this);
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
   *      #include <connections/connections.h>
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
   *      #include <connections/connections.h>
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
   *      #include <connections/connections.h>
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


  /**
   * \brief Enable random stalling debug message support on an input port.
   * \ingroup Connections
   *
   * Enable random stalling debug messages locally for an input port. Random stalling
   * must already be enabled using CONN_RAND_STALL, enable_global_rand_stall(), or
   * enable_local_rand_stall().
   *
   * Can also enable globally with enable_global_rand_stall_print_debug(). This command takes
   * precendence over the global setting.
   *
   * \par A Simple Example
   * \code
   *      #include <connections/connections.h>
   *
   *      int sc_main(int argc, char *argv[])
   *      {
   *      ...
   *      my_testbench.dut.my_input_port.enable_local_rand_stall_print_debug();
   *      ...
   *      }
   * \endcode
   * \par
   *
   */
  void enable_local_rand_stall_print_debug() {
    local_rand_stall_print_debug_override = true;
    local_rand_stall_print_debug_enable = true;
  }

  /**
   * \brief Disable random stalling debug message support on an input port.
   * \ingroup Connections
   *
   * Disable random stalling debug message.
   *
   * Can also disable globally with disable_global_rand_stall_print_debug(). This command takes
   * precendence over the global setting.
   *
   * \par A Simple Example
   * \code
   *      #include <connections/connections.h>
   *
   *      int sc_main(int argc, char *argv[])
   *      {
   *      ...
   *      enable_global_rand_stall();
   *      enable_global_rand_stall_print_debug();
   *      my_testbench.dut.my_input_port.disable_local_rand_stall_print_debug();
   *      ...
   *      }
   * \endcode
   * \par
   *
   */
  void disable_local_rand_stall_print_debug() {
    local_rand_stall_print_debug_override = true;
    local_rand_stall_print_debug_enable = false;
  }
  
  /**
   * \brief Remove a random stall debug message setting on an input port.
   * \ingroup Connections
   *
   * Removes a random stall debug message setting for an input port.
   *  Effectively cancels enable_local_rand_stall_print_debug() and disable_local_rand_stall_print_debug(),
   * reverting to enable_global_rand_stall_print_debug() or disable_global_rand_stall_print_debug() settings. 
   *
   * \par A Simple Example
   * \code
   *      #include <connections/connections.h>
   *
   *      int sc_main(int argc, char *argv[])
   *      {
   *      ...
   *      // globally enable rand stalling
   *      enable_global_rand_stall();
   *      enable_global_rand_stall_print_debug();
   *      // locally disable rand stalling
   *      my_testbench.dut.my_input_port.disable_local_rand_stall_print_debug();
   *      ...
   *      // cancel local rand stall directive, re-enabling global setting
   *      // on this port
   *      my_testbench.dut.my_input_port.cancel_local_rand_stall_print_debug();
   *      }
   * \endcode
   * \par
   *
   */
  void cancel_local_rand_stall_print_debug() {
    local_rand_stall_print_debug_override = false;
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
  bool local_rand_stall_print_debug_override;
  bool local_rand_stall_print_debug_enable;
  unsigned long rand_stall_counter;
  sc_process_b *actual_process_b;
#endif

  void Init_SIM(const char* name) {
    data_val = false;
    rdy_set_by_api = false;
    get_conManager().add(this);
#ifdef __CONN_RAND_STALL_FEATURE
    double x = rand()%100;
    double y = rand()%100;
    post_pacer = new Pacer(x/100, y/100);
    pacer_stall = false;
    local_rand_stall_override = false;
    local_rand_stall_enable = false;
    local_rand_stall_print_debug_override = false;
    local_rand_stall_print_debug_enable = false;
    actual_process_b = 0;
#endif
  }

  void Reset_SIM() {
#ifdef __CONN_RAND_STALL_FEATURE
    actual_process_b = sc_core::sc_get_current_process_b();
#endif
      
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
    if ((local_rand_stall_override ? local_rand_stall_enable : get_rand_stall_enable()) && pacer_stall) {
      ++rand_stall_counter;
      return true;
    }
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
    if((local_rand_stall_override ? local_rand_stall_enable : get_rand_stall_enable())) {
      if (post_pacer->tic()) {
	if((local_rand_stall_print_debug_override ? local_rand_stall_print_debug_enable : get_rand_stall_print_debug_enable()) && (!pacer_stall)) {
	  std::string name = this->val.name();
	  if(name.substr(name.length() - 4,4) == "_val") { name.erase(name.length() - 4,4); }
	  if(actual_process_b) {
	    CONNECTIONS_COUT("Entering random stall on port " << name << " in thread '" << actual_process_b->basename() << "'." << endl);
	  } else {
	    CONNECTIONS_COUT("Entering random stall on port " << name << " in UNKNOWN thread (port needs to be Reset to register thread)." << endl);
	  }
	  rand_stall_counter = 0;
	}
	pacer_stall=true;
      } else {
	if((local_rand_stall_print_debug_override ? local_rand_stall_print_debug_enable : get_rand_stall_print_debug_enable()) && (pacer_stall)) {
	  std::string name = this->val.name();
	  if(name.substr(name.length() - 4,4) == "_val") { name.erase(name.length() - 4,4); }
	  if(actual_process_b) {
	    CONNECTIONS_COUT("Exiting random stall on port " << name << " in thread '" << actual_process_b->basename() << "'. Was stalled for " << rand_stall_counter << " cycles." << endl);
	  } else {
	    CONNECTIONS_COUT("Exiting random stall on port " << name << " in thread UNKNOWN thread (port needs to be Reset to register thread). Was stalled for " << rand_stall_counter << " cycles." << endl);
	  }
	}
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
    CONNECTIONS_ASSERT_MSG(data_val, "Unreachable state, asked to consume but data isn't valid!");
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
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
 *      #include <connections/connections.h>
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
                                          msg(CONNECTIONS_CONCAT(name, "msg")) {}

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

#ifdef CONNECTIONS_SIM_ONLY
  in_port_marker marker;
#endif


  InBlocking() : InBlocking_SimPorts_abs<Message>(),
    msg(sc_gen_unique_name("in_msg"))
   {}
  
  explicit InBlocking(const char* name) : 
      InBlocking_SimPorts_abs<Message>(name)
    , msg(CONNECTIONS_CONCAT(name, "msg"))
#ifdef CONNECTIONS_SIM_ONLY
    , marker(ccs_concat(name, "in_port_marker"), width, &(this->val), &(this->rdy), &msg)
#endif
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
    marker.top_port = true;
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
    msg(CONNECTIONS_CONCAT(name, "msg"))
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
    i_fifo(CONNECTIONS_CONCAT(name, "i_fifo"))
      {
#ifdef __CONN_RAND_STALL_FEATURE
	Init_SIM(name);
#endif
      }
  
  // Reset read
  void Reset() {
    this->read_reset_check.reset();
    Message temp;
    while (i_fifo->nb_get(temp));
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    this->read_reset_check.check();
#ifdef __CONN_RAND_STALL_FEATURE
    while((local_rand_stall_override ? local_rand_stall_enable : get_rand_stall_enable()) && post_pacer->tic()) { wait(); }
#endif
    return i_fifo->get();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    this->read_reset_check.check();
    return i_fifo->peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data, const bool& do_wait = true) {
    this->read_reset_check.check();
#ifdef __CONN_RAND_STALL_FEATURE
    if((local_rand_stall_override ? local_rand_stall_enable : get_rand_stall_enable()) && post_pacer->tic()) { return false; }
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

  void enable_local_rand_stall_print_debug() {
    local_rand_stall_print_debug_override = true;
    local_rand_stall_print_debug_enable = true;
  }

  void disable_local_rand_stall_print_debug() {
    local_rand_stall_print_debug_override = true;
    local_rand_stall_print_debug_enable = false;
  }

  void cancel_local_rand_stall_print_debug() {
    local_rand_stall_print_debug_override = false;
  }
#endif // __CONN_RAND_STALL_FEATURE
  
 protected:
  sc_port<tlm::tlm_fifo_get_if<Message> > i_fifo;
  
#ifdef __CONN_RAND_STALL_FEATURE
  Pacer *post_pacer;
  bool local_rand_stall_override;
  bool local_rand_stall_enable;
  bool local_rand_stall_print_debug_override;
  bool local_rand_stall_print_debug_enable;
  
  void Init_SIM(const char* name) {
    double x = rand()%100;
    double y = rand()%100;
    post_pacer = new Pacer(x/100, y/100);
    local_rand_stall_override = false;
    local_rand_stall_enable = false;
    local_rand_stall_print_debug_override = false;
    local_rand_stall_print_debug_enable = false;
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

  ResetChecker write_reset_check;
  
  // Default constructor
 OutBlocking_abs()
   :
#ifdef CONNECTIONS_SIM_ONLY
  Blocking_abs(),
#endif
  write_reset_check("unnamed_out")
    {}
  
  // Constructor
  explicit OutBlocking_abs(const char* name)
    :
#ifdef CONNECTIONS_SIM_ONLY
  Blocking_abs(),
#endif
  write_reset_check(name)
    {}
  
 public:

  // Reset write
  void Reset() {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
      {
#ifndef __SYNTHESIS__
	this->write_reset_check.set_val_name(val.name());
#endif
      }
  
  // Constructor
  explicit OutBlocking_Ports_abs(const char* name)
    : OutBlocking_abs<Message>(name),
    val(CONNECTIONS_CONCAT(name, "val")),
    rdy(CONNECTIONS_CONCAT(name, "rdy"))
      {
#ifndef __SYNTHESIS__
	this->write_reset_check.set_val_name(val.name());
#endif
      }
  
 public:

  // Reset write
  void Reset() {
    this->write_reset_check.reset();
    val.write(false);
    reset_msg();
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    this->write_reset_check.check();
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
    this->write_reset_check.check();
    val.write(true);
    write_msg(m);
    wait();
    val.write(false);
    invalidate_msg();
    return rdy.read();
  }

 protected:
    virtual void reset_msg() {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    virtual void write_msg(const Message &m) {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    virtual void invalidate_msg() {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
    this->write_reset_check.reset();
    Reset_SIM();
#else
    OutBlocking_Ports_abs<Message>::Reset();
#endif
  }

  // Push
#pragma design modulario < out >
  void Push(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    this->write_reset_check.check();
    return Push_SIM(m);
#else
    OutBlocking_Ports_abs<Message>::Push(m);
#endif
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
#ifdef CONNECTIONS_SIM_ONLY
    this->write_reset_check.check();
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
    get_conManager().remove(this);
  } 

 protected:
  bool data_val;
  Message data_buf;
  bool val_set_by_api;

  void Init_SIM(const char* name) {
    data_val = false;
    val_set_by_api = false;
    get_conManager().add(this);
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
    CONNECTIONS_ASSERT_MSG(!data_val, "Unreachable state, asked to fill buffer but buffer already full!");
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
  
  explicit OutBlocking(const char* name) : 
      OutBlocking_Ports_abs<Message>(name)
    , msg(CONNECTIONS_CONCAT(name, "msg"))
   {}

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
    // Only allow message invalidation of not going through SLEC design check,
    // since this is assigning to an uninitilized variable, but intentional in
    // HLS for x-prop.
#ifndef SLEC_CPC    
    MsgBits dc_bits;
    msg.write(dc_bits);
#endif
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
#ifdef CONNECTIONS_SIM_ONLY
  out_port_marker marker;
  OutBlocking<Message, MARSHALL_PORT>* driver;
#endif

  OutBlocking() : OutBlocking_SimPorts_abs<Message>(),
    msg(sc_gen_unique_name("out_msg"))
#ifdef CONNECTIONS_SIM_ONLY
    , driver(0)
#endif
    {}
  
  explicit OutBlocking(const char* name) 
    : OutBlocking_SimPorts_abs<Message>(name)
    , msg(CONNECTIONS_CONCAT(name, "msg"))
#ifdef CONNECTIONS_SIM_ONLY
    , marker(ccs_concat(name, "out_port_marker"), width, &(this->val), &(this->rdy), &msg)
    , driver(0)
#endif
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
    return OutBlocking_SimPorts_abs<Message>::PushNB(m,do_wait);
  }
  
  // Bind to OutBlocking
  void Bind(OutBlocking<Message, MARSHALL_PORT>& rhs) {
#ifdef CONNECTIONS_SIM_ONLY
    rhs.disable_spawn();
    rhs.driver = this;
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
    rhs.driver = this;
    rhs.in_bound = true;
    rhs.in_ptr = this;
    marker.top_port = true;
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

#ifdef CONNECTIONS_SIM_ONLY
  Message traced_msg;
#endif
  
  void write_msg(const Message &m) {
#ifdef CONNECTIONS_SIM_ONLY
    traced_msg = m;
#endif    
    Marshaller<WMessage::width> marshaller;
    WMessage wm(m);
    wm.Marshall(marshaller);
    MsgBits bits = marshaller.GetResult();
    msg.write(bits);
  }

  void invalidate_msg() {
    // Only allow message invalidation of not going through SLEC design check,
    // since this is assigning to an uninitilized variable, but intentional in
    // HLS for x-prop.
#ifndef SLEC_CPC    
    MsgBits dc_bits;
    msg.write(dc_bits);
#endif
  }

public:
#ifdef CONNECTIONS_SIM_ONLY
  void set_trace(sc_trace_file* trace_file_ptr, std::string full_name)
  {
    sc_trace(trace_file_ptr, traced_msg, full_name);
  }
#endif
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
    msg(CONNECTIONS_CONCAT(name, "msg"))
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
    o_fifo(CONNECTIONS_CONCAT(name,"o_fifo"))
      {}
  
  // Reset write
  void Reset() {
    this->write_reset_check.reset();
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    this->write_reset_check.check();
    o_fifo->put(m);
    wait(sc_core::SC_ZERO_TIME);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m, const bool& do_wait = true) {
    this->write_reset_check.check();
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
     CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
   }
   
   void disable_annotate() {
     CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
   }

   virtual const char *src_name() {
     CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
     return 0;
   }
   
   virtual const char *dest_name() {
     CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
     return 0;
   }
 };
#endif

template <typename Message>
class Combinational_abs {
  // Abstract class
 protected:
  ResetChecker read_reset_check, write_reset_check;
  
  // Default constructor
  Combinational_abs()
    : read_reset_check("unnamed_comb"),
      write_reset_check("unnamed_comb")
  {}

  // Constructor
  explicit Combinational_abs(const char* name)
    : read_reset_check(name),
      write_reset_check(name)
  {}

 public:
  // Reset
  void ResetRead() {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }

  void ResetWrite() {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    Message m;
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    return m;
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    Message m;
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    return m;
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    return false;
  }

// ekrimer: commenting these out, should really be EmptyWrite and EmptyRead, and in anycase will not work in verilog currently
  // Empty
//  bool Empty() { return !val.read(); }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
      {
#ifndef __SYNTHESIS__
	this->read_reset_check.set_val_name(val.name());
	this->write_reset_check.set_val_name(val.name());
#endif
      }

  // Constructor
  explicit Combinational_Ports_abs(const char* name)
    : Combinational_abs<Message>(name),
    val(CONNECTIONS_CONCAT(name, "val")),
    rdy(CONNECTIONS_CONCAT(name, "rdy")) 
      {
#ifndef __SYNTHESIS__
	this->read_reset_check.set_val_name(val.name());
	this->write_reset_check.set_val_name(val.name());
#endif
      }

 public:
  // Reset
  void ResetRead() {
    this->read_reset_check.reset();
    rdy.write(false); 
  }

  void ResetWrite() {
    this->write_reset_check.reset();
    val.write(false);
    reset_msg();
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    this->read_reset_check.check();
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
    this->read_reset_check.check();
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
    this->read_reset_check.check();
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
    this->write_reset_check.check();
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
    this->write_reset_check.check();
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
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    virtual void read_msg(Message &m) {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    virtual void write_msg(const Message &m) {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    virtual void invalidate_msg() {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
    , in_str(0), out_str(0)
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
    
    , Connections_BA_abs(CONNECTIONS_CONCAT(name, "comb_BA"))
    
    //, in_msg(CONNECTIONS_CONCAT(name, "comb_in_msg"))
    , in_val(CONNECTIONS_CONCAT(name, "comb_in_val"))
    , in_rdy(CONNECTIONS_CONCAT(name, "comb_in_rdy"))
    //, out_msg(CONNECTIONS_CONCAT(name, "comb_out_msg"))
    , out_val(CONNECTIONS_CONCAT(name, "comb_out_val"))
    , out_rdy(CONNECTIONS_CONCAT(name, "comb_out_rdy"))
    
    , current_cycle(0)
    , latency(0)
    
    , out_bound(false), in_bound(false)
    , in_str(0), out_str(0)
    , sim_out(CONNECTIONS_CONCAT(name,"sim_out")), sim_in(CONNECTIONS_CONCAT(name, "sim_in"))
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

    this->read_reset_check.reset();
    
    sim_in.Reset();
    Reset_SIM();
#else
    Combinational_Ports_abs<Message>::ResetRead();
#endif
  }

  void ResetWrite() {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! in_bound); */

    this->write_reset_check.reset();
    
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
    
    this->read_reset_check.check();

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
    
    this->read_reset_check.check();

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
    
    this->read_reset_check.check();

    return sim_in.PopNB(data);
#else
    return Combinational_Ports_abs<Message>::PopNB(data);
#endif
  }

// ekrimer: commenting these out, should really be EmptyWrite and EmptyRead, and in anycase will not work in verilog currently
  // Empty
//  bool Empty() { return !val.read(); }

#ifdef CONNECTIONS_SIM_ONLY
  Message traced_msg;
#endif

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
#ifdef CONNECTIONS_SIM_ONLY
    /* assert(! in_bound); */
    
    this->write_reset_check.check();
    traced_msg = m;

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
    
    this->write_reset_check.check();

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
  const char *in_str, *out_str;
  
  inline bool is_bypass() {
    return (latency == 0);
  }
    
  void annotate(unsigned long latency, unsigned int capacity) {
    this->latency = latency;
    assert(! (latency == 0 && capacity > 0)); // latency == 0 && capacity > 0 is not supported.
    assert(! (latency > 0 && capacity == 0)); // latency > 0 but capacity == 0 is not supported.
    if(capacity > 0) {
      this->b.resize(capacity);
    } else {
      this->b.resize(1);
    }
  }

  void disable_annotate() {
    Connections::get_conManager().remove_annotate(this);
  }
  
  const char *src_name() {
    if(in_str) {
      return in_str;
    } else if(in_bound) {
      if(in_ptr)
	return in_ptr->val.name();
      else
	return "TLM_INTERFACE";
    } else {
      return "UNBOUND";
    }
  }
  
  const char *dest_name() {
    if(out_str) {
      return out_str;
    } else if(out_bound) {
      if(out_ptr)
	return out_ptr->val.name();
      else
	return "TLM_INTERFACE";
    } else {
      return "UNBOUND";
    }
  }

  void enable_local_rand_stall() {
    sim_in.enable_local_rand_stall();
  }

  void disable_local_rand_stall() {
    sim_in.disable_local_rand_stall();
  }

  void cancel_local_rand_stall() {
    sim_in.cancel_local_rand_stall();
  }
  
  void enable_local_rand_stall_print_debug() {
    sim_in.enable_local_rand_stall_print_debug();
  }

  void disable_local_rand_stall_print_debug() {
    sim_in.disable_local_rand_stall_print_debug();
  }

  void cancel_local_rand_stall_print_debug() {
    sim_in.cancel_local_rand_stall_print_debug();
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
    Connections::get_conManager().add(this);
    Connections::get_conManager().add_annotate(this);
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
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }
  virtual void read_msg(Message &m) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }
  virtual void write_msg(const Message &m) {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
  }
  virtual void invalidate_msg() {
    CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
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
                                             msg(CONNECTIONS_CONCAT(name, "msg"))
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
    // Only allow message invalidation of not going through SLEC design check,
    // since this is assigning to an uninitilized variable, but intentional in
    // HLS for x-prop.
#ifndef SLEC_CPC    
    MsgBits dc_bits;
    msg.write(dc_bits);
#endif
  }
};  

#ifdef CONNECTIONS_SIM_ONLY
class sc_trace_marker {
public:
  virtual void set_trace(sc_trace_file* trace_file_ptr) = 0;
};
#endif


 
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
  OutBlocking<Message>* driver;
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
      driver = 0;
      
      //SC_METHOD(do_bypass);
      declare_method_process(do_bypass_handle, sc_gen_unique_name("do_bypass"), SC_CURRENT_USER_MODULE, do_bypass);
      this->sensitive << in_msg << this->in_val << this->out_rdy;
#endif
    }
  
  explicit Combinational(const char* name) : Combinational_SimPorts_abs<Message, MARSHALL_PORT>(name)

#ifdef CONNECTIONS_SIM_ONLY
    ,in_msg(CONNECTIONS_CONCAT(name, "comb_in_msg"))
    ,out_msg(CONNECTIONS_CONCAT(name, "comb_out_msg"))
    ,dummyPortManager(CONNECTIONS_CONCAT(name, "dummyPortManager"), this->sim_in, this->sim_out, *this)
#else
    ,msg(CONNECTIONS_CONCAT(name, "msg"))
#endif
    {
#ifdef CONNECTIONS_SIM_ONLY
      driver = 0;
      
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
    // Only allow message invalidation of not going through SLEC design check,
    // since this is assigning to an uninitilized variable, but intentional in
    // HLS for x-prop.
#ifndef SLEC_CPC    
    MsgBits dc_bits;
#ifdef CONNECTIONS_SIM_ONLY
    out_msg.write(dc_bits);
#else // ifdef CONNECTIONS_SIM_ONLY
    msg.write(dc_bits);
#endif // ifdef CONNECTIONS_SIM_ONLY
#endif // ifndef SLEC_CPC
  }

#ifdef CONNECTIONS_SIM_ONLY
 protected:
    class DummyPortManager : public sc_module, public sc_trace_marker {
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

    virtual void set_trace(sc_trace_file* trace_file_ptr)
    {
      sc_trace(trace_file_ptr, parent.out_val, parent.out_val.name());
      sc_trace(trace_file_ptr, parent.out_rdy, parent.out_rdy.name());

      if (!parent.driver)
      {
        sc_trace(trace_file_ptr, parent.traced_msg, parent.out_msg.name());
      }
      else
      {
        OutBlocking<Message, MARSHALL_PORT>* driver = parent.driver;
        while (driver->driver)
          driver = driver->driver;

        driver->set_trace(trace_file_ptr, parent.out_msg.name());
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
    ,in_msg(CONNECTIONS_CONCAT(name, "comb_in_msg"))
    ,out_msg(CONNECTIONS_CONCAT(name, "comb_out_msg"))
    ,dummyPortManager(CONNECTIONS_CONCAT(name, "dummyPortManager"), this->sim_in, this->sim_out, *this)
#else
    ,msg(CONNECTIONS_CONCAT(name, "msg"))
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
                    ,fifo(CONNECTIONS_CONCAT(name, "fifo"), 1) {}

  // Reset
  void ResetRead() {
    this->read_reset_check.reset();
    Message temp;
    while (fifo.nb_get(temp));
  }

  void ResetWrite() {
    this->write_reset_check.reset();
  }

// Pop
#pragma design modulario < in >
  Message Pop() {
    this->read_reset_check.check();
    return fifo.get();
  }

// Peek
#pragma design modulario < in >
  Message Peek() {
    this->read_reset_check.check();
    return fifo.peek();
  }

// PopNB
#pragma design modulario < in >
  bool PopNB(Message& data) {
    this->read_reset_check.check();
    return fifo.nb_get(data);
  }

// Push
#pragma design modulario < out >
  void Push(const Message& m) {
    this->write_reset_check.check();
    fifo.put(m);
  }

// PushNB
#pragma design modulario < out >
  bool PushNB(const Message& m) {
    this->write_reset_check.check();
    return fifo.nb_put(m);
  }

 protected:
    void reset_msg() {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    void read_msg(Message &m) {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    void write_msg(const Message &m) {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }
    void invalidate_msg() {
      CONNECTIONS_ASSERT_MSG(0, "Unreachable virtual function in abstract class!");
    }

 public:
  tlm::tlm_fifo<Message> fifo;
};
#endif
	  
}  // namespace Connections

#endif  // __CONNECTIONS__CONNECTIONS_H__
