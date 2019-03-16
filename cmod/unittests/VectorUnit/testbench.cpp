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
#include "VectorUnit.h"
#include <match_scverify.h>

#ifndef NUM_ITER
#define NUM_ITER 10000
#endif

template < typename VectorType>
void get_rand_vector(VectorType& data) {
  for (unsigned i = 0; i < VectorType::length; i++) {
    data[i] = rand();
  } 
}


CCS_MAIN(int argc, char *argv[]) {
    InVectorType in1, in2, in3;
    OpType op;
    OutVectorType out, out_ref;

    for (int i=0; i< NUM_ITER; ++i)
    {
        op = rand()%MAXOP;
        get_rand_vector(in1);
        get_rand_vector(in2);
        get_rand_vector(in3);
        CCS_DESIGN(VectorUnit)(op, in1, in2, in3, out);
        switch(op) {
          case Mul:  for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[i] = in1[i] * in2[i];
                     } 
                     assert(out == out_ref);
                     break;
          case Add:  for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[i] = in1[i] + in2[i];
                     } 
                     assert(out == out_ref);
                     break;
          case Sub:  for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[i] = in1[i] - in2[i];
                     } 
                     assert(out == out_ref);
                     break;
          case Reduction:  out_ref[0] = 0;
                     for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[0] += in1[i];
                     } 
                     assert(out[0] == out_ref[0]);
                     break;
          case DP:  out_ref[0] = 0;
                     for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[0] += in1[i]*in2[i];
                     } 
                     assert(out[0] == out_ref[0]);
                     break;
          case MAC:  for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[i] = in1[i] * in2[i] + in3[i];
                     } 
                     assert(out == out_ref);
                     break;
          case DPACC:  out_ref[0] = in3[0];
                     for (int i = 0; i < VECTOR_LENGTH; i++) {
                        out_ref[0] += in1[i]*in2[i];
                     } 
                     assert(out[0] == out_ref[0]);
                     break;


        }; 
    }

    DCOUT("CMODEL PASS" << endl);
    CCS_RETURN(0) ;
}

