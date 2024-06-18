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


static const char* make_permanent(const char* s) {
#ifdef __SYNTHESIS__
  return s;
#else
  std::string* str = new std::string(s);  // this is an intentional memory leak..
  return str->c_str();
#endif
}

// nv_array_bank_array_no_assert_base is the base class for banked arrays, 
// and typically is not directly used in user models.

template <typename B, size_t C>
class nv_array_bank_array_no_assert_base;

template <typename B>
class nv_array_bank_array_no_assert_base<B, 1>
{
  B a;

public:

  nv_array_bank_array_no_assert_base() : a() {}

  nv_array_bank_array_no_assert_base(const char* prefix) 
   : a(make_permanent(sc_gen_unique_name(prefix)))
  {}

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

  nv_array_bank_array_no_assert_base() : a0(), a1() {}

  nv_array_bank_array_no_assert_base(const char* prefix)
   : a0(make_permanent(sc_gen_unique_name(prefix)))
   , a1(make_permanent(sc_gen_unique_name(prefix)))
  {}

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



/**
 * \brief An implementation of array that declares \a VectorLength variables for
 * array of size \a VectorLength
 * \ingroup nv_array
 *
 * \tparam Type             Datatype
 * \tparam VectorLength      Size of array
 *
 * \par Overview
 * - Declares VectorLength variables to realize an array of size VectorLength
 * - Helpful when HLS tool does not recognize your array correctly and requires
 * unrolling array
 * - nv_array also has specialization for size 0 arrays
 * - Usage of this class is not recommended for new models. Instead,
 *   1) If the array is modeling a RAM/ROM, use a dedicated class for this purpose such as ac_bank_array.
 *   2) If the array is not modeling a RAM/ROM, use a dedicated class for this purpose.
 *    In both cases, the dedicated classes should always assert on invalid indexes.
 *  
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_array.h>
 *
 *      ...
 *      nvhls::nv_array<NVUINT32, ArraySize> array;
 *      for (unsigned i = 0; i < ArraySize; i++) {
 *        array[i] = i;
 *      }
 *
 *
 * \endcode
 * \par
 *
 */
template <typename Type, unsigned int VectorLength>
class nv_array : public nv_array_bank_array_no_assert_base<Type, VectorLength>
{
public:
  typedef nv_array_bank_array_no_assert_base<Type,VectorLength> base_t;
  nv_array() {}
  nv_array(const char* prefix) : base_t(prefix) {}
  nv_array(sc_module_name prefix) : base_t(prefix) {}
  nv_array(sc_module_name prefix, const unsigned int& id) : base_t(prefix) {}
};

};

#endif
