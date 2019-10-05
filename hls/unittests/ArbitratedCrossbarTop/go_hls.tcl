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
set LEN_INPUT_BUFFER_LIST {8 }
set LEN_OUTPUT_BUFFER_LIST {0 }
set NUM_INPUTS_LIST {4 }
set NUM_OUTPUTS_LIST {4 }

foreach LEN_INPUT_BUFFER $LEN_INPUT_BUFFER_LIST {
    foreach LEN_OUTPUT_BUFFER $LEN_OUTPUT_BUFFER_LIST {
        foreach NUM_INPUTS $NUM_INPUTS_LIST {
            foreach NUM_OUTPUTS $NUM_OUTPUTS_LIST {

                proc nvhls::set_compiler_flags {HLS_CATAPULT COMPILER_FLAGS} {
                    global LEN_INPUT_BUFFER LEN_OUTPUT_BUFFER NUM_INPUTS NUM_OUTPUTS
                    if { $HLS_CATAPULT eq "1" } {
                        set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
                    } else {
                        set HLS_CATAPULT_FLAG ""
                    }
                    set FLAG_STR ""
                    foreach flag $COMPILER_FLAGS {
                        append FLAG_STR "-D$flag "
                    }
                    puts "##### LEN_INPUT_BUFFER=$LEN_INPUT_BUFFER  LEN_OUTPUT_BUFFER=$LEN_OUTPUT_BUFFER  NUM_INPUTS=$NUM_INPUTS  NUM_OUTPUTS=$NUM_OUTPUTS #####"
                    options set Input/CompilerFlags "-D_SYNTHESIS_ $HLS_CATAPULT_FLAG $FLAG_STR -DNUM_INPUTS=$NUM_INPUTS -DNUM_OUTPUTS=$NUM_OUTPUTS -DLEN_INPUT_BUFFER=$LEN_INPUT_BUFFER -DLEN_OUTPUT_BUFFER=$LEN_OUTPUT_BUFFER"
                }

                proc nvhls::usercmd_post_assembly {} {
                    global MAIN_PIPELINE_II
                    upvar TOP_NAME TOP_NAME
                    set DESIGN_TOP "/$TOP_NAME/core"
                    directive set ${DESIGN_TOP}/main -PIPELINE_INIT_INTERVAL ${MAIN_PIPELINE_II}
                    # TODO - memory mapping
                }
                
                nvhls::run
            }
        }
    }
}
