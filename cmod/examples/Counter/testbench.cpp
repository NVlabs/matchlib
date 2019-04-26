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
#include "Counter.h"
#include <systemc.h>
#include <mc_scverify.h>  // needed for scverify to work
#include <testbench/nvhls_rand.h>


SC_MODULE(testbench) {
    CCS_DESIGN(Counter) counter;
    sc_clock clk;
    sc_signal<bool> rst;
    sc_signal<bool> inc;
    sc_signal<Counter::Data> out;

    // this is a good practice to have a reference implementation to verify C++ code
    Counter::Data counter_ref; 

    SC_CTOR(testbench):
        counter("counter"),
        clk("clk", 2, SC_NS, 0.5,0,SC_NS,true),
        rst("rst") {
        counter.clk(clk);
        counter.rst(rst);
        counter.inc(inc);
        counter.out(out);

        SC_THREAD(run);
    }

    void run() {
        // reset
        rst = 0;
        inc = 0;
        cout << "@" << sc_time_stamp() << " BEGIN RESET" << endl;
        wait(2, SC_NS);
        rst = 1;
        counter_ref = 0;
        cout << "@" << sc_time_stamp() <<" END RESET" << endl;

        // operation
        for (unsigned int i=0; i < 100; i++) {
            cout << "@" << sc_time_stamp() << " VALUE = " << out << endl;
            assert(out == counter_ref);

            if (i & 2) {  // this way I cover all "inc" signal transitions
                inc = 1;
                cout << "@" << sc_time_stamp() << " ++" << endl;
                ++counter_ref;
            } else {
                inc = 0;
            }

            wait(2, SC_NS);
        }

        sc_stop();
    }
};



int sc_main(int argc, char *argv[]) {
  nvhls::set_random_seed();
  testbench my_testbench("my_testbench");
  sc_report_handler::set_actions (SC_ID_LOGIC_X_TO_BOOL_,SC_DO_NOTHING);
  sc_start();
  return 0;
}
