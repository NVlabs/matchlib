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
/*
 * Copyright (c) 2015 Nvidia Corporation. All rights reserved.
 *
 * trace.hpp
 *
 *  Created on: Sep 8, 2015
 *    Author: mpellauer
 */

#ifndef MATCH_TRACE_H_
#define MATCH_TRACE_H_

#ifndef __SYNTHESIS__

#include <iostream>
#include <sstream>

#endif

namespace match {

// Dummy class indicating it's time to flush.
class Flusher {
 public:
  Flusher() {}
  ~Flusher() {}
};

// Buffers messages locally then dumps its local
// contents to the shared buffer.
// Also supports trace levels.
/**
 * \brief Tracer class to dump simulation stats to an output stream (stdout by default). 
 * \ingroup Tracer
 *
 * \par A Simple Example
 * \code
 * #include <nvhls_module.h>
 *
 * class CrossbarModuleConnectionsInterface : public match::Module {
 *  public:
 *   ... 
 *   void Process();
 * 
 *   SC_HAS_PROCESS(CrossbarModuleConnectionsInterface);
 *   CrossbarModuleConnectionsInterface(sc_module_name name_)
 *       : match::Module(name_) {
 *     SC_THREAD(Process);
 *     sensitive << clk.pos();
 *     NVHLS_NEG_RESET_SIGNAL_IS(rst);
 * 
 *     this->SetTraceLevel(2); // Set trace level
 *   }
 * };
 * 
 * void CrossbarModuleConnectionsInterface::Process() {
 *   ...
 * 
 *   while(1) {
 *     wait();
 *     T(1) << " Entered DUT" << EndT; // Trace print statements
 * 
 *     #pragma hls_unroll yes
 *     for(int inp_lane=0; inp_lane<NumInputLanes; inp_lane++) {
 *       data_in_reg[inp_lane] = data_in[inp_lane].Pop();
 *       T(2) << "data_in["   << inp_lane << "] = " << data_in_reg[inp_lane] << EndT;
 *     }
 *     ...
 *   }
 * }
 *
 * \endcode
 *
 */

class Tracer {
 protected:
#ifndef __SYNTHESIS__
  std::ostream* ostr_;
  int trace_level_;
  int cur_level_;
  bool fatal_;
#endif
 public:
#ifndef __SYNTHESIS__
  explicit Tracer(std::ostream* o = NULL)
      : ostr_(o),
        trace_level_(0),
        cur_level_(0),
        fatal_(false)
#else
  explicit Tracer()
#endif
  {
#ifndef __SYNTHESIS__
    // Default to stdout
    if (!ostr_) {
      ostr_ = &std::cout;
    }
#endif
  }
  Tracer(const Tracer& other)
#ifndef __SYNTHESIS__
      : ostr_(other.ostr_),
        trace_level_(other.trace_level_),
        cur_level_(other.cur_level_),
        fatal_(other.fatal_)
#endif
  {
  }
  Tracer& operator=(const Tracer& other) {
#ifndef __SYNTHESIS__
    ostr_ = other.ostr_;
    trace_level_ = other.trace_level_;
    cur_level_ = other.cur_level_;
    fatal_ = other.fatal_;
#endif
    return *this;
  }
#ifndef __SYNTHESIS__
  template <typename T_MSG>
  Tracer& operator<<(T_MSG t) {
    if (cur_level_ <= trace_level_) {
      (*ostr_) << t;
    }
    return *this;
  }

  Tracer& operator<<(std::ostream& (*f)(std::ostream&)) {
    if (cur_level_ <= trace_level_) {
      // Handle all the std::ostream stuff
      (*ostr_) << f;
    }
    return *this;
  }

  Tracer& operator<<(Tracer& (*f)(Tracer&)) {
    // Just apply the function.
    return f(*this);
  }

  Tracer& operator<<(Flusher& endt) {
    // Just flush the buffer.
    return FlushBuffer();
  }

  Tracer& FlushBuffer() {
    if (cur_level_ <= trace_level_) {
      (*ostr_) << std::endl;
    }
    if (fatal_)
      exit(1);
    return *this;
  }
#else
  template <typename T_MSG>
  Tracer& operator<<(T_MSG t) {
    return *this;
  }
#endif
  int GetTraceLevel() {
#ifndef __SYNTHESIS__
    return trace_level_;
#else
    return 0;
#endif
  }

  void SetTraceLevel(int l) {
#ifndef __SYNTHESIS__
    trace_level_ = l;
#endif
  }
  void SetCurrentLevel(int l) {
#ifndef __SYNTHESIS__
    cur_level_ = l;
#endif
  }
  void SetFatal() {
#ifndef __SYNTHESIS__
    fatal_ = true;  // Exit after next buffer dump.
#endif
  }
};

}  // namspace match

#endif  // INCLUDE_TRACE_H_
