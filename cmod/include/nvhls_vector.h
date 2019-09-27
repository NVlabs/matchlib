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
#ifndef NVHLS_VECTOR_H
#define NVHLS_VECTOR_H

#include <systemc.h>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <hls_globals.h>
#include <nvhls_array.h>
#include <ccs_p2p.h>
#include <nvhls_assert.h>
#include <nvhls_message.h>

namespace nvhls {

inline std::string synth_to_string(const unsigned int& n) {
#ifndef __SYNTHESIS__
  std::ostringstream stm;
  stm << n;
  return stm.str();
#else
  return "DUMMY";
#endif
}

// SystemC vector class
// TYPE can be one of the SystemC types - sc_int, sc_uint, sc_bigint, sc_biguint
// or AC type ac_int
// TYPE cannot be nv_scvector i.e. we cannot have vector of vectors .. Need to
// add operators for vectors
/**
 * \brief Vector helper container with vector operations 
 * \ingroup nvhls_vector
 *
 * \tparam Type             ScalarType 
 * \tparam VectorLength     Length of vector 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1;
 *      Type tmp;
 *      tmp = v1[3];                         // Access an entry in vector
 *      ...
 *      NVUINT16 v1_raw = v1.to_rawbits();   // Convert vector to integer bitstream
 *      nv_scvector<NVUINT2, 8> v2 = v1_raw; // Initialize vector with integer bitstream
 *      v1 = v2;                             // Assign vectors    
 *      #ifdef DEBUG 
 *      cout << v1 << " " << v2 << endl;     // Print vectors 
 *      #endif
 *      ...
 *      if (v1 == v2) {                      // Check if 2 vectors are equal
 *      ...
 *      }
 * \endcode
 * \par
 *
 */

template <typename Type, unsigned int VectorLength>
class nv_scvector : public nvhls_message {
 public:
  Type data[VectorLength];
  typedef Type type;
  static const unsigned int type_width = Wrapped<Type>::width;
  static const unsigned int length = VectorLength;
  static const unsigned int width = type_width * VectorLength;
  static const bool is_signed = Wrapped<Type>::is_signed;

  nv_scvector() {
  }
  nv_scvector(const nv_scvector<Type, VectorLength>& that) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      data[i] = that.data[i];
  }
  nv_scvector(const Type newdata[VectorLength]) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      data[i] = newdata[i];
  }
  // Constructing vector from rawbits of NVUINT type
  nv_scvector(const NVUINTW(width) & rawbits) {
#pragma hls_unroll yes
    for (unsigned int i = 0; i < VectorLength; i++) {
      data[i] = nvhls::get_slc<type_width>(rawbits, i * type_width);
    }
  }
  nv_scvector(const int& rawbits) {
    const NVUINTW(width) & rawbits_local = rawbits;
#pragma hls_unroll yes
    for (unsigned int i = 0; i < VectorLength; i++) {
      data[i] = nvhls::get_slc<type_width>(rawbits_local, i * type_width);
    }
  }
  nv_scvector<Type, VectorLength>& operator=(
      const nv_scvector<Type, VectorLength>& that) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      data[i] = that.data[i];
    return *this;
  }

  inline void copy(nv_scvector<Type, VectorLength>& out) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      out.data[i] = data[i];
  }
  Type& operator[](unsigned int i) {
    return this->data[i];
  }
  const Type& operator[](unsigned int i) const {
    return this->data[i];
  }
  // Converting vector to rawbits of NVUINT type
  NVUINTW(width) to_rawbits() {
    NVUINTW(width) rawbits = 0;
#pragma hls_unroll yes
    for (unsigned int i = 0; i < VectorLength; i++) {
      rawbits = nvhls::set_slc(rawbits, data[i], i * type_width);
    }
    return rawbits;
  }
  // Converting rawbits to vector
  void to_vector(NVUINTW(width) rawbits) {
#pragma hls_unroll yes
    for (unsigned int i = 0; i < VectorLength; i++) {
      data[i] = nvhls::get_slc<type_width>(rawbits, i * type_width);
    }
  }

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    #pragma hls_unroll yes
    for (unsigned int i = 0; i < VectorLength; i++) {
      m& data[i];
    }
  }
};

template <typename Type, unsigned int VectorLength>
inline bool operator==(
    const nv_scvector<Type, VectorLength>& lhs,
    const nv_scvector<Type, VectorLength>& rhs) {
    bool is_equal = true;
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      is_equal &= (lhs.data[i] == rhs.data[i]);
    return is_equal;
  }

template <typename Type, unsigned int VectorLength>
inline std::ostream& operator<<(std::ostream& os,
                                const nv_scvector<Type, VectorLength>& vec) {
  for (unsigned i = 0; i < VectorLength; i++) {
    os << vec.data[i] << " ";
  }
  return os;
}

