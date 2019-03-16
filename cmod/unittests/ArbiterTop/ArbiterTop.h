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
#ifndef ARBITER_TOP_H
#define ARBITER_TOP_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <Arbiter.h>
#include <hls_globals.h>

#ifndef NUM_INPUTS
#define NUM_INPUTS 5
#endif

#ifndef ARBITER_TYPE
#define ARBITER_TYPE Roundrobin
#endif

typedef Arbiter<NUM_INPUTS, ARBITER_TYPE> arbiter_t;
typedef arbiter_t::Mask     mask_t;

void ArbiterTop(const mask_t& valid, mask_t& select);

#endif
