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

/*
 * When indicated below in comments, some compute functions in this file have
 * been derived from code in the Algorithmic C Datatypes v3.7.1 distributed
 * with the following license:
 */
/**************************************************************************
 *                                                                        *
 *  Algorithmic C (tm) Datatypes                                          *
 *                                                                        *
 *  Software Version: 3.7                                                 *
 *                                                                        *
 *  Release Date    : Sat Jun 25 13:27:03 PDT 2016                        *
 *  Release Type    : Production Release                                  *
 *  Release Build   : 3.7.1                                               *
 *                                                                        *
 *  Copyright 2004-2016, Mentor Graphics Corporation,                     *
 *                                                                        *
 *  All Rights Reserved.                                                  *
 *
 **************************************************************************
 *  Licensed under the Apache License, Version 2.0 (the "License");       *
 *  you may not use this file except in compliance with the License.      *
 *  You may obtain a copy of the License at                               *
 *                                                                        *
 *      http://www.apache.org/licenses/LICENSE-2.0                        *
 *                                                                        *
 *  Unless required by applicable law or agreed to in writing, software   *
 *  distributed under the License is distributed on an "AS IS" BASIS,     *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or       *
 *  implied.                                                              *
 *  See the License for the specific language governing permissions and   *
 *  limitations under the License.                                        *
 **************************************************************************
 *                                                                        *
 *  The most recent version of this package is available at github.       *
 *                                                                        *
 *************************************************************************/

#ifndef NVHLS_INT_H
#define NVHLS_INT_H

#include <systemc.h>
#include <nvhls_marshaller.h>
#ifdef HLS_CATAPULT
#include <ac_sc.h>
#include <ac_int.h>
#endif

