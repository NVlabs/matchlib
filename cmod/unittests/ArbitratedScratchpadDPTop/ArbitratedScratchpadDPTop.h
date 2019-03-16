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

#ifndef __ARBITRATEDSCRATCHPADDP_TOP__
#define __ARBITRATEDSCRATCHPADDP_TOP__

#include <ArbitratedScratchpadDP.h>


#ifndef NUM_READ_PORTS
#define NUM_READ_PORTS 4
#endif

#ifndef NUM_WRITE_PORTS
#define NUM_WRITE_PORTS 4
#endif


#ifndef NUM_BANKS
#define NUM_BANKS 4
#endif

#ifndef NUM_ENTRIES_PER_BANK
#define NUM_ENTRIES_PER_BANK 64
#endif

#ifndef DATA_TYPE
#define DATA_TYPE NVUINT32
#endif

const unsigned int kNumBanks = NUM_BANKS;
const unsigned int kNumReadPorts = NUM_READ_PORTS;
const unsigned int kNumWritePorts = NUM_WRITE_PORTS;
const unsigned int kEntriesPerBank = NUM_ENTRIES_PER_BANK;
const unsigned int kAddressSize = nvhls::index_width<NUM_BANKS * NUM_ENTRIES_PER_BANK>::val;
typedef NVUINTW(kAddressSize) Address;

void ArbitratedScratchpadDPTop(Address read_address[NUM_READ_PORTS], bool read_req_valid[NUM_READ_PORTS],
                    Address write_address[NUM_WRITE_PORTS], bool write_req_valid[NUM_WRITE_PORTS],
                    DATA_TYPE write_data[NUM_WRITE_PORTS], 
                    bool read_ack[NUM_READ_PORTS], bool write_ack[NUM_WRITE_PORTS], 
		    DATA_TYPE port_read_out[NUM_READ_PORTS], bool port_read_out_valid[NUM_READ_PORTS]);

#endif
