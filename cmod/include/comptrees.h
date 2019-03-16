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
#ifndef _COMP_TREES_H_
#define _COMP_TREES_H_

/* Minmax and search tree implementations using template metaprogramming.
 *
 * These implementations are written for efficient HLS in mind, not for ease of
 * programming.
 */

#include <iostream>
#include <nvhls_marshaller.h>

using namespace std;

/**
 * \brief Compile-time minmax tree 
 * \ingroup comptrees
 *
 * \tparam ArrT   Array type 
 * \tparam ElemT  Element type 
 * \tparam IdxT   type of the index. This is to restrict the index's bitwidth 
 * \tparam is_max true if this is a max function, false for min 
 * \tparam Width  The width of this minmax tree, that is, the number of elements this
 *                instance of the tree is comparing. This must be a power of 2, but this
 *                is not (necessarily) the same as the size of the input array. This is
 *                mostly to define a base condition for the metaprogramming recursion
 *                (which is when Width=2)
 *
 * \par Overview
 * This returns the index of either the largest or smallest element in the
 * input array, which must be sized to a power of two.
 *
 * This specialization is for arrays implemented as a bitvector in which each
 * element is of the same bitwidth as that of ElemT. As SC datatypes do not
 * have a slice operator, we use bitshifts to access the corresponding element.
 * 
 * \par A Simple Example
 * \code
 *      #include <comptrees.h>
 *
 *      ...
 *      typedef NVUINTW(ASSOC_LOG2) lru_t;
 *      typedef NVUINTW(ASSOC_LOG2 * ASSOC) lru_set_t;
 *      typedef NVUINTW(ASSOC_LOG2) way_idx_t;
 *      typedef Minmax<lru_set_t, lru_t, way_idx_t, true, ASSOC> LruMinmaxTree;
 *
 *      lru_set_t lru_set;
 *      way_idx_t way = LruMinmaxTree::minmax(lru_set, 0, ASSOC - 1);    
 *      ...
 *
 * \endcode
 *
 */


template <typename ArrT, typename ElemT, typename IdxT, bool is_max,
          unsigned Width>
class Minmax {
 public:
  static IdxT minmax(ArrT inputs, IdxT start, IdxT end) {
    const unsigned TotalElems = Wrapped<ArrT>::width / Wrapped<ElemT>::width;
    const unsigned ElemWidth = Wrapped<ElemT>::width;
    ElemT temp[TotalElems];
#pragma hls_unroll yes
    for (unsigned i = 0; i < TotalElems; i++)
      temp[i] = inputs.range((i + 1) * ElemWidth - 1, i * ElemWidth);

    IdxT upper_branch = Minmax<ArrT, ElemT, IdxT, is_max, Width / 2>::minmax(
        inputs, (start + end) / 2 + 1, end);
    IdxT lower_branch = Minmax<ArrT, ElemT, IdxT, is_max, Width / 2>::minmax(
        inputs, start, (start + end) / 2);
    if (is_max)
      return temp[upper_branch] > temp[lower_branch] ? upper_branch
                                                     : lower_branch;
    else
      return temp[upper_branch] < temp[lower_branch] ? upper_branch
                                                     : lower_branch;
  }
};

/* Base condition for MPL minmax. */
template <typename ArrT, typename ElemT, typename IdxT, bool is_max>
class Minmax<ArrT, ElemT, IdxT, is_max, 2> {
 public:
  static IdxT minmax(ArrT inputs, IdxT start, IdxT end) {
    const unsigned TotalElems = Wrapped<ArrT>::width / Wrapped<ElemT>::width;
    const unsigned ElemWidth = Wrapped<ElemT>::width;
    ElemT temp[TotalElems];
#pragma hls_unroll yes
    for (unsigned i = 0; i < TotalElems; i++)
      temp[i] = inputs.range((i + 1) * ElemWidth - 1, i * ElemWidth);

    if (is_max)
      return temp[start] > temp[end] ? start : end;
    else
      return temp[start] < temp[end] ? start : end;
  }
};

