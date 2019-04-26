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

#include "ArbiterTop.h"

#ifndef NUM_ITERS
#define NUM_ITERS 1000
#endif

mask_t semi_random(const int& iter)
{
    if (iter%17==0) 
    {
        //sprinkle zero valids
        return 0;
    }

    mask_t result;
    do {
        result = rand();
    } while (result==0);

    return result;    
}

void rotate_right(mask_t& mask)
{
    mask_t lsb=mask&0x1;
    mask = (mask>>1) | (lsb<<(NUM_INPUTS-1));
}

mask_t reference_arbiter(const mask_t& valid)
{
    if (ARBITER_TYPE == Roundrobin) {
      static mask_t iter = 1;
      if (valid == 0)
          return 0;

      int i=0;
      do {
          rotate_right(iter); ++i;
          if (iter & valid)
              return iter;
      } while (i<=NUM_INPUTS);
    }
    if (ARBITER_TYPE == Static) {
      mask_t iter = 1;
      if (valid == 0)
          return 0;

      int i=0;
      do {
          rotate_right(iter); ++i;
          if (iter & valid)
              return iter;
      } while (i<=NUM_INPUTS);
    }
    assert(0); // should never get here, valid!=0 but nothing was sellected
    return 0;
}

CCS_MAIN(int argc, char *argv[]) { 
    nvhls::set_random_seed();
    mask_t valid,select,ref;

    for (int i = 0; i< NUM_ITERS; i++) {
        valid = semi_random(i);
        ref = reference_arbiter(valid);
        CCS_DESIGN(ArbiterTop)(valid, select);
        assert(ref == select);
    }

    DCOUT("CMODEL PASS" << endl);
    CCS_RETURN(0) ;
}

