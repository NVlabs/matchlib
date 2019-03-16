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

include ../unittests_Makefile

sim_test1: $(wildcard *.h) $(wildcard *.cpp) $(wildcard $(CWD)/../include/*.h)
	$(CC) -o sim_test1 -DNUM_INPUTS=5 -DARBITER_TYPE=Roundrobin $(CFLAGS) $(USER_FLAGS) -I$(CWD)/../include $(wildcard *.cpp) $(BOOSTLIBS) $(LIBS)

sim_test2: $(wildcard *.h) $(wildcard *.cpp) $(wildcard $(CWD)/../include/*.h)
	$(CC) -o sim_test2 -DNUM_INPUTS=1 -DARBITER_TYPE=Roundrobin $(CFLAGS) $(USER_FLAGS) -I$(CWD)/../include $(wildcard *.cpp) $(BOOSTLIBS) $(LIBS)

sim_test3: $(wildcard *.h) $(wildcard *.cpp) $(wildcard $(CWD)/../include/*.h)
	$(CC) -o sim_test3 -DNUM_INPUTS=5 -DARBITER_TYPE=Static $(CFLAGS) $(USER_FLAGS) -I$(CWD)/../include $(wildcard *.cpp) $(BOOSTLIBS) $(LIBS)

run1:
	./sim_test1
run2:
	./sim_test2
run3:
	./sim_test3

cov1:
	make cov COV_XML=coverage1.xml MAKE_TARGET="sim_test1 run1"
cov2:
	make cov COV_XML=coverage2.xml MAKE_TARGET="sim_test2 run2"
cov3:
	make cov COV_XML=coverage3.xml MAKE_TARGET="sim_test3 run3"

merge_cov: CC = $(CTC) $(CC_)
merge_cov: cov1 cov2 cov3
	ctcxmlmerge			\
		coverage1.xml coverage2.xml coverage3.xml		\
		-x coverage_merged.xml	\
		-p coverage_profile.txt
	ctc2html			\
		-i coverage_profile.txt	\
		-o web-report

