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

#ifndef __CONNECTIONS__PACER_H__
#define __CONNECTIONS__PACER_H__
#include <cstdlib>

// TODO: add operator+ that combindes two pacers
// TODO: add a deterministic Pacer as well
/**
 * \brief Class used to inject random stalls in testbench 
 * \ingroup Pacer
 * 
 * \par Overview
 *      Class to inject random stalls in tests
 *
 * \par A Simple Example
 * \code
 *   #include <connections/Pacer.h>
 *  
 *   ...
 *  SC_MODULE(Source) {
 *   Pacer pacer;
 * 
 *   void run() {
 *     ...
 *
 *     while (1) {
 *       wait();
 * 
 *       ... 
 *       while (pacer.tic())
 *         wait();
 *     }
 *   }
 * 
 *   SC_HAS_PROCESS(Source);
 * 
 *   Source(sc_module_name name_, const Pacer& pacer_)
 *       : sc_module(name_),
 *         clk("clk"),
 *         rst("rst"),
 *         pacer(pacer_) {
 *     SC_THREAD(run);
 *     sensitive << clk.pos();
 *     NEG_RESET_SIGNAL_IS(rst);
 *   }
 * }
 *      
 * \endcode
 * \par
 *
 */



class Pacer {
 protected:
  static const int precision = 1000;

  int stall_prob;
  int hold_stall_prob;
  bool stalled;

  virtual bool random(const int& prob) {
    return (rand() % (precision + 1) < prob);
  }

 public:
  Pacer(const float& stall_prob_, const float& hold_stall_prob_)
      : stall_prob(stall_prob_ * static_cast<float>(precision)),
        hold_stall_prob(hold_stall_prob_ * static_cast<float>(precision)),
        stalled(false) {}

  virtual void reset() { stalled = false; }

  // return whether we should stall this cycle
  virtual bool tic() {
#ifdef DISABLE_PACER
    return false;
#endif
    if (stalled) {
      if (!random(hold_stall_prob)) {
        stalled = false;
      }
    } else {
      if (random(stall_prob)) {
        stalled = true;
      }
    }
    return stalled;
  }
};

#endif // __CONNECTIONS__PACER_H_
