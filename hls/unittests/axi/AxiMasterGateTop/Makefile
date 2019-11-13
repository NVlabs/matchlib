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

ROOT            := ../../../..
SRC_PATH				:= $(ROOT)/cmod/unittests/axi

include $(ROOT)/hls/hls_Makefile

COMPILER_FLAGS += HLS_ALGORITHMICC

# The tetstbench is not very sophisticated, so certain random seeds
# will cause reads to overtake writes.
COMPILER_FLAGS += RAND_SEED=1000
