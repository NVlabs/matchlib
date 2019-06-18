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
#ifndef NVHLS_ARRAY
#define NVHLS_ARRAY

#include <nvhls_marshaller.h>
#include <nvhls_message.h>
#include <nvhls_module.h>

namespace nvhls {
/**
 * \brief An implementation of array that declares \a VectorLength variables for array of size \a VectorLength
 * \ingroup nv_array
 *
 * \tparam Type             Datatype 
 * \tparam VectorLength      Size of array 
 *
 * \par Overview
 * - Declares VectorLength variables to realize an array of size VectorLength
 * - Helpful when HLS tool does not recognize your array correctly and requires unrolling array
 * - nv_array also has specialization for size 0 arrays
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
 public:
  template <typename A, unsigned int K>
  class NNode;
  template <typename A>
  class NNode<A, 0> {
   public:
    A data0;
    NNode() {}
    NNode(sc_module_name nm) : data0(nvhls_concat(nm, "0")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 1> {
   public:
    A data0;
    A data1;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")), data1(nvhls_concat(nm, "1")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0), data1(nvhls_concat(nm, "1"), 1) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 2> {
   public:
    A data0;
    A data1;
    A data2;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 3> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
  };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<A, 4> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 5> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 6> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 7> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 8> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 9> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 10> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 11> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 12> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 13> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 14> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 15> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 16> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 17> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 18> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 19> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 20> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 21> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 22> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 23> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 24> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 25> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 26> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    A data26;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25),
          data26(nvhls_concat(nm, "26"), 26) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 27> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    A data26;
    A data27;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25),
          data26(nvhls_concat(nm, "26"), 26),
          data27(nvhls_concat(nm, "27"), 27) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 28> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    A data26;
    A data27;
    A data28;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25),
          data26(nvhls_concat(nm, "26"), 26),
          data27(nvhls_concat(nm, "27"), 27),
          data28(nvhls_concat(nm, "28"), 28) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 29> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    A data26;
    A data27;
    A data28;
    A data29;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25),
          data26(nvhls_concat(nm, "26"), 26),
          data27(nvhls_concat(nm, "27"), 27),
          data28(nvhls_concat(nm, "28"), 28),
          data29(nvhls_concat(nm, "29"), 29) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 30> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    A data26;
    A data27;
    A data28;
    A data29;
    A data30;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25),
          data26(nvhls_concat(nm, "26"), 26),
          data27(nvhls_concat(nm, "27"), 27),
          data28(nvhls_concat(nm, "28"), 28),
          data29(nvhls_concat(nm, "29"), 29),
          data30(nvhls_concat(nm, "30"), 30) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<A, 31> {
   public:
    A data0;
    A data1;
    A data2;
    A data3;
    A data4;
    A data5;
    A data6;
    A data7;
    A data8;
    A data9;
    A data10;
    A data11;
    A data12;
    A data13;
    A data14;
    A data15;
    A data16;
    A data17;
    A data18;
    A data19;
    A data20;
    A data21;
    A data22;
    A data23;
    A data24;
    A data25;
    A data26;
    A data27;
    A data28;
    A data29;
    A data30;
    A data31;
    NNode() {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")),
          data31(nvhls_concat(nm, "31")) {}
    NNode(sc_module_name nm, const unsigned& id)
        : data0(nvhls_concat(nm, "0"), 0),
          data1(nvhls_concat(nm, "1"), 1),
          data2(nvhls_concat(nm, "2"), 2),
          data3(nvhls_concat(nm, "3"), 3),
          data4(nvhls_concat(nm, "4"), 4),
          data5(nvhls_concat(nm, "5"), 5),
          data6(nvhls_concat(nm, "6"), 6),
          data7(nvhls_concat(nm, "7"), 7),
          data8(nvhls_concat(nm, "8"), 8),
          data9(nvhls_concat(nm, "9"), 9),
          data10(nvhls_concat(nm, "10"), 10),
          data11(nvhls_concat(nm, "11"), 11),
          data12(nvhls_concat(nm, "12"), 12),
          data13(nvhls_concat(nm, "13"), 13),
          data14(nvhls_concat(nm, "14"), 14),
          data15(nvhls_concat(nm, "15"), 15),
          data16(nvhls_concat(nm, "16"), 16),
          data17(nvhls_concat(nm, "17"), 17),
          data18(nvhls_concat(nm, "18"), 18),
          data19(nvhls_concat(nm, "19"), 19),
          data20(nvhls_concat(nm, "20"), 20),
          data21(nvhls_concat(nm, "21"), 21),
          data22(nvhls_concat(nm, "22"), 22),
          data23(nvhls_concat(nm, "23"), 23),
          data24(nvhls_concat(nm, "24"), 24),
          data25(nvhls_concat(nm, "25"), 25),
          data26(nvhls_concat(nm, "26"), 26),
          data27(nvhls_concat(nm, "27"), 27),
          data28(nvhls_concat(nm, "28"), 28),
          data29(nvhls_concat(nm, "29"), 29),
          data30(nvhls_concat(nm, "30"), 30),
          data31(nvhls_concat(nm, "31"), 31) {}
    A& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
    const A& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
  };
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef HLS_CATAPULT  
  template <typename A>
  class NNode<p2p<>::in<A>, 0> {
   public:
    p2p<>::in<A> data0;
    NNode() : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")) {}
    NNode(sc_module_name nm) : data0(nvhls_concat(nm, "0")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 1> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")), data1(nvhls_concat(nm, "1")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 2> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 3> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
  };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p<>::in<A>, 4> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 5> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 6> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 7> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 8> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 9> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 10> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 11> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 12> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 13> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 14> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 15> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 16> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 17> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 18> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 19> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 20> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 21> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 22> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 23> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 24> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 25> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 26> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    p2p<>::in<A> data26;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 27> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    p2p<>::in<A> data26;
    p2p<>::in<A> data27;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 28> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    p2p<>::in<A> data26;
    p2p<>::in<A> data27;
    p2p<>::in<A> data28;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 29> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    p2p<>::in<A> data26;
    p2p<>::in<A> data27;
    p2p<>::in<A> data28;
    p2p<>::in<A> data29;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 30> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    p2p<>::in<A> data26;
    p2p<>::in<A> data27;
    p2p<>::in<A> data28;
    p2p<>::in<A> data29;
    p2p<>::in<A> data30;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::in<A>, 31> {
   public:
    p2p<>::in<A> data0;
    p2p<>::in<A> data1;
    p2p<>::in<A> data2;
    p2p<>::in<A> data3;
    p2p<>::in<A> data4;
    p2p<>::in<A> data5;
    p2p<>::in<A> data6;
    p2p<>::in<A> data7;
    p2p<>::in<A> data8;
    p2p<>::in<A> data9;
    p2p<>::in<A> data10;
    p2p<>::in<A> data11;
    p2p<>::in<A> data12;
    p2p<>::in<A> data13;
    p2p<>::in<A> data14;
    p2p<>::in<A> data15;
    p2p<>::in<A> data16;
    p2p<>::in<A> data17;
    p2p<>::in<A> data18;
    p2p<>::in<A> data19;
    p2p<>::in<A> data20;
    p2p<>::in<A> data21;
    p2p<>::in<A> data22;
    p2p<>::in<A> data23;
    p2p<>::in<A> data24;
    p2p<>::in<A> data25;
    p2p<>::in<A> data26;
    p2p<>::in<A> data27;
    p2p<>::in<A> data28;
    p2p<>::in<A> data29;
    p2p<>::in<A> data30;
    p2p<>::in<A> data31;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")),
          data31(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "31")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")),
          data31(nvhls_concat(nm, "31")) {}
    p2p<>::in<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
    const p2p<>::in<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
  };
#endif //DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p<>::out<A>, 0> {
   public:
    p2p<>::out<A> data0;
    NNode() : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")) {}
    NNode(sc_module_name nm) : data0(nvhls_concat(nm, "0")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 1> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")), data1(nvhls_concat(nm, "1")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 2> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 3> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
  };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p<>::out<A>, 4> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 5> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 6> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 7> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 8> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 9> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 10> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 11> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 12> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 13> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 14> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 15> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 16> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 17> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 18> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 19> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 20> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 21> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 22> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 23> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 24> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 25> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 26> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    p2p<>::out<A> data26;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 27> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    p2p<>::out<A> data26;
    p2p<>::out<A> data27;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 28> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    p2p<>::out<A> data26;
    p2p<>::out<A> data27;
    p2p<>::out<A> data28;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 29> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    p2p<>::out<A> data26;
    p2p<>::out<A> data27;
    p2p<>::out<A> data28;
    p2p<>::out<A> data29;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 30> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    p2p<>::out<A> data26;
    p2p<>::out<A> data27;
    p2p<>::out<A> data28;
    p2p<>::out<A> data29;
    p2p<>::out<A> data30;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::out<A>, 31> {
   public:
    p2p<>::out<A> data0;
    p2p<>::out<A> data1;
    p2p<>::out<A> data2;
    p2p<>::out<A> data3;
    p2p<>::out<A> data4;
    p2p<>::out<A> data5;
    p2p<>::out<A> data6;
    p2p<>::out<A> data7;
    p2p<>::out<A> data8;
    p2p<>::out<A> data9;
    p2p<>::out<A> data10;
    p2p<>::out<A> data11;
    p2p<>::out<A> data12;
    p2p<>::out<A> data13;
    p2p<>::out<A> data14;
    p2p<>::out<A> data15;
    p2p<>::out<A> data16;
    p2p<>::out<A> data17;
    p2p<>::out<A> data18;
    p2p<>::out<A> data19;
    p2p<>::out<A> data20;
    p2p<>::out<A> data21;
    p2p<>::out<A> data22;
    p2p<>::out<A> data23;
    p2p<>::out<A> data24;
    p2p<>::out<A> data25;
    p2p<>::out<A> data26;
    p2p<>::out<A> data27;
    p2p<>::out<A> data28;
    p2p<>::out<A> data29;
    p2p<>::out<A> data30;
    p2p<>::out<A> data31;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")),
          data31(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "31")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")),
          data31(nvhls_concat(nm, "31")) {}
    p2p<>::out<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
    const p2p<>::out<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
  };
#endif //DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p<>::chan<A>, 0> {
   public:
    p2p<>::chan<A> data0;
    NNode() : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")) {}
    NNode(sc_module_name nm) : data0(nvhls_concat(nm, "0")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 1> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")), data1(nvhls_concat(nm, "1")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 2> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 3> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
  };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p<>::chan<A>, 4> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 5> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 6> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 7> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 8> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 9> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 10> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 11> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 12> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 13> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 14> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 15> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 16> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 17> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 18> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 19> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 20> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 21> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 22> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 23> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 24> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 25> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 26> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    p2p<>::chan<A> data26;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 27> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    p2p<>::chan<A> data26;
    p2p<>::chan<A> data27;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 28> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    p2p<>::chan<A> data26;
    p2p<>::chan<A> data27;
    p2p<>::chan<A> data28;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 29> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    p2p<>::chan<A> data26;
    p2p<>::chan<A> data27;
    p2p<>::chan<A> data28;
    p2p<>::chan<A> data29;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 30> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    p2p<>::chan<A> data26;
    p2p<>::chan<A> data27;
    p2p<>::chan<A> data28;
    p2p<>::chan<A> data29;
    p2p<>::chan<A> data30;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p<>::chan<A>, 31> {
   public:
    p2p<>::chan<A> data0;
    p2p<>::chan<A> data1;
    p2p<>::chan<A> data2;
    p2p<>::chan<A> data3;
    p2p<>::chan<A> data4;
    p2p<>::chan<A> data5;
    p2p<>::chan<A> data6;
    p2p<>::chan<A> data7;
    p2p<>::chan<A> data8;
    p2p<>::chan<A> data9;
    p2p<>::chan<A> data10;
    p2p<>::chan<A> data11;
    p2p<>::chan<A> data12;
    p2p<>::chan<A> data13;
    p2p<>::chan<A> data14;
    p2p<>::chan<A> data15;
    p2p<>::chan<A> data16;
    p2p<>::chan<A> data17;
    p2p<>::chan<A> data18;
    p2p<>::chan<A> data19;
    p2p<>::chan<A> data20;
    p2p<>::chan<A> data21;
    p2p<>::chan<A> data22;
    p2p<>::chan<A> data23;
    p2p<>::chan<A> data24;
    p2p<>::chan<A> data25;
    p2p<>::chan<A> data26;
    p2p<>::chan<A> data27;
    p2p<>::chan<A> data28;
    p2p<>::chan<A> data29;
    p2p<>::chan<A> data30;
    p2p<>::chan<A> data31;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")),
          data31(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "31")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")),
          data31(nvhls_concat(nm, "31")) {}
    p2p<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
    const p2p<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
  };
#endif //DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 0> {
   public:
    p2p_fifo<>::chan<A> data0;
    NNode() : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")) {}
    NNode(sc_module_name nm) : data0(nvhls_concat(nm, "0")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 1> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")), data1(nvhls_concat(nm, "1")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 2> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 3> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else
        return data0;
    }
  };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 4> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 5> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 6> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 7> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 8> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 9> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 10> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 11> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 12> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 13> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 14> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 15> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 16> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 17> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 18> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 19> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 20> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 21> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 22> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 23> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 24> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 25> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 26> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    p2p_fifo<>::chan<A> data26;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 27> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    p2p_fifo<>::chan<A> data26;
    p2p_fifo<>::chan<A> data27;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 28> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    p2p_fifo<>::chan<A> data26;
    p2p_fifo<>::chan<A> data27;
    p2p_fifo<>::chan<A> data28;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 29> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    p2p_fifo<>::chan<A> data26;
    p2p_fifo<>::chan<A> data27;
    p2p_fifo<>::chan<A> data28;
    p2p_fifo<>::chan<A> data29;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 30> {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    p2p_fifo<>::chan<A> data26;
    p2p_fifo<>::chan<A> data27;
    p2p_fifo<>::chan<A> data28;
    p2p_fifo<>::chan<A> data29;
    p2p_fifo<>::chan<A> data30;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else
        return data0;
    }
  };
  template <typename A>
  class NNode<p2p_fifo<>::chan<A>, 31> : public nvhls_message {
   public:
    p2p_fifo<>::chan<A> data0;
    p2p_fifo<>::chan<A> data1;
    p2p_fifo<>::chan<A> data2;
    p2p_fifo<>::chan<A> data3;
    p2p_fifo<>::chan<A> data4;
    p2p_fifo<>::chan<A> data5;
    p2p_fifo<>::chan<A> data6;
    p2p_fifo<>::chan<A> data7;
    p2p_fifo<>::chan<A> data8;
    p2p_fifo<>::chan<A> data9;
    p2p_fifo<>::chan<A> data10;
    p2p_fifo<>::chan<A> data11;
    p2p_fifo<>::chan<A> data12;
    p2p_fifo<>::chan<A> data13;
    p2p_fifo<>::chan<A> data14;
    p2p_fifo<>::chan<A> data15;
    p2p_fifo<>::chan<A> data16;
    p2p_fifo<>::chan<A> data17;
    p2p_fifo<>::chan<A> data18;
    p2p_fifo<>::chan<A> data19;
    p2p_fifo<>::chan<A> data20;
    p2p_fifo<>::chan<A> data21;
    p2p_fifo<>::chan<A> data22;
    p2p_fifo<>::chan<A> data23;
    p2p_fifo<>::chan<A> data24;
    p2p_fifo<>::chan<A> data25;
    p2p_fifo<>::chan<A> data26;
    p2p_fifo<>::chan<A> data27;
    p2p_fifo<>::chan<A> data28;
    p2p_fifo<>::chan<A> data29;
    p2p_fifo<>::chan<A> data30;
    p2p_fifo<>::chan<A> data31;
    NNode()
        : data0(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "0")),
          data1(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "1")),
          data2(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "2")),
          data3(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "3")),
          data4(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "4")),
          data5(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "5")),
          data6(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "6")),
          data7(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "7")),
          data8(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "8")),
          data9(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "9")),
          data10(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "10")),
          data11(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "11")),
          data12(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "12")),
          data13(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "13")),
          data14(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "14")),
          data15(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "15")),
          data16(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "16")),
          data17(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "17")),
          data18(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "18")),
          data19(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "19")),
          data20(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "20")),
          data21(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "21")),
          data22(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "22")),
          data23(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "23")),
          data24(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "24")),
          data25(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "25")),
          data26(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "26")),
          data27(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "27")),
          data28(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "28")),
          data29(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "29")),
          data30(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "30")),
          data31(nvhls_concat(sc_gen_unique_name("BOOST_PP_EMPTY"), "31")) {}
    NNode(sc_module_name nm)
        : data0(nvhls_concat(nm, "0")),
          data1(nvhls_concat(nm, "1")),
          data2(nvhls_concat(nm, "2")),
          data3(nvhls_concat(nm, "3")),
          data4(nvhls_concat(nm, "4")),
          data5(nvhls_concat(nm, "5")),
          data6(nvhls_concat(nm, "6")),
          data7(nvhls_concat(nm, "7")),
          data8(nvhls_concat(nm, "8")),
          data9(nvhls_concat(nm, "9")),
          data10(nvhls_concat(nm, "10")),
          data11(nvhls_concat(nm, "11")),
          data12(nvhls_concat(nm, "12")),
          data13(nvhls_concat(nm, "13")),
          data14(nvhls_concat(nm, "14")),
          data15(nvhls_concat(nm, "15")),
          data16(nvhls_concat(nm, "16")),
          data17(nvhls_concat(nm, "17")),
          data18(nvhls_concat(nm, "18")),
          data19(nvhls_concat(nm, "19")),
          data20(nvhls_concat(nm, "20")),
          data21(nvhls_concat(nm, "21")),
          data22(nvhls_concat(nm, "22")),
          data23(nvhls_concat(nm, "23")),
          data24(nvhls_concat(nm, "24")),
          data25(nvhls_concat(nm, "25")),
          data26(nvhls_concat(nm, "26")),
          data27(nvhls_concat(nm, "27")),
          data28(nvhls_concat(nm, "28")),
          data29(nvhls_concat(nm, "29")),
          data30(nvhls_concat(nm, "30")),
          data31(nvhls_concat(nm, "31")) {}
    p2p_fifo<>::chan<A>& Get(unsigned int idx) {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
    const p2p_fifo<>::chan<A>& Get(unsigned int idx) const {
      if (idx == 0)
        return data0;
      else if (idx == 1)
        return data1;
      else if (idx == 2)
        return data2;
      else if (idx == 3)
        return data3;
      else if (idx == 4)
        return data4;
      else if (idx == 5)
        return data5;
      else if (idx == 6)
        return data6;
      else if (idx == 7)
        return data7;
      else if (idx == 8)
        return data8;
      else if (idx == 9)
        return data9;
      else if (idx == 10)
        return data10;
      else if (idx == 11)
        return data11;
      else if (idx == 12)
        return data12;
      else if (idx == 13)
        return data13;
      else if (idx == 14)
        return data14;
      else if (idx == 15)
        return data15;
      else if (idx == 16)
        return data16;
      else if (idx == 17)
        return data17;
      else if (idx == 18)
        return data18;
      else if (idx == 19)
        return data19;
      else if (idx == 20)
        return data20;
      else if (idx == 21)
        return data21;
      else if (idx == 22)
        return data22;
      else if (idx == 23)
        return data23;
      else if (idx == 24)
        return data24;
      else if (idx == 25)
        return data25;
      else if (idx == 26)
        return data26;
      else if (idx == 27)
        return data27;
      else if (idx == 28)
        return data28;
      else if (idx == 29)
        return data29;
      else if (idx == 30)
        return data30;
      else if (idx == 31)
        return data31;
      else
        return data0;
    }
  };

#endif // HLS_CATAPULT
  
#endif //DOXYGEN_SHOULD_SKIP_THIS

 public:
  NNode<Type, VectorLength - 1> array_impl;
  nv_array() {}
  nv_array(const char* prefix) : array_impl(prefix) {}
  nv_array(sc_module_name prefix) : array_impl(prefix) {}
  nv_array(sc_module_name prefix, const unsigned int& id)
      : array_impl(prefix, id) {}
  nv_array(const nv_array<Type, VectorLength>& that) {
    for (unsigned i = 0; i < VectorLength; i++)
      array_impl.Get(i) = that.array_impl.Get(i);
  }
  nv_array(const Type newdata[VectorLength]) {
    for (unsigned i = 0; i < VectorLength; i++)
      array_impl.Get(i) = newdata[i];
  }
  nv_array<Type, VectorLength>& operator=(
      const nv_array<Type, VectorLength>& that) {
    for (unsigned i = 0; i < VectorLength; i++)
      array_impl.Get(i) = that.array_impl.Get(i);
    return *this;
  }
  inline void copy(nv_array<Type, VectorLength>& out) {
    for (unsigned i = 0; i < VectorLength; i++)
      out.array_impl.Get(i) = array_impl.Get(i);
  }
  Type& operator[](unsigned int i) { return this->array_impl.Get(i); }
  const Type& operator[](unsigned int i) const {
    return this->array_impl.Get(i);
  }
  static const unsigned int width = Wrapped<Type>::width * VectorLength;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    if (Wrapped<Type>::width == 0) return;
    for (unsigned int x = 0; x < VectorLength; x++) {
      m& array_impl.Get(x);
    }
  }
  template <typename Msg>
  void DynamicPush(unsigned int idx, const Msg& val) {
    if (idx == 0)
      array_impl.Get(0).Push(val);
    else if (idx == 1)
      array_impl.Get(1).Push(val);
    else if (idx == 2)
      array_impl.Get(2).Push(val);
    else if (idx == 3)
      array_impl.Get(3).Push(val);
    else if (idx == 4)
      array_impl.Get(4).Push(val);
    else if (idx == 5)
      array_impl.Get(5).Push(val);
    else if (idx == 6)
      array_impl.Get(6).Push(val);
    else if (idx == 7)
      array_impl.Get(7).Push(val);
    else if (idx == 8)
      array_impl.Get(8).Push(val);
    else if (idx == 9)
      array_impl.Get(9).Push(val);
    else if (idx == 10)
      array_impl.Get(10).Push(val);
    else if (idx == 11)
      array_impl.Get(11).Push(val);
    else if (idx == 12)
      array_impl.Get(12).Push(val);
    else if (idx == 13)
      array_impl.Get(13).Push(val);
    else if (idx == 14)
      array_impl.Get(14).Push(val);
    else if (idx == 15)
      array_impl.Get(15).Push(val);
    else if (idx == 16)
      array_impl.Get(16).Push(val);
    else if (idx == 17)
      array_impl.Get(17).Push(val);
    else if (idx == 18)
      array_impl.Get(18).Push(val);
    else if (idx == 19)
      array_impl.Get(19).Push(val);
    else if (idx == 20)
      array_impl.Get(20).Push(val);
    else if (idx == 21)
      array_impl.Get(21).Push(val);
    else if (idx == 22)
      array_impl.Get(22).Push(val);
    else if (idx == 23)
      array_impl.Get(23).Push(val);
    else if (idx == 24)
      array_impl.Get(24).Push(val);
    else if (idx == 25)
      array_impl.Get(25).Push(val);
    else if (idx == 26)
      array_impl.Get(26).Push(val);
    else if (idx == 27)
      array_impl.Get(27).Push(val);
    else if (idx == 28)
      array_impl.Get(28).Push(val);
    else if (idx == 29)
      array_impl.Get(29).Push(val);
    else if (idx == 30)
      array_impl.Get(30).Push(val);
    else if (idx == 31)
      array_impl.Get(31).Push(val);
    else
      array_impl.Get(0).Push(val);
  }
  template <typename Msg>
  bool DynamicFull(unsigned int idx) {
    if (idx == 0)
      return array_impl.Get(0).Full();
    else if (idx == 1)
      return array_impl.Get(1).Full();
    else if (idx == 2)
      return array_impl.Get(2).Full();
    else if (idx == 3)
      return array_impl.Get(3).Full();
    else if (idx == 4)
      return array_impl.Get(4).Full();
    else if (idx == 5)
      return array_impl.Get(5).Full();
    else if (idx == 6)
      return array_impl.Get(6).Full();
    else if (idx == 7)
      return array_impl.Get(7).Full();
    else if (idx == 8)
      return array_impl.Get(8).Full();
    else if (idx == 9)
      return array_impl.Get(9).Full();
    else if (idx == 10)
      return array_impl.Get(10).Full();
    else if (idx == 11)
      return array_impl.Get(11).Full();
    else if (idx == 12)
      return array_impl.Get(12).Full();
    else if (idx == 13)
      return array_impl.Get(13).Full();
    else if (idx == 14)
      return array_impl.Get(14).Full();
    else if (idx == 15)
      return array_impl.Get(15).Full();
    else if (idx == 16)
      return array_impl.Get(16).Full();
    else if (idx == 17)
      return array_impl.Get(17).Full();
    else if (idx == 18)
      return array_impl.Get(18).Full();
    else if (idx == 19)
      return array_impl.Get(19).Full();
    else if (idx == 20)
      return array_impl.Get(20).Full();
    else if (idx == 21)
      return array_impl.Get(21).Full();
    else if (idx == 22)
      return array_impl.Get(22).Full();
    else if (idx == 23)
      return array_impl.Get(23).Full();
    else if (idx == 24)
      return array_impl.Get(24).Full();
    else if (idx == 25)
      return array_impl.Get(25).Full();
    else if (idx == 26)
      return array_impl.Get(26).Full();
    else if (idx == 27)
      return array_impl.Get(27).Full();
    else if (idx == 28)
      return array_impl.Get(28).Full();
    else if (idx == 29)
      return array_impl.Get(29).Full();
    else if (idx == 30)
      return array_impl.Get(30).Full();
    else if (idx == 31)
      return array_impl.Get(31).Full();
    else
      return array_impl.Get(0).Full();
  }
  template <typename Msg>
  Msg DynamicPop(unsigned int idx) {
    if (idx == 0)
      return array_impl.Get(0).Pop();
    else if (idx == 1)
      return array_impl.Get(1).Pop();
    else if (idx == 2)
      return array_impl.Get(2).Pop();
    else if (idx == 3)
      return array_impl.Get(3).Pop();
    else if (idx == 4)
      return array_impl.Get(4).Pop();
    else if (idx == 5)
      return array_impl.Get(5).Pop();
    else if (idx == 6)
      return array_impl.Get(6).Pop();
    else if (idx == 7)
      return array_impl.Get(7).Pop();
    else if (idx == 8)
      return array_impl.Get(8).Pop();
    else if (idx == 9)
      return array_impl.Get(9).Pop();
    else if (idx == 10)
      return array_impl.Get(10).Pop();
    else if (idx == 11)
      return array_impl.Get(11).Pop();
    else if (idx == 12)
      return array_impl.Get(12).Pop();
    else if (idx == 13)
      return array_impl.Get(13).Pop();
    else if (idx == 14)
      return array_impl.Get(14).Pop();
    else if (idx == 15)
      return array_impl.Get(15).Pop();
    else if (idx == 16)
      return array_impl.Get(16).Pop();
    else if (idx == 17)
      return array_impl.Get(17).Pop();
    else if (idx == 18)
      return array_impl.Get(18).Pop();
    else if (idx == 19)
      return array_impl.Get(19).Pop();
    else if (idx == 20)
      return array_impl.Get(20).Pop();
    else if (idx == 21)
      return array_impl.Get(21).Pop();
    else if (idx == 22)
      return array_impl.Get(22).Pop();
    else if (idx == 23)
      return array_impl.Get(23).Pop();
    else if (idx == 24)
      return array_impl.Get(24).Pop();
    else if (idx == 25)
      return array_impl.Get(25).Pop();
    else if (idx == 26)
      return array_impl.Get(26).Pop();
    else if (idx == 27)
      return array_impl.Get(27).Pop();
    else if (idx == 28)
      return array_impl.Get(28).Pop();
    else if (idx == 29)
      return array_impl.Get(29).Pop();
    else if (idx == 30)
      return array_impl.Get(30).Pop();
    else if (idx == 31)
      return array_impl.Get(31).Pop();
    else
      return array_impl.Get(0).Pop();
  }
  template <typename Msg>
  Msg DynamicPeek(unsigned int idx) {
    if (idx == 0)
      return array_impl.Get(0).Peek();
    else if (idx == 1)
      return array_impl.Get(1).Peek();
    else if (idx == 2)
      return array_impl.Get(2).Peek();
    else if (idx == 3)
      return array_impl.Get(3).Peek();
    else if (idx == 4)
      return array_impl.Get(4).Peek();
    else if (idx == 5)
      return array_impl.Get(5).Peek();
    else if (idx == 6)
      return array_impl.Get(6).Peek();
    else if (idx == 7)
      return array_impl.Get(7).Peek();
    else if (idx == 8)
      return array_impl.Get(8).Peek();
    else if (idx == 9)
      return array_impl.Get(9).Peek();
    else if (idx == 10)
      return array_impl.Get(10).Peek();
    else if (idx == 11)
      return array_impl.Get(11).Peek();
    else if (idx == 12)
      return array_impl.Get(12).Peek();
    else if (idx == 13)
      return array_impl.Get(13).Peek();
    else if (idx == 14)
      return array_impl.Get(14).Peek();
    else if (idx == 15)
      return array_impl.Get(15).Peek();
    else if (idx == 16)
      return array_impl.Get(16).Peek();
    else if (idx == 17)
      return array_impl.Get(17).Peek();
    else if (idx == 18)
      return array_impl.Get(18).Peek();
    else if (idx == 19)
      return array_impl.Get(19).Peek();
    else if (idx == 20)
      return array_impl.Get(20).Peek();
    else if (idx == 21)
      return array_impl.Get(21).Peek();
    else if (idx == 22)
      return array_impl.Get(22).Peek();
    else if (idx == 23)
      return array_impl.Get(23).Peek();
    else if (idx == 24)
      return array_impl.Get(24).Peek();
    else if (idx == 25)
      return array_impl.Get(25).Peek();
    else if (idx == 26)
      return array_impl.Get(26).Peek();
    else if (idx == 27)
      return array_impl.Get(27).Peek();
    else if (idx == 28)
      return array_impl.Get(28).Peek();
    else if (idx == 29)
      return array_impl.Get(29).Peek();
    else if (idx == 30)
      return array_impl.Get(30).Peek();
    else if (idx == 31)
      return array_impl.Get(31).Peek();
    else
      return array_impl.Get(0).Peek();
  }
  template <typename Msg>
  bool DynamicEmpty(unsigned int idx) {
    if (idx == 0)
      return array_impl.Get(0).Empty();
    else if (idx == 1)
      return array_impl.Get(1).Empty();
    else if (idx == 2)
      return array_impl.Get(2).Empty();
    else if (idx == 3)
      return array_impl.Get(3).Empty();
    else if (idx == 4)
      return array_impl.Get(4).Empty();
    else if (idx == 5)
      return array_impl.Get(5).Empty();
    else if (idx == 6)
      return array_impl.Get(6).Empty();
    else if (idx == 7)
      return array_impl.Get(7).Empty();
    else if (idx == 8)
      return array_impl.Get(8).Empty();
    else if (idx == 9)
      return array_impl.Get(9).Empty();
    else if (idx == 10)
      return array_impl.Get(10).Empty();
    else if (idx == 11)
      return array_impl.Get(11).Empty();
    else if (idx == 12)
      return array_impl.Get(12).Empty();
    else if (idx == 13)
      return array_impl.Get(13).Empty();
    else if (idx == 14)
      return array_impl.Get(14).Empty();
    else if (idx == 15)
      return array_impl.Get(15).Empty();
    else if (idx == 16)
      return array_impl.Get(16).Empty();
    else if (idx == 17)
      return array_impl.Get(17).Empty();
    else if (idx == 18)
      return array_impl.Get(18).Empty();
    else if (idx == 19)
      return array_impl.Get(19).Empty();
    else if (idx == 20)
      return array_impl.Get(20).Empty();
    else if (idx == 21)
      return array_impl.Get(21).Empty();
    else if (idx == 22)
      return array_impl.Get(22).Empty();
    else if (idx == 23)
      return array_impl.Get(23).Empty();
    else if (idx == 24)
      return array_impl.Get(24).Empty();
    else if (idx == 25)
      return array_impl.Get(25).Empty();
    else if (idx == 26)
      return array_impl.Get(26).Empty();
    else if (idx == 27)
      return array_impl.Get(27).Empty();
    else if (idx == 28)
      return array_impl.Get(28).Empty();
    else if (idx == 29)
      return array_impl.Get(29).Empty();
    else if (idx == 30)
      return array_impl.Get(30).Empty();
    else if (idx == 31)
      return array_impl.Get(31).Empty();
    else
      return array_impl.Get(0).Empty();
  }
  template <typename Msg>
  void DynamicWrite(unsigned int idx, Msg val) {
    if (idx == 0)
      array_impl.Get(0).write(val);
    else if (idx == 1)
      array_impl.Get(1).write(val);
    else if (idx == 2)
      array_impl.Get(2).write(val);
    else if (idx == 3)
      array_impl.Get(3).write(val);
    else if (idx == 4)
      array_impl.Get(4).write(val);
    else if (idx == 5)
      array_impl.Get(5).write(val);
    else if (idx == 6)
      array_impl.Get(6).write(val);
    else if (idx == 7)
      array_impl.Get(7).write(val);
    else if (idx == 8)
      array_impl.Get(8).write(val);
    else if (idx == 9)
      array_impl.Get(9).write(val);
    else if (idx == 10)
      array_impl.Get(10).write(val);
    else if (idx == 11)
      array_impl.Get(11).write(val);
    else if (idx == 12)
      array_impl.Get(12).write(val);
    else if (idx == 13)
      array_impl.Get(13).write(val);
    else if (idx == 14)
      array_impl.Get(14).write(val);
    else if (idx == 15)
      array_impl.Get(15).write(val);
    else if (idx == 16)
      array_impl.Get(16).write(val);
    else if (idx == 17)
      array_impl.Get(17).write(val);
    else if (idx == 18)
      array_impl.Get(18).write(val);
    else if (idx == 19)
      array_impl.Get(19).write(val);
    else if (idx == 20)
      array_impl.Get(20).write(val);
    else if (idx == 21)
      array_impl.Get(21).write(val);
    else if (idx == 22)
      array_impl.Get(22).write(val);
    else if (idx == 23)
      array_impl.Get(23).write(val);
    else if (idx == 24)
      array_impl.Get(24).write(val);
    else if (idx == 25)
      array_impl.Get(25).write(val);
    else if (idx == 26)
      array_impl.Get(26).write(val);
    else if (idx == 27)
      array_impl.Get(27).write(val);
    else if (idx == 28)
      array_impl.Get(28).write(val);
    else if (idx == 29)
      array_impl.Get(29).write(val);
    else if (idx == 30)
      array_impl.Get(30).write(val);
    else if (idx == 31)
      array_impl.Get(31).write(val);
    else
      array_impl.Get(0).write(val);
  }
  template <typename Msg>
  Msg DynamicRead(unsigned int idx) {
    if (idx == 0)
      return array_impl.Get(0).read();
    else if (idx == 1)
      return array_impl.Get(1).read();
    else if (idx == 2)
      return array_impl.Get(2).read();
    else if (idx == 3)
      return array_impl.Get(3).read();
    else if (idx == 4)
      return array_impl.Get(4).read();
    else if (idx == 5)
      return array_impl.Get(5).read();
    else if (idx == 6)
      return array_impl.Get(6).read();
    else if (idx == 7)
      return array_impl.Get(7).read();
    else if (idx == 8)
      return array_impl.Get(8).read();
    else if (idx == 9)
      return array_impl.Get(9).read();
    else if (idx == 10)
      return array_impl.Get(10).read();
    else if (idx == 11)
      return array_impl.Get(11).read();
    else if (idx == 12)
      return array_impl.Get(12).read();
    else if (idx == 13)
      return array_impl.Get(13).read();
    else if (idx == 14)
      return array_impl.Get(14).read();
    else if (idx == 15)
      return array_impl.Get(15).read();
    else if (idx == 16)
      return array_impl.Get(16).read();
    else if (idx == 17)
      return array_impl.Get(17).read();
    else if (idx == 18)
      return array_impl.Get(18).read();
    else if (idx == 19)
      return array_impl.Get(19).read();
    else if (idx == 20)
      return array_impl.Get(20).read();
    else if (idx == 21)
      return array_impl.Get(21).read();
    else if (idx == 22)
      return array_impl.Get(22).read();
    else if (idx == 23)
      return array_impl.Get(23).read();
    else if (idx == 24)
      return array_impl.Get(24).read();
    else if (idx == 25)
      return array_impl.Get(25).read();
    else if (idx == 26)
      return array_impl.Get(26).read();
    else if (idx == 27)
      return array_impl.Get(27).read();
    else if (idx == 28)
      return array_impl.Get(28).read();
    else if (idx == 29)
      return array_impl.Get(29).read();
    else if (idx == 30)
      return array_impl.Get(30).read();
    else if (idx == 31)
      return array_impl.Get(31).read();
    else
      return array_impl.Get(0).read();
  }
  template <typename Msg>
  bool DynamicNBRead(unsigned int idx, Msg& val) {
    if (idx == 0)
      return array_impl.Get(0).nb_read(val);
    else if (idx == 1)
      return array_impl.Get(1).nb_read(val);
    else if (idx == 2)
      return array_impl.Get(2).nb_read(val);
    else if (idx == 3)
      return array_impl.Get(3).nb_read(val);
    else if (idx == 4)
      return array_impl.Get(4).nb_read(val);
    else if (idx == 5)
      return array_impl.Get(5).nb_read(val);
    else if (idx == 6)
      return array_impl.Get(6).nb_read(val);
    else if (idx == 7)
      return array_impl.Get(7).nb_read(val);
    else if (idx == 8)
      return array_impl.Get(8).nb_read(val);
    else if (idx == 9)
      return array_impl.Get(9).nb_read(val);
    else if (idx == 10)
      return array_impl.Get(10).nb_read(val);
    else if (idx == 11)
      return array_impl.Get(11).nb_read(val);
    else if (idx == 12)
      return array_impl.Get(12).nb_read(val);
    else if (idx == 13)
      return array_impl.Get(13).nb_read(val);
    else if (idx == 14)
      return array_impl.Get(14).nb_read(val);
    else if (idx == 15)
      return array_impl.Get(15).nb_read(val);
    else if (idx == 16)
      return array_impl.Get(16).nb_read(val);
    else if (idx == 17)
      return array_impl.Get(17).nb_read(val);
    else if (idx == 18)
      return array_impl.Get(18).nb_read(val);
    else if (idx == 19)
      return array_impl.Get(19).nb_read(val);
    else if (idx == 20)
      return array_impl.Get(20).nb_read(val);
    else if (idx == 21)
      return array_impl.Get(21).nb_read(val);
    else if (idx == 22)
      return array_impl.Get(22).nb_read(val);
    else if (idx == 23)
      return array_impl.Get(23).nb_read(val);
    else if (idx == 24)
      return array_impl.Get(24).nb_read(val);
    else if (idx == 25)
      return array_impl.Get(25).nb_read(val);
    else if (idx == 26)
      return array_impl.Get(26).nb_read(val);
    else if (idx == 27)
      return array_impl.Get(27).nb_read(val);
    else if (idx == 28)
      return array_impl.Get(28).nb_read(val);
    else if (idx == 29)
      return array_impl.Get(29).nb_read(val);
    else if (idx == 30)
      return array_impl.Get(30).nb_read(val);
    else if (idx == 31)
      return array_impl.Get(31).nb_read(val);
    else
      return array_impl.Get(0).nb_read(val);
  }
  template <typename Msg>
  bool DynamicNBWrite(unsigned int idx, Msg val) {
    if (idx == 0)
      return array_impl.Get(0).nb_write(val);
    else if (idx == 1)
      return array_impl.Get(1).nb_write(val);
    else if (idx == 2)
      return array_impl.Get(2).nb_write(val);
    else if (idx == 3)
      return array_impl.Get(3).nb_write(val);
    else if (idx == 4)
      return array_impl.Get(4).nb_write(val);
    else if (idx == 5)
      return array_impl.Get(5).nb_write(val);
    else if (idx == 6)
      return array_impl.Get(6).nb_write(val);
    else if (idx == 7)
      return array_impl.Get(7).nb_write(val);
    else if (idx == 8)
      return array_impl.Get(8).nb_write(val);
    else if (idx == 9)
      return array_impl.Get(9).nb_write(val);
    else if (idx == 10)
      return array_impl.Get(10).nb_write(val);
    else if (idx == 11)
      return array_impl.Get(11).nb_write(val);
    else if (idx == 12)
      return array_impl.Get(12).nb_write(val);
    else if (idx == 13)
      return array_impl.Get(13).nb_write(val);
    else if (idx == 14)
      return array_impl.Get(14).nb_write(val);
    else if (idx == 15)
      return array_impl.Get(15).nb_write(val);
    else if (idx == 16)
      return array_impl.Get(16).nb_write(val);
    else if (idx == 17)
      return array_impl.Get(17).nb_write(val);
    else if (idx == 18)
      return array_impl.Get(18).nb_write(val);
    else if (idx == 19)
      return array_impl.Get(19).nb_write(val);
    else if (idx == 20)
      return array_impl.Get(20).nb_write(val);
    else if (idx == 21)
      return array_impl.Get(21).nb_write(val);
    else if (idx == 22)
      return array_impl.Get(22).nb_write(val);
    else if (idx == 23)
      return array_impl.Get(23).nb_write(val);
    else if (idx == 24)
      return array_impl.Get(24).nb_write(val);
    else if (idx == 25)
      return array_impl.Get(25).nb_write(val);
    else if (idx == 26)
      return array_impl.Get(26).nb_write(val);
    else if (idx == 27)
      return array_impl.Get(27).nb_write(val);
    else if (idx == 28)
      return array_impl.Get(28).nb_write(val);
    else if (idx == 29)
      return array_impl.Get(29).nb_write(val);
    else if (idx == 30)
      return array_impl.Get(30).nb_write(val);
    else if (idx == 31)
      return array_impl.Get(31).nb_write(val);
    else
      return array_impl.Get(0).nb_write(val);
  }
};
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
  template <typename Msg>
  void DynamicPush(unsigned int idx, const Msg& val) {}
  template <typename Msg>
  bool DynamicFull(unsigned int idx) {
    return false;
  }
  template <typename Msg>
  Msg DynamicPop(unsigned int idx) {
    return NULL;
  }
  template <typename Msg>
  Msg DynamicPeek(unsigned int idx) {
    return NULL;
  }
  template <typename Msg>
  bool DynamicEmpty(unsigned int idx) {
    return true;
  }
  template <typename Msg>
  void DynamicWrite(unsigned int idx, Msg val) {}
  template <typename Msg>
  Msg DynamicRead(unsigned int idx) {
    return NULL;
  }
  template <typename Msg>
  bool DynamicNBRead(unsigned int idx, Msg& val) {
    return false;
  }
  template <typename Msg>
  bool DynamicNBWrite(unsigned int idx, Msg val) {
    return false;
  }
};
};

#endif
