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
//-----------------------------------------------------
// Testbench for scratchpad
//-----------------------------------------------------
#include <systemc.h>
#include <hls_globals.h>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <testbench/nvhls_rand.h>
#include <Scratchpad.h>

#include "ScratchpadTop.h"
#include "ScratchpadConfig.h"

#define NVHLS_VERIFY_BLOCKS (ScratchpadTop)
#include <nvhls_verify.h>

#ifndef _SYNTHESIS_
#include <boost/timer/timer.hpp>
#endif

//#define DEBUG 1
typedef cli_req_t<data32_t, SCRATCHPAD_ADDR_WIDTH, SCRATCHPAD_BANKS> tb_cli_req_t;
typedef cli_rsp_t<data32_t, SCRATCHPAD_BANKS> tb_cli_rsp_t;

// *************************************************
// Use a reference memory class for testing the code
// *************************************************
class memmodel {
private:
  data32_t mem [REF_MEM_SIZE_IN_BYTES];

public:
  int size;
  memmodel(bool init_to_zero);
  
  void exec_store ( tb_cli_req_t cli_req );
  bool check_response ( tb_cli_rsp_t cli_rsp, tb_cli_req_t cli_req );
  data32_t read ( NVUINTC(SCRATCHPAD_ADDR_WIDTH) addr );

};


memmodel::memmodel (bool init_to_zero) {
  size = REF_MEM_SIZE_IN_BYTES;
  for (int i=0; i<size; i++) {
    if (init_to_zero) {
      mem[i] = 0;
    } else {
      mem[i] = rand() % 256;
    }
  }
}

void memmodel::exec_store ( tb_cli_req_t cli_req ) {
  // Loop over banks
  for (int i=0; i<SCRATCHPAD_BANKS; i++) {
    if (cli_req.valids[i] == true)
      mem[cli_req.addr[i]] = cli_req.data[i];
  }
}

data32_t memmodel::read ( NVUINTC(SCRATCHPAD_ADDR_WIDTH) addr ) {
  return mem[addr];
}

bool memmodel::check_response ( tb_cli_rsp_t cli_rsp, tb_cli_req_t cli_req ) {
  unsigned int pass_count = 0;
  // Loop over banks
  for (int i=0; i<SCRATCHPAD_BANKS; i++) {
    if ( (cli_req.valids[i] == true) && (mem[cli_req.addr[i]]==cli_rsp.data[i]) )
      pass_count++;
  }
  if (pass_count==SCRATCHPAD_BANKS)
    return true;
  else
    return false;
}

// Set up checker code
static memmodel refmem(false);

typedef deque<tb_cli_req_t> Fifo;

// Input and Output for Testbench
SC_MODULE (TbIO) {
  sc_in_clk clk;
  sc_in <bool> rst;
  Connections::Out< tb_cli_req_t > cli_req;
  Connections::In< tb_cli_rsp_t > cli_rsp;
  tb_cli_req_t curr_cli_req;
  tb_cli_rsp_t curr_cli_rsp;

  Fifo fifo;

  void source();
  void sink();
  SC_HAS_PROCESS(TbIO);
  TbIO(sc_module_name name_) : sc_module(name_),
  clk("clk"),
  rst("rst"),
  cli_req("cli_req"),
  cli_rsp("cli_rsp")
  {
    SC_THREAD(source);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);  
    
    SC_THREAD(sink);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }  
};


