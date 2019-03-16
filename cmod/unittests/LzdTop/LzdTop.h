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
#ifndef LZD_TOP_H
#define LZD_TOP_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <hls_globals.h>

#ifndef NUM_BITS
#define NUM_BITS 32
#endif

typedef NVUINTC(NUM_BITS) Data;
typedef NVUINTC(nvhls::nbits<NUM_BITS-1>::val) Count;


void LzdTop(const Data& in, Count& out);


#endif
