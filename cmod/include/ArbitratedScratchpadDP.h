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

#ifndef __ARBITRATEDSCRATCHPADDP__
#define __ARBITRATEDSCRATCHPADDP__

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_assert.h>
#include <nvhls_message.h>
#include <arbitrated_crossbar.h>
#include <crossbar.h>
#include <hls_globals.h>

/**
 * \brief ArbitratedScratchpad with dual port support 
 * \ingroup ArbitratedScratchpad
 *
 * \tparam kNumBanks        Number of Banks 
 * \tparam kNumReadPorts    Number of read ports 
 * \tparam kNumWritePorts   Number of write ports 
 * \tparam kEntriesPerBank  Number of entries per bank 
 * \tparam WordType         WordType of entry in memory 
 * \tparam isSF             Is Store-Forward enabled for simultaneous read and write to same address 
 * \tparam IsSPRAM          Is memory mapped to single-port RAM. Either read or write is allowed per cycle 
 *
 * \par A Simple Example
 * \code
 *      #include <ArbitratedScratchpadDP.h>
 *
 *      ...
 *      ArbitratedScratchpadDP<kNumBanks, kNumReadPorts, kNumWritePorts, kEntriesPerBank, WordType, false, false> scratchpad;
 *      scratchpad.run(read_address, read_address_valid,
 *                  write_address, write_address_valid,
 *                  write_data, read_ack, write_ack, read_ready,
 *                  read_data_out, read_data_out_valid);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template<unsigned int kNumBanks, unsigned int kNumReadPorts,unsigned int
kNumWritePorts, unsigned int kEntriesPerBank, typename WordType, bool isSF=true, bool IsSPRAM=false>
class ArbitratedScratchpadDP {

  static const unsigned int kReadPortIndexSize = nvhls::index_width<kNumReadPorts>::val;
  static const unsigned int kWritePortIndexSize = nvhls::index_width<kNumWritePorts>::val;
  static const unsigned int kBankIndexSize = nvhls::index_width<kNumBanks>::val;
  static const unsigned int kLocalIndexSize = nvhls::index_width<kEntriesPerBank>::val;
  typedef NVUINTW(kReadPortIndexSize) ReadPortIndex;
  typedef NVUINTW(kWritePortIndexSize) WritePortIndex;
  typedef NVUINTW(kBankIndexSize) BankIndex;
  typedef NVUINTW(kLocalIndexSize) LocalIndex;
  static const unsigned int kAddressSize = nvhls::index_width<kNumBanks * kEntriesPerBank>::val;
  typedef NVUINTW(kAddressSize) Address;
  typedef NVUINTW(kNumReadPorts) ReadPortBitVector;
  typedef NVUINTW(kNumWritePorts) WritePortBitVector;
  typedef bool Ack;


  class bankwrite_req_t : public nvhls_message {
   public:
    LocalIndex localindex;
    WordType data;
    static const int width = WordType::width + LocalIndex::width;
    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& localindex;
      m& data;
    }
  };

  class bankread_req_t : public nvhls_message {
   public:
    LocalIndex localindex;
    static const int width = LocalIndex::width;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& localindex;
    }
  };
  
  class bankread_rsp_t : public nvhls_message {
   public:
    bool     valid;
    WordType rdata;
    static const int width = 1 + WordType::width;

    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m& rdata;
      m& valid;
    }
  };

  // Local state
  mem_array_sep <WordType, kNumBanks * kEntriesPerBank, kNumBanks> banks;
  ArbitratedCrossbar<bankread_req_t, kNumReadPorts, kNumBanks, 0, 0> read_arbxbar;
  ArbitratedCrossbar<bankwrite_req_t, kNumWritePorts, kNumBanks, 0, 0> write_arbxbar;

  void compute_bankread_request(Address read_address[kNumReadPorts], bool read_req_valid[kNumReadPorts], 
                            bankread_req_t bankread_req[kNumReadPorts],
                            BankIndex bankread_sel[kNumReadPorts],
                            bool bankread_req_valid[kNumReadPorts]) {

    // For each request, compute the bank request fields
    #pragma hls_unroll yes
    for (unsigned in_chan = 0; in_chan < kNumReadPorts; in_chan++) {
      bankread_sel[in_chan] = GetBankIndex(read_address[in_chan]);
      bankread_req[in_chan].localindex = GetLocalIndex(read_address[in_chan]);
      bankread_req_valid[in_chan] = (read_req_valid[in_chan] == true);
    }
  }

  void compute_bankwrite_request(Address write_address[kNumWritePorts], bool write_req_valid[kNumWritePorts], 
                            WordType write_data[kNumWritePorts],  
                            bankwrite_req_t bankwrite_req[kNumWritePorts],
                            BankIndex bankwrite_sel[kNumWritePorts],
                            bool bankwrite_req_valid[kNumWritePorts]) {

    // For each request, compute the bank request fields
    #pragma hls_unroll yes
    for (unsigned in_chan = 0; in_chan < kNumWritePorts; in_chan++) {
      bankwrite_sel[in_chan] = GetBankIndex(write_address[in_chan]);
      bankwrite_req[in_chan].localindex = GetLocalIndex(write_address[in_chan]);
      bankwrite_req[in_chan].data = write_data[in_chan];
      bankwrite_req_valid[in_chan] = (write_req_valid[in_chan] == true);
    }
  }


  void banks_load_store(bankread_req_t bankread_req[kNumBanks], 
                        bool bankread_req_valid[kNumBanks],
                        bankwrite_req_t bankwrite_req[kNumBanks],
                        bool bankwrite_req_valid[kNumBanks],
                        bankread_rsp_t bankread_rsp[kNumBanks],
                        NVUINTW(kEntriesPerBank) valid_entry[kNumBanks]
                    ) {
    #pragma hls_unroll yes
    for (unsigned bank = 0; bank < kNumBanks; bank++) {
      if (IsSPRAM == false) {
        if (bankread_req_valid[bank] == true) {
          bankread_rsp[bank].valid = true;
          // Check for data forwarding
          if (isSF && (bankwrite_req_valid[bank] == true) && (bankread_req[bank].localindex == bankwrite_req[bank].localindex)) {
            bankread_rsp[bank].rdata = bankwrite_req[bank].data;
          } else if (valid_entry[bank][bankread_req[bank].localindex]) {
            bankread_rsp[bank].rdata = banks.read(bankread_req[bank].localindex, bank);
          } else {
            bankread_rsp[bank].rdata = 0; 
          }
        } else {
          bankread_rsp[bank].rdata = 0; 
          bankread_rsp[bank].valid = false;
        } 
        if (bankwrite_req_valid[bank] == true) {
          banks.write(bankwrite_req[bank].localindex, bank, bankwrite_req[bank].data);
        }
      } else {
        //if (bankread_req_valid[bank] == true) {
        //  bankread_rsp[bank].valid = true;
        //  // Check for data forwarding
        //  if ((bankwrite_req_valid[bank] == true) && (bankread_req[bank].localindex == bankwrite_req[bank].localindex)) {
        //    bankread_rsp[bank].rdata = bankwrite_req[bank].data;
        //  } else {
        //    if (valid_entry[bank][bankread_req[bank].localindex]) {
        //      bankread_rsp[bank].rdata = banks.read(bankread_req[bank].localindex, bank);
        //    }
        //  }
        //} else {
        //  bankread_rsp[bank].rdata = 0; 
        //  bankread_rsp[bank].valid = false;
        //  if (bankwrite_req_valid[bank] == true) {
        //    banks.write(bankwrite_req[bank].localindex, bank, bankwrite_req[bank].data);
        //  }
        //}

        if (bankwrite_req_valid[bank] == true) {
          banks.write(bankwrite_req[bank].localindex, bank, bankwrite_req[bank].data);
          NVHLS_ASSERT_MSG(bankread_req_valid[bank] == false, "Bank_read_and_write_valid_cannot_be_true_simultaneously_for_single-port_RAM");
          bankread_rsp[bank].rdata = 0;
          bankread_rsp[bank].valid = false;
        } else {
          if (bankread_req_valid[bank] == true) {
            bankread_rsp[bank].valid = true;
            if (valid_entry[bank][bankread_req[bank].localindex]) {
              bankread_rsp[bank].rdata = banks.read(bankread_req[bank].localindex, bank);
            } 
            else {
            // waived code from coverage review
            #ifdef COV_ENABLE
               "CTC SKIP";
            #endif
            
              bankread_rsp[bank].rdata = 0; 
            
            #ifdef COV_ENABLE
               "CTC ENDSKIP";
            #endif
            }
          }
        }
      } 
    }
  }

  public:

  // Helper Functions
  BankIndex GetBankIndex(Address a) {
    if (kNumBanks == 1) {
      return 0;
    } else {
      return nvhls::get_slc(a, kBankIndexSize - 1, 0);
    }
  }

  LocalIndex GetLocalIndex(Address a) {
    if (kNumBanks == 1) {
      return a;
    } else {
      return nvhls::get_slc(a, kAddressSize - 1, kBankIndexSize);
    }
  }
  
  void clear() {
   // waived code from coverage review
   #ifdef COV_ENABLE
     "CTC SKIP";
   #endif
    banks.reset();
   #ifdef COV_ENABLE
     "CTC ENDSKIP";
   #endif
  }

  void run(Address read_address[kNumReadPorts], bool read_req_valid[kNumReadPorts],
                    Address write_address[kNumWritePorts], bool write_req_valid[kNumWritePorts], 
                    WordType write_data[kNumWritePorts], 
                    Ack read_ack[kNumReadPorts], Ack write_ack[kNumWritePorts],
                    bool read_ready[kNumReadPorts], 
                    WordType port_read_out[kNumReadPorts], bool port_read_out_valid[kNumReadPorts] 
) {
    NVUINTW(kEntriesPerBank) valid_entry[kNumBanks];
    #pragma hls_unroll yes
    for (unsigned i = 0; i < kNumBanks; i++) {
      valid_entry[i] = ~0;
    }
    run(read_address, read_req_valid,
                    write_address, write_req_valid, 
                    write_data, 
                    read_ack, write_ack,
                    read_ready, 
                    port_read_out, port_read_out_valid, valid_entry); 

}
  void run(Address read_address[kNumReadPorts], bool read_req_valid[kNumReadPorts],
                    Address write_address[kNumWritePorts], bool write_req_valid[kNumWritePorts], 
                    WordType write_data[kNumWritePorts], 
                    Ack read_ack[kNumReadPorts], Ack write_ack[kNumWritePorts],
                    bool read_ready[kNumReadPorts], 
                    WordType port_read_out[kNumReadPorts], bool port_read_out_valid[kNumReadPorts],
                    NVUINTW(kEntriesPerBank) valid_entry[kNumBanks]  
) {

    bankread_req_t bankread_req[kNumReadPorts];
    BankIndex bankread_sel[kNumReadPorts];
    bool bankread_req_valid[kNumReadPorts];
    compute_bankread_request(read_address, read_req_valid, bankread_req, bankread_sel,
                            bankread_req_valid);
    
    bankwrite_req_t bankwrite_req[kNumWritePorts];
    BankIndex bankwrite_sel[kNumWritePorts];
    bool bankwrite_req_valid[kNumWritePorts];
    compute_bankwrite_request(write_address, write_req_valid, write_data, 
                            bankwrite_req, bankwrite_sel,
                            bankwrite_req_valid);

    bool read_done[kNumReadPorts];
    bankread_req_t bankread_req_winner[kNumBanks];
    bool bankread_req_winner_valid[kNumBanks];
    ReadPortIndex read_source[kNumBanks];
    read_arbxbar.run(bankread_req, bankread_sel, bankread_req_valid, bankread_req_winner,
                     bankread_req_winner_valid, read_done, read_source);

    #pragma unroll yes
    for (unsigned int i=0; i < kNumReadPorts; i++) {
      read_ack[i] = read_done[i] && read_req_valid[i];
      read_ready[i] = read_done[i] || (!read_req_valid[i]);
    }



    bankwrite_req_t bankwrite_req_winner[kNumBanks];
    bool bankwrite_req_winner_valid[kNumBanks];
    bool write_ready[kNumWritePorts];
    WritePortIndex write_source[kNumBanks];
    write_arbxbar.run(bankwrite_req, bankwrite_sel, bankwrite_req_valid, bankwrite_req_winner,
                     bankwrite_req_winner_valid, write_ready, write_source);

    #pragma unroll yes
    for (unsigned int i=0; i < kNumWritePorts; i++)
      write_ack[i] = write_ready[i] && write_req_valid[i];
    if (IsSPRAM) {
    #pragma unroll yes
    for (unsigned bank = 0; bank < kNumBanks; bank++) {
      if (bankread_req_winner_valid[bank] && bankwrite_req_winner_valid[bank]) {
        bankread_req_winner_valid[bank] = false;
        read_ack[read_source[bank]] = false;
      }
    }
    }
    bankread_rsp_t bankread_rsp[kNumBanks];
    banks_load_store(bankread_req_winner, 
                     bankread_req_winner_valid,
                     bankwrite_req_winner,
                     bankwrite_req_winner_valid,
                     bankread_rsp, valid_entry); 

    // Prepare the inputs for response crossbar
    WordType   bank_read_out[kNumBanks];
    bool       bank_read_out_valid[kNumBanks];
    BankIndex bank_read_data_source[kNumReadPorts];
    #pragma hls_unroll yes
    for (unsigned bank = 0; bank < kNumBanks; bank++) {
      bank_read_out_valid[bank] = bankread_rsp[bank].valid;
      bank_read_out[bank]  = bankread_rsp[bank].rdata;
    }
    #pragma hls_unroll yes
    for (unsigned i = 0; i < kNumReadPorts; i++) {
      bank_read_data_source[i] = bankread_sel[i];
    }
    // response crossbar traversal
    WordType port_read_out_local[kNumReadPorts];
    bool port_read_out_valid_local[kNumReadPorts];
    crossbar<WordType, kNumBanks, kNumReadPorts>(bank_read_out, bank_read_out_valid, bank_read_data_source, read_ack,
                                            port_read_out_local, port_read_out_valid_local);
    #pragma hls_unroll yes
    for (unsigned i = 0; i < kNumReadPorts; i++) {
      port_read_out_valid[i] = port_read_out_valid_local[i];
    }
    #pragma hls_unroll yes
    for (unsigned i = 0; i < kNumReadPorts; i++) {
      if (port_read_out_valid[i]) {
        port_read_out[i] = port_read_out_local[i];
      }
    }
  }

};


#endif
