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

#include "MemModel.h"
#include "mem_config.h"

#ifndef NUM_ITER
#define NUM_ITER 1000
#endif

MemWord_t wideRand ();

CCS_MAIN(int argc, char *argv[]) { 

  nvhls::set_random_seed();

  const int count = NUM_ITER;
  int start = 0;


  MemAddr_t address[count];
  MemWord_t ref[NUM_ENTRIES];
  MemWord_t write_data;
  MemWord_t read_data;

  /* Do a write and a read of zero first to clear out some x's */
  MemOp_t mem_op = WRITE;
  MemAddr_t addr = 0;
  write_data = 0;
  ref[addr] = write_data;
  CCS_DESIGN(MemModel)(write_data, read_data, mem_op, addr);
  mem_op = READ;
  CCS_DESIGN(MemModel)(write_data, read_data, mem_op, addr);


  for (int i = start; i< start+count; i++) {
    MemOp_t mem_op = WRITE;
    MemAddr_t addr = rand() % NUM_ENTRIES;
    write_data = wideRand();
    //MemAddr_t addr = i;
    address[i] = addr;
    ref[addr] = write_data;
    DCOUT("Memory Op: Write, Address: " << hex << addr.to_uint64() << " Data: " << write_data.to_uint64() << endl);
    CCS_DESIGN(MemModel)(write_data, read_data, mem_op, addr);
  }

  for (int i = start; i< start+count; i++) {
    MemOp_t mem_op = READ;
    MemAddr_t addr = address[i];
    CCS_DESIGN(MemModel)(write_data, read_data, mem_op, addr);
    DCOUT("Memory Op: Read, Address: "<< addr.to_uint64() << " Data: "<< read_data.to_uint64() << " " << ref[addr] << endl);
    assert(ref[addr]==read_data);
  }
  CCS_RETURN(0) ;

}

MemWord_t wideRand () {
  MemWord_t retval;
  if (WORDSIZE<=32) {
    retval = rand();
  } else {
    NVUINTC(32) tmp;
    for (int bitpos=0; bitpos<WORDSIZE; bitpos+=32) {
      tmp = rand();
      retval = nvhls::set_slc(retval, tmp, bitpos);
    }
  }
  return retval;
}
