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
#ifndef ARBITRATED_SCRATCHPAD_TYPES_H
#define ARBITRATED_SCRATCHPAD_TYPES_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_message.h>
#include <comptrees.h>

// Declare client input and output interfaces as structs
class CLITYPE_T : public nvhls_message {
 public:
  enum { LOAD, STORE };
  sc_uint<1> val;
  static const int width = 1;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& val;
  }
};

template <typename T, unsigned int AddrWidth, unsigned int N>
class cli_req_t : public nvhls_message {
 public:
  CLITYPE_T type;
  bool valids[N];
  NVUINTW(AddrWidth) addr[N];
  T data[N];
  static const unsigned int type_width = Wrapped<T>::width;
  static const int width = 1 + N + N * (AddrWidth + type_width);

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    for(unsigned i=0; i<N; i++) {
      m& data[i];
      m& addr[i];
      m& valids[i];
    }
    m& type;
  }
};

template <typename T, unsigned int N>
class cli_rsp_t : public nvhls_message {
 public:
  bool valids[N];
  T data[N];
  static const unsigned int type_width = Wrapped<T>::width;
  static const int width = N + type_width * N;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    for(unsigned i=0; i<N; i++) {
      m& data[i];
      m& valids[i];
    }
  }
};

#endif
