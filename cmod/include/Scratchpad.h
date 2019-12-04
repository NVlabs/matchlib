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
#ifndef SC_SCRATCHPAD
#define SC_SCRATCHPAD

#include <cstdio>
#include <systemc.h>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <hls_globals.h>
#include <nvhls_connections.h>
#include <crossbar.h>

/**
 * \brief Parameterized banked scratchpad memory 
 * \ingroup Scratchpad 
 *
 * \tparam T                   EntryType 
 * \tparam N                   Number of requests 
 * \tparam CAPACITY_IN_BYTES 
 *
 * \par Overview
 *   -Assumptions:  All N requests are guaranteed conflict-free.
 *  
 *   Ports:
 *     1 input port for requests (load OR store) from client
 *     1 output port for replies (load only) back to client
 *  
 *   Input parameters:
 *     Number of banks (assumed to be same as the number of requests)
 *     Address Width
 *  
 *
 * \par A Simple Example
 * \code
 *      #include <Scratchpad.h>
 *
 *      ...
 *      Connections::In< cli_req_t<data32_t, ADDR_WIDTH,N> > cli_req;
 *      Connections::Out< cli_rsp_t<data32_t, N> > cli_rsp;
 *      Scratchpad<data32_t, SCRATCHPAD_BANKS,SCRATCHPAD_CAPACITY> myscratchpad;

 *      ...
 *        // Connect the DUT
 *        myscratchpad.clk(clk);
 *        myscratchpad.rst(rst);
 *        myscratchpad.cli_req(cli_req);
 *        myscratchpad.cli_rsp(cli_rsp);
 *      ...
 *
 * \endcode
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/Scratchpad/run/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 *
 */

#include <Scratchpad/ScratchpadTypes.h>
#include <mem_array.h>

template <typename T, int N, int CAPACITY_IN_BYTES>
class Scratchpad : public sc_module {
 public:
  static const int ADDR_WIDTH = nvhls::nbits<CAPACITY_IN_BYTES - 1>::val;
  sc_in_clk clk;
  sc_in<bool> rst;
  Connections::In<cli_req_t<T, ADDR_WIDTH, N> > cli_req;
  Connections::Out<cli_rsp_t<T, N> > cli_rsp;

  //------------Constants Here---------------------------
  // Derived parameters
  static const int NBANKS_LOG2 = nvhls::nbits<N - 1>::val;

  //------------Local typedefs---------------------------
  typedef NVUINTW(NBANKS_LOG2) bank_sel_t;
  typedef NVUINTW(ADDR_WIDTH - NBANKS_LOG2) bank_addr_t;
  struct bank_req_t {
    bank_addr_t addr;
    T wdata;
  };

  //------------Local Variables Here---------------------
  mem_array_sep<T, CAPACITY_IN_BYTES, N> banks;
  bank_req_t input_reqs[N];
  bool input_reqs_valid[N];
  bank_req_t bank_reqs[N];
  bool bank_reqs_valid[N];
  T bank_rsps_data[N];
  bool bank_rsps_valid[N];
  bool load_rsps_valid[N];
  cli_req_t<T, ADDR_WIDTH, N> curr_cli_req;
  cli_rsp_t<T, N> load_rsp;
  bank_sel_t bank_src_lane[N];
  bank_sel_t bank_dst_lane[N];

  //----------- Constructor -----------------------------
  //   -Allocate and declare sub-modules
  //   -Declare all SC_METHODs and SC_THREADs
  SC_HAS_PROCESS(Scratchpad);
  Scratchpad(sc_module_name name_) : sc_module(name_) {
    SC_THREAD(run);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
    //DCOUT("Capacity (bytes): " << CAPACITY_IN_BYTES << ", banks: " << N
    //                           << ", addr width: " << ADDR_WIDTH << endl);
  }

  void run() {

    // Reset behavior
    cli_req.Reset();
    cli_rsp.Reset();
    wait();

    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (true) {

      bool is_load;
      is_load = false;

      // Read client request
      // (Implemented as a blocking read since there's no other work to do if no
      // valid request in the channel)
      curr_cli_req = cli_req.Pop();
      is_load = (curr_cli_req.opcode == LOAD);

// Pre-process the bank requests and compute lane selects from addresses
#pragma hls_unroll yes
      for (int i = 0; i < N; i++) {
        // For each request, figure out the target bank and update its bank_req
        // fields
        bank_sel_t bank_sel;
        bank_sel = nvhls::get_slc<NBANKS_LOG2>(curr_cli_req.addr[i], 0);
        bank_src_lane[bank_sel] = i;

        // Save the lane->bank mapping for the response xbar
        bank_dst_lane[i] = bank_sel;

        // Convert from curr_cli_req to internal format
        input_reqs_valid[i] = (curr_cli_req.valids[i] == true);
        input_reqs[i].addr = nvhls::get_slc<ADDR_WIDTH - NBANKS_LOG2>(
            curr_cli_req.addr[i], NBANKS_LOG2);
        if (!is_load)
          input_reqs[i].wdata = curr_cli_req.data[i];
      }

      // Bank request crossbar
      crossbar<bank_req_t, N, N>(input_reqs, input_reqs_valid, bank_src_lane,
                                 bank_reqs, bank_reqs_valid);

// Loop over scratchpad banks, execute load or store on each bank
#pragma hls_unroll yes
      for (int i = 0; i < N; i++) {
        if ((bank_reqs_valid[i] == true) && is_load) {
          bank_rsps_valid[i] = true;
          bank_rsps_data[i] = banks.read(bank_reqs[i].addr, i);
        } else if ((bank_reqs_valid[i] == true) && !is_load) {
          banks.write(bank_reqs[i].addr, i, bank_reqs[i].wdata);
          bank_rsps_valid[i] = false;
        } else {
          bank_rsps_valid[i] = false;
        }
      }

      // Bank response crossbar
      crossbar<T, N, N>(bank_rsps_data, bank_rsps_valid, bank_dst_lane,
                        load_rsp.data, load_rsps_valid);
      #pragma hls_unroll yes
      for (int i = 0; i < N; i++) {
        load_rsp.valids[i] = load_rsps_valid[i];  // sc_lv to bool conversion
      }
      // Write client responses
      if (is_load) {
        cli_rsp.Push(load_rsp);
      }

      wait();
    }
  }
};

#endif
