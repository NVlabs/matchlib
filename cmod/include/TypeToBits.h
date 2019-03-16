/*
 * Copyright (c) 2018-2019, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef __TYPETOBITS_H__
#define __TYPETOBITS_H__

#include <systemc.h>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_marshaller.h>

/**
 * \brief Convert Type to logic vector 
 * \ingroup TypeToBits
 *
 * \tparam T    DataType      
 *
 * \param[in]   in               Input variable of type T 
 * \param[out]  ReturnVal        input variable converted to logic vector

 * \par A Simple Example
 * \code
 *      #include <TypeToBits.h>
 *
 *      ...
 *      NVUINT4 input = 4;
 *      sc_lv<4> output = TypeToBits<NVUINT4>(input);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename T>
sc_lv<Wrapped<T>::width> TypeToBits(T in)
{
  Marshaller<Wrapped<T>::width> marshaller;
  Wrapped<T> wm(in);
  wm.Marshall(marshaller);
  sc_lv<Wrapped<T>::width>  bits = marshaller.GetResult();
  return bits;
}

/**
 * \brief Convert logic vector to type 
 * \ingroup TypeToBits
 *
 * \tparam T    DataType      
 *
 * \param[in]   in               Input variable of sc_lv type
 * \param[out]  ReturnVal        input variable converted to target datatype

 * \par A Simple Example
 * \code
 *      #include <TypeToBits.h>
 *
 *      ...
 *      sc_lv<4> input = 4;
 *      NVUINT4 output = BitsToType<NVUINT4>(input);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename T>
T BitsToType(sc_lv<Wrapped<T>::width> mbits)
{
  Marshaller<Wrapped<T>::width> marshaller(mbits);
  Wrapped<T> result;
  result.Marshall(marshaller);
  return result.val;
}

/**
 * \brief Convert Type to NVUINT 
 * \ingroup TypeToBits
 *
 * \tparam T    DataType      
 *
 * \param[in]   in               Input variable of type T 
 * \param[out]  ReturnVal        input variable converted to NVUINT

 * \par A Simple Example
 * \code
 *      #include <TypeToBits.h>
 *
 *      ...
 *      Word_t input;
 *      ...
 *      NVUINT4 output = TypeToNVUINT<Word_t>(input);
 *      ...
 *
 * \endcode
 * \par
 *
 */


template <typename T>
NVUINTW(Wrapped<T>::width) TypeToNVUINT(T in)
{
  return BitsToType<NVUINTW(Wrapped<T>::width)>(TypeToBits(in));
}

/**
 * \brief Convert NVUINT to type 
 * \ingroup TypeToBits
 *
 * \tparam T    DataType      
 *
 * \param[in]   in               Input variable of NVUINT type
 * \param[out]  ReturnVal        input variable converted to target datatype

 * \par A Simple Example
 * \code
 *      #include <TypeToBits.h>
 *
 *      ...
 *      NVUINT4 input = 4;
 *      Word_t output = NVUINTToType<Word_t>(input);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename T>
T NVUINTToType(const NVUINTW(Wrapped<T>::width) & uintbits)
{
  return BitsToType<T>(TypeToBits(uintbits));
}

#endif
