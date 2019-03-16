/*
 * Copyright (c) 2017-2019, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef ONE_HOT_TO_BIN_H
#define ONE_HOT_TO_BIN_H

#include <nvhls_types.h>
/**
 * \brief One hot to binary conversion 
 * \ingroup one_hot_to_bin
 *
 * \tparam OneHotLen        Width of one-hot representation 
 * \tparam BinLen           Width of binary output
 *
 * \param[in]   one_hot_in     One hot input 
 * \param[out]  bin_out        Binary output 

 * \par A Simple Example
 * \code
 *      #include <one_hot_to_bin.h>
 *
 *      ...
 *      NVUINT4 input = 4;
 *      NVUINT2 output;
 *      ...           
 *      one_hot_to_bin<4, 2>(input, output);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <unsigned OneHotLen, unsigned BinLen>
void one_hot_to_bin(const NVUINTW(OneHotLen) & one_hot_in,
                    NVUINTW(BinLen) & bin_out) {

#pragma hls_unroll yes
  for (unsigned bin = 0; bin < BinLen; bin++) {

    // constructs mask which matches one-hot to binary outputs
    NVUINTW(OneHotLen) tmp;
#pragma hls_unroll yes
    for (unsigned bit = 0; bit < OneHotLen; bit++) {
      NVUINTW(OneHotLen) ind = bit;
      tmp[bit] = ind[bin];
    }

    tmp = (tmp & one_hot_in);

    // Reduction OR
    NVUINTW(1) bit_tmp = 0;
#pragma hls_unroll yes
    for (unsigned i = 0; i < OneHotLen; i++) {
      bit_tmp = bit_tmp | tmp[i];
    }

    // Update output
    bin_out[bin] = bit_tmp;
  }
}

#endif
