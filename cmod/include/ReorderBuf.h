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

#ifndef __REORDERBUF_H__
#define __REORDERBUF_H__

#include <fifo.h>
#include <nvhls_types.h>
#include <mem_array.h>
#include <nvhls_assert.h>

/**
 * \brief Reorder Buffer that allows out-of-order writes to queue and in-order reads 
 * \ingroup ReorderBuffer
 *
 * \tparam Data             DataType 
 * \tparam Depth            Depth of queue 
 * \tparam InFlight         Number of inflight entries
 *
 * \par A Simple Example
 * \code
 *      #include <ReorderBuf.h>
 *
 *      ...
 * void ReorderBufTop( const OpType& op, 
 *                     const DataType& in_data,
 *                     const Rob::Id& in_id,
 *                     Rob::Id& out_id,
 *                     DataType& out_data,
 *                     bool& out_resp)
 * {
 *     static Rob rob;
 *     out_id = 0; out_resp = false; out_data =0;
 *     switch (op)
 *     {
 *         case canAccept:     out_resp =  rob.canAcceptRequest();         break;
 *         case addRequest:    out_id   =  rob.addRequest();               break;
 *         case top:           out_resp =  rob.topResponseReady();         break;
 *         case addResponse:               rob.addResponse(in_id, in_data);break;
 *         case popResponse:   out_data =  rob.popResponse();              break;
 *         case reset:                     rob.reset();                    break;
 *         case isEmpty:       out_resp =  rob.isEmpty();                  break;
 *         default:
 *             NVHLS_ASSERT_MSG(0,"invalid_op"); //never get here
 *     }
 * }
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename Data, unsigned int Depth, unsigned int InFlight>
class ReorderBuf {

public:
    ReorderBuf(): idrep(0)
    {
    }

    typedef sc_uint<nvhls::index_width<InFlight>::val> Id;

protected:
    mem_array_sep<Data, Depth, 1> storage;
    typedef FIFO<bool, Depth> VBits;
    VBits vbits;

    typedef NVUINTW(InFlight)   IdRepository;

    IdRepository idrep;
    typedef typename VBits::FifoIdx EntryNum;

    typedef mem_array_sep<EntryNum, InFlight,1> Id2Entry;
    Id2Entry id2entry;
    
    bool get_next_avail_id(Id& id, IdRepository& id_repository)
    {
        #pragma hls_unroll yes
        for (int i=0; i<static_cast<int>(InFlight); ++i)
        {
            if (id_repository[i]==0) 
            {
                id = i;
                id_repository[i]=1;
                return true;
            }
        }
        return false;
    }

public:
    bool canAcceptRequest()
    {
        return ((idrep != static_cast<IdRepository>(~0)) && !vbits.isFull());
    }

    Id addRequest()
    {
        Id id;
        bool success = get_next_avail_id(id, idrep);
        NVHLS_ASSERT_MSG(success, "get_next_avail_id_unsuccessful");

        id2entry.write(static_cast<typename Id2Entry::LocalIndex>(id), 0, vbits.get_tail()); 
        vbits.push(false);

        return id;
    }

    bool topResponseReady()
    {
        if (isEmpty()){
            return false;
        } else {
            return vbits.peek();
        }
    }

    void addResponse(const Id& id, const Data& data)
    {
        EntryNum entryNum = id2entry.read(static_cast<typename Id2Entry::LocalIndex>(id), 0);
        storage.write(entryNum, 0, data);
        vbits.fifo_body.write(static_cast<typename VBits::FifoIdx>(entryNum), 0, true);
        NVHLS_ASSERT_MSG(idrep[static_cast<int>(id)] == 1, "idrep[id]!=1");
        idrep[static_cast<int>(id)]=0;
    }

    Data popResponse()
    {
        NVHLS_ASSERT_MSG(topResponseReady(),"topResponseNotReady");

        Data result = storage.read( vbits.get_head(), 0); 
        vbits.incrHead();

        return result;
    }

    void reset()
    {
        vbits.reset();
        idrep = 0;
    }

    bool isEmpty()
    {
        return vbits.isEmpty();
    }
    
};

#endif