/**
 * \brief Function implementing vector multiplication 
 * \ingroup nvhls_vector
 *
 * \tparam InType1          Input1 Scalar Type 
 * \tparam InType2          Input2 Scalar Type 
 * \tparam OutType          Output Scalar Type 
 * \tparam VectorLength     Length of vector 
 * \tparam Unroll           Template parameter to control unrolling 
 *
 * \par Overview
 * This function multiplies two input vectors of InType1 and InType2 type and
 * produces output of type OutType. Length of all vectors is VectorLength.
 * If InType1, InType2, OutType are user-defined types, then OutType =
 * InType1*InType2 should be defined by the user using operator overloading

 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1, v2;
 *      nv_scvector<NVUINT4, 8> out;
 *      ...
 *      nvhls::vector_mul<NVUINT2, NVUINT2, NVUINT4, 8, true>(v1,v2,out);
 *      ...
 * \endcode
 * \par
 *
 */

template <typename InType1, typename InType2, typename OutType,
          unsigned int VectorLength, bool Unroll>
void vector_mul(nv_scvector<InType1, VectorLength> in1,
                nv_scvector<InType2, VectorLength> in2,
                nv_scvector<OutType, VectorLength>& out) {

  if (Unroll == true) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] * in2[i];
  } else {
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] * in2[i];
  }
}


/**
 * \brief Function implementing vector addition 
 * \ingroup nvhls_vector
 *
 * \tparam InType1          Input1 Scalar Type 
 * \tparam InType2          Input2 Scalar Type 
 * \tparam OutType          Output Scalar Type 
 * \tparam VectorLength     Length of vector 
 * \tparam Unroll           Template parameter to control unrolling 
 *
 * \par Overview
 * This function adds two input vectors of InType1 and InType2 type and
 * produces output of type OutType. Length of all vectors is VectorLength.
 * If InType1, InType2, OutType are user-defined types, then OutType =
 * InType1+InType2 should be defined by the user using operator overloading

 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1, v2;
 *      nv_scvector<NVUINT3, 8> out;
 *      ...
 *      nvhls::vector_add<NVUINT2, NVUINT2, NVUINT3, 8, true>(v1,v2,out);
 *      ...
 * \endcode
 * \par
 *
 */

template <typename InType1, typename InType2, typename OutType,
          unsigned int VectorLength, bool Unroll>
void vector_add(nv_scvector<InType1, VectorLength> in1,
                nv_scvector<InType2, VectorLength> in2,
                nv_scvector<OutType, VectorLength>& out) {
  if (Unroll == true) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] + in2[i];
  } else {
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] + in2[i];
  }
}


/**
 * \brief Function implementing vector subtraction 
 * \ingroup nvhls_vector
 *
 * \tparam InType1          Input1 Scalar Type 
 * \tparam InType2          Input2 Scalar Type 
 * \tparam OutType          Output Scalar Type 
 * \tparam VectorLength     Length of vector 
 * \tparam Unroll           Template parameter to control unrolling 
 *
 * \par Overview
 * This function adds two input vectors of InType1 and InType2 type and
 * produces output of type OutType. Length of all vectors is VectorLength.
 * If InType1, InType2, OutType are user-defined types, then OutType =
 * InType1-InType2 should be defined by the user using operator overloading

 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1, v2;
 *      nv_scvector<NVINT3, 8> out;
 *      ...
 *      nvhls::vector_sub<NVUINT2, NVUINT2, NVINT3, 8, true>(v1,v2,out);
 *      ...
 * \endcode
 * \par
 *
 */
template <typename InType1, typename InType2, typename OutType,
          unsigned int VectorLength, bool Unroll>
void vector_sub(nv_scvector<InType1, VectorLength> in1,
                nv_scvector<InType2, VectorLength> in2,
                nv_scvector<OutType, VectorLength>& out) {
  if (Unroll == true) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] - in2[i];
  } else {
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] - in2[i];
  }
}


/**
 * \brief Function implementing vector reduction 
 * \ingroup nvhls_vector
 *
 * \tparam InType           Input Scalar Type 
 * \tparam OutType          Output Scalar Type 
 * \tparam VectorLength     Length of vector 
 * \tparam UseReduceTree    Template parameter to control datapath optimization by HLS tool 
 *
 * \par Overview
 * This function performs redcution of input vector of type InType and
 * produces scalar output of type OutType. Length of all vectors is VectorLength.
 * If InType and OutType, OutType are user-defined types, then 
 * OutType += InType, OutType = InType should be defined by the user using operator overloading

 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1;
 *      NVUINT5 out;
 *      ...
 *      nvhls::reduction<NVUINT2, NVUINT5, 8, true>(v1,out);
 *      ...
 * \endcode
 * \par
 *
 */

template <typename InType, typename OutType, unsigned int VectorLength,
          bool UseReduceTree>
void reduction(nv_scvector<InType, VectorLength> in, OutType& out) {
  OutType sum = 0;

  if (VectorLength > 1) {
    if (UseReduceTree == true) {
#pragma hls_unroll yes
#pragma cluster addtree
#pragma cluster_type both
      for (unsigned i = 0; i < VectorLength; i++)
        sum += in[i];
    } else {
      for (unsigned i = 0; i < VectorLength; i++)
        sum += in[i];
    }
  } else {
    sum = in[0];
  }
  out = sum;
}


