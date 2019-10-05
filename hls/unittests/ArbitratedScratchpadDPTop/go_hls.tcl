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

proc nvhls::usercmd_post_assembly {} {
    upvar TOP_NAME TOP_NAME
    set DESIGN_TOP "/$TOP_NAME/core"
    set NUM_BANKS 4
    set MAIN_PIPELINE_II 1

    for {set k 0} {$k < $NUM_BANKS} {incr k} {
        directive set ${DESIGN_TOP}/scratchpad_inst.banks.bank.array_impl.data$k:rsc -MAP_TO_MODULE ram_sample-065nm-separate_beh_dc.RAM_separateRW
    }
    directive set ${DESIGN_TOP}/main -PIPELINE_INIT_INTERVAL ${MAIN_PIPELINE_II}
}

nvhls::run
