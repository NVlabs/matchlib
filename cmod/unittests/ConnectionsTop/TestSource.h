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
// TestSource.h
//========================================================================
// Modules in this file are not mean't to be synthesized

#ifndef TEST_SOURCE_H
#define TEST_SOURCE_H

#include <vector>
#include <iomanip>
#include <systemc.h>
#include <nvhls_connections.h>
#include <testbench/Pacer.h>

//------------------------------------------------------------------------
// TestSourceBlocking
//------------------------------------------------------------------------

template< typename T >
class TestSourceBlocking : public sc_module {
  SC_HAS_PROCESS(TestSourceBlocking);

 public:
  // Module Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::Out<T> out;
  Pacer pacer;
  std::vector<T>& msgs;

  TestSourceBlocking(sc_module_name name, const Pacer& pacer_, std::vector<T>& data)
    : sc_module(name),
      clk("clk"),
      rst("rst"),
      out("out"),
      pacer(pacer_),
      msgs(data)
  {
    SC_CTHREAD(tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  void tick() {
    out.Reset();
    bool done = false;
    go = false;
    typename std::vector<T>::iterator it = msgs.begin();
    while (!done) {
      if (go) {
        out.Push(*it);
        //cout << "@" << sc_time_stamp() << "\t" << name() << " Push x: " << x << endl;
        ++it;
        if (it == msgs.end()) {
          done = true;
          go = false;
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
#ifndef CONNECTIONS_FAST_SIM // Ports don't exist in fast sim (TLM mode)
    if (rst.read()) {
      unsigned int width = (T().length()/4);
      if (out.val.read() && out.rdy.read()) {
        std::cout << sc_time_stamp() << " " << std::hex << std::setw(width) << out.msg.read();
      }

      else {
        std::cout << sc_time_stamp() << " " << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
#endif
  }
  void Go() {
    go = true;
  }
 private:
  bool go;
};

//------------------------------------------------------------------------
// TestSourceNonBlocking
//------------------------------------------------------------------------

template< typename T >
class TestSourceNonBlocking : public sc_module {
  SC_HAS_PROCESS(TestSourceNonBlocking);

 public:
  // Module Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::Out<T> out;
  Pacer pacer;
  std::vector<T>& msgs;

  TestSourceNonBlocking(sc_module_name name, const Pacer& pacer_, std::vector<T>& data)
    : sc_module(name),
      clk("clk"),
      rst("rst"),
      out("out"),
      pacer(pacer_),
      msgs(data)
  {
    SC_CTHREAD(tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  void tick() {
    out.Reset();
    bool done = false;
    go = false;
    typename std::vector<T>::iterator it = msgs.begin();
    while (!done) {
      if (go) {
        if (out.PushNB(*it)) {
          ++it;
          if (it == msgs.end()) {
            done = true;
            go = false;
          }
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
      if (out.val.read() && out.rdy.read()) {
        std::cout << sc_time_stamp() << " " << std::hex << std::setw(width) << out.msg.read();
      }
      else {
        std::cout << sc_time_stamp() << " " << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
  }
  void Go() {
    go = true;
  }
 private:
  bool go;
};

//------------------------------------------------------------------------
// TestSourceNonBlocking
//------------------------------------------------------------------------

template< typename T, int W >
class TestSourceBuffered : public sc_module {
  SC_HAS_PROCESS(TestSourceBuffered);

 public:
  // Module Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::OutBuffered<T, W> out;
  Pacer pacer;
  std::vector<T>& msgs;

  TestSourceBuffered(sc_module_name name, const Pacer& pacer_, std::vector<T>& data)
    : sc_module(name),
      clk("clk"),
      rst("rst"),
      out("out"),
      pacer(pacer_),
      msgs(data)
  {
    SC_CTHREAD(tick, clk.pos());
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }

  void tick() {
    out.Reset();
    bool done = false;
    go = false;
    typename std::vector<T>::iterator it = msgs.begin();
    while (1) {
      out.TransferNB();
      if (go && !done) {
        if (!out.Full()) {
          out.Push(*it);
          ++it;
          if (it == msgs.end()) {
            done = true;
            go = false;
          }
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
      if (out.val.read() && out.rdy.read()) {
        std::cout << sc_time_stamp() << " " << std::hex << std::setw(width) << out.msg.read();
      }
      else {
        std::cout << sc_time_stamp() << " " << std::setw(width+1) << " ";
      }
      std::cout << " | ";
    }
#endif    
  }
  void Go() {
    go = true;
  }
 private:
  bool go;
};

#endif  // TEST_SOURCE_H
