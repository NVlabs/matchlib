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

#ifndef ARBITRATED_SCRATCHPAD_TOP_CONFIG_H
#define ARBITRATED_SCRATCHPAD_TOP_CONFIG_H

#include "nvhls_int.h"
#include "nvhls_types.h"
#include "ArbitratedScratchpad/ArbitratedScratchpadTypes.h"

#ifndef DATA_TYPE
#define DATA_TYPE NVUINT32
#endif

#ifndef NUM_BANK_ENTRIES
#define NUM_BANK_ENTRIES 256
#endif

#ifndef NUM_BANKS
#define NUM_BANKS 4
#endif

#ifndef NUM_INPUTS
#define NUM_INPUTS NUM_BANKS*2
#endif

#ifndef LEN_INPUT_BUFFER
#define LEN_INPUT_BUFFER 0
#endif

const unsigned NumBanks            = NUM_BANKS;
const unsigned ScratchpadCapacity  = NUM_BANKS * NUM_BANK_ENTRIES;
const unsigned ScratchpadAddrWidth = nvhls::nbits<ScratchpadCapacity - 1>::val;
const unsigned NumInputs           = NUM_INPUTS;
const unsigned InputQueueLength    = LEN_INPUT_BUFFER;
const unsigned RefMemSizeInBytes   = 16 * 1024 * 1024;

typedef DATA_TYPE DataType;

// client request and response types with TB parameters
typedef cli_req_t<DataType, ScratchpadAddrWidth, NumInputs> tb_cli_req_t;
typedef cli_rsp_t<DataType, NumInputs> tb_cli_rsp_t;

#endif  // end ARBITRATED_SCRATCHPAD_TOP_CONFIG_H
