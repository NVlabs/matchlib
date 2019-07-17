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

#include "Adder.h"
#include <systemc.h>
#include <mc_scverify.h>

#define NVHLS_VERIFY_BLOCKS (Adder)
#include <nvhls_verify.h>
#include <testbench/nvhls_rand.h>

#include <deque>
using namespace::std;
#include <testbench/Pacer.h>

typedef deque<int> Fifo;

SC_MODULE (Source) {
    Connections::Out<Adder::Data> x_out;

    sc_in <bool> clk;
    sc_in <bool> rst;
    const int start_val;
    Pacer pacer;

    Fifo fifo;

    void run()
    {
        x_out.Reset();
        pacer.reset();
        fifo.clear();

        Adder::Data x = start_val;
        
	// Wait for initial reset.
	wait(20.0, SC_NS);

        wait();
	
        while(1) {
            //cout << "@" << sc_time_stamp() << "\t" << name() << " sending X=" << x << endl ;
            x_out.Push(x);            
            //cout << "@" << sc_time_stamp() << "\t" << name() << " DONE" << endl ;
            fifo.push_back(x);
            ++x;

            wait();
            while (pacer.tic()) { 
                //cout << "@" << sc_time_stamp() << "\t" << name() << " STALL" << endl ;
                wait(); 
            }
        }
    }

    SC_HAS_PROCESS(Source);

    Source(sc_module_name name_, const int& start_val_, const Pacer& pacer_) : sc_module(name_),
    x_out("x_out"),
    clk("clk"),
    rst("rst"),
    start_val(start_val_),
    pacer(pacer_)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
        NVHLS_NEG_RESET_SIGNAL_IS(rst);
    }
};

SC_MODULE (Dest) {
    Connections::In<Adder::Data> sum_in;

    sc_in <bool> clk;
    sc_in <bool> rst;

    Fifo& fifo_a;
    Fifo& fifo_b;

    Pacer pacer;

    void run() {
        sum_in.Reset();
        pacer.reset();
        Adder::Data sum;

	// Wait for initial reset.
	wait(20.0, SC_NS);
	
        wait();

        while(1) {
            //cout << "@" << sc_time_stamp() << "\t\t\t\t\t\t Dest checking for result" << endl ;
            sum = sum_in.Pop();  
            //cout << "@" << sc_time_stamp() << "\t\t\t\t\t\t       GOT " << sum << endl;
            
            assert (sum == ((fifo_a.front() + fifo_b.front()) & 0xffff)); 
            fifo_a.pop_front();
            fifo_b.pop_front();
            wait();

            while (pacer.tic()) { 
                //cout << "@" << sc_time_stamp() << "\t" << name() << " STALL" << endl ;
                wait(); 
            }
        }
    }

    SC_HAS_PROCESS(Dest);
    Dest(sc_module_name name_, Fifo& fifo_a_, Fifo& fifo_b_, const Pacer& pacer_) : sc_module(name_),
    sum_in("sum_in"),
    clk("clk"),
    rst("rst"),
    fifo_a(fifo_a_),
    fifo_b(fifo_b_),
    pacer(pacer_)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
        NVHLS_NEG_RESET_SIGNAL_IS(rst);
    }
};


SC_MODULE (testbench) {
    NVHLS_DESIGN(Adder) adder;
    Source srca,srcb;
    Dest dest;

    Connections::Combinational<Adder::Data> a,b,sum;
    

    sc_clock clk;
    sc_signal<bool> rst;


    SC_CTOR(testbench) :
        adder("adder"),
        srca("srca", 7, Pacer(0.3, 0.7)),
        srcb("srcb", 13, Pacer(0.2, 0.5)),
        dest("dest", srca.fifo, srcb.fifo, Pacer(0.2, 0.5)),  //using a member inside constrction sequence could lead to a runtime bug
        a("a"),
        b("b"),
        sum("sum"),
        clk("clk", 1, SC_NS, 0.5,0,SC_NS,true),
        rst("rst")
    {
        adder.clk(clk);
        adder.rst(rst);

        srca.clk(clk);
        srca.rst(rst);

        srcb.clk(clk);
        srcb.rst(rst);

        dest.clk(clk);
        dest.rst(rst);

        srca.x_out(a);
        srcb.x_out(b);

        adder.a_in(a);
        adder.b_in(b);
        adder.sum_out(sum);

        dest.sum_in(sum);

        SC_THREAD(run);
    }

    void run() {
        //reset
        rst = 1;
        wait(10.5, SC_NS);
        rst = 0;
        //cout << "@" << sc_time_stamp() << " Asserting Reset " << endl ;
        wait(1, SC_NS);
        //cout << "@" << sc_time_stamp() << " Deasserting Reset " << endl ;
        rst = 1;
        wait(1000,SC_NS);
        //cout << "@" << sc_time_stamp() << " Stop " << endl ;
        sc_stop();
    }
};



int sc_main(int argc, char *argv[])
{
    nvhls::set_random_seed();
    testbench my_testbench("my_testbench");
    sc_start();
    cout << "CMODEL PASS" << endl;
    return 0;
};

