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

#ifndef REORDERBUF_TOP_H
#define REORDERBUF_TOP_H

#include <ReorderBuf.h>
#include <hls_globals.h>
#include <nvhls_assert.h>

#ifndef ROB_DATA
#define ROB_DATA NVUINTC(16)
#endif

#ifndef ROB_DEPTH
#define ROB_DEPTH 6
#endif

#ifndef ROB_INFLIGHT
#define ROB_INFLIGHT 3 
#endif


enum RobOp {
    canAccept=0,
    addRequest,
    top,
    addResponse,
    popResponse,
    reset,
    isEmpty,
    MAXOP
};

typedef ROB_DATA DataType;
typedef ReorderBuf<DataType, ROB_DEPTH, ROB_INFLIGHT> Rob;
typedef NVUINTC(nvhls::nbits<MAXOP -1 >::val) OpType;

void ReorderBufTop( const OpType& op, 
                    const DataType& in_data,
                    const Rob::Id& in_id,
                    Rob::Id& out_id,
                    DataType& out_data,
                    bool& out_resp);

#endif
