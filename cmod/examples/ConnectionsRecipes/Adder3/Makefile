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

include ../../../cmod_Makefile

all: sim_adder3

# Location of rapidjson
RAPIDJSON_HOME ?= ../../../../rapidjson

# = instead of += to avoid -DCONN_RAND_STALL
USER_FLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES -DCONNECTIONS_ACCURATE_SIM -DAUTO_PORT=MARSHALL_PORT
run:
	./sim_adder3

sim_adder3: $(wildcard *.h) $(wildcard *.cpp)
	$(CC) -o sim_adder3 $(CFLAGS) $(USER_FLAGS) -I../Adder2 -I../../../include -I${RAPIDJSON_HOME}/include $(wildcard *.cpp) $(BOOSTLIBS) $(LIBS)

sim_clean:
	rm -rf *.o sim_*
