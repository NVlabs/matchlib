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
#ifndef SC_SCRATCHPAD_TOP_H
#define SC_SCRATCHPAD_TOP_H

#include <Scratchpad.h>
#include "ScratchpadConfig.h"

//Wrapper module for scratchpad. Wrapper module is required to get Catapult SCVERIFY working
SC_MODULE (ScratchpadTop) {
 public:
  sc_in_clk clk ;      // Clock input of the design
  sc_in <bool> rst ;      // active high, synchronous Reset input
  static const int N = SCRATCHPAD_BANKS;
  static const int CAPACITY_IN_BYTES = SCRATCHPAD_CAPACITY ;
  static const int ADDR_WIDTH = SCRATCHPAD_ADDR_WIDTH;
  Connections::In< cli_req_t<data32_t, ADDR_WIDTH,N> > cli_req;
  Connections::Out< cli_rsp_t<data32_t, N> > cli_rsp;
  Scratchpad<data32_t, SCRATCHPAD_BANKS,SCRATCHPAD_CAPACITY> myscratchpad;

  SC_HAS_PROCESS(ScratchpadTop);
  ScratchpadTop(sc_module_name name) : sc_module(name),
    clk("clk"),
    rst("rst"),
    cli_req("cli_req"),
    cli_rsp("cli_rsp"),
    myscratchpad("myscratchpad")
  {
    // Connect the DUT
    myscratchpad.clk(clk);
    myscratchpad.rst(rst);
    myscratchpad.cli_req(cli_req);
    myscratchpad.cli_rsp(cli_rsp);
  }

};

#endif
