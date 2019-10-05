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

set NUM_INPUTS_LIST {4 }
set NUM_OUTPUTS_LIST {8 }
set DATATYPE_LIST {"NVINT32" }

foreach NUM_INPUTS $NUM_INPUTS_LIST {
    foreach NUM_OUTPUTS $NUM_OUTPUTS_LIST {
        foreach DATATYPE $DATATYPE_LIST {

            proc nvhls::set_compiler_flags {HLS_CATAPULT COMPILER_FLAGS} {
                global NUM_INPUTS NUM_OUTPUTS DATATYPE
                if { $HLS_CATAPULT eq "1" } {
                    set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
                } else {
                    set HLS_CATAPULT_FLAG ""
                }
                set FLAG_STR ""
                foreach flag $COMPILER_FLAGS {
                    append FLAG_STR "-D$flag "
                }
                puts "########## NumInputLanes=$NUM_INPUTS NumOutputLanes=$NUM_OUTPUTS DataType=$DATATYPE ##########"
                options set Input/CompilerFlags "-D_SYNTHESIS_ $HLS_CATAPULT_FLAG $FLAG_STR -DNUM_INPUTS=$NUM_INPUTS -DNUM_OUTPUTS=$NUM_OUTPUTS -DDATA_TYPE=$DATATYPE"
            }

            proc nvhls::usercmd_post_assembly {} {
                upvar TOP_NAME TOP_NAME
                directive set /$TOP_NAME/core/main -PIPELINE_INIT_INTERVAL 1
                directive set /$TOP_NAME/data_in:rsc -MAP_TO_MODULE mgc_ioport.mgc_in_wire
                directive set /$TOP_NAME/source:rsc -MAP_TO_MODULE mgc_ioport.mgc_in_wire
                directive set /$TOP_NAME/data_out:rsc -MAP_TO_MODULE mgc_ioport.mgc_out_reg
            }
            
            nvhls::run
        }
    }
}

