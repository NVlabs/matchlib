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
#include "ArbiterModule.h"
#include <systemc.h>
#include <nvhls_connections.h>
#include <fifo.h>
#include <testbench/nvhls_rand.h>

#define NVHLS_VERIFY_BLOCKS (ArbiterModule)
#include <nvhls_verify.h>
typedef ArbiterModule::mask_t     mask_t;

#ifndef NUM_ITERS
#define NUM_ITERS 1000
#endif

#define TB_FIFO_LEN 2000
FIFO<mask_t, TB_FIFO_LEN > g_tb_fifo;

mask_t semi_random(const int& iter)
{
    if (iter%17==0)
    {
        //sprinkle zero valids
        return 0;
    }

    mask_t result;
    do {
        result = rand();
    } while (result==0);

    return result;
}

void rotate_right(mask_t& mask)
{
    mask_t lsb=mask&0x1;
    mask = (mask>>1) | (lsb<<(NUM_INPUTS-1));
}

mask_t reference_arbiter(const mask_t& valid)
{
    static mask_t iter = 1;
    if (valid == 0)
        return 0;

    int i=0;
    do {
        rotate_right(iter); ++i;
        if (iter & valid)
            return iter;
    } while (i<=NUM_INPUTS);

    assert(0); // should never get here, valid!=0 but nothing was sellected
    return 0;
}



SC_MODULE (Source) {
  Connections::Out<mask_t> out;
  sc_in <bool> clk;
  sc_in <bool> rst;
  const unsigned int id;

  void run() {
    mask_t valid_reg, ref;
    out.Reset();

    wait();
    for (int i =0; i< NUM_ITERS; i++) {
        valid_reg = semi_random(i);
        ref = reference_arbiter(valid_reg);
        g_tb_fifo.push(ref);
        out.Push(valid_reg);
    }
  }

  SC_HAS_PROCESS(Source);

  Source(sc_module_name name_, const unsigned int& id_) : sc_module(name_),
  out("out"),
  clk("clk"),
  rst("rst"),
  id(id_)
  {
    SC_THREAD(run);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

SC_MODULE (Dest) {
  Connections::In<mask_t> in;
  sc_in <bool> clk;
  sc_in <bool> rst;
  const unsigned int id;

  void run() {
    in.Reset();

    wait();
    mask_t select_reg, ref;
    for (int i=0; i<NUM_ITERS; i++) {
        select_reg = in.Pop();
        if (!g_tb_fifo.isEmpty()) {
            ref = g_tb_fifo.pop();
        }
        assert(ref == select_reg);

    }
    sc_stop();
  }

  SC_HAS_PROCESS(Dest);

  Dest(sc_module_name name_, const unsigned int& id_) : sc_module(name_),
  in("in"),
  clk("clk"),
  rst("rst"),
  id(id_)
  {
    SC_THREAD(run);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

SC_MODULE (testbench) {
    NVHLS_DESIGN(ArbiterModule) my_inst;

    Source source_inst;
    Dest dest_inst;

    Connections::Combinational<mask_t> valid_chan;
    Connections::Combinational<mask_t> select_chan;

    sc_clock clk;
    sc_signal<bool> rst;


    SC_CTOR(testbench) :
        my_inst("my_inst"),
        source_inst("source_inst",0),
        dest_inst("dest_inst",0),
        valid_chan("valid_chan"),
        select_chan("select_chan"),
        clk("clk", 1, SC_NS, 0.5,0,SC_NS,true),
        rst("rst")
    {
        my_inst.clk(clk);
        my_inst.rst(rst);

        source_inst.rst(rst);
        source_inst.clk(clk);

        dest_inst.rst(rst);
        dest_inst.clk(clk);

        source_inst.out( valid_chan );
        my_inst.valid_in( valid_chan );
        my_inst.select_out( select_chan );
        dest_inst.in( select_chan );

        SC_THREAD(reset_driver);
    }

    void reset_driver() {
        rst.write(0);
        wait(8.5, SC_NS);
        rst.write(1);
    }

};

int sc_main(int argc, char *argv[])
{
  nvhls::set_random_seed();
  testbench my_testbench("my_testbench");
  sc_report_handler::set_actions(SC_ERROR, SC_DISPLAY);
  sc_start();
  bool rc = (sc_report_handler::get_count(SC_ERROR) > 0);
  if (rc)
    DCOUT("CMODEL FAIL" << endl);
  else
    DCOUT("CMODEL PASS" << endl);
  return rc;
};

