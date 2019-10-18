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
// TestSink.h
//========================================================================
// Modules in this file are not mean't to be synthesized

#ifndef TEST_SINK_H
#define TEST_SINK_H

#include <vector>
#include <iomanip>
#include <systemc.h>
#include <nvhls_connections.h>
#include <testbench/Pacer.h>

//------------------------------------------------------------------------
// TestSinkBlocking
//------------------------------------------------------------------------

template< typename T >
class TestSinkBlocking : public sc_module {
  SC_HAS_PROCESS(TestSinkBlocking);

 public:
  // Module Interface
  sc_in_clk            clk;
  sc_in<bool>          rst;
  Connections::In< T > in_;
  Pacer pacer;

  TestSinkBlocking(sc_module_name name, const Pacer& pacer_, std::vector<T>& data)
    : sc_module(name),
      clk("clk"),
      rst("rst"),
      in_("in_"),
      pacer(pacer_),
      msgs(data),
      count(0)
  {
    SC_CTHREAD(tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  void tick() {
    in_.Reset();
    go = false;
    typename std::vector<T>::iterator it = msgs.begin();
    while (1) {
      if (go) {
        T msg = in_.Pop();
        if (msg != *it) {
          std::cout << "FAILED: msg[" << count << "] != ref["
            << count << "] (" << std::hex << msg << " != "
            << *it << " )" << std::endl;
          sc_stop();
        }
        ++count;
        ++it;
        if (it == msgs.end())
          sc_stop();
        wait();
        while (pacer.tic()) {
          cout << "@" << sc_time_stamp() << "\t" << name() << " STALL" << endl;
          wait();
        }
      } else {
        wait();
      }
    }
  }

  void line_trace() {
#ifndef CONNECTIONS_FAST_SIM // Ports don't exist in fast sim (TLM mode)
    if (rst.read()) {
      unsigned int width = (T().length()/4);
      if (in_.val.read() && in_.rdy.read()) {
        std::cout << std::hex << std::setw(width) << in_.msg.read();
      }
      else {
        std::cout << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
#endif    
  }
  void Go() {
    go = true;
  }

 private:
  std::vector<T>& msgs;
  unsigned int    count;
  bool go;
};

//------------------------------------------------------------------------
// TestSinkNonBlocking
//------------------------------------------------------------------------

template< typename T >
class TestSinkNonBlocking : public sc_module {
  SC_HAS_PROCESS(TestSinkNonBlocking);

 public:
  // Module Interface
  sc_in_clk            clk;
  sc_in<bool>          rst;
  Connections::In< T > in_;
  Pacer pacer;

  TestSinkNonBlocking(sc_module_name name, const Pacer& pacer_, std::vector<T>& data)
    : sc_module(name),
      clk("clk"),
      rst("rst"),
      in_("in_"),
      pacer(pacer_),
      msgs(data),
      count(0)
  {
    SC_CTHREAD(tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  void tick() {
    in_.Reset();
    go = false;
    typename std::vector<T>::iterator it = msgs.begin();
    while (1) {
      if (go) {
        T msg;
        if (in_.PopNB(msg)) {
          if (msg != *it) {
            std::cout << "FAILED: msg[" << count << "] != ref["
              << count << "] (" << std::hex << msg << " != "
              << *it << " )" << std::endl;
            sc_stop();
          }
          ++count;
          ++it;
          if (it == msgs.end())
            sc_stop();
        }
        wait();
        while (pacer.tic()) {
          cout << "@" << sc_time_stamp() << "\t" << name() << " STALL" << endl;
          wait();
        }
      } else {
        wait();
      }
    }
  }

  void line_trace() {
    if (rst.read()) {
      unsigned int width = (T().length()/4);
      if (in_.val.read() && in_.rdy.read()) {
        std::cout << std::hex << std::setw(width) << in_.msg.read();
      }
      else {
        std::cout << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
  }
  void Go() {
    go = true;
  }

 private:
  unsigned int    count;
  std::vector<T>& msgs;
  bool go;
};



//------------------------------------------------------------------------
// TestSinkBuffered
//------------------------------------------------------------------------

template< typename T, int W >
class TestSinkBuffered : public sc_module {
  SC_HAS_PROCESS(TestSinkBuffered);

 public:
  // Module Interface
  sc_in_clk            clk;
  sc_in<bool>          rst;
  Connections::InBuffered< T, W > in_;
  Pacer pacer;

  TestSinkBuffered(sc_module_name name, const Pacer& pacer_, std::vector<T>& data)
    : sc_module(name),
      clk("clk"),
      rst("rst"),
      in_("in_"),
      pacer(pacer_),
      count(0),
      msgs(data)
  {
    SC_CTHREAD(tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  void tick() {
    in_.Reset();
    go = false;
    typename std::vector<T>::iterator it = msgs.begin();
    while (1) {
      in_.TransferNB();
      if (go) {
        T msg;
        if (!in_.Empty()) {
          msg = in_.Pop();
          if (msg != *it) {
            std::cout << "FAILED: msg[" << count << "] != ref["
              << count << "] (" << std::hex << msg << " != "
              << *it << " )" << std::endl;
            sc_stop();
          }
          ++count;
          ++it;
          if (it == msgs.end())
            sc_stop();
        }
        wait();
        while (pacer.tic()) {
          cout << "@" << sc_time_stamp() << "\t" << name() << " STALL" << endl;
          wait();
        }
      } else {
        wait();
      }
    }
  }

  void line_trace() {
#ifndef CONNECTIONS_FAST_SIM      
    if (rst.read()) {
      unsigned int width = (T().length()/4);
      if (in_.val.read() && in_.rdy.read()) {
        std::cout << std::hex << std::setw(width) << in_.msg.read();
      }
      else {
        std::cout << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
#endif    
  }
  void Go() {
    go = true;
  }

 private:
  unsigned int    count;
  std::vector<T>& msgs;
  bool go;
};


#endif  // TEST_SINK_H
