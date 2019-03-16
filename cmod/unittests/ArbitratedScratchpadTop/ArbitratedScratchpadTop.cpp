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

#include <systemc.h>
#include <ArbitratedScratchpad.h>
#include "ArbitratedScratchpadConfig.h"
#include "ArbitratedScratchpadTop.h"

void ArbitratedScratchpadTop(tb_cli_req_t& curr_cli_req,
                             tb_cli_rsp_t& curr_cli_rsp,
                             bool ready[NumInputs]) {
  // Instantiate DUT and reset it
  static ArbitratedScratchpad<DataType, ScratchpadCapacity, NumInputs, NumBanks, InputQueueLength> dut;
  tb_cli_req_t curr_cli_req_local = curr_cli_req;
  tb_cli_rsp_t curr_cli_rsp_local;
  bool ready_local[NumInputs];
  // Store to DUT

  #ifdef DEBUG_OUTPUT
  cout << "load_store" << endl;
  cout << "curr_cli_req_local: " << "type=" << curr_cli_req_local.type.val << endl;
  for(unsigned i =0; i<NumInputs; ++i) {
    cout << i << " : valid=" << curr_cli_req_local.valids[i] << " addr=" << curr_cli_req_local.addr[i] << " data=" << curr_cli_req_local.data[i] << endl;
  }
  cout << "-----" << endl;
  #endif
  dut.load_store(curr_cli_req_local, curr_cli_rsp_local, ready_local);

  #pragma hls_unroll yes
  for(unsigned i = 0; i < NumInputs; i++) {
    ready[i] = ready_local[i];
  }
  curr_cli_rsp = curr_cli_rsp_local;

  #ifdef DEBUG_OUTPUT
  cout << " curr_cli_rsp : " << endl;
  for (unsigned i=0; i<NumInputs; ++i) {
    cout << i << " : ready=" << ready[i] << " valid=" << curr_cli_rsp.valids[i] << " data=" << curr_cli_rsp.data[i] << endl;
  }
  cout << "-----" << endl;
  cout << "OUTBOUND curr_cli_req_local: " << "type=" << curr_cli_req_local.type.val << endl;
  for(unsigned i =0; i<NumInputs; ++i) {
    cout << i << " : valid=" << curr_cli_req_local.valids[i] << " addr=" << curr_cli_req_local.addr[i] << " data=" << curr_cli_req_local.data[i] << endl;
  }
  cout << "-----" << endl;
  #endif
}
