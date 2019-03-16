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
#include <match_scverify.h>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <hls_globals.h>
#include <fifo.h>
#include <testbench/nvhls_rand.h>

#include <ArbitratedScratchpad.h>
#include <ArbitratedScratchpad/ArbitratedScratchpadTypes.h>
#include "ArbitratedScratchpadConfig.h"
#include "ArbitratedScratchpadTop.h"

#ifndef NUM_ITERS
#define NUM_ITERS 10000
#endif

typedef NVUINTC(ScratchpadAddrWidth) LoadAddrType;
const int LoadAddrFifoLen = NUM_ITERS*2;
FIFO<LoadAddrType, LoadAddrFifoLen> load_addr_fifo[NumBanks][NumInputs];

// *************************************************
// Use a reference memory class for testing the code
// *************************************************
class memmodel {
 private:
  DataType mem[NumInputs][RefMemSizeInBytes];

 public:
  int size;
  memmodel(bool init_to_zero);

  void exec_store(tb_cli_req_t cli_req);
  bool check_response(tb_cli_rsp_t cli_rsp);
  DataType read(NVUINTC(ScratchpadAddrWidth) addr, int input = 0);
};

memmodel::memmodel(bool init_to_zero) {
  size = RefMemSizeInBytes;
  for(unsigned j=0; j<NumInputs; j++) {
    for(int i=0; i<size; i++) {
      if(init_to_zero) {
        mem[j][i] = 0;
      } else {
        mem[j][i] = rand() % 256;
      }
    }
  }
}

void memmodel::exec_store(tb_cli_req_t cli_req) {
  for(unsigned i=0; i<NumInputs; i++) {
    if(cli_req.valids[i] == true) {
      mem[i][cli_req.addr[i]] = cli_req.data[i];
      cout << "mem[" << i << "][" << cli_req.addr[i] << "]=" << cli_req.data[i] << endl;
    }
  }
}

DataType memmodel::read(NVUINTC(ScratchpadAddrWidth) addr, int input) {
  return mem[input][addr];
}

bool memmodel::check_response(tb_cli_rsp_t cli_rsp) {
  bool valid_data_rcvd = false;
  for(unsigned i=0; i<NumInputs; i++) {
    if(cli_rsp.valids[i]) {
      valid_data_rcvd = true;
      bool load_data_matched = false;

      for(unsigned j=0; j<NumBanks; j++) {
        if(!load_addr_fifo[j][i].isEmpty()) {
	      LoadAddrType load_addr_tmp;
	      load_addr_tmp = load_addr_fifo[j][i].peek();
          for(unsigned k=0; k<NumInputs; k++) {
	        cout << "Comparing " << cli_rsp.data[i] << " with mem[" << k << "][" << load_addr_tmp << "] = " << mem[k][load_addr_tmp] << endl;
	        if(cli_rsp.data[i] == mem[k][load_addr_tmp]) {
	          load_data_matched = true;
	          load_addr_fifo[j][i].pop();
	          break;
	        }
          }
	    }
      }
      assert(load_data_matched);
    }
  }
  return valid_data_rcvd;
}

// Set up checker code
static memmodel refmem(false);

