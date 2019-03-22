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
#ifndef ARBITRATED_SCRATCHPAD_H
#define ARBITRATED_SCRATCHPAD_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_message.h>
#include <fifo.h>
#include <Arbiter.h>
#include <mem_array.h>
#include <arbitrated_crossbar.h>
#include <ArbitratedScratchpad/ArbitratedScratchpadTypes.h>
#include <crossbar.h>
/**
 * \brief Scratchpad Memories with arbitration and queuing 
 * \ingroup ArbitratedScratchpad
 *
 * \tparam DataType         DataType if input and output 
 * \tparam CapacityInBytes  Capacity of scratchpad 
 * \tparam NumInputs        Number of Inputs 
 * \tparam NumBanks         Number of Banks 
 * \tparam LenInputBuffer   Length of Input Buffer 
 * \tparam LenOutputBuffer  Length of Output Buffer 
 *
 * \par A Simple Example
 * \code
 *      #include <ArbitratedScratchpad.h>
 *
 *      ...
 *      ArbitratedScratchpad<DataType, ScratchpadCapacity, NumInputs, NumBanks, InputQueueLength> dut;
 *      dut.reset(); // optional in this case, suitable if xbar is a class member rather than local variable
 *      ...
 *      dut.load_store(curr_cli_req, curr_cli_rsp, ready);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename DataType, unsigned int CapacityInBytes,
          unsigned int NumInputs, unsigned int NumBanks,
          unsigned int InputQueueLen>
class ArbitratedScratchpad {

 public:
  //------------Constants Here---------------------------
  // Derived parameters
  static const int addr_width = nvhls::nbits<CapacityInBytes - 1>::val;
  static const int log2_nbanks = nvhls::nbits<NumBanks - 1>::val;
  static const int log2_inputs = nvhls::nbits<NumInputs - 1>::val;

  //------------Local typedefs---------------------------
  typedef NVUINTW(log2_nbanks) bank_sel_t;                // index of bank
  typedef NVUINTW(addr_width - log2_nbanks) bank_addr_t;  // address within bank
  typedef NVUINTW(log2_inputs) input_sel_t;               // index of input

  struct bank_req_t : public nvhls_message {
    NVUINT1 do_store;
    bank_addr_t addr;
    DataType    wdata;
    input_sel_t input_chan;
    static const int width = 1 + addr_width-log2_nbanks + Wrapped<DataType>::width + log2_inputs;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& do_store;
      m& addr;
      m& wdata;
      m& input_chan;
    }
  };
  struct bank_rsp_t : public nvhls_message {
    NVUINT1 valid;
    DataType rdata;
    static const int width = 1 + Wrapped<DataType>::width;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& rdata;
      m& valid;
    }
  };

  typedef cli_req_t<DataType, addr_width, NumInputs> req_t; // request input type
  typedef cli_rsp_t<DataType, NumInputs> rsp_t;             // response output type

  //------------Local Variables Here---------------------
  mem_array_sep<DataType, CapacityInBytes, NumBanks> banks;
  bank_req_t bank_reqs[NumInputs];
  bank_rsp_t bank_rsps[NumInputs];

  ArbitratedCrossbar<bank_req_t, NumInputs, NumBanks, InputQueueLen, 0>
      request_xbar;

  void compute_bank_request(req_t &curr_cli_req, bank_req_t bank_req[NumInputs],
                            bank_sel_t bank_sel[NumInputs],
                            bool bank_req_valid[NumInputs]) {

    // For each request, compute the bank request fields
    #pragma hls_unroll yes
    for (unsigned in_chan = 0; in_chan < NumInputs; in_chan++) {

      // Get the target bank
      bank_sel[in_chan] =
          nvhls::get_slc<log2_nbanks>(curr_cli_req.addr[in_chan], 0);

      // Compile the bank request
      bank_req[in_chan].do_store = (curr_cli_req.valids[in_chan] == true) &&
                                   (curr_cli_req.type.val == CLITYPE_T::STORE);
      bank_req[in_chan].addr = nvhls::get_slc<addr_width - log2_nbanks>(
          curr_cli_req.addr[in_chan], log2_nbanks);
      if (bank_req[in_chan].do_store) {
        bank_req[in_chan].wdata = curr_cli_req.data[in_chan];
      }

      // Cast current input to input index type
      input_sel_t input_idx        = in_chan;
      bank_req[in_chan].input_chan = input_idx;

      // Set the request valid flag
      bank_req_valid[in_chan] = (curr_cli_req.valids[in_chan] == true);
    }
  }

  void banks_load_store(bank_req_t bank_req[NumBanks],
                        bool bank_req_valid[NumBanks],
                        bank_rsp_t bank_rsp[NumBanks]) {
    #pragma hls_unroll yes
    for (unsigned bank = 0; bank < NumBanks; bank++) {
      if (bank_req_valid[bank] == true) {
        if (!bank_req[bank].do_store) {
          bank_rsp[bank].valid = true;
          bank_rsp[bank].rdata = banks.read(bank_req[bank].addr, bank);
        } else {
          banks.write(bank_req[bank].addr, bank, bank_req[bank].wdata);
          bank_rsp[bank].valid = false;
        }
      } else {
        bank_rsp[bank].valid = false;
      }
    }
  }

 public:
  ArbitratedScratchpad() { reset(); }

  void reset() { request_xbar.reset(); }

  #ifdef HLS_ALGORITHMICC
  void load_store(req_t &curr_cli_req, rsp_t &load_rsp,
                  bool input_ready[NumInputs]) {
    bank_req_t bank_req[NumInputs];
    bank_sel_t bank_sel[NumInputs];
    bool bank_req_valid[NumInputs];

    compute_bank_request(curr_cli_req, bank_req, bank_sel, bank_req_valid);
  #else
  void load_store(bank_req_t bank_req[NumInputs],
                  bank_sel_t bank_sel[NumInputs],
		  bool       bank_req_valid[NumInputs],
		  rsp_t &load_rsp, bool input_ready[NumInputs]) {
  #endif
    DCOUT("\tinputs:" << endl);
    for (unsigned i = 0; i < NumInputs; ++i) {
      DCOUT("\t" << i << " :"
           << " valid="  << bank_req_valid[i]
           << " select=" << bank_sel[i]
	   << " addr="   << bank_req[i].addr
           << " wdata="  << bank_req[i].wdata
	   << " load="   << !bank_req[i].do_store
           << " store="  << bank_req[i].do_store
	   << " input="  << bank_req[i].input_chan << endl);
    }
    DCOUT("\t------" << endl);

    bank_req_t bank_req_winner[NumBanks];
    bool bank_req_winner_valid[NumBanks];
    request_xbar.run(bank_req, bank_sel, bank_req_valid, bank_req_winner,
                     bank_req_winner_valid, input_ready);

    DCOUT("\t\tbank winner transactions:" << endl);
    for (unsigned i = 0; i < NumBanks; ++i) {
      DCOUT("\t\t" << i << " :"
           << " valid=" << bank_req_winner_valid[i]
           << " addr="  << bank_req_winner[i].addr
           << " wdata=" << bank_req_winner[i].wdata
           << " load="  << !bank_req_winner[i].do_store
           << " store=" << bank_req_winner[i].do_store
	   << " input=" << bank_req_winner[i].input_chan << endl);
    }
    DCOUT("\t\t------" << endl);
    DCOUT("\t\tinput_ready:" << endl);
    for (unsigned i = 0; i < NumInputs; ++i) {
      DCOUT("\t\t" << i << " : ready=" << input_ready[i] << endl);
    }
    DCOUT("\t\t------" << endl);

    bank_rsp_t bank_rsp[NumBanks];
    banks_load_store(bank_req_winner, bank_req_winner_valid, bank_rsp);

    // Prepare the inputs for response crossbar
    DataType   data_in[NumBanks];
    bool       valid_in[NumBanks];
    bank_sel_t source[NumInputs];
    bool       valid_src[NumInputs];
    DataType   data_out[NumInputs];
    bool       valid_out[NumInputs];

    #pragma hls_unroll yes
    for (unsigned out = 0; out < NumInputs; out++) {
      valid_src[out] = false;
    }

    #pragma hls_unroll yes
    for (unsigned bank = 0; bank < NumBanks; bank++) {
      valid_in[bank] = bank_rsp[bank].valid;
      data_in[bank]  = bank_rsp[bank].rdata;
      if (bank_rsp[bank].valid) {
        source[bank_req_winner[bank].input_chan]    = bank;
        valid_src[bank_req_winner[bank].input_chan] = true;
      }
    }

    crossbar<DataType, NumBanks, NumInputs>(data_in, valid_in, source,
                                            valid_src, data_out, valid_out);

    #pragma hls_unroll yes
    for (unsigned out = 0; out < NumInputs; out++) {
      load_rsp.valids[out] = valid_out[out];
      load_rsp.data[out]   = data_out[out];
    }
  }

};  // end ArbitratedScratchpad class

#endif  // end ARBITRATED_SCRATCHPAD_H
