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

#ifndef ARBITRATED_SCRATCHPAD_TOP_H
#define ARBITRATED_SCRATCHPAD_TOP_H

#include <systemc.h>
#include <ArbitratedScratchpad.h>
#include "ArbitratedScratchpadConfig.h"

void ArbitratedScratchpadTop(tb_cli_req_t& curr_cli_req,
                             tb_cli_rsp_t& curr_cli_rsp,
                             bool ready[NumInputs]);

#endif  // end ARBITRATED_SCRATCHPAD_TOP_H
