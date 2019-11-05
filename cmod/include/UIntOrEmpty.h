/*
 * Copyright (c) 2017-2019, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef _UINTOREMPTY_H_
#define _UINTOREMPTY_H_

#include <systemc>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_assert.h>
#include <nvhls_message.h>

///////////////////
// Provides an NVUINT field that can also be of width = 0
// May eventually integrate this more into Connections marshaller

namespace nvhls {

/**
 * \brief The EmptyField class is used for rudimentary support for members of a struct that can be configured to have zero width.
 */
struct EmptyField : public nvhls_message {
  template <typename T>
  EmptyField operator=(T const &) {
    NVHLS_ASSERT_MSG(true,"EmptyField_should_never_be_assigned_or_accessed");   // If an assignment actually occurs during runtime
                                                                                // you've done something wrong
    return EmptyField();
  }
  uint64 to_uint64() { return 0; }
  const sc_bit operator[](std::size_t idx) const { return static_cast<sc_bit>(0); }
};

/* Operator << for EmptyField. */
inline std::ostream &operator<<(ostream &os, const EmptyField &empty) {
  return os << "EMPTYFIELD";
}

/* Operator & for Marshaller and EmptyField. */
template <unsigned int Size>
Marshaller<Size> &operator&(Marshaller<Size> &m, EmptyField &rhs) {
  return m;  // just do nothing
}

/**
 * \brief A class to determine whether to instantiate an NVUINT or an EmptyField.
 */
template <bool, int W>
class UIntOrEmptywCheck;

/**
 * \brief Template specialization to instantiate an NVUINT if the width of the UIntOrEmpty is greater than 0.
 */
template <int W>
class UIntOrEmptywCheck<true, W> {
 public:
  typedef NVUINTW(W) T;
};

/**
 * \brief Template specialization to instantiate an EmptyField if the width of the UIntOrEmpty is 0.
 */
template <int W>
class UIntOrEmptywCheck<false, W> {
 public:
  typedef EmptyField T;
};

/**
 * \brief The UIntOrEmpty class is used to define a bitvector that can have a bitwidth of 0.
 *
 * \tparam W     The width of the bitfield (can be 0).
 *
 * \par Overview
 * UIntOrEmpty allows fields to be elided entirely from a struct in the case that they are not used.
 * If W=0, an EmptyField is instantiated instead of an NVUINT.
 * EmptyField is ignored by the Marshaller and overloads some common operators so that
 * code using it does not need to be special-cased for zero-width parameterizations.
 */
template <int W>
class UIntOrEmpty {
 public:
  typedef typename UIntOrEmptywCheck<(W > 0), W>::T T;
};

};
 
//////////////////

#endif

