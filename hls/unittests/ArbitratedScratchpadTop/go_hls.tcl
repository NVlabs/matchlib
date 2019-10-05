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

set MAIN_PIPELINE_II 1
set DATA_TYPE_LIST {"NVUINT32" }
set NUM_BANK_ENTRIES_LIST {256 }
set LEN_INPUT_BUFFER_LIST {8 }
set NUM_BANKS_LIST {2 }
set NUM_INPUTS_LIST {4 }

foreach DATA_TYPE $DATA_TYPE_LIST {
    foreach NUM_BANK_ENTRIES $NUM_BANK_ENTRIES_LIST {
        foreach LEN_INPUT_BUFFER $LEN_INPUT_BUFFER_LIST {
            foreach NUM_BANKS $NUM_BANKS_LIST {
                foreach NUM_INPUTS $NUM_INPUTS_LIST {

                    proc nvhls::set_compiler_flags {HLS_CATAPULT COMPILER_FLAGS} {
                        global DATA_TYPE NUM_BANK_ENTRIES LEN_INPUT_BUFFER NUM_BANKS NUM_INPUTS
                        if { $HLS_CATAPULT eq "1" } {
                            set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
                        } else {
                            set HLS_CATAPULT_FLAG ""
                        }
                        set FLAG_STR ""
                        foreach flag $COMPILER_FLAGS {
                            append FLAG_STR "-D$flag "
                        }
                        puts "##### DATA_TYPE=$DATA_TYPE  NUM_BANK_ENTRIES=$NUM_BANK_ENTRIES  LEN_INPUT_BUFFER=$LEN_INPUT_BUFFER  NUM_BANKS=$NUM_BANKS  NUM_INPUTS=$NUM_INPUTS #####"
                        options set Input/CompilerFlags "-D_SYNTHESIS_ $HLS_CATAPULT_FLAG $FLAG_STR -DDATA_TYPE=$DATA_TYPE -DNUM_BANK_ENTRIES=$NUM_BANK_ENTRIES -DLEN_INPUT_BUFFER=$LEN_INPUT_BUFFER -DNUM_BANKS=$NUM_BANKS -DNUM_INPUTS=$NUM_INPUTS"
                    }

                    proc nvhls::usercmd_post_assembly {} {
                        global MAIN_PIPELINE_II NUM_BANKS
                        upvar TOP_NAME TOP_NAME
                        set DESIGN_TOP "/$TOP_NAME/core"
                        directive set ${DESIGN_TOP}/main -PIPELINE_INIT_INTERVAL ${MAIN_PIPELINE_II}

                        directive set -REGISTER_THRESHOLD 2048
                        directive set -MEM_MAP_THRESHOLD  2048
                        for {set k 0} {$k < $NUM_BANKS} {incr k} {
                            directive set ${DESIGN_TOP}/dut.banks.bank.array_impl.data$k:rsc -MAP_TO_MODULE ram_sample-065nm-singleport_beh_dc.RAM
                        }
                    }
                    
                    nvhls::run
                }
            }
        }
    }
}
