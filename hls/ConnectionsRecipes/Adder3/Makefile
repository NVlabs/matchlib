#
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
#

TOP_NAME      =  Adder2
SRC_PATH      = $(ROOT)/cmod/examples/ConnectionsRecipes
SEARCH_PATH   = $(ROOT)/cmod \
                $(ROOT)/cmod/include \
                $(ROOT)/cmod/examples/ConnectionsRecipes/$(TOP_NAME) \
		$(CONNECTIONS_HOME)/include \
                $(RAPIDJSON_HOME)/include


COMPILER_FLAGS  = SC_INCLUDE_DYNAMIC_PROCESSES CONNECTIONS_ACCURATE_SIM

# Must always use these modes for this test, since it is cycle count
# dependent.
override SIM_MODE=1
override RAND_STALL=0

include ../../../hls/hls_Makefile
