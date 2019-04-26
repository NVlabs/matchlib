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

#include "CrossbarTop.h"

#ifndef NUM_ITERS
#define NUM_ITERS 1000
#endif

CCS_MAIN(int argc, char *argv[]) {

  nvhls::set_random_seed();

  typedef DATA_TYPE Word_t;
  typedef NVUINTC(nvhls::nbits<NUM_INPUTS - 1>::val) Src_t;

  Word_t data_in[NUM_INPUTS];
  Src_t
      src_in[NUM_OUTPUTS]; // src_in[x]=y means lane x output comes from lane y
  Word_t data_out[NUM_OUTPUTS];

  bool valid_in[NUM_INPUTS];
  bool valid_src[NUM_OUTPUTS];
  bool valid_out[NUM_OUTPUTS];
  // Writing inputs
  for (int test = 0; test < NUM_ITERS; test++) {
    for (unsigned in = 0; in < NUM_INPUTS; in++) {
      int tmp = rand();
      data_in[in] = tmp;
      valid_in[in] = rand() % 2;
    }

    for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
      // src_in[out] = NUM_OUTPUTS - 1 - out;
      // src_in[out] = (src_in[out] + test) % NUM_INPUTS;
      src_in[out] = rand() % NUM_INPUTS;
      valid_src[out] = rand() % 2;
    }

    CCS_DESIGN(CrossbarTop)(
        data_in,
#ifndef NO_XBAR_VALID_IN_OUT
        valid_in,
#endif
        src_in,
#ifndef NO_XBAR_VALID_SOURCE
        valid_src,
#endif
        data_out
#ifndef NO_XBAR_VALID_IN_OUT
        ,valid_out
#endif
        );

#ifdef NO_XBAR_VALID_IN_OUT
    for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
      for (unsigned in = 0; in < NUM_INPUTS; in++) {
        if (src_in[out] == in) {
          assert(data_out[out] == data_in[in]);
        }
      }
    }
#else
#ifdef NO_XBAR_VALID_SOURCE
    for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
      for (unsigned in = 0; in < NUM_INPUTS; in++) {
        if (src_in[out] == in) {
          if (valid_in[in]) {
            assert(data_out[out] == data_in[in]);
            assert(valid_out[out] == true);
          } else {
            assert(data_out[out] == 0);
            assert(valid_out[out] == false);
          }
        }
      }
    }
#else
    for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
      if (!valid_src[out]) {
        assert(data_out[out] == 0);
        assert(valid_out[out] == false);
      } else {
        for (unsigned in = 0; in < NUM_INPUTS; in++) {
          if (src_in[out] == in) {
            if (valid_in[in]) {
              assert(data_out[out] == data_in[in]);
              assert(valid_out[out] == true);
            } else {
              assert(data_out[out] == 0);
              assert(valid_out[out] == false);
            }
          }
        }
      }
    }

#endif
#endif

 




  }

  DCOUT("CMODEL PASS" << endl);
  CCS_RETURN(0);
}
