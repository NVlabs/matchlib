# Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
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

source ../nvhls_exec.tcl

set PIPE_INIT_INTERVAL 1
set WORDSIZE_LIST {8 }
set BANKS_LIST {1 }
set NUM_ENTRIES_LIST {1024 }

foreach WORDSIZE $WORDSIZE_LIST {
    foreach BANKS $BANKS_LIST {
        foreach NUM_ENTRIES $NUM_ENTRIES_LIST {

            proc nvhls::set_compiler_flags {HLS_CATAPULT COMPILER_FLAGS} {
                global WORDSIZE BANKS NUM_ENTRIES
                if { $HLS_CATAPULT eq "1" } {
                    set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
                } else {
                    set HLS_CATAPULT_FLAG ""
                }
                set FLAG_STR ""
                foreach flag $COMPILER_FLAGS {
                    append FLAG_STR "-D$flag "
                }
                puts "WordSize=$WORDSIZE Banks=$BANKS NumEntries=$NUM_ENTRIES"
                options set Input/CompilerFlags "-D_SYNTHESIS_ $HLS_CATAPULT_FLAG $FLAG_STR -DWORDSIZE=$WORDSIZE -DNBANKS=$BANKS -DNUM_ENTRIES=$NUM_ENTRIES"
            }

            proc nvhls::usercmd_post_assembly {} {
                global BANKS PIPE_INIT_INTERVAL
                upvar TOP_NAME TOP_NAME
                for {set k 0} {$k < $BANKS} {incr k} {
                    directive set /$TOP_NAME/core/banks.bank.array_impl.data$k:rsc/MAP_TO_MODULE ram_sample-065nm-singleport_beh_dc.RAM
                }
                directive set /$TOP_NAME/core/main -PIPELINE_INIT_INTERVAL $PIPE_INIT_INTERVAL
            }
            
            proc nvhls::usercmd_post_architect {} {
                global BANKS
                upvar TOP_NAME TOP_NAME
                for {set k 0} {$k < $BANKS} {incr k} {
                    ignore_memory_precedences -from /$TOP_NAME/core/core:rlp/main/banks.read:for:read_mem(banks.bank.array_impl.data$k:rsc.@) -to /$TOP_NAME/core/core:rlp/main/banks.write:if:for:if:write_mem(banks.bank.array_impl.data$k:rsc.@)
                    ignore_memory_precedences -from /$TOP_NAME/core/core:rlp/main/banks.write:if:for:if:write_mem(banks.bank.array_impl.data$k:rsc.@) -to /$TOP_NAME/core/core:rlp/main/banks.read:for:read_mem(banks.bank.array_impl.data$k:rsc.@)
                }
            }

            nvhls::run
        }
    }
}
