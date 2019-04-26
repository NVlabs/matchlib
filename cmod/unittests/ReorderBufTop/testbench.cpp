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
#include <stdio.h>
#include "ReorderBufTop.h"
#include <match_scverify.h>
#include <testbench/nvhls_rand.h>

#include <deque>

#ifndef NUM_ITER
#define NUM_ITER 10000
#endif


template <typename Data, unsigned int Depth, unsigned int InFlight>
class RobRef
{
    public:
    
    RobRef() { reset(); };
    typedef sc_uint<nvhls::index_width<InFlight>::val> Id;

    private:

    struct Entry {
        int id;
        bool valid;
        Data data;
    };
    
    typedef std::deque<Entry> Storage;
    Storage storage;

    unsigned int inflight;

    public:

    bool canAcceptRequest() const
    {
        return ((inflight < InFlight) && (storage.size()<Depth));
    }

    void addRequest(const Id& dut_id)
    {
        assert(canAcceptRequest());

        for (typename Storage::iterator it=storage.begin(); it!=storage.end(); ++it)
        {
            assert ((*it).id != static_cast<int>(dut_id));
        }

        Entry entry;

        entry.id = dut_id;
        entry.valid = false;
        
        storage.push_back(entry);
        ++inflight;
    }

    bool topResponseReady() const
    {
        if (storage.empty()) return false;
        return storage.front().valid;
    }

    void addResponse(const Id& id, const Data& data)
    {
        bool found = false;
        for (typename Storage::iterator it=storage.begin(); it!=storage.end(); ++it)
        {
            if ((*it).id == static_cast<int>(id))
            {
                assert(!found); //should find only once
                found = true;

                assert((*it).valid == false);
                (*it).valid = true;
                (*it).id = -1;

                (*it).data = data;
            }
        }
        --inflight;
    }

    Data popResponse()
    {
        assert(!storage.empty());
        Entry entry = storage.front();
        storage.pop_front();

        assert(entry.valid);
        return entry.data;
    }

    void reset()
    {
        inflight = 0;
        storage.clear();
    }

    bool isEmpty() const
    {
        return storage.empty();
    }
    
    bool waitsForAnyResponse() const
    {
        for (typename Storage::const_iterator it=storage.begin(); it!=storage.end(); ++it)
        {
            if ((*it).valid == false) return true;
        }
        
        return false;
    }

    Id randomPendingResponseId() const 
    {
        std::deque<Id> pending;

        for (typename Storage::const_iterator it=storage.begin(); it!=storage.end(); ++it)
        {
            if ((*it).valid == false) pending.push_back((*it).id);
        }
        
        assert(!pending.empty());
        int rnd_idx = rand()%pending.size();
        return pending[rnd_idx];
    }
};


typedef RobRef<ROB_DATA, ROB_DEPTH, ROB_INFLIGHT> Ref;

OpType semi_rand_op(Ref& ref)
{
    OpType op;

    do {
        op = rand()%MAXOP;
    } while (  
                ((op == addRequest) && (!ref.canAcceptRequest())) ||
                ((op == addResponse) && (!ref.waitsForAnyResponse())) ||
                ((op == popResponse) && (!ref.topResponseReady()))
            ); 
    return op;
}

CCS_MAIN(int argc, char *argv[]) {

    nvhls::set_random_seed();

    Ref ref;

    for (int i=0; i< NUM_ITER; ++i)
    {
        OpType op = semi_rand_op(ref);
        ROB_DATA out_data, in_data = rand();
        Ref::Id out_id, in_id;
        bool out_resp;
        if (op == addResponse) in_id=ref.randomPendingResponseId();
        CCS_DESIGN(ReorderBufTop)(op, in_data, in_id, out_id, out_data, out_resp);

#ifdef DEBUGMODE
        cout << "\top = " << op
                << "\tin_id = " << in_id
                << "\tout_id = " << out_id
                << "\tout_resp = " << out_resp
                << endl;
#endif

        switch (op)
        {
            case canAccept:
                            assert(out_resp == ref.canAcceptRequest());
                            break;
        
            case addRequest:    
                            ref.addRequest(out_id);
                            break;
        
            case top:           
                            assert(out_resp == ref.topResponseReady());
                            break;

            case addResponse:
                            ref.addResponse(in_id, in_data);
                            break;

            case popResponse:   
                            assert(out_data == ref.popResponse());
                            break;

            case reset:
                            ref.reset();
                            break;

            case isEmpty:   
                            assert(out_resp == ref.isEmpty());
                            break;

            default:
            assert(0);
        }
    }

    DCOUT("CMODEL PASS" << endl);
    CCS_RETURN(0) ;
}

