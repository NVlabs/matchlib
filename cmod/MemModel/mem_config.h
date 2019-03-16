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
#ifndef MEM_CONFIG_H
#define MEM_CONFIG_H

#include "nvhls_int.h"
#include "nvhls_types.h"

#ifndef WORDSIZE
#define WORDSIZE 64
#endif
#ifndef NBANKS
#define NBANKS 1
#endif
#ifndef NUM_ENTRIES
#define NUM_ENTRIES 1024
#endif

enum MemOp_t {
READ,
WRITE
};


#define ADDR_WIDTH ((NUM_ENTRIES==1)?1:nvhls::nbits<NUM_ENTRIES-1>::val)
#define NBANKS_LOG2 ((NBANKS==1)?1:nvhls::nbits<NBANKS-1>::val)
#define NUM_ENTRIES_PER_BANK (NUM_ENTRIES/NBANKS)
#define BANK_ADDR_WIDTH ((NUM_ENTRIES_PER_BANK==1)?1:nvhls::nbits<NUM_ENTRIES_PER_BANK-1>::val)

typedef NVUINTC(WORDSIZE) MemWord_t;
typedef NVUINTC(ADDR_WIDTH) MemAddr_t; 
typedef NVUINTC(NBANKS_LOG2) BankSel_t;
typedef NVUINTC(BANK_ADDR_WIDTH) BankAddr_t; 

#endif
