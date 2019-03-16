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
#ifndef SCRATCHPAD_CONFIG_H
#define SCRATCHPAD_CONFIG_H

#include <nvhls_int.h>


#define REF_MEM_SIZE_IN_BYTES 16 * 1024 * 1024

// These are defines that scripts will over-ride at the command line to test the scratchpad
#ifndef SCRATCHPAD_BANKS
  #define SCRATCHPAD_BANKS 4
#endif
#ifndef SCRATCHPAD_CAPACITY
  #define SCRATCHPAD_CAPACITY SCRATCHPAD_BANKS * 256
#endif
#define SCRATCHPAD_ADDR_WIDTH nvhls::nbits<SCRATCHPAD_CAPACITY-1>::val


// Some convenience typedefs
typedef NVUINT32 data32_t;  // Data returned to the client.


#endif