namespace nvhls {

/**************************************************************************
 *  Begin                                                                 *
 *  Algorithmic C (tm) Datatypes                                          *
 *************************************************************************/

// Helper structs for statically computing log2 like functions (nbits,
// log2_floor, log2_ceil) using recursive templates

template <unsigned char N>
struct s_N {
  template <unsigned X>
  struct s_X {
    enum {
      X2 = X >> N,
      N_div_2 = N >> 1,
      nbits = X ? (X2 ? N + (int)s_N<N_div_2>::template s_X<X2>::nbits
                      : (int)s_N<N_div_2>::template s_X<X>::nbits)
                : 0
    };
  };
};
template <>
struct s_N<0> {
  template <unsigned X>
  struct s_X {
    enum { nbits = !!X };
  };
};

// compiler time constant for log2 like functions
/**
 * \brief Compute number of bits to represent a constant
 * \ingroup nvhls_int
 *
 * \tparam X         Must be a constant
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...        
 *      const int N = 8;
 *      const int NumBits = nvhls::nbits<N>::val;
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <unsigned X>
struct nbits {
  enum { val = s_N<16>::s_X<X>::nbits };
};
/**
 * \brief Compute Floor of log2 of a constant
 * \ingroup nvhls_int
 *
 * \tparam X         Must be a constant
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...        
 *      const int N = 8;
 *      const int Log2N_floor = nvhls::log2_floor<N>::val;
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <unsigned X>
struct log2_floor {
  enum { val = nbits<X>::val - 1 };
};

/**
 * \brief log2 of 0 is not defined: generate compiler error
 * \ingroup nvhls_int
 *
 */
template <>
struct log2_floor<0> {};
/**
 * \brief Compute Celing of log2 of a constant
 * \ingroup nvhls_int
 *
 * \tparam X         Must be a constant
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...        
 *      const int N = 8;
 *      const int Log2N_ceil = nvhls::log2_ceil<N>::val;
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <unsigned X>
struct log2_ceil {
  enum { lf = log2_floor<X>::val, val = (X == (1 << lf) ? lf : lf + 1) };
};

/**
 * \brief log2 of 0 is not defined: generate compiler error
 * \ingroup nvhls_int
 *
 */
template <>
struct log2_ceil<0> {};


/**
 * \brief Compute power of 2 value greater than a given value
 * \ingroup nvhls_int
 *
 * \tparam n_val         Must be a constant <= 65536
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...        
 *      const int N = 8;
 *      const int next_pow2 = nvhls::next_pow2<N>::val;
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <int n_val>
struct next_pow2 {
  enum {
    val =
        n_val <= 1
            ? 1
            : n_val <= 2
                  ? 2
                  : n_val <= 4
                        ? 4
                        : n_val <= 8
                              ? 8
                              : n_val <= 16
                                    ? 16
                                    : n_val <= 32
                                          ? 32
                                          : n_val <= 64
                                                ? 64
                                                : n_val <= 128
                                                      ? 128
                                                      : n_val <= 256
                                                            ? 256
                                                            : n_val <= 512
                                                                  ? 512
                                                                  : n_val <=
                                                                            1024
                                                                        ? 1024
                                                                        : n_val <=
                                                                                  2048
                                                                              ? 2048
                                                                              : n_val <=
                                                                                        4096
                                                                                    ? 4096
                                                                                    : n_val <=
                                                                                              8192
                                                                                          ? 8192
                                                                                          : n_val <=
                                                                                                    16384
                                                                                                ? 16384
                                                                                                : n_val <=
                                                                                                          32768
                                                                                                      ? 32768
                                                                                                      : n_val <=
                                                                                                                65536
                                                                                                            ? 65536
                                                                                                            : 1048576
  };
};

/**************************************************************************
 *  End                                                                   *
 *  Algorithmic C (tm) Datatypes                                          *
 *************************************************************************/

/**
 * \brief Compute index width of a constant
 * \ingroup nvhls_int
 *
 * \tparam X         Must be a constant
 *
 * \par Overview
 * - Returns 1 if X==1. otherwise log2_ceil<X>.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...        
 *      const int ArraySize = 8;
 *      const int AddressWidth = nvhls::index_width<ArraySize>::val;
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <unsigned X>
struct index_width {
  enum { val = (X==1)? 1 : log2_ceil<X>::val };
};

// Definition of vendor agnostic data types. We can add more data types to
// nvhls_t struct and move it to a separate file if needed
/**
 * \brief Definition of vendor agnostic integer data types
 * \ingroup nvhls_int
 *
 * \tparam N         Bitwidth of integer
 *
 * \par Overview
 * - Wrapper to unify use of different integer implementation and simplify use of ac_int, sc_int and sc_bigint types
 * - Contains nvuint_t and nvint_t typedefs that conditionally map to ac_int or sc_int types based on CFLAG
 * - Specifying CFLAG HLS_CATAPULT typedefs ac_int to nvint, otherwise typedefs sc_int to nvint 
 * - nvint also supports conditional typedef of sc_int and sc_bigint depending on bitwidth 
 * - Simple macros to declare integers are defined in nvhls_types.h 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *      const GLOBAL_CONST_WIDTH = 5;
 *      ...        
 *      template <int N>
 *      void myfunc() {
 *        nvhls::nvhls_t<5>::nvuint_t var1;  // Unsigned integer of width 5 
 *        nvhls::nvhls_t<3>::nvint_t var2;   // Signed integer of width 3 
 *        NVUINT5 var3;                      // Simplified unsigned integer of width 5 defined using macro
 *        NVINT3 var4;                       // Simplified signed integer of width 3 defined using macro
 *        NVUINTW(N) var5;                   // Simplified unsigned integer where width is template parameter
 *        NVINTW(N) var6;                    // Simplified signed integer where width is template parameter
 *        NVUINTC(GLOBAL_CONST_WIDTH) var7;  // Simplified unsigned integer where width is global constant 
 *        NVINTC(GLOBAL_CONST_WIDTH) var8;   // Simplified signed integer where width is global constant
 *      }
 *      ...
 *
 * \endcode
 * \par
 *
 */

#ifdef HLS_CATAPULT
template <unsigned int N>
struct nvhls_t {
  typedef ac_int<N, true> nvint_t;
  typedef ac_int<N, false> nvuint_t;
};
#else
template <unsigned int N, bool B = (N <= 64)>
struct nvhls_t;

template <unsigned int N>
struct nvhls_t<N, true> {
  typedef sc_int<N> nvint_t;
  typedef sc_uint<N> nvuint_t;
};

template <unsigned int N>
struct nvhls_t<N, false> {
  typedef sc_bigint<N> nvint_t;
  typedef sc_biguint<N> nvuint_t;
};
#endif

/**
 * \brief Function that replaces slice of bits. 
 * \ingroup nvhls_int
 *
 * \tparam type1                Datatype of X
 * \tparam type2                Datatype of Y
 *
 * \param[in]    X              Variable whose bits are to be replaced
 * \param[in]    Y              Variable that stores new bits for the bit slice 
 * \param[in]    i              Starting LSB position in variable X 
 * \param[out]  ReturnType      variable of type1 with slice of bits replaced.


 * \par Overview
 * - Function that replaces slice of bits [i,i+W2-1] in X with Y.
 * - type1 and type2 can be: nvint or nvuint type.
 * - W2 is width of Y.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *
 *      ...
 *      NVUINT8 X = 5;
 *      NVUINT2 Y = 3;
 *      int pos = 3;
 *      X = nvhls::set_slc(X, Y, pos); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <typename type1, typename type2>
type1 set_slc(type1 X, type2 Y, const unsigned int i) {
  type1 X_temp = X;
#ifdef HLS_CATAPULT
  X_temp.set_slc(i, Y);
  return X_temp;
#else
  const unsigned int W2 = Wrapped<type2>::width;
  X_temp.range(i + W2 - 1, i) = Y;
  return X_temp;
#endif
}

/**
 * \brief Function that returns slice of bits. 
 * \ingroup nvhls_int
 *
 * \tparam type                 Datatype
 * \tparam W                    width of slice
 *
 * \param[in]    X              Variable whose bit slice is requested
 * \param[in]    i              Starting LSB position in variable X 
 * \param[out]  ReturnType      Slice of bits 


 * \par Overview
 * - Function that returns slice of bits [i+W-1,i] in X. 
 * - W should be a constant.
 * - type can be: nvint or nvuint type.
 * - Stratus does not work with typename nvhls_t<W>::nvuint_t as return type. Modifying it to scuint<W>. This will be a problem if W>64.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *
 *      ...
 *      NVUINT8 X = 5;
 *      int pos = 3;
 *      NVUINT2 Y = nvhls::get_slc<2>(X, pos) 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <unsigned int W, typename type>
#ifdef HLS_STRATUS
sc_biguint<W> get_slc(type X, const unsigned int i){
#else
typename nvhls_t<W>::nvuint_t get_slc(type X, const unsigned int i) {
#endif
    // Assuming i>=0,
    type X_temp = X;
typename nvhls_t<W>::nvuint_t Z;
#ifdef HLS_CATAPULT
Z = X_temp.template slc<W>(i);
return Z;
#else
  Z = X_temp.range(i + W - 1, i);
  return Z;
#endif
}

/**
 * \brief Function that returns slice of bits with dynamic range. 
 * \ingroup nvhls_int
 *
 * \tparam type                 Datatype
 *
 * \param[in]    X              Variable whose bit slice is requested
 * \param[in]    i              LSB position for bit-slice in variable X 
 * \param[in]    j              MSB position for bit-slice in variable X 
 * \param[out]  ReturnType      Slice of bits 


 * \par Overview
 * - Function that returns slice of bits [i, j] in X. 
 * - i >= j.
 * - type can be: nvint or nvuint type.
 * - SystemC supports this with range function. ac_int does not have equivalent functionality. 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *
 *      ...
 *      NVUINT8 X = 5;
 *      const int pos1 = 5;
 *      const int pos2 = 3;
 *      NVUINT2 Y = nvhls::get_slc(X, pos1, pos2) 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <typename type>
type get_slc(type X, const unsigned int i, const unsigned int j) {
  // Assuming i>=0,j>=0, i>=j
  type X_temp = X;
  type Z;
#ifdef HLS_CATAPULT
  const unsigned int W = type::width;
  Z = (X_temp >> j) & ((((ac_int<W + 1>)1) << (i - j + 1)) - 1);
#else
  Z = X_temp.range(i, j);
#endif
  return Z;
}

// Leading zero detector function
// template <typename type1, typename type2>
// type2 lzd_impl(type1 X) {
//  type1 X_temp = X;
//  type2 Z;
//  const unsigned int W1 = Wrapped<type1>::width;
//  const bool is_signed = Wrapped<type1>::is_signed;
//  int i;
//  if (is_signed) {
//    for (i = W1 - 2; i >= 0; i--) {
//      if (X_temp[i] != 0) break;
//    }
//    Z = W1 - 2 - i;
//  } else {
//    for (i = W1 - 1; i >= 0; i--) {
//      if (X_temp[i] != 0) break;
//    }
//    Z = W1 - 1 - i;
//  }
//  return Z;
//}

/**************************************************************************
 *  Begin                                                                 *
 *  Algorithmic C (tm) Datatypes                                          *
 *************************************************************************/
// Based on MG implementation
/**
 * \brief LeadingOne Detector 
 * \ingroup nvhls_int
 *
 * \tparam type1                 InputDatatype
 * \tparam type2                 ReturnType
 * \tparam W1                    Width of type1
 *
 * \param[in]    X              Variable whose leading one pos is requested
 * \param[out]  ReturnType      Position of leading one 


 * \par Overview
 * - Function that returns position of leading one. 
 * - type can be: nvint or nvuint type.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *
 *      ...
 *      NVUINT8 X = 5;
 *      NVUINT3 pos =
              nvhls::leading_ones<8, NVUINT8,
                                  NVUINT3>(X); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <unsigned int W1, typename type1, typename type2>
inline type2 leading_ones(type1 X) {
  const unsigned int W2 = log2_ceil<W1>::val;
  enum { P2 = next_pow2<(W1 + 1) / 2>::val };

  typename nvhls_t<W1>::nvuint_t X_temp = X;
  enum { Wu = (W1 > P2) ? (W1 - P2) : 1 };
  typename nvhls_t<Wu>::nvuint_t upper = get_slc<Wu>(X_temp, P2);
  typename nvhls_t<P2>::nvuint_t lower = get_slc<P2>(X_temp, 0);
  typename nvhls_t<W2>::nvuint_t idx = 0;

  enum { logWu = (log2_ceil<Wu>::val > 0) ? log2_ceil<Wu>::val : 1 };
  enum { logP2 = (log2_ceil<P2>::val > 0) ? log2_ceil<P2>::val : 1 };
  typename nvhls_t<logWu>::nvuint_t idxu = 0;
  typename nvhls_t<P2>::nvuint_t idxl = 0;
  if (upper != 0) {
    idxu = leading_ones<Wu, typename nvhls_t<Wu>::nvuint_t,
                        typename nvhls_t<logWu>::nvuint_t>(upper);
    idx = idxu | P2;
  } else {
    idxl = leading_ones<P2, typename nvhls_t<P2>::nvuint_t,
                        typename nvhls_t<logP2>::nvuint_t>(lower);
    idx = idxl;
  }
  return idx;
}
/**************************************************************************
 *  End                                                                   *
 *  Algorithmic C (tm) Datatypes                                          *
 *************************************************************************/

template <>
inline nvhls_t<1>::nvuint_t leading_ones<
    1, nvhls_t<1>::nvuint_t, nvhls_t<1>::nvuint_t>(nvhls_t<1>::nvuint_t X) {
  return 0;
}

/**
 * \brief Leading zero detector 
 * \ingroup nvhls_int
 *
 * \tparam type1                 InputDatatype
 *
 * \param[in]    X              Variable whose leading zero position is requested
 * \param[out]  ReturnType      Position of leading one 


 * \par Overview
 * - Function that returns position of leading zero. 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *
 *      ...
 *      NVUINT8 X = 5;
 *      NVUINT3 pos =
              nvhls::lzd<NVUINT8>(X); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <typename type1>
unsigned int lzd(type1 X) {
  unsigned int l;
#ifdef HLS_CATAPULT
  if (X!=0) {
    l = X.leading_sign();
  } else {
    l = Wrapped<type1>::width -1;
  }
  return l;
#else
#if defined(DPOPT)
  HLS_DPOPT_REGION(DPOPT_DEFAULT, "leading_ones", "leading_ones");
  HLS_CONSTRAIN_REGION(0, 1);
#endif
  const unsigned int W1 = Wrapped<type1>::width;
  const unsigned int W2 = log2_ceil<W1>::val;
  typedef typename nvhls_t<W2>::nvuint_t type2;
  const bool is_signed = Wrapped<type1>::is_signed;
  l = leading_ones<W1, type1, type2>(X);
  if (is_signed)
    l = W1 - 2 - l;
  else
    l = W1 - 1 - l;
  return l;
#endif
}

/**
 * \brief Minimum Value of a type 
 * \ingroup nvhls_int
 *
 * \tparam type                 Datatype
 *
 * \param[out]  ReturnVal      Minimum value of the type 


 * \par Overview
 * - Returns minumum value of nvint and nvuint depending on their bitwidth
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *      #include <nvhls_types.h>
 *
 *      ...
 *      NVINT3 min_val =
              nvhls::get_min_val<NVINT3>(); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <typename type>
type get_min_val() {
  const unsigned int W = Wrapped<type>::width;
  const bool is_signed = Wrapped<type>::is_signed;
  type out;
  if (is_signed)
    out = (-((((type)(1)) << (W - 1)) - 1));
  else
    out = 0;
  return out;
}

/**
 * \brief Function that performs left shift while preserving sign 
 * \ingroup nvhls_int
 *
 * \tparam type                 Datatype
 *
 * \param[in]  X               Input variable 
 * \param[in]  shift           Number of bits to be shifted
 * \param[out] ReturnVal       X shifted left while preserving sign


 * \par Overview
 * - Function that performs left shift while preserving sign
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...
 *      NVINT8 X = -5;
 *      NVINT8 ShiftedX =
              nvhls::left_shift<NVINT8>(X, 2); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */
template <typename type>
type left_shift(type X, int shift) {
#ifdef HLS_CATAPULT
  return (X << shift);
#else
  const unsigned int W = Wrapped<type>::width;
  const bool is_signed = Wrapped<type>::is_signed;
  type mag, out;
  if (is_signed) {
    mag = X.range(W - 2, 0);
    // Catapult reports an error for the following line
    // mag = (shift > 0) ? static_cast<type>(mag << shift) :
    // static_cast<type>(mag >> (-shift));
    if (shift > 0)
      mag <<= shift;
    else
      mag >>= (-shift);
    out = mag;
    out[W - 1] = X[W - 1];
  } else {
    mag = X;
    // mag = (shift > 0) ? (mag << shift) : (mag >> (-shift));
    if (shift > 0)
      mag <<= shift;
    else
      mag >>= (-shift);
    out = mag;
  }
  return out;
#endif
}
/**
 * \brief Function that performs right shift while preserving sign 
 * \ingroup nvhls_int
 *
 * \tparam type                 Datatype
 *
 * \param[in]  X               Input variable 
 * \param[in]  shift           Number of bits to be shifted
 * \param[out] ReturnVal       X shifted right while preserving sign


 * \par Overview
 * - Function that performs right shift while preserving sign
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...
 *      NVINT8 X = -5;
 *      NVINT8 ShiftedX =
              nvhls::right_shift<NVINT8>(X, 2); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

template <typename type>
type right_shift(type X, int shift) {
#ifdef HLS_CATAPULT
  return (X >> shift);
#else
  const unsigned int W = Wrapped<type>::width;
  const bool is_signed = Wrapped<type>::is_signed;
  type mag, out;
  if (is_signed) {
    mag = X.range(W - 2, 0);
    // mag = (shift > 0) ? static_cast<type>(mag >> shift) :
    // static_cast<type>(mag << (-shift));
    if (shift > 0)
      mag >>= shift;
    else
      mag <<= (-shift);
    out = mag;
    out[W - 1] = X[W - 1];
  } else {
    mag = X;
    // mag = (shift > 0) ? static_cast<type>(mag << shift) :
    // static_cast<type>(mag >> (-shift));
    if (shift > 0)
      mag >>= shift;
    else
      mag <<= (-shift);
    out = mag;
  }
  return out;
#endif
}

/**
 * \brief Normalize function 
 * \ingroup nvhls_int
 *
 * \tparam type1               MantissaType
 * \tparam type2               ExponentType
 *
 * \param[inout]  X            mantissa
 * \param[inout]  Y            exponent
 * \param[out] ReturnVal       success?


 * \par Overview
 * - Function that takes exponent and mantissa as input and returns normalized mantissa and exponent
 * - Return value indicates success or failure
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_int.h>
 *
 *      ...
 *      NVINT8 X = -5;
 *      NVINT8 Y = 2;
 *      bool is_normalized =
              nvhls::normalize<NVINT8, NVINT8>(X, Y); 
 *      ...           
 *
 * \endcode
 * \par
 *
 */

// Normalize function for unsigned exponent
// X is treated as mantissa and Y is treated as exp
template <typename type1, typename type2>
bool normalize(type1& X, type2& Y) {
  bool b;
#ifdef HLS_CATAPULT
  b = X.normalize(Y);
  return b;
#else
  const unsigned int W1 = Wrapped<type1>::width;
  const unsigned int W2 = Wrapped<type2>::width;
  const bool type2_signed = Wrapped<type2>::is_signed;
  const unsigned int logW1 = log2_ceil<W1>::val;
  typedef typename nvhls_t<logW1>::nvuint_t lzd_type;
  typedef typename nvhls_t<logW1 + 1>::nvint_t signed_lzd_type;
  lzd_type l = lzd(X);
  lzd_type shift;
  type2 min_Y = get_min_val<type2>();
  // If W>logW1, then (Y-l) can be stored in type2 if it is signed or
  // signed_type2 if type2 is unsigned
  if (W2 > logW1) {
    if (type2_signed) {
      type2 Y_l = Y - l;
      if (Y_l < min_Y) {
        shift = Y - min_Y;
        Y = min_Y;
        b = false;
      } else {
        shift = l;
        Y = Y_l;
        b = true;
      }
    } else {
      typedef typename nvhls_t<W2 + 1>::nvint_t signed_type2;
      signed_type2 Y_l = Y - l;
      if (Y_l < ((signed_type2)min_Y)) {
        shift = Y - min_Y;
        Y = min_Y;
        b = false;
      } else {
        shift = l;
        Y = Y_l;
        b = true;
      }
    }
  } else {
    signed_lzd_type Y_l = Y - l;
    if (Y_l < ((signed_lzd_type)min_Y)) {
      shift = Y - min_Y;
      Y = min_Y;
      b = false;
    } else {
      shift = l;
      Y = Y_l;
      b = true;
    }
  }
  if (X == (type1)0) {
    Y = min_Y;
    b = true;
  }
  X = left_shift(X, shift);
  return b;
#endif
}
}
;

#endif