/**
 * \brief Function implementing vector dot-product 
 * \ingroup nvhls_vector
 *
 * \tparam InType1          Input1 Scalar Type 
 * \tparam InType2          Input2 Scalar Type 
 * \tparam OutType          Output Scalar Type 
 * \tparam VectorLength     Length of vector 
 * \tparam UseReduceTree    Template parameter to control datapath optimization by HLS tool 
 *
 * \par Overview
 * This function performs dot-product of 2 input vectors of "InType1" and
 * "InType2" types and produces output of type "OutType".
 * Input vector length is "VectorLength".
 * If InType1, InType2, and OutType are user-defined types, then OutType +=
 * InType1*InType2, OutType = InType1*InType2 operations should be defined by
 * the user

 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1, v2;
 *      NVUINT7 out;
 *      ...
 *      nvhls::dp<NVUINT2, NVUINT2, NVUINT7, 8, true>(v1, v2, out);
 *      ...
 * \endcode
 * \par
 *
 */

template <typename InType1, typename InType2, typename OutType,
          unsigned int VectorLength, bool UseReduceTree>
void dp(nv_scvector<InType1, VectorLength> in1,
        nv_scvector<InType2, VectorLength> in2, OutType& out) {
  OutType sum = 0;
  if (VectorLength > 1) {
    if (UseReduceTree == true) {
#pragma hls_unroll yes
#pragma cluster multadd addtree
#pragma cluster_type both
      for (unsigned i = 0; i < VectorLength; i++)
        sum += in1[i] * in2[i];
    } else {
      for (unsigned i = 0; i < VectorLength; i++)
        sum += in1[i] * in2[i];
    }
  } else {
    sum = in1[0] * in2[0];
  }

  out = sum;
}

/**
 * \brief Function implementing vector multiply and add 
 * \ingroup nvhls_vector
 *
 * \tparam InType1          Input1 Scalar Type 
 * \tparam InType2          Input2 Scalar Type 
 * \tparam InType3          Input3 Scalar Type 
 * \tparam OutType          Output Scalar Type 
 * \tparam VectorLength     Length of vector 
 * \tparam UseReduceTree    Template parameter to control datapath optimization by HLS tool 
 *
 * \par Overview
 * This function multiplies two input vectors of InType1 and InType2 type and
 * adds another vector of type InType3 to produce output of type OutType.
 * Length of all vectors is VectorLength.  If InType1, InType2, OutType are
 * user-defined types, then OutType =
 * InType1*InType2 + InType3 should be defined by the user using operator overloading

 * \par A Simple Example
 * \code
 *      #include <nvhls_vector.h>
 *
 *      ...
 *      nv_scvector<NVUINT2, 8> v1, v2, v3;
 *      nv_scvector<NVUINT8, 8> out;
 *      ...
 *      nvhls::vector_mac<NVUINT2, NVUINT2, NVUINT2, NVUINT8, 8, true>(v1, v2, v3, out);
 *      ...
 * \endcode
 * \par
 *
 */

template <typename InType1, typename InType2, typename InType3, typename OutType,
          unsigned int VectorLength, bool Unroll>
void vector_mac(nv_scvector<InType1, VectorLength> in1,
                nv_scvector<InType2, VectorLength> in2,
                nv_scvector<InType3, VectorLength> in3,
                nv_scvector<OutType, VectorLength>& out) {

  if (Unroll == true) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] * in2[i] + in3[i];
  } else {
    for (unsigned i = 0; i < VectorLength; i++)
      out[i] = in1[i] * in2[i] + in3[i];
  }
}


// Function implementing vector dot product followed by accumulate Template
// parameters: InType1, InType2,InType3 OutType are the data types for
// inputs and output. This function performs dot-product of 2 input vectors
// of "InType1" and "InType2" types, adds the results to 3rd scalar input of
//"InType3" type and produces scalar output of type "OutType".  Vector
// length of first 2 inputs is "VectorLength".  If InType1, InType2, InType3
// and OutType are user-defined types, then OutType = InType3, OutType +=
// InType1*InType2 operations should be defined by the user
template <typename InType1, typename InType2, typename InType3,
          typename OutType, unsigned int VectorLength, bool UseReduceTree>
void dpacc(nv_scvector<InType1, VectorLength> in1,
           nv_scvector<InType2, VectorLength> in2, InType3 in3, OutType& out) {
  OutType sum = in3;
  if (UseReduceTree == true) {
#pragma hls_unroll yes
#pragma cluster addtree
#pragma cluster_type both
    for (unsigned i = 0; i < VectorLength; i++)
      sum += in1[i] * in2[i];
  } else {
    for (unsigned i = 0; i < VectorLength; i++)
      sum += in1[i] * in2[i];
  }
  out = sum;
}
};

#endif
