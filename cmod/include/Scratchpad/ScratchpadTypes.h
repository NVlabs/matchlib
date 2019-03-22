/*
 * Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCRATCHPAD_TYPES_H
#define SCRATCHPAD_TYPES_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_message.h>
#include <comptrees.h>


// Define enums for request type
static const unsigned int kScratchpadOpcodeSize = 1;
enum ScratchpadOpcode {LOAD, STORE};
MarshallEnum(ScratchpadOpcode, kScratchpadOpcodeSize);

template <typename T, unsigned int AddrWidth, unsigned int N>
class cli_req_t : public nvhls_message 
{
 public:
  static const unsigned int type_width = Wrapped<T>::width;
  static const unsigned int width = 1 + N + N * (AddrWidth + type_width);
  
  ScratchpadOpcode opcode;
  sc_lv<N> valids;
  NVUINTW(AddrWidth) addr [N];
  T data [N];

  template<unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m & opcode;
    m & valids;
    #pragma hls_unroll yes
    for (unsigned int i=0; i<N; i++) m & addr[i];
    #pragma hls_unroll yes
    for (unsigned int i=0; i<N; i++) m & data[i];
  }
};


template <typename T, unsigned int N>
class cli_rsp_t : public nvhls_message 
{
public:
  sc_lv<N> valids;
  T data [N];
  static const unsigned int type_width = Wrapped<T>::width;
  static const unsigned int width = N + type_width * N;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m & valids;
    #pragma hls_unroll yes
    for (unsigned int i=0; i<N; i++) m & data[i];
  }
  
};

#endif
