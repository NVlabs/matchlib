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
#ifndef CROSSBAR_H
#define CROSSBAR_H

#include "nvhls_int.h"
#include "nvhls_types.h"
#include <stdio.h>

// this section probably deserves it's own h file
// in context of crossbar this function is used to zero out bits for any data
// type in case the output is invalid. We do not want to deal with Xes.
template <class T>
T zero_bits() {
    T t;
    t = (T){0};
    return t;
}

/**
 * \brief Main entry point for crossbar - most generic implementation. 
 * \ingroup Crossbar
 *
 * \tparam DataType             Datatype of input and output of each lane 
 * \tparam NumInputLanes        Number of input lanes
 * \tparam NumOutputLanes       Number of output lanes
 *
 * \param[in]   data_in         Array of data inputs for each lane, indexed by input lane id.
 * \param[in]   valid_in        Array of boolean valid bits, indexed by input lane id.. False means data can be ignored.
 * \param[in]   source          Array indicating the origin lane for each output lane id. Indexed by output lane id.
 * \param[in]   valid_source    Array of boolean valid bits, indexed by output lane id. Indicated validity of source selection for given output.
 * \param[out]  data_out        Array of outputs, indexed by output lane id.
 * \param[out]  valid_out       Array of outputs, indexed by output lane id.


 * \par Overview
 * - Routing input data into output based on source information.
 * - Output will be invalid if either source information for that output was invalid or selected input lane is invalid.
 * - Invalid output will have 0 on data bits.
 *
 * \par A Simple Example
 * \code
 *      #include <crossbar.h>
 *
 *      ...
 *      Src_t   src_in[NUM_OUTPUTS]; // src_in[x]=y means lane x output comes from lane y
 *      Word_t  data_out[NUM_OUTPUTS];
 *      bool    valid_in[NUM_INPUTS];
 *      bool    valid_src[NUM_OUTPUTS];
 *      bool    valid_out[NUM_OUTPUTS];
 *      
 *      ...           
 *      crossbar<DATA_TYPE, NUM_INPUTS, NUM_OUTPUTS>(
 *          data_in, valid_in,
 *          source,  valid_source,
 *          data_out,valid_out);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename DataType, unsigned NumInputLanes, unsigned NumOutputLanes>
void crossbar(DataType data_in[NumInputLanes], bool valid_in[NumInputLanes],
        NVUINTW(nvhls::index_width<NumInputLanes >::val) source[NumOutputLanes],
        bool valid_source[NumOutputLanes],
        DataType data_out[NumOutputLanes],
        bool valid_out[NumOutputLanes]) {

    NVUINTW(nvhls::index_width<NumInputLanes>::val) source_tmp;
    bool valid_in_tmp;
    DataType data_in_tmp;

#pragma hls_unroll yes
    for (unsigned dst = 0; dst < NumOutputLanes; dst++) {
        source_tmp = source[dst];
        if (!valid_source[dst]) {
            source_tmp = 0;
        }
        valid_in_tmp = valid_in[source_tmp];
        data_in_tmp = data_in[source_tmp];

        if (valid_source[dst] && valid_in_tmp) {
            data_out[dst] = data_in_tmp;
            valid_out[dst] = true;
        } else {
            data_out[dst] = zero_bits<DataType>();
            valid_out[dst] = false;
        }
    }
}

/**
 * \brief Simplified specialization with no `valid_source` parameter. All `source` selections are assumed to be valid.
 * \ingroup Crossbar
 */
template <typename DataType, unsigned NumInputLanes, unsigned NumOutputLanes>
void crossbar(DataType data_in[NumInputLanes], bool valid_in[NumInputLanes],
        NVUINTW(nvhls::index_width<NumInputLanes>::val) source[NumOutputLanes],
        DataType data_out[NumOutputLanes],
        bool valid_out[NumOutputLanes]) {

    bool valid_source[NumOutputLanes];
    for (unsigned i = 0; i < NumOutputLanes; ++i)
        valid_source[i] = true;

    crossbar<DataType, NumInputLanes, NumOutputLanes>(
            data_in, valid_in, source, valid_source, data_out, valid_out);
}

/**
 * \brief Simplified specialization with no valid parameter at all. Inputs are assumed to be valid. Validity of outputs is not reported.
 * \ingroup Crossbar
 */
template <typename DataType, unsigned NumInputLanes, unsigned NumOutputLanes>
void crossbar(DataType data_in[NumInputLanes],
        NVUINTW(nvhls::index_width<NumInputLanes>::val) source[NumOutputLanes],
        DataType data_out[NumOutputLanes]) {

    bool valid_out[NumOutputLanes];
    bool valid_in[NumInputLanes];
    for (unsigned int i = 0; i < NumInputLanes; ++i)
        valid_in[i] = true;

    crossbar<DataType, NumInputLanes, NumOutputLanes>(data_in, valid_in, source,
            data_out, valid_out);
}

#endif  // CROSSBAR_H
