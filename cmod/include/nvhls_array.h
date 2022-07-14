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

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/static_assert.hpp>

#include <nvhls_marshaller.h>
#include <nvhls_message.h>
#include <nvhls_module.h>

// Set the largest supported array size. Larger values increase compile times.
#ifndef NV_ARRAY_MAX_SPECIALIZATIONS
#define NV_ARRAY_MAX_SPECIALIZATIONS 64
#endif

namespace nvhls {

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
 * - Preprocessor macro NV_ARRAY_MAX_SPECIALIZATIONS sets the maximum array size
 * supported by nv_array. It is small by default to improve compile times.
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
class nv_array {
  BOOST_STATIC_ASSERT_MSG(NV_ARRAY_MAX_SPECIALIZATIONS >= VectorLength,
          "NV_ARRAY_MAX_SPECIALIZATIONS is to small to support the requested array size.");
 public:
  // Alternate array class with no arrays using boost preprocessor to
  // define template secializations.
  template <typename A, unsigned int K>
  class NNode;

#define ACCESSOR(Z, N, TEXT) \
  BOOST_PP_IF(N, else, ) if (idx == N) return data##N;
#define NMINIT(Z, N, TEXT) BOOST_PP_COMMA_IF(N) data##N(nvhls_concat(nm, #N))
#define NMIDINIT(Z, N, TEXT) \
  BOOST_PP_COMMA_IF(N) data##N(nvhls_concat(nm, #N), N)
#define DECL(Z, N, TEXT) TEXT data##N;

#define SPECIALIZATION(Z, N, TEXT)                                         \
  template <typename A>                                                    \
  class NNode<A, N> {                                                      \
   public:                                                                 \
    BOOST_PP_REPEAT(BOOST_PP_ADD(N, 1), DECL, A)                           \
    NNode() {}                                                             \
    NNode(sc_module_name nm)                                               \
        : BOOST_PP_REPEAT(BOOST_PP_ADD(N, 1), NMINIT, BOOST_PP_EMPTY) {}   \
    NNode(sc_module_name nm, const unsigned& id)                           \
        : BOOST_PP_REPEAT(BOOST_PP_ADD(N, 1), NMIDINIT, BOOST_PP_EMPTY) {} \
    A& Get(unsigned int idx) {                                             \
      BOOST_PP_REPEAT(BOOST_PP_ADD(N, 1), ACCESSOR, BOOST_PP_EMPTY)        \
      else return data0;                                                   \
    }                                                                      \
    const A& Get(unsigned int idx) const {                                 \
      BOOST_PP_REPEAT(BOOST_PP_ADD(N, 1), ACCESSOR, BOOST_PP_EMPTY)        \
      else return data0;                                                   \
    }                                                                      \
  };

  BOOST_PP_REPEAT(NV_ARRAY_MAX_SPECIALIZATIONS, SPECIALIZATION, BOOST_PP_EMPTY)

  NNode<Type, VectorLength - 1> array_impl;
  // END WORKAROUND
  // Type array_impl[VectorLength];

  nv_array() {}
  nv_array(const char* prefix) : array_impl(prefix) {}
  nv_array(sc_module_name prefix) : array_impl(prefix) {}
  nv_array(sc_module_name prefix, const unsigned int& id)
      : array_impl(prefix, id) {}
  nv_array(const nv_array<Type, VectorLength>& that) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      array_impl.Get(i) = that.array_impl.Get(i);
  }
  nv_array(const Type newdata[VectorLength]) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      array_impl.Get(i) = newdata[i];
  }
  nv_array<Type, VectorLength>& operator=(
      const nv_array<Type, VectorLength>& that) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      array_impl.Get(i) = that.array_impl.Get(i);
    return *this;
  }
  inline void copy(nv_array<Type, VectorLength>& out) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      out.array_impl.Get(i) = array_impl.Get(i);
  }
  Type& operator[](unsigned int i) {
    // assert(i<VectorLength);
    return this->array_impl.Get(i);
  }
  const Type& operator[](unsigned int i) const {
    // assert(i<VectorLength);
    return this->array_impl.Get(i);
  }
  static const unsigned int width = Wrapped<Type>::width * VectorLength;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    for (unsigned int x = 0; x < VectorLength; x++) {
      m& array_impl.Get(x);
    }
  }

};  // class nv_array
template <typename Type>
class nv_array<Type, 0> : public nvhls_message {
 public:
  nv_array() {}
  nv_array(const char* prefix) {}
  nv_array(sc_module_name prefix) {}
  nv_array(sc_module_name prefix, const unsigned int& id) {}
  nv_array(const nv_array<Type, 0>& that) {}
  nv_array(const Type newdata[0]) {}
  nv_array<Type, 0>& operator=(const nv_array<Type, 0>& that) { return *this; }
  inline void copy(nv_array<Type, 0>& out) {}
  Type& operator[](unsigned int i) {
    static Type dummy;
    return dummy;
  }
  const Type& operator[](unsigned int i) const { return NULL; }
  static const unsigned int width = 0;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {}
};
};

#endif
