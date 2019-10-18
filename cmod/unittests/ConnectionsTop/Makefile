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

include ../../cmod_Makefile

ifeq ($(SIM_MODE),0)
all: sim_combinational sim_bypass sim_buffer sim_pipeline sim_multchain sim_network sim_credit sim_serdes sim_comb_buff sim_comb_chan
endif

ifeq ($(SIM_MODE),1)
all: sim_combinational sim_bypass sim_buffer sim_pipeline sim_multchain sim_comb_buff sim_comb_chan
endif

ifeq ($(SIM_MODE),2)
all: sim_combinational sim_comb_buff sim_comb_chan
endif

ifeq ($(SIM_MODE),0)
run:
	./sim_combinational
	./sim_bypass
	./sim_buffer
	./sim_pipeline
	./sim_multchain
	./sim_network
	./sim_credit
	./sim_serdes
	./sim_comb_buff
	./sim_comb_chan
endif

ifeq ($(SIM_MODE),1)
run:
	./sim_combinational
	./sim_bypass
	./sim_buffer
	./sim_pipeline
	./sim_multchain
#	./sim_network
#	./sim_credit
#	./sim_serdes
	./sim_comb_buff
	./sim_comb_chan
endif

ifeq ($(SIM_MODE),2)
run:
	./sim_combinational
#	./sim_bypass
#	./sim_buffer
#	./sim_pipeline
#	./sim_multchain
#	./sim_network
#	./sim_credit
#	./sim_serdes
	./sim_comb_buff
	./sim_comb_chan
endif


sim_combinational: $(wildcard *.h) TestCombinational.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_combinational $(CFLAGS) $(USER_FLAGS) -I../../include TestCombinational.cpp $(BOOSTLIBS) $(LIBS)

sim_bypass: $(wildcard *.h) TestBypass.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_bypass $(CFLAGS) $(USER_FLAGS) -I../../include TestBypass.cpp $(BOOSTLIBS) $(LIBS)

sim_buffer: $(wildcard *.h) TestBuffer.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_buffer $(CFLAGS) $(USER_FLAGS) -I../../include TestBuffer.cpp $(BOOSTLIBS) $(LIBS)

sim_pipeline: $(wildcard *.h) TestPipeline.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_pipeline $(CFLAGS) $(USER_FLAGS) -I../../include TestPipeline.cpp $(BOOSTLIBS) $(LIBS)

sim_multchain: $(wildcard *.h) TestMultChain.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_multchain $(CFLAGS) $(USER_FLAGS) -I../../include TestMultChain.cpp $(BOOSTLIBS) $(LIBS)

sim_network: $(wildcard *.h) TestNetwork.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_network $(CFLAGS) $(USER_FLAGS) -I../../include TestNetwork.cpp $(BOOSTLIBS) $(LIBS)

sim_credit: $(wildcard *.h) TestNetworkCredit.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_credit $(CFLAGS) $(USER_FLAGS) -I../../include TestNetworkCredit.cpp $(BOOSTLIBS) $(LIBS)

sim_serdes: $(wildcard *.h) TestSerdesNetwork.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_serdes $(CFLAGS) $(USER_FLAGS) -I../../include TestSerdesNetwork.cpp $(BOOSTLIBS) $(LIBS)

sim_comb_buff: $(wildcard *.h) TestCombinationalBufferedEnds.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_comb_buff $(CFLAGS) $(USER_FLAGS) -I../../include TestCombinationalBufferedEnds.cpp $(BOOSTLIBS) $(LIBS)

sim_comb_chan: $(wildcard *.h) TestCombinationalIntoChan.cpp $(wildcard ../../include/*.h) $(wildcard ../../include/*.h)
	$(CC) -o sim_comb_chan $(CFLAGS) $(USER_FLAGS) -I../../include TestCombinationalIntoChan.cpp $(BOOSTLIBS) $(LIBS)

sim_clean:
	rm -rf *.o sim_*
