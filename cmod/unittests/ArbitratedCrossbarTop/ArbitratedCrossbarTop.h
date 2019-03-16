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

#ifndef ARBITRATED_CROSSBAR_TOP_H
#define ARBITRATED_CROSSBAR_TOP_H

#include <systemc.h>
#include <arbitrated_crossbar.h>

typedef NVUINT16 Word_t;
#ifndef NUM_INPUTS
#define NUM_INPUTS 4
#endif

#ifndef NUM_OUTPUTS
#define NUM_OUTPUTS 8
#endif

#ifndef LEN_INPUT_BUFFER
#define LEN_INPUT_BUFFER 4
#endif

#ifndef LEN_OUTPUT_BUFFER
#define LEN_OUTPUT_BUFFER 2
#endif

typedef Word_t DataInArray[NUM_INPUTS];
typedef Word_t DataOutArray[NUM_OUTPUTS];
static const int log2_outputs = nvhls::index_width<NUM_OUTPUTS>::val;
typedef NVUINTW(log2_outputs) OutputIdx;
typedef OutputIdx OutputIdxArray[NUM_INPUTS];
typedef bool ReadyArray[NUM_INPUTS];
typedef bool ValidInArray[NUM_INPUTS];
typedef bool ValidOutArray[NUM_OUTPUTS];
typedef bool ReadyArray[NUM_INPUTS];

void ArbitratedCrossbarTop(DataInArray data_in, OutputIdxArray dest_in,
           ValidInArray valid_in, DataOutArray data_out,
	   ValidOutArray valid_out, ReadyArray ready);
#endif  // end ARBITRATED_CROSSBAR_TOP_H
