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

source $env(ROOT)/hls/run_hls_global_setup.tcl

namespace eval nvhls {
    proc run {} {

        # Get all input variables set from Makefile
        global env
        set USER_VARS {TOP_NAME CLK_PERIOD SRC_PATH SEARCH_PATH HLS_CATAPULT RUN_SCVERIFY COMPILER_FLAGS}

        echo "***USER SETTINGS***"
        foreach var $USER_VARS {
            if [info exists env($var)] {
                echo "$var = $env($var)"
                set $var $env($var)
            } else { 
                echo "Warning: $var not set by user"
                set $var ""
            }
        }
        
        options set Input/SearchPath ". $SEARCH_PATH"

        set_input_files $SRC_PATH $TOP_NAME
        set_compiler_flags $HLS_CATAPULT $COMPILER_FLAGS
        go analyze
        setup_libs
        setup_clocks $CLK_PERIOD
        setup_hier $TOP_NAME
        go compile
        go libraries
        go assembly
        usercmd_post_assembly
        go architect
        usercmd_post_architect
        go allocate
        go schedule
        go dpfsm
        go extract
        
        if { $RUN_SCVERIFY eq "1" } { go switching }

        project save

        exit
    }
  
    proc set_input_files {SRC_PATH TOP_NAME} {
        set DESIGN_FILES [list $SRC_PATH/$TOP_NAME/$TOP_NAME.h]
        set TB_FILES [list $SRC_PATH/$TOP_NAME/testbench.cpp]
        
        foreach design_file $DESIGN_FILES {
            solution file add $design_file -type SYSTEMC
        }
        foreach tb_file $TB_FILES {
            solution file add $tb_file -type SYSTEMC -exclude true
        }
    }

    proc set_compiler_flags {HLS_CATAPULT COMPILER_FLAGS} {
        if { $HLS_CATAPULT eq "1" } {
            set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
        } else {
            set HLS_CATAPULT_FLAG ""
        }
        set FLAG_STR ""
        foreach flag $COMPILER_FLAGS {
            append FLAG_STR "-D$flag "
        }
        options set Input/CompilerFlags "-D_SYNTHESIS_ $HLS_CATAPULT_FLAG $FLAG_STR"
    }

    proc setup_libs {} {
        solution library add mgc_sample-065nm-dw_beh_dc -- -rtlsyntool DesignCompiler -vendor Sample -technology 065nm -Designware Yes
        solution library add ram_sample-065nm-singleport_beh_dc
    }

    proc setup_clocks {period} {
        set name clk
        set CLK_PERIODby2 [expr $period/2]
        directive set -CLOCKS "$name \"-CLOCK_PERIOD $period -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME $CLK_PERIODby2 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND sync -RESET_SYNC_ACTIVE high -RESET_ASYNC_ACTIVE low -ENABLE_NAME {} -ENABLE_ACTIVE high\"    "
        directive set -CLOCK_NAME $name
    }
    
    proc setup_hier {TOP_NAME} {
        directive set -DESIGN_HIERARCHY "$TOP_NAME"
    } 

    proc usercmd_post_assembly {} {}
    proc usercmd_post_architect {} {}

}
