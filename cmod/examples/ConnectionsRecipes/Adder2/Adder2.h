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
#ifndef __ADDER2_H__
#define __ADDER2_H__

#include <systemc.h>
#include <nvhls_connections.h>

// adder2: inputs a and b; output sum=a+b_val?b:0
// forward progress even if b is not present, use 0

SC_MODULE(Adder2)
{
    public:
        sc_in_clk     clk;
        sc_in<bool>   rst;

        typedef sc_uint<16> Data;

        Connections::In<Data> a_in;
        Connections::In<Data> b_in;

        Connections::Out<Data> sum_out;

        SC_HAS_PROCESS(Adder2);
        Adder2(sc_module_name name_) : sc_module(name_),
        a_in("a_in"), b_in("b_in"), sum_out("sum_out")
        {
            SC_THREAD (run); 
            sensitive << clk.pos(); 
            NVHLS_NEG_RESET_SIGNAL_IS(rst);
        }

        void run()
        {
            a_in.Reset();
            b_in.Reset();
            sum_out.Reset();

            Data a=13,b=45, sum=20;
            Data zero = 0;
            while (1)
            {
                wait();

                a=a_in.Pop();                   // no forward progress until a is received 
                if (!b_in.PopNB(b)) b= zero;    // non-blocking read of b, otherwise set to zero
                sum_out.Push(a + b);            // no forward progress until result is written out
            }
        }
};

#endif
