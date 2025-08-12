/*
 * Copyright (c) 2016-2022, NVIDIA CORPORATION.  All rights reserved.
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
  static const int kDebugLevel = 2;
  // Derived parameters
  static const int addr_width = nvhls::nbits<CapacityInBytes - 1>::val;
  static const int log2_nbanks = (NumBanks == 1) ? 1 : nvhls::nbits<NumBanks - 1>::val;
  static const int log2_inputs = (NumInputs == 1) ? 1 : nvhls::nbits<NumInputs - 1>::val;

  static const bool is_nbanks_power_of_2 = (NumBanks & (NumBanks - 1)) == 0;
  static const int bank_addr_width = (is_nbanks_power_of_2 && (NumBanks > 1)) ? (addr_width - log2_nbanks) : (addr_width - log2_nbanks + 1);

  //------------Local typedefs---------------------------
  typedef NVUINTW(log2_nbanks) bank_sel_t;        // index of bank
  typedef NVUINTW(bank_addr_width) bank_addr_t;   // address within bank
  typedef NVUINTW(log2_inputs) input_sel_t;       // index of input

  struct bank_req_t : public nvhls_message {
    NVUINT1 do_store;
    bank_addr_t addr;
    DataType    wdata;
    input_sel_t input_chan;
    static const int width = 1 + bank_addr_width + Wrapped<DataType>::width + log2_inputs;

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
      if (NumInputs == 1) {
        bank_sel[in_chan] = 0;
      } else {
        if (is_nbanks_power_of_2) {
          bank_sel[in_chan] = nvhls::get_slc<log2_nbanks>(curr_cli_req.addr[in_chan], 0);
        } else {
          bank_sel[in_chan] = curr_cli_req.addr[in_chan] % NumBanks;
        }
      }
      // Compile the bank request
      bank_req[in_chan].do_store = (curr_cli_req.valids[in_chan] == true) &&
                                   (curr_cli_req.type.val == CLITYPE_T::STORE);

      if (NumInputs == 1) {
        bank_req[in_chan].addr = curr_cli_req.addr[in_chan];
      } else {
        if (is_nbanks_power_of_2) {
          bank_req[in_chan].addr = nvhls::get_slc<addr_width - log2_nbanks>(curr_cli_req.addr[in_chan], log2_nbanks);
        } else {
          bank_req[in_chan].addr = curr_cli_req.addr[in_chan] / NumBanks;
        }
      }

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

  #pragma map_to_operator [CCORE]
  #pragma ccore_type combinational
  void pseudo_cam(const bank_req_t bank_req_winner[NumBanks], const bank_rsp_t bank_rsp[NumBanks],
                  unsigned bank, unsigned &res, bool &en) {
    res = 0;
    en = false;
    for (unsigned i = 0; i < NumBanks; i++)
    {
      if ( (bank_req_winner[i].input_chan == bank) && bank_rsp[i].valid) 
      {
        res = i;
        en = true;
      }
    }
  }

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
    CDCOUT("\tinputs:" << endl, kDebugLevel);
    for (unsigned i = 0; i < NumInputs; ++i) {
      CDCOUT("\t" << i << " :"
           << " valid="  << bank_req_valid[i]
           << " select=" << bank_sel[i]
	   << " addr="   << bank_req[i].addr
           << " wdata="  << bank_req[i].wdata
	   << " load="   << !bank_req[i].do_store
           << " store="  << bank_req[i].do_store
	   << " input="  << bank_req[i].input_chan << endl, kDebugLevel);
    }
    CDCOUT("\t------" << endl, kDebugLevel);

    bank_req_t bank_req_winner[NumBanks];
    bool bank_req_winner_valid[NumBanks];
    request_xbar.run(bank_req, bank_sel, bank_req_valid, bank_req_winner,
                     bank_req_winner_valid, input_ready);

    CDCOUT("\t\tbank winner transactions:" << endl, kDebugLevel);
    for (unsigned i = 0; i < NumBanks; ++i) {
      CDCOUT("\t\t" << i << " :"
           << " valid=" << bank_req_winner_valid[i]
           << " addr="  << bank_req_winner[i].addr
           << " wdata=" << bank_req_winner[i].wdata
           << " load="  << !bank_req_winner[i].do_store
           << " store=" << bank_req_winner[i].do_store
	   << " input=" << bank_req_winner[i].input_chan << endl, kDebugLevel);
    }
    CDCOUT("\t\t------" << endl, kDebugLevel);
    CDCOUT("\t\tinput_ready:" << endl, kDebugLevel);
    for (unsigned i = 0; i < NumInputs; ++i) {
      CDCOUT("\t\t" << i << " : ready=" << input_ready[i] << endl, kDebugLevel);
    }
    CDCOUT("\t\t------" << endl, kDebugLevel);

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
    }

    #pragma hls_unroll yes
    for (unsigned inp = 0; inp < NumInputs; inp++) {
      unsigned res;
      bool en;
      pseudo_cam(bank_req_winner, bank_rsp, inp, res, en);
      source[inp] = res;
      valid_src[inp] = en;
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
