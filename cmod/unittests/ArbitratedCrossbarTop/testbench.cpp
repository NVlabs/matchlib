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
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <match_scverify.h>

#include <fifo.h>
#include "ArbitratedCrossbarTop.h"
#include <testbench/nvhls_rand.h>

const int g_test_len = 100;
const int TB_FIFO_LEN = g_test_len + 50;

CCS_MAIN(int argc, char *argv[]) {

    nvhls::set_random_seed();

    // FIFOs used for storing expected values and then checking
    FIFO< Word_t, TB_FIFO_LEN > tb_fifos[NUM_OUTPUTS][NUM_INPUTS];
    DataInArray data_in;
    OutputIdxArray dest_in;
    ValidInArray valid_in;
    DataOutArray data_out;
    ValidOutArray valid_out;
    ReadyArray ready;
    for (unsigned in = 0; in < NUM_INPUTS; in++) {
      dest_in[in] = 0;
      data_in[in] = 0;
      valid_in[in] = true;
    }
    for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
      data_out[out] = 0;
      valid_out[out] = 0;
    }
    for (unsigned in = 0; in < NUM_INPUTS; in++) {
      ready[in] = 0;
    }

    // "TB" buffer to store the outputs (which may arrive "out of order")
    // We will then sort it to compare with the expected FIFO which is pushed to in order
    // This allows us to ignore the order in which the arbiters service simultaneous output requests
    // We also keep track of how many outputs are actually detected so we know what part of the outputs buffer array is valid
    bool sim_pass = true;
    cout << "Total number of inputs: " << g_test_len << endl;
    for (int test = 0; test < g_test_len; test++) {
      cout << "Test iteration: " << test << endl;

      // Send deterministic data to random destination
      for (unsigned in = 0; in < NUM_INPUTS; in++) {
        int dest = rand()%NUM_OUTPUTS;
        dest_in[in] = dest;
        data_in[in] = in;
        cout << "data_in[" << dest_in[in] << "][" << in << "] = " << data_in[in] << "\t valid: " << valid_in[in] << endl;
      }

      // Run DUT
      CCS_DESIGN(ArbitratedCrossbarTop)(data_in, dest_in, valid_in, data_out, valid_out, ready);
      for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
        cout << "data_out[" << out << "] = " << data_out[out] << "\t valid: " << valid_out[out] << endl;
      }
      for (unsigned in = 0; in < NUM_INPUTS; in++) {
        cout << "ready[" << in << "] = " << ready[in] << "; ";
        if (ready[in]) {
	  cout << "Pushing " << data_in[in] << " into tb_fifos[" << dest_in[in] << "][" << in << "]";
          tb_fifos[dest_in[in]][in].push(data_in[in]);
        }
	cout << endl;
      }

      // Read from each output channel if it is not empty
      for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
        if (valid_out[out]) {
          bool is_match = false;
          for (unsigned in = 0; in < NUM_INPUTS; in++) {
            if (!tb_fifos[out][in].isEmpty()) {
              cout << " Checking data_out[" << out << "] = " << data_out[out] << " with tb_fifos[" << out << "][" << in << "] = " << tb_fifos[out][in].peek() << endl;
              if (data_out[out] == tb_fifos[out][in].peek()) {
                tb_fifos[out][in].pop();
                is_match = true;
		break;
              }
            }
          }
          assert(is_match==true);
	  cout << endl;
        }
      }
    }

    // Flush the DUT: Run it enough times to make sure all queues are empty
    for (unsigned in = 0; in < NUM_INPUTS; in++) {
        valid_in[in] = false;
    }
    for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
        data_out[out] = 0;
        valid_out[out] = 0;
    }
    for (unsigned in = 0; in < NUM_INPUTS; in++) {
        ready[in] = 0;
    }
    bool expected_fifo_empty = false;
    while (!expected_fifo_empty) {
	  CCS_DESIGN(ArbitratedCrossbarTop)(data_in, dest_in, valid_in, data_out, valid_out, ready);
	  for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
        cout << "Output: " << out << "\t valid: " << valid_out[out] << "\t data: " << data_out[out] << endl;
      }
      // Read from each output channel if it is not empty
	  for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
	    if (valid_out[out]) {
          bool is_match = false;
          for (unsigned in = 0; in < NUM_INPUTS; in++) {
            if (!tb_fifos[out][in].isEmpty()) {
              if (data_out[out] == tb_fifos[out][in].peek()) {
                tb_fifos[out][in].pop();
                is_match = true;
              }
            }
          }
          assert(is_match==true);
	    }
	  }
      bool fifo_empty_local = true;
	  for (unsigned out = 0; out < NUM_OUTPUTS; out++) {
	    for (unsigned in = 0; in < NUM_INPUTS; in++) {
          fifo_empty_local &= tb_fifos[out][in].isEmpty();
          if (!tb_fifos[out][in].isEmpty())
            cout << "Testbench fifo[" << out<<"]["<<in << "] not empty\n";
        }
      }
      cout << "FIFO Empty? : " << fifo_empty_local << endl;
      expected_fifo_empty = fifo_empty_local;
    }

    if(sim_pass) {
      cout << "\n[PASSED] All tests successful." << endl;
    }
    else {
      cout << "\n[FAILED]: Test failed. Check log. " << endl;
    }
    CCS_RETURN(0);
}
