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
#include "VectorUnit.h"

void VectorUnit( const OpType& op, 
                 const InVectorType& in1, 
                 const InVectorType& in2, 
                 const InVectorType& in3, 
                 OutVectorType& out)
{
    switch (op)
    {
        case Mul:       nvhls::vector_mul<InScalarType, InScalarType, OutScalarType, VECTOR_LENGTH, true> (in1, in2, out); break;
        case Add:       nvhls::vector_add<InScalarType, InScalarType, OutScalarType, VECTOR_LENGTH, true> (in1, in2, out); break;
        case Sub:       nvhls::vector_sub<InScalarType, InScalarType, OutScalarType, VECTOR_LENGTH, true> (in1, in2, out); break;
        case Reduction: nvhls::reduction<InScalarType, OutScalarType, VECTOR_LENGTH, true>(in1,out[0]); break;
        case DP:        nvhls::dp<InScalarType, InScalarType, OutScalarType, VECTOR_LENGTH, true>(in1, in2, out[0]); break;
        case MAC:       nvhls::vector_mac<InScalarType, InScalarType, InScalarType, OutScalarType, VECTOR_LENGTH, true>(in1, in2, in3, out); break;
        case DPACC:     nvhls::dpacc<InScalarType, InScalarType, InScalarType, OutScalarType, VECTOR_LENGTH, true>(in1, in2, in3[0], out[0]); break;
        default:
            NVHLS_ASSERT_MSG(0, "op_not_supported");
    }
}
