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
#include <match_scverify.h>
#include <testbench/nvhls_rand.h>

#include "LzdTop.h"

#ifndef NUM_ITERS
#define NUM_ITERS 1000
#endif

Data semi_random()
{
    Data data = rand(); 

    if (rand()&0x1) {
        //return as is
        return data;
    }

    if (rand()&0x1) {
        return data>>(rand()%NUM_BITS);
    } else {
        return data<<(rand()%NUM_BITS);
    }

}

Count reference_lzd(const Data& in) 
{
    for (int i = 0; i < NUM_BITS; ++i) 
    {
        if (in[NUM_BITS-1-i] != 0) return i;
    }
    return (NUM_BITS-1);
}



CCS_MAIN(int argc, char *argv[]) 
{
    nvhls::set_random_seed();

    for (int i =0; i < NUM_ITERS; ++i) 
    { 

        Data data = semi_random();    
        Count count, count_ref;

        CCS_DESIGN(LzdTop)(data, count);
        count_ref = reference_lzd(data);
        assert(count == count_ref);
    }

    DCOUT("CMODEL PASS" << endl);
    CCS_RETURN(0) ;
}

