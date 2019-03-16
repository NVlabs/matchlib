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

source ../../run_hls_global_setup.tcl
options set Input/SearchPath ". ../../../cmod ../../../cmod/include"

#global variables across all steps
set TOP_NAME "Adder"
set CLK_NAME clk
set CLK_PERIOD 2

set DESIGN_FILES [list ../../../cmod/examples/ConnectionsRecipes/$TOP_NAME/$TOP_NAME.h]
set TB_FILES [list ../../../cmod/examples/ConnectionsRecipes/$TOP_NAME/testbench.cpp]

if { [info exists env(HLS_CATAPULT)] && ($env(HLS_CATAPULT) eq "1") } {
  set HLS_CATAPULT_FLAG "-DHLS_CATAPULT"
} else {
  set HLS_CATAPULT_FLAG ""
}

# Add your design here
foreach design_file $DESIGN_FILES {
	solution file add $design_file -type SYSTEMC
}
foreach tb_file $TB_FILES {
	solution file add $tb_file -type SYSTEMC -exclude true
}
options set Input/CompilerFlags "-DSC_INCLUDE_DYNAMIC_PROCESSES -DCONNECTIONS_ACCURATE_SIM $HLS_CATAPULT_FLAG"
go analyze
solution library add mgc_sample-065nm-dw_beh_dc -- -rtlsyntool DesignCompiler -vendor Sample -technology 065nm -Designware Yes
solution library add ram_sample-065nm-singleport_beh_dc

# Clock, interface constrain
set CLK_PERIODby2 [expr $CLK_PERIOD/2]
directive set -CLOCKS "$CLK_NAME \"-CLOCK_PERIOD $CLK_PERIOD -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME $CLK_PERIODby2 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND sync -RESET_SYNC_ACTIVE high -RESET_ASYNC_ACTIVE low -ENABLE_NAME {} -ENABLE_ACTIVE high\"    "
directive set -CLOCK_NAME $CLK_NAME

directive set -DESIGN_HIERARCHY "$TOP_NAME"
go compile
go libraries
go assembly

directive set /$TOP_NAME/run/while -PIPELINE_INIT_INTERVAL 1 
directive set /$TOP_NAME/run/while -PIPELINE_STALL_MODE flush

go architect
go allocate
go schedule
go dpfsm
go extract
go switching
project save

exit
