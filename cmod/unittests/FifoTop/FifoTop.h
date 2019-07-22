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
#ifndef FIFO_TOP_H
#define FIFO_TOP_H

#include <fifo.h>
#include <hls_globals.h>
#include <nvhls_assert.h>

#ifndef FIFO_LENGTH
#define FIFO_LENGTH 3
#endif

#ifndef WORD_WIDTH
#define WORD_WIDTH 16
#endif

#ifndef NUM_BANKS
#define NUM_BANKS 4
#endif


typedef NVUINTC(WORD_WIDTH) MemWord_t; 


enum FifoOp {
    push=0,
    pop,
    incrHead,
    peek,
    isEmpty,
    isFull,
    reset,
    get_head,
    get_tail,
    MAXOP
};

typedef FIFO<MemWord_t, FIFO_LENGTH, NUM_BANKS> Fifo_;
typedef MemWord_t DataType;
typedef Fifo_::FifoIdx FifoIdx;
typedef DataType OutType;
typedef Fifo_::BankIdx BankIdx;
typedef NVUINTC(nvhls::nbits<MAXOP -1 >::val) OpType;

void FifoTop( const OpType& op, const DataType& data, const BankIdx& idx, OutType& out);

#endif