// *************************************************
// Testbench
// *************************************************
CCS_MAIN(int argc, char *argv[]) {

  nvhls::set_random_seed();

  // Declare IO to dut access method
  tb_cli_req_t curr_cli_req;
  tb_cli_rsp_t curr_cli_rsp;
  bool ready[NumInputs];

  for (unsigned i=0; i<NumInputs; i++) {
    curr_cli_rsp.valids[i] = false;
    curr_cli_rsp.data[i] = 0;
  }

  // -------------------------------------
  // Initiate sequential stores to the memory
  // -------------------------------------
  curr_cli_req.type.val = CLITYPE_T::STORE;
  bool complete;

  for(unsigned i=0; i<ScratchpadCapacity / NumInputs; i++) {
    for(unsigned j=0; j<NumInputs; j++) {
      // Create request
      curr_cli_req.valids[j] = true;
      curr_cli_req.addr[j]   = NumInputs * i + j;
      curr_cli_req.data[j]   = 0x12341234 + NumInputs * i + j;
    }
        
    // Update the memory model here:
    refmem.exec_store(curr_cli_req);       

    complete = false;
    while(!complete) {
      // Store to DUT
      CCS_DESIGN(ArbitratedScratchpadTop)(curr_cli_req, curr_cli_rsp, ready);

      complete = true;
      for(unsigned i=0; i<NumInputs; i++) {
        if(curr_cli_req.valids[i]==true) {
          if(!ready[i]) {
            complete = false;
          } else {
            curr_cli_req.valids[i] = false; 
          }
        }
      }
    }
  }

  // ----------------------------------------
  // Read out memory and check that it's correct
  // ----------------------------------------
  curr_cli_req.type.val = CLITYPE_T::LOAD;

  // Due to the latency of the Arbitrated Scratchpad, for now we do
  // error checking by waiting until a valid response is received, 
  // and then assuming that all responses will occur in order.
  // This should be correct since we don't have bank conflicts for now.
  for(unsigned i=0; i<ScratchpadCapacity / NumInputs; i++) {
    for(unsigned j=0; j<NumInputs; j++) {
      curr_cli_req.data[j]   = 0xdeadbeef;
      curr_cli_req.addr[j]   = NumInputs * i + j;
      curr_cli_req.valids[j] = true;
    }

    complete = false;
    while(!complete) {
      // Store to DUT
      CCS_DESIGN(ArbitratedScratchpadTop)(curr_cli_req, curr_cli_rsp, ready);
      complete = true;

      for(unsigned j=0; j<NumInputs; ++j) {
        if(curr_cli_req.valids[j]==true) {
          if(!ready[j]) {
            complete = false;
          } else {
            curr_cli_req.valids[j] = false; 

            // Push the load address into a FIFO since the load request could be
	    // serviced out of order in the event of a bank conflict.
	    int bank_idx = curr_cli_req.addr[j] % NumBanks;
	    load_addr_fifo[bank_idx][j].push(curr_cli_req.addr[j]);
          }
        }
      }
      refmem.check_response(curr_cli_rsp);
    }
  }

  // Add nop cycles for all the loads to complete
  bool valid_data_rcvd;
  for(int it=0; it<NUM_ITERS/100; it++) {
    cout << "Load-wait it = " << it << endl;
    for(unsigned i=0; i<NumInputs; i++) {
      curr_cli_req.data[i] = 0;
      curr_cli_req.addr[i] = 0;
      curr_cli_req.valids[i] = false;
    }
    CCS_DESIGN(ArbitratedScratchpadTop)(curr_cli_req, curr_cli_rsp, ready);
    valid_data_rcvd = refmem.check_response(curr_cli_rsp);
    if(!valid_data_rcvd) {
      break;
    }
  }

  // There is a design-bug when initiating stores to random
  // addresses in the presence of an input FIFO per channel.
  // Right now, the design cannot guarantee that the writes
  // are carried out in the initiated order.
  // ----------------------------------------
  // Generate stores to random addresses
  // ----------------------------------------
  tb_cli_req_t dummy_req;
  bool served;
  bool requested;
  for(int it=0; it<NUM_ITERS; it++) {
    cout << "Store it = " << it << endl;
    curr_cli_req.type.val = CLITYPE_T::STORE;

    requested = false;
    for(unsigned i=0; i<NumInputs; i++) {
      curr_cli_req.data[i] = rand();
      curr_cli_req.addr[i] = rand()%ScratchpadCapacity - i; // Multiple inputs cannot write to the same address
      bool valid = rand()%2;
      curr_cli_req.valids[i] = valid;
      requested = requested || valid; // Track whether at least one request was valid
    }
    dummy_req = curr_cli_req;

    CCS_DESIGN(ArbitratedScratchpadTop)(curr_cli_req, curr_cli_rsp, ready);
    served = false;
    for(unsigned i=0; i<NumInputs; i++) {
      if(!ready[i]) {
        dummy_req.valids[i] = false;
      } else {
        served =true;
      }
    }
    assert(!requested || served); // At least one request should have been served or nothing was requested
    refmem.exec_store(dummy_req);       
  }

  // Add nop cycles for the random stores to complete
  for(int it=0; it<NUM_ITERS/100; it++) {
    cout << "Store-wait it = " << it << endl;
    for(unsigned i=0; i<NumInputs; i++) {
      curr_cli_req.data[i] = 0;
      curr_cli_req.addr[i] = 0;
      curr_cli_req.valids[i] = false;
    }
    CCS_DESIGN(ArbitratedScratchpadTop)(curr_cli_req, curr_cli_rsp, ready);
  }

  // ----------------------------------------
  // Initiate loads from random addresses
  // ----------------------------------------
  for(int it=0; it<NUM_ITERS; it++) {
    cout << "Load it = " << it << endl;
    curr_cli_req.type.val = CLITYPE_T::LOAD;

    requested = false;
    for(unsigned i=0; i<NumInputs; i++) {
      curr_cli_req.data[i] = rand();
      curr_cli_req.addr[i] = rand()%ScratchpadCapacity;
      bool valid = rand()%2;
      curr_cli_req.valids[i] = valid;
      requested = requested || valid; // Track whether at least one request was valid
    }

    CCS_DESIGN(ArbitratedScratchpadTop)(curr_cli_req, curr_cli_rsp, ready);
    served = false;
    for(unsigned i=0; i<NumInputs; i++) {
      if(ready[i]) {
        served =true;

	// Push the load address into a FIFO since the load request could be
	// serviced out of order in the event of a bank conflict.
	if(curr_cli_req.valids[i]) {
	  int bank_idx = curr_cli_req.addr[i] % NumBanks;
	  load_addr_fifo[bank_idx][i].push(curr_cli_req.addr[i]);
	  cout << "Pushing addr"          << curr_cli_req.addr[i]
               << " into load_addr_fifo[" << bank_idx << "][" << i << "]" << endl;
	}
      }
    }
    assert(!requested || served); // At least one request should have been served or nothing was requested
    refmem.check_response(curr_cli_rsp);
  }
  DCOUT("CMODEL PASS" << endl);
  CCS_RETURN(0);
}
