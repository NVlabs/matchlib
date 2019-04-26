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
#include <stdio.h>
#include <match_scverify.h>
#include <testbench/nvhls_rand.h>

#include "ArbitratedScratchpadDPTop.h"

DATA_TYPE get_rand_data() {
  return rand();
}

DATA_TYPE ref_mem[kNumBanks*kEntriesPerBank];
const int LoadAddrFifoLen = 100;
FIFO<Address, LoadAddrFifoLen> load_addr_fifo[kNumBanks][kNumReadPorts];
bool check_response(DATA_TYPE port_read_out[kNumReadPorts], bool port_read_out_valid[kNumReadPorts]) {
  bool valid_data_rcvd = false;
  for(unsigned i=0; i<kNumReadPorts; i++) {
    if(port_read_out_valid[i]) {
      valid_data_rcvd = true;
      bool load_data_matched = false;

      for(unsigned j=0; j<kNumBanks; j++) {
        if(!load_addr_fifo[j][i].isEmpty()) {
	      Address load_addr_tmp;
	      load_addr_tmp = load_addr_fifo[j][i].peek();
          bool is_equal = true;
	      if(port_read_out[i] == ref_mem[load_addr_tmp]) {
            is_equal &= true;
          }
          if (is_equal) {
	        load_data_matched = true;
	        load_addr_fifo[j][i].pop();
	        break;
	      }
	    }
      }
      assert(load_data_matched);
    }
  }
  return valid_data_rcvd;
}

CCS_MAIN (int argc, char *argv[]) {

  nvhls::set_random_seed();

  Address read_address[kNumReadPorts];
  bool read_req_valid[kNumReadPorts];
  Address write_address[kNumWritePorts];
  bool write_req_valid[kNumWritePorts];
  DATA_TYPE write_data[kNumWritePorts]; 
  bool read_ack[kNumReadPorts];
  bool write_ack[kNumWritePorts];
  DATA_TYPE port_read_out[kNumReadPorts];
  bool port_read_out_valid[kNumReadPorts];
  bool complete;
  for (unsigned k = 0; k < kNumReadPorts; k++) {
    read_address[k] = 0;
    read_req_valid[k] = false;
  }
  for (unsigned k = 0; k < kNumWritePorts; k++) {
    write_address[k] = 0; 
    write_req_valid[k] = false;
    write_data[k] = 0; 
  }
  for (unsigned i = 0; i < kNumBanks*kEntriesPerBank/kNumWritePorts; i++) {
    for (unsigned k = 0; k < kNumWritePorts; k++) {
      write_address[k] = i*kNumWritePorts + k;
      write_req_valid[k] = true;
      write_data[k] =  get_rand_data();
      ref_mem[i*kNumWritePorts + k] = write_data[k];
    }
    complete = false;
    while (!complete) {
      CCS_DESIGN(ArbitratedScratchpadDPTop)(read_address, read_req_valid,
                        write_address, write_req_valid, write_data, 
                        read_ack, write_ack, 
                        port_read_out, port_read_out_valid);
      complete = true;
      cout << "Write Request: ";
      for (unsigned k = 0; k < kNumWritePorts; k++) {
        cout << "Port-" << k << " " << write_address[k] << " " << write_req_valid[k] << " " << write_data[k] <<  " " << write_ack[k];
      }
      cout << endl;
      for(unsigned k=0; k<kNumWritePorts; k++) {
        if(write_req_valid[k]==true) {
          if(!write_ack[k]) {
            complete = false;
          } else {
            write_req_valid[k] = false; 
          }
        }
      }
    }
  }
  for (unsigned k = 0; k < kNumWritePorts; k++) {
    write_address[k] = 0; 
    write_req_valid[k] = false;
    write_data[k] = 0; 
  }

  for (unsigned i = 0; i < kNumBanks*kEntriesPerBank/kNumReadPorts; i++) {
    for (unsigned k = 0; k < kNumReadPorts; k++) {
      read_address[k] = i * kNumReadPorts + k;
      read_req_valid[k] = true;
      port_read_out[k] = 0;
      port_read_out_valid[k] = 0;
    }
    complete = false;
    while(!complete) {
      CCS_DESIGN(ArbitratedScratchpadDPTop)(read_address, read_req_valid,
                        write_address, write_req_valid, write_data, 
                        read_ack, write_ack, 
                        port_read_out, port_read_out_valid);
      cout << "Read Request: \n";
      for (unsigned k = 0; k < kNumReadPorts; k++) {
        cout << "Port-" << k << " " << read_address[k] << " " << read_req_valid[k] << " " << read_ack[k] << " " << port_read_out[k] <<  " " << port_read_out_valid[k] << endl;
      }
      cout << endl;
      complete = true;
      for (unsigned k = 0; k < kNumReadPorts; k++) {
        if(read_req_valid[k]==true) {
          if(!read_ack[k]) {
            complete = false;
          } else {
            read_req_valid[k] = false; 
            // Push the load address into a FIFO since the load request could be
	        // serviced out of order in the event of a bank conflict.
	        int bank_idx = read_address[k] % kNumBanks;
	        load_addr_fifo[bank_idx][k].push(read_address[k]);
          }
        }
      }
      bool resp = check_response(port_read_out, port_read_out_valid);
      assert(resp);
    }
  }

  CCS_RETURN(0);

}
