#
# Copyright (c) 2016-2020, NVIDIA CORPORATION.  All rights reserved.
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

USER_FLAGS +=  -Wno-unused-local-typedefs

all: sim_test sim_test_interrupts

include ../../../cmod_Makefile

sim_test: testbench.cpp $(wildcard *.h) $(wildcard $(CWD)/../include/*.h)
	$(CC) -o $@ $(CFLAGS) $(USER_FLAGS) -I../../../include $< $(BOOSTLIBS) $(LIBS)

sim_test_interrupts: testbench_interrupts.cpp $(wildcard *.h) $(wildcard $(CWD)/../include/*.h)
	$(CC) -o $@ $(CFLAGS) $(USER_FLAGS) -I../../../include $< $(BOOSTLIBS) $(LIBS)

run:
	./sim_test
	./sim_test_interrupts

sim_clean:
	rm -rf *.o sim_*
