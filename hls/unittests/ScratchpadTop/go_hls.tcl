# Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

source ../../nvhls_exec.tcl

set CAPACITY_PER_BANK 128
set BANKS 8
set CAPACITY [expr $CAPACITY_PER_BANK*$BANKS]

proc nvhls::usercmd_pre_compile {} {
    # Enable clock gating
    directive set -GATE_REGISTERS {true}
    directive set -GATE_EFFORT {high}
    directive set -GATE_MIN_WIDTH {4}
    directive set -GATE_EXPAND_MIN_WIDTH {4}
}

proc nvhls::usercmd_post_assembly {} {
    global BANKS CAPACITY
    upvar TOP_NAME TOP_NAME

    for {set k 0} {$k < $BANKS} {incr k} {
        directive set /${TOP_NAME}/Scratchpad<data32_t,$BANKS,$CAPACITY>/run/banks.bank.array_impl.data$k:rsc -MAP_TO_MODULE ram_sample-065nm-separate_beh_dc.RAM_separateRW
    }
}

nvhls::run
