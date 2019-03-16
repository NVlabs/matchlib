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
#include <mem_array.h>
#include <hls_globals.h>
#include "MemModel.h"
#include "mem_config.h"

// Top-level function used to model mem_array 
// data represent the i/o data line
// mem_op is memory operation type (READ/WRITE)
// addr is the memory address
void MemModel(MemWord_t write_data, MemWord_t & read_data, NVUINT1 mem_op, MemAddr_t addr) {
  static mem_array_sep <MemWord_t, NUM_ENTRIES, NBANKS> banks;
  BankSel_t bank_sel;
  BankAddr_t bank_addr;
  if (NBANKS>1) {
    bank_sel = nvhls::get_slc<NBANKS_LOG2>(addr,0);
    bank_addr = nvhls::get_slc<BANK_ADDR_WIDTH>(addr,NBANKS_LOG2);
  } else {
    bank_sel = 0;
    bank_addr = addr;
  }


  if (mem_op == READ) {
    read_data = banks.read(bank_addr,bank_sel);
    DCOUT("Memory Op: Read, BankAddress: "<< hex << bank_addr.to_uint64() << " BankSel: " << bank_sel << " Data: " << read_data.to_uint64() << endl);
  }
  if (mem_op ==WRITE) {
    MemWord_t data_local = write_data;
    banks.write(bank_addr,bank_sel,data_local);
    DCOUT("Memory Op: Write, BankAddress: "<< hex << bank_addr.to_uint64() << " BankSel: " << bank_sel << " Data: " << write_data.to_uint64() << endl);
  }

}

