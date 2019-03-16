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
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <crossbar.h>
#include <hls_globals.h>
#include "CrossbarTop.h"

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
    )
{

    crossbar<DATA_TYPE, NUM_INPUTS, NUM_OUTPUTS>(
        data_in,
#ifndef NO_XBAR_VALID_IN_OUT
        valid_in,
#endif
        source,
#ifndef NO_XBAR_VALID_SOURCE
        valid_source,
#endif
        data_out
#ifndef NO_XBAR_VALID_IN_OUT
        ,valid_out
#endif
        );
}
