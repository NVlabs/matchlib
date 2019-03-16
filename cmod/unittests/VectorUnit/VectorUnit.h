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
#ifndef VECTOR_UNIT_H
#define VECTOR_UNIT_H

#include <nvhls_vector.h>
#include <hls_globals.h>
#include <nvhls_assert.h>

#ifndef VECTOR_LENGTH
#define VECTOR_LENGTH 8
#endif

typedef NVUINT8 InScalarType;
typedef NVUINT32 OutScalarType;
typedef nvhls::nv_scvector<InScalarType, VECTOR_LENGTH> InVectorType;
typedef nvhls::nv_scvector<OutScalarType, VECTOR_LENGTH> OutVectorType;

enum VectorOp {
    Mul=0,
    Add,
    Sub,
    Reduction,
    DP,
    MAC,
    DPACC,
    MAXOP
};

typedef NVUINTC(nvhls::nbits<MAXOP -1 >::val) OpType;

void VectorUnit( const OpType& op, 
                 const InVectorType& in1, 
                 const InVectorType& in2, 
                 const InVectorType& in3, 
                 OutVectorType& out);

#endif
