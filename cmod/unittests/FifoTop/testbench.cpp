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
#include <stdio.h>
#include "FifoTop.h"
#include <match_scverify.h>
#include <testbench/nvhls_rand.h>

#include <deque>

#ifndef NUM_ITER
#define NUM_ITER 10000
#endif


OpType semi_rand_op(int size)
{
    OpType op;

    do {
        op = rand()%MAXOP;
    } while (   // make sure to avoid data access or header increase while empty and pushing while full, any of these would fail assertions
                (size == 0 && (op==pop || op==incrHead || op==peek)) ||
                (size==FIFO_LENGTH && (op==push)));

    return op;
}

CCS_MAIN(int argc, char *argv[]) {

    nvhls::set_random_seed();

    MemWord_t data;
    OpType op;
    BankIdx idx;
    OutType out;

    std::deque<MemWord_t> ref_q[NUM_BANKS];
    unsigned int ref_head[NUM_BANKS], ref_tail[NUM_BANKS];
    int j;

    for (j=0; j< NUM_BANKS; ++j)
    {
        ref_head[j] = ref_tail[j] = 0;
    }

    for (int i=0; i< NUM_ITER; ++i)
    {
        idx = rand()%NUM_BANKS;
        op = semi_rand_op(ref_q[idx].size());
        data = rand();
        CCS_DESIGN(FifoTop)(op, data, idx, out);

        switch (op)
        {
            case push:
                            assert(ref_q[idx].size()<FIFO_LENGTH);
                            ref_q[idx].push_back(data);
                            ++ref_tail[idx];
                            break;

            case pop:
                            assert(!ref_q[idx].empty());
                            assert(out == ref_q[idx].front());
                            ref_q[idx].pop_front();
                            ++ref_head[idx];
                            break;

            case incrHead:
                            assert(!ref_q[idx].empty());
                            ref_q[idx].pop_front();
                            ++ref_head[idx];
                            break;

            case peek:
                            assert(!ref_q[idx].empty());
                            assert(out == ref_q[idx].front());
                            break;

            case isEmpty:
                            assert(out == ref_q[idx].empty());
                            break;

            case isFull:
                            assert(out == (ref_q[idx].size() == FIFO_LENGTH));
                            break;

            case reset:
                            for (int j=0; j< NUM_BANKS; ++j)
                            {
                                ref_q[j].clear();
                                ref_head[j] = ref_tail[j] = 0;
                            }
                            break;

            case get_head:
                            assert(out == (ref_head[idx]%(FIFO_LENGTH)));
                            break;

            case get_tail:
                            assert(out == (ref_tail[idx]%(FIFO_LENGTH)));
                            break;

            default:
            assert(0);
        }
    }

    DCOUT("CMODEL PASS" << endl);
    CCS_RETURN(0) ;
}

