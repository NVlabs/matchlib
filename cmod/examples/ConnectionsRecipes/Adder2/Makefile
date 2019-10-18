#
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
#

include ../../../cmod_Makefile

all: sim_adder2

USER_FLAGS += -DCONN_RAND_STALL_PRINT_DEBUG
run:
	./sim_adder2

sim_adder2: $(wildcard *.h) $(wildcard *.cpp)
	$(CC) -o sim_adder2 $(CFLAGS) $(USER_FLAGS) -I../../../include $(wildcard *.cpp) $(BOOSTLIBS) $(LIBS)

sim_clean:
	rm -rf *.o sim_*

