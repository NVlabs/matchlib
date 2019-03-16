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
#ifndef MEM_MODEL_H
#define MEM_MODEL_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <mem_array.h>
#include <hls_globals.h>
#include "mem_config.h"

// Top-level function used to model mem_array 
// data represent the i/o data line
// mem_op is memory operation type (READ/WRITE)
// addr is the memory address
void MemModel(MemWord_t write_data, MemWord_t & read_data, NVUINT1 mem_op, MemAddr_t addr);

#endif
