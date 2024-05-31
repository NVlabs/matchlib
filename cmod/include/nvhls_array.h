/*
 * Copyright (c) 2016-2020, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef NVHLS_ARRAY
#define NVHLS_ARRAY


#include <cstddef>
#include <ac_assert.h>
#include "systemc.h"

namespace nvhls {

template <size_t W>
struct nv_array_pow2;

template <>
struct nv_array_pow2<1>
{
  static const size_t P = 1;
};

template <size_t W>
struct nv_array_pow2
{
  typedef nv_array_pow2<(W>>1)> SUB;
  static const size_t P = SUB::P << 1;
};



// nv_array_bank_array_no_assert_base is the base class for banked arrays, 
// and typically is not directly used in user models.
// The nv_array_bank* classes are temporary: eventually these will be deleted
// and we will include these classes from a header file in the Catapult installation.

template <typename B, size_t C>
class nv_array_bank_array_no_assert_base;

template <typename B>
class nv_array_bank_array_no_assert_base<B, 1>
{
  B a;
public:
  B &operator[](size_t idx) { return a; }
  const B &operator[](size_t idx) const { return a; }
};


template <typename B, size_t C>
class nv_array_bank_array_no_assert_base
{
  static const size_t W = nv_array_pow2<C-1>::P;
  nv_array_bank_array_no_assert_base<B, W  > a0;
  nv_array_bank_array_no_assert_base<B, C-W> a1;
public:
  B &operator[](size_t idx) { 
#ifndef __SYNTHESIS__
    assert(idx < C);
#endif
    size_t aidx = idx & (W-1); return idx&W ? a1[aidx] : a0[aidx]; 
  }

  const B &operator[](size_t idx) const {
#ifndef __SYNTHESIS__
    assert(idx < C);
#endif
    size_t aidx = idx & (W-1); return idx&W ? a1[aidx] : a0[aidx]; 
  }
};



template <typename Type, unsigned int VectorLength>
class nv_array : public nv_array_bank_array_no_assert_base<Type, VectorLength>
{
public:
  nv_array() {}
  nv_array(const char* prefix) {}
  nv_array(sc_module_name prefix) {}
  nv_array(sc_module_name prefix, const unsigned int& id) {}
};

};

#endif