// Base condition for Width = 1.
template <typename ArrT, typename ElemT, typename IdxT, bool is_max>
class Minmax<ArrT, ElemT, IdxT, is_max, 1> {
 public:
  // Comparing an element to itself must always just return itself.
  static IdxT minmax(ArrT inputs, IdxT start, IdxT end) { return start; }
};

/* Compile time SC datatype concatenator tree.
 *
 * This class can be used to concatenate NumElements number of SC datatype
 * objects
 * together. This is required to avoid using a for
 * loop on the concat() method. The objects are stored in an array.
 *
 * Template args:
 *   BaseTemplate: the underlying class storing each element (e.g. sc_int,
 * sc_uint).
 *   ElemW: The width of each element to be concatenated.
 *   NumElements: The number of elements to be concatenated.
 */
template <template <int> class BaseTemplate, unsigned ElemW,
          unsigned NumElements>
class Concat {
 public:
  typedef BaseTemplate<ElemW * NumElements> concat_t;
  typedef BaseTemplate<ElemW * NumElements / 2> concat_half_t;

  static concat_t concat(BaseTemplate<ElemW> components[], unsigned start,
                         unsigned end) {
    BaseTemplate<ElemW*(NumElements - 1)> retval =
        Concat<BaseTemplate, ElemW, NumElements - 1>::concat(components,
                                                             start + 1, end);
    return (retval, components[start]);
  }
};

// Base case for concatenating 2 elements
template <template <int> class BaseTemplate, unsigned ElemW>
class Concat<BaseTemplate, ElemW, 2> {
 public:
  typedef BaseTemplate<ElemW * 2> concat_t;

  static concat_t concat(BaseTemplate<ElemW> components[], unsigned start,
                         unsigned end) {
    return (components[end], components[start]);
  }
};

// Base case for "concatenating" 1 element
template <template <int> class BaseTemplate, unsigned ElemW>
class Concat<BaseTemplate, ElemW, 1> {
 public:
  typedef BaseTemplate<ElemW> concat_t;

  static concat_t concat(BaseTemplate<ElemW> components[], unsigned start,
                         unsigned end) {
    return components[start];
  }
};

/* Priority Encoder:
 * Find first (starting from LSB) logic false or logic true in a bitvector.
 * Returns bit position of first true/false if found.
 * Returns -1 if desired logic value not found in bitvector.
 *
 * Template parameters:
 *   VecT: Bitvector type.
 *   ValT: Value type.
 *   IdxT: type of the return variable (should be size log2(Width)+1)
 *   Width: The size of the range to search in the array. Used to identify the
 * base
 *     condition for MPL recursion.
 */

// Base condition
template <typename VecT, typename ValT, typename IdxT, unsigned Width>
class PriEnc {
 public:
  // This is the pri_enc function to call:
  static IdxT val(VecT inputs, ValT comp_value) {
    // This call will expand into some compile-time TMP recursion:
    IdxT retval =
        PriEnc<VecT, ValT, IdxT, Width>::val(inputs, comp_value, 0, Width - 1);
    return retval;
  }

  static IdxT val(VecT inputs, ValT comp_value, IdxT start, IdxT end) {
    if (inputs[start] == comp_value) {
      return start;
    } else if (start == end) {
      return -1;
    } else {
      IdxT retval = PriEnc<VecT, ValT, IdxT, Width - 1>::val(
          inputs, comp_value, start + 1, start + Width - 1);
      return retval;
    }
  }
};

// Specialized cases for Width = 1
template <typename VecT, typename ValT, typename IdxT>
class PriEnc<VecT, ValT, IdxT, 1> {
 public:
  static IdxT val(VecT inputs, ValT comp_value) {
    if (inputs[0] == comp_value) {
      return 0;
    } else {
      return -1;
    }
  }

  static IdxT val(VecT inputs, ValT comp_value, IdxT start, IdxT end) {
    if (inputs[end] == comp_value) {
      return end;
    } else {
      return -1;
    }
  }
};

#endif
