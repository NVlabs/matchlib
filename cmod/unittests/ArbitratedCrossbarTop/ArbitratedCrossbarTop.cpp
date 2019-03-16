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

#include <systemc.h>
#include <arbitrated_crossbar.h>
#include "ArbitratedCrossbarTop.h"

void ArbitratedCrossbarTop(DataInArray data_in, OutputIdxArray dest_in,
           ValidInArray valid_in, DataOutArray data_out,
           ValidOutArray valid_out, ReadyArray ready) {
  // Instantiate DUT and reset it
  static ArbitratedCrossbar<Word_t, NUM_INPUTS, NUM_OUTPUTS,
                              LEN_INPUT_BUFFER, LEN_OUTPUT_BUFFER> dut;
  Word_t data_in_local[NUM_INPUTS];
  OutputIdx dest_in_local[NUM_INPUTS];
  bool valid_in_local[NUM_INPUTS];
  Word_t data_out_local[NUM_OUTPUTS];
  bool valid_out_local[NUM_OUTPUTS];
  bool ready_local[NUM_INPUTS];


  #pragma unroll yes
  for (int i = 0; i < NUM_INPUTS; i++) {
    data_in_local[i] = data_in[i];
    dest_in_local[i] = dest_in[i];
    valid_in_local[i] = valid_in[i];
  }
  dut.run(data_in_local,dest_in_local,valid_in_local,data_out_local,valid_out_local,ready_local);
  if (LEN_OUTPUT_BUFFER > 0) {
    dut.pop_all_lanes(valid_out_local);
  }
  #pragma unroll yes
  for (int i = 0; i < NUM_OUTPUTS; i++) {
    data_out[i] = data_out_local[i];
    valid_out[i] = valid_out_local[i];
    // Reset to 0 to avoid driving X
    if (!valid_out[i]) {
      data_out[i] = 0;
    }
  }
  #pragma unroll yes
  for (int i = 0; i < NUM_INPUTS; i++) {
    ready[i] = ready_local[i];
  }
}
