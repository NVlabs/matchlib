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

# Must always use these modes for this test, since it is cycle count
# dependent.
override SIM_MODE=1
override RAND_STALL=0

include ../../../cmod_Makefile

all: sim_adder3

run:
	./sim_adder3

sim_adder3: $(wildcard *.h) $(wildcard *.cpp)
	$(CC) -o sim_adder3 $(CFLAGS) $(USER_FLAGS) -I../Adder2 -I../../../include $(wildcard *.cpp) $(BOOSTLIBS) $(LIBS)

sim_clean:
	rm -rf *.o sim_*

