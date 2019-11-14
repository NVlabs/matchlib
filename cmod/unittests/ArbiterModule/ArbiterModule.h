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
#ifndef ARBITER_MODULE_H
#define ARBITER_MODULE_H

#include <systemc.h>

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_connections.h>
#include <hls_globals.h>
#include <Arbiter.h>

#ifndef NUM_INPUTS
#define NUM_INPUTS 5
#endif

class ArbiterModule : public sc_module {
public:
  typedef Arbiter<NUM_INPUTS> arbiter_t;
  typedef arbiter_t::Mask     mask_t;
  sc_in_clk clk;
  sc_in<bool> rst;

  Connections::In<mask_t> valid_in;
  Connections::Out<mask_t> select_out;
  enum {
    width = 0
  };

  arbiter_t arbiter;

  void Process();

  SC_HAS_PROCESS(ArbiterModule);
  ArbiterModule(sc_module_name name)
      : sc_module(name), clk("clk"), rst("rst"), valid_in("valid_in"),
        select_out("select_out") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  };
};

void ArbiterModule::Process() {
  valid_in.Reset();
  select_out.Reset();
  wait();

  #pragma hls_pipeline_init_interval 1
  while (1) {
    mask_t valid_reg;
    mask_t select_reg;

    if (valid_in.PopNB(valid_reg)){
       select_reg = arbiter.pick(valid_reg);
       select_out.Push(select_reg);
    }
    wait();
  }
}


#endif
