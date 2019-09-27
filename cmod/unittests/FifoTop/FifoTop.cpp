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
#include <fifo.h>
#include <hls_globals.h>
#include <nvhls_assert.h>
#include "FifoTop.h"

void FifoTop( const OpType& op, const DataType& data, const BankIdx& idx, OutType& out)
{
    static Fifo_ fifo;
    out = 0;
    NVHLS_ASSERT_MSG(idx < NUM_BANKS, "Bank_index_is_less_than_number_of_banks");
    switch (op)
    {
        case push:      fifo.push(data, idx); break;
        case pop:       out = fifo.pop(idx); break;
        case incrHead:  fifo.incrHead(idx); break;
        case peek:      out = fifo.peek(idx); break;
        case isEmpty:   out = fifo.isEmpty(idx); break;
        case isFull:    out = fifo.isFull(idx); break;
        case reset:     fifo.reset(); break;
        case get_head:  out = fifo.get_head(idx); break;
        case get_tail:  out = fifo.get_tail(idx); break;
        default:
            NVHLS_ASSERT_MSG(0, "op_not_supported");
            fifo.reset(); break;
    }
}
