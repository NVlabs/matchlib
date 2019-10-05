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

set PACKETIDWIDTH 1


set VC_LIST {1}
set BUFF_LIST {4 }
set NUM_LPORTS_LIST {1 }
set NUM_RPORTS_LIST {2 }
set FLIT_DATAWIDTH_LIST {64}

foreach VC $VC_LIST {
    foreach BUFFSIZE $BUFF_LIST {
        foreach NUM_LPORTS $NUM_LPORTS_LIST {
            foreach NUM_RPORTS $NUM_RPORTS_LIST {
                foreach FLIT_DATAWIDTH $FLIT_DATAWIDTH_LIST {

                    proc nvhls::set_compiler_flags {HLS_CATAPULT COMPILER_FLAGS} {
                        global VC BUFFSIZE NUM_LPORTS NUM_RPORTS FLIT_DATAWIDTH
                        if { $HLS_CATAPULT eq "1" } {
                            set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
                        } else {
                            set HLS_CATAPULT_FLAG ""
                        }
                        set FLAG_STR ""
                        foreach flag $COMPILER_FLAGS {
                            append FLAG_STR "-D$flag "
                        }
                        options set Input/CompilerFlags "-D_SYNTHESIS_ $HLS_CATAPULT_FLAG $FLAG_STR -DNUM_LPORTS=$NUM_LPORTS -DNUM_RPORTS=$NUM_RPORTS -DFLIT_DATAWIDTH=$FLIT_DATAWIDTH -DNUM_VCHANNELS=$VC -DBUFFERSIZE=$BUFFSIZE"
                    }

                    proc nvhls::usercmd_post_assembly {} {
                        global NUM_LPORTS NUM_RPORTS VC BUFFERSIZE
                        upvar TOP_NAME TOP_NAME
                        set DESIGN_TOP "/$TOP_NAME/core"

                        directive set /WHVCRouterTop/WHVCSourceRouter<$NUM_LPORTS,$NUM_RPORTS,$VC,$BUFFSIZE,WHVCRouterTop::Flit_t,16>/process/while -PIPELINE_INIT_INTERVAL 1
                    }

                    nvhls::run
                }
            }
        }
    }
}
