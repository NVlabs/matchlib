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
#include <ReorderBuf.h>
#include <hls_globals.h>
#include <nvhls_assert.h>
#include "ReorderBufTop.h"

void ReorderBufTop( const OpType& op, 
                    const DataType& in_data,
                    const Rob::Id& in_id,
                    Rob::Id& out_id,
                    DataType& out_data,
                    bool& out_resp)
{
    static Rob rob;
    out_id = 0; out_resp = false; out_data =0;
    switch (op)
    {
        case canAccept:     out_resp =  rob.canAcceptRequest();         break;
        case addRequest:    out_id   =  rob.addRequest();               break;
        case top:           out_resp =  rob.topResponseReady();         break;
        case addResponse:               rob.addResponse(in_id, in_data);break;
        case popResponse:   out_data =  rob.popResponse();              break;
        case reset:                     rob.reset();                    break;
        case isEmpty:       out_resp =  rob.isEmpty();                  break;
        default:
            NVHLS_ASSERT_MSG(0, "op_not_supported"); //never get here
    }
}
