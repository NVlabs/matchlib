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

#include <ArbitratedScratchpadDP.h>
#include "ArbitratedScratchpadDPTop.h"

void ArbitratedScratchpadDPTop(Address read_address[NUM_READ_PORTS], bool read_req_valid[NUM_READ_PORTS],
                    Address write_address[NUM_WRITE_PORTS], bool write_req_valid[NUM_WRITE_PORTS],
                    DATA_TYPE write_data[NUM_WRITE_PORTS], 
                    bool read_ack[NUM_READ_PORTS], bool write_ack[NUM_WRITE_PORTS], 
                    DATA_TYPE port_read_out[NUM_READ_PORTS], bool port_read_out_valid[NUM_READ_PORTS]) {
    typedef ArbitratedScratchpadDP<NUM_BANKS, NUM_READ_PORTS, NUM_WRITE_PORTS, NUM_ENTRIES_PER_BANK, DATA_TYPE> scratchpad_t;
    static scratchpad_t scratchpad_inst;
    bool read_ready[NUM_READ_PORTS];
    scratchpad_inst.run(read_address, read_req_valid,
                        write_address, write_req_valid, write_data,
                        read_ack, write_ack, read_ready,
                        port_read_out, port_read_out_valid);
 
}
