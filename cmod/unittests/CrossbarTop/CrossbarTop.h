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
#ifndef CROSSBAR_TOP_H
#define CROSSBAR_TOP_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <crossbar.h>
#include <hls_globals.h>

#ifndef NUM_INPUTS
#define NUM_INPUTS 4
#endif

#ifndef NUM_OUTPUTS 
#define NUM_OUTPUTS 8
#endif

#ifndef DATA_TYPE
#define DATA_TYPE NVINT32
#endif

void CrossbarTop(
    DATA_TYPE data_in[NUM_INPUTS], 
#ifndef NO_XBAR_VALID_IN_OUT
    bool valid_in[NUM_INPUTS], 
#endif
    NVUINTC(nvhls::nbits<NUM_INPUTS-1>::val) source[NUM_OUTPUTS], 
#ifndef NO_XBAR_VALID_SOURCE
    bool valid_source[NUM_OUTPUTS],
#endif
    DATA_TYPE data_out[NUM_OUTPUTS]
#ifndef NO_XBAR_VALID_IN_OUT
    ,bool valid_out[NUM_OUTPUTS] 
#endif
    );


#endif