void TbIO::source() {

  fifo.clear();
    
  cli_req.Reset();
#ifndef _SYNTHESIS_
  boost::timer::cpu_timer timer;
  double sim_timestamp = sc_simulation_time();
  DCOUT("@" << sc_time_stamp() <<" Sending requests\n" << endl);
#endif

  wait(2, SC_NS);
  /*
    Initialize the memory with a bunch of pre-defined data
  */
  curr_cli_req.opcode = STORE;
  
  DCOUT("=========================================================" << endl);
  DCOUT("@" << sc_time_stamp() << ":  Initializing addresses" << endl);
  for (int i=0; i<SCRATCHPAD_CAPACITY/SCRATCHPAD_BANKS; i++) {
    wait();
    for (int j=0; j<SCRATCHPAD_BANKS; j++) {
      curr_cli_req.valids[j] = true;
      curr_cli_req.addr[j] = SCRATCHPAD_BANKS*i + j;
      //curr_cli_req.data[j] = SCRATCHPAD_BANKS*i + j;
      curr_cli_req.data[j] = rand();
      DCOUT("@" << sc_time_stamp() << hex << ":  Lane " << j << ": addr 0x" << curr_cli_req.addr[j].to_uint64() << ", data 0x" << curr_cli_req.data[j].to_uint64() << endl);
    }
    // Send the store request to the DUT here:
    cli_req.Push(curr_cli_req);
    // Update the memory model here:
    refmem.exec_store(curr_cli_req);
  }
  DCOUT("@" << sc_time_stamp() << ":  Done initializing memory" << endl);
  
  /*
    Do a whole bunch of reads from random addresses, then check the values
  */
  
  curr_cli_req.opcode = LOAD;
  for (int i=0; i<SCRATCHPAD_BANKS; i++) {
    curr_cli_req.valids[i] = true;
  }
  
  for (int i=0; i<SCRATCHPAD_CAPACITY/SCRATCHPAD_BANKS; i++) {
    wait();
    for (int j=0; j<SCRATCHPAD_BANKS; j++) {
      curr_cli_req.data[j] = 0xdeadbeef;
      curr_cli_req.addr[j] = SCRATCHPAD_BANKS*i + j;
    }
    cli_req.Push(curr_cli_req);
    // Push the requested load addresses into a fifo here for checking in the sink function:
    fifo.push_back(curr_cli_req);
  }
  

  // Wait for any transactions in the DUT to clear out
  wait(20, SC_NS);
  
#ifndef _SYNTHESIS_
  boost::timer::cpu_times elapsed = timer.elapsed();
  double sim_ns = sc_simulation_time() - sim_timestamp;
  DCOUT(" CPU TIME: " << (elapsed.user + elapsed.system) / 1e6 << " ms" << std::endl);
  DCOUT(" SIMULATION SPEED: " << 1e9 * (sim_ns) / (elapsed.user + elapsed.system) << " Hz" << std::endl);
#endif

  DCOUT("@" << sc_time_stamp() <<" Done.  Terminating simulation\n" << endl);  
  sc_stop();
  
}

void TbIO::sink() {
    bool pass;
    tb_cli_req_t ref_cli_req;
    cli_rsp.Reset();
    wait(2, SC_NS);
    while (1) {
      wait();
      curr_cli_rsp = cli_rsp.Pop();
      ref_cli_req = fifo.front();
      DCOUT("=========================================================" << endl);
      std::stringstream ss;
      for (int j=0; j<SCRATCHPAD_BANKS; j++) {
	if (curr_cli_rsp.valids[j]==true) {
	  DCOUT("  Lane " << j << ": Requested addr 0x" << ref_cli_req.addr[j].to_uint64() << ", received data 0x" << curr_cli_rsp.data[j].to_uint64() << endl);
	} else {
	  DCOUT("  Lane " << j << ": No valid data received." << endl);
	}
      }
      pass = refmem.check_response(curr_cli_rsp, ref_cli_req);
      if (!pass) {
	DCOUT("ERROR: Mismatch " << "\n");
	DCOUT("@" << sc_time_stamp() << "\t **FAIL**" << endl);
	for (int j=0; j<SCRATCHPAD_BANKS; j++) {
	  DCOUT("\t\t   Expected data[" << j << "]: " << refmem.read(ref_cli_req.addr[j]).to_uint64() << endl);
	}
	assert(false);
      }
      else {
	DCOUT("@" << sc_time_stamp() << "\t Memory check passed!" << endl);
      }
      fifo.pop_front();
    }
  }

// ************************************
// Here's the testbench for the scratchpad
// ************************************
SC_MODULE (TESTBENCH) {

  // DUT and its i/o signals
  NVHLS_DESIGN(ScratchpadTop) dut;
  TbIO TbIO_inst;
  sc_clock clk;
  sc_signal<bool> rst;
  Connections::Combinational< tb_cli_req_t > cli_req;
  Connections::Combinational< tb_cli_rsp_t > cli_rsp;
  
  SC_CTOR(TESTBENCH) : 
    dut("dut"),
    TbIO_inst("TbIO_inst"),
    clk("clk", 1, SC_NS, 0.5, 0, SC_NS, true),
    rst("rst"),
    cli_req("cli_req"),
    cli_rsp("cli_rsp")
  {
    // Connect the DUT
    dut.clk(clk);
    dut.rst(rst);
    dut.cli_req(cli_req);
    dut.cli_rsp(cli_rsp);

    TbIO_inst.clk(clk);
    TbIO_inst.rst(rst);
    TbIO_inst.cli_req(cli_req);
    TbIO_inst.cli_rsp(cli_rsp);

    SC_THREAD(init); 

  }
  void init() {
    rst = 0;
    cout << "@" << sc_time_stamp() << " Asserting Reset " << endl ;
    wait(2, SC_NS);
    cout << "@" << sc_time_stamp() << " Deasserting Reset " << endl ;
    rst = 1;
  }
};


int sc_main(int argc, char *argv[]) {
  nvhls::set_random_seed();
  TESTBENCH my_testbench("my_testbench");
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  sc_report_handler::set_actions(SC_ERROR, SC_DISPLAY);
  sc_start();
  bool rc = (sc_report_handler::get_count(SC_ERROR) > 0);
  if (rc) {
    DCOUT("Simulation Failed" << endl);
    return 1;
  } else {
    DCOUT("CMODEL PASS" << endl);
    return 0;
  }
}
