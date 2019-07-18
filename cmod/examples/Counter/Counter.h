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
#ifndef __COUNTER_H__
#define __COUNTER_H__

#include <systemc.h>
#include <nvhls_module.h>

SC_MODULE(Counter) {
    public:
    enum _ {
        WIDTH = 4
    };

    typedef sc_uint<WIDTH> Data;

    sc_in_clk   clk;
    sc_in<bool> rst;
    sc_in<bool> inc;

    sc_out<Data> out;
    Data         value;

    SC_CTOR(Counter):
        inc("inc") {
        SC_THREAD(increment);
        sensitive << clk.pos();
        NVHLS_NEG_RESET_SIGNAL_IS(rst);
    }

    private:
    void increment();
};

#endif
