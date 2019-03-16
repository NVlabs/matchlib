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
#ifndef HLS_TYPEDEF_H
#define HLS_TYPEDEF_H
#include <nvhls_int.h>

// Use the following when using NVUINT with template parameters
/**
 * \def NVUINTW(width)
 * \ingroup nvhls_int 
 * Macro to declare nvuint type with template parameter width 
 * \par A Simple Example
 * \code
 *      #include <nvhls_types.h>
 *
 *      template <unsigned NumInputLanes, unsigned NumOutputLanes>
 *      void func(NVUINTW(NumInputLanes), NVUINTW(NumOutputLanes));
 *
 * \endcode
 */

#define NVUINTW(width) typename nvhls::nvhls_t<width>::nvuint_t

/**
 * \def NVINTW(width)
 * \ingroup nvhls_int 
 * Macro to declare nvint type with template parameter width 
 * \par A Simple Example
 * \code
 *      #include <nvhls_types.h>
 *
 *      template <unsigned NumInputLanes, unsigned NumOutputLanes>
 *      void func(NVINTW(NumInputLanes), NVINTW(NumOutputLanes));
 *
 * \endcode
 */

#define NVINTW(width) typename nvhls::nvhls_t<width>::nvint_t

// Use the following when using NVUINT with constant parameters
/**
 * \def NVINTC(width)
 * \ingroup nvhls_int 
 * Macro to declare nvuint type with constant width 
 * \par A Simple Example
 * \code
 *      #include <nvhls_types.h>
 *      static const int NumInputs = 4;
 *      NVUINTC(NumInputs) input;
 *
 * \endcode
 */
#define NVUINTC(width) nvhls::nvhls_t<width>::nvuint_t
/**
 * \def NVUINTC(width)
 * \ingroup nvhls_int 
 * Macro to declare nvint type with constant width 
 * \par A Simple Example
 * \code
 *      #include <nvhls_types.h>
 *      static const int NumInputs = 4;
 *      NVINTC(NumInputs) input;
 *
 * \endcode
 */
#define NVINTC(width) nvhls::nvhls_t<width>::nvint_t

// Fixed width Unsigned INT types
typedef nvhls::nvhls_t<1>::nvuint_t NVUINT1;
typedef nvhls::nvhls_t<2>::nvuint_t NVUINT2;
typedef nvhls::nvhls_t<3>::nvuint_t NVUINT3;
typedef nvhls::nvhls_t<4>::nvuint_t NVUINT4;
typedef nvhls::nvhls_t<5>::nvuint_t NVUINT5;
typedef nvhls::nvhls_t<6>::nvuint_t NVUINT6;
typedef nvhls::nvhls_t<7>::nvuint_t NVUINT7;
typedef nvhls::nvhls_t<8>::nvuint_t NVUINT8;
typedef nvhls::nvhls_t<9>::nvuint_t NVUINT9;
typedef nvhls::nvhls_t<10>::nvuint_t NVUINT10;
typedef nvhls::nvhls_t<11>::nvuint_t NVUINT11;
typedef nvhls::nvhls_t<12>::nvuint_t NVUINT12;
typedef nvhls::nvhls_t<13>::nvuint_t NVUINT13;
typedef nvhls::nvhls_t<14>::nvuint_t NVUINT14;
typedef nvhls::nvhls_t<15>::nvuint_t NVUINT15;
typedef nvhls::nvhls_t<16>::nvuint_t NVUINT16;
typedef nvhls::nvhls_t<17>::nvuint_t NVUINT17;
typedef nvhls::nvhls_t<18>::nvuint_t NVUINT18;
typedef nvhls::nvhls_t<19>::nvuint_t NVUINT19;
typedef nvhls::nvhls_t<20>::nvuint_t NVUINT20;
typedef nvhls::nvhls_t<21>::nvuint_t NVUINT21;
typedef nvhls::nvhls_t<22>::nvuint_t NVUINT22;
typedef nvhls::nvhls_t<23>::nvuint_t NVUINT23;
typedef nvhls::nvhls_t<24>::nvuint_t NVUINT24;
typedef nvhls::nvhls_t<25>::nvuint_t NVUINT25;
typedef nvhls::nvhls_t<26>::nvuint_t NVUINT26;
typedef nvhls::nvhls_t<27>::nvuint_t NVUINT27;
typedef nvhls::nvhls_t<28>::nvuint_t NVUINT28;
typedef nvhls::nvhls_t<29>::nvuint_t NVUINT29;
typedef nvhls::nvhls_t<30>::nvuint_t NVUINT30;
typedef nvhls::nvhls_t<31>::nvuint_t NVUINT31;
typedef nvhls::nvhls_t<32>::nvuint_t NVUINT32;
typedef nvhls::nvhls_t<33>::nvuint_t NVUINT33;
typedef nvhls::nvhls_t<34>::nvuint_t NVUINT34;
typedef nvhls::nvhls_t<35>::nvuint_t NVUINT35;
typedef nvhls::nvhls_t<36>::nvuint_t NVUINT36;
typedef nvhls::nvhls_t<37>::nvuint_t NVUINT37;
typedef nvhls::nvhls_t<38>::nvuint_t NVUINT38;
typedef nvhls::nvhls_t<39>::nvuint_t NVUINT39;
typedef nvhls::nvhls_t<40>::nvuint_t NVUINT40;
typedef nvhls::nvhls_t<41>::nvuint_t NVUINT41;
typedef nvhls::nvhls_t<42>::nvuint_t NVUINT42;
typedef nvhls::nvhls_t<43>::nvuint_t NVUINT43;
typedef nvhls::nvhls_t<44>::nvuint_t NVUINT44;
typedef nvhls::nvhls_t<45>::nvuint_t NVUINT45;
typedef nvhls::nvhls_t<46>::nvuint_t NVUINT46;
typedef nvhls::nvhls_t<47>::nvuint_t NVUINT47;
typedef nvhls::nvhls_t<48>::nvuint_t NVUINT48;
typedef nvhls::nvhls_t<49>::nvuint_t NVUINT49;
typedef nvhls::nvhls_t<50>::nvuint_t NVUINT50;
typedef nvhls::nvhls_t<51>::nvuint_t NVUINT51;
typedef nvhls::nvhls_t<52>::nvuint_t NVUINT52;
typedef nvhls::nvhls_t<53>::nvuint_t NVUINT53;
typedef nvhls::nvhls_t<54>::nvuint_t NVUINT54;
typedef nvhls::nvhls_t<55>::nvuint_t NVUINT55;
typedef nvhls::nvhls_t<56>::nvuint_t NVUINT56;
typedef nvhls::nvhls_t<57>::nvuint_t NVUINT57;
typedef nvhls::nvhls_t<58>::nvuint_t NVUINT58;
typedef nvhls::nvhls_t<59>::nvuint_t NVUINT59;
typedef nvhls::nvhls_t<60>::nvuint_t NVUINT60;
typedef nvhls::nvhls_t<61>::nvuint_t NVUINT61;
typedef nvhls::nvhls_t<62>::nvuint_t NVUINT62;
typedef nvhls::nvhls_t<63>::nvuint_t NVUINT63;
typedef nvhls::nvhls_t<64>::nvuint_t NVUINT64;
typedef nvhls::nvhls_t<128>::nvuint_t NVUINT128;

// Fixed width Signed INT types
typedef nvhls::nvhls_t<1>::nvint_t NVINT1;
typedef nvhls::nvhls_t<2>::nvint_t NVINT2;
typedef nvhls::nvhls_t<3>::nvint_t NVINT3;
typedef nvhls::nvhls_t<4>::nvint_t NVINT4;
typedef nvhls::nvhls_t<5>::nvint_t NVINT5;
typedef nvhls::nvhls_t<6>::nvint_t NVINT6;
typedef nvhls::nvhls_t<7>::nvint_t NVINT7;
typedef nvhls::nvhls_t<8>::nvint_t NVINT8;
typedef nvhls::nvhls_t<9>::nvint_t NVINT9;
typedef nvhls::nvhls_t<10>::nvint_t NVINT10;
typedef nvhls::nvhls_t<11>::nvint_t NVINT11;
typedef nvhls::nvhls_t<12>::nvint_t NVINT12;
typedef nvhls::nvhls_t<13>::nvint_t NVINT13;
typedef nvhls::nvhls_t<14>::nvint_t NVINT14;
typedef nvhls::nvhls_t<15>::nvint_t NVINT15;
typedef nvhls::nvhls_t<16>::nvint_t NVINT16;
typedef nvhls::nvhls_t<17>::nvint_t NVINT17;
typedef nvhls::nvhls_t<18>::nvint_t NVINT18;
typedef nvhls::nvhls_t<19>::nvint_t NVINT19;
typedef nvhls::nvhls_t<20>::nvint_t NVINT20;
typedef nvhls::nvhls_t<21>::nvint_t NVINT21;
typedef nvhls::nvhls_t<22>::nvint_t NVINT22;
typedef nvhls::nvhls_t<23>::nvint_t NVINT23;
typedef nvhls::nvhls_t<24>::nvint_t NVINT24;
typedef nvhls::nvhls_t<25>::nvint_t NVINT25;
typedef nvhls::nvhls_t<26>::nvint_t NVINT26;
typedef nvhls::nvhls_t<27>::nvint_t NVINT27;
typedef nvhls::nvhls_t<28>::nvint_t NVINT28;
typedef nvhls::nvhls_t<29>::nvint_t NVINT29;
typedef nvhls::nvhls_t<30>::nvint_t NVINT30;
typedef nvhls::nvhls_t<31>::nvint_t NVINT31;
typedef nvhls::nvhls_t<32>::nvint_t NVINT32;
typedef nvhls::nvhls_t<33>::nvint_t NVINT33;
typedef nvhls::nvhls_t<34>::nvint_t NVINT34;
typedef nvhls::nvhls_t<35>::nvint_t NVINT35;
typedef nvhls::nvhls_t<36>::nvint_t NVINT36;
typedef nvhls::nvhls_t<37>::nvint_t NVINT37;
typedef nvhls::nvhls_t<38>::nvint_t NVINT38;
typedef nvhls::nvhls_t<39>::nvint_t NVINT39;
typedef nvhls::nvhls_t<40>::nvint_t NVINT40;
typedef nvhls::nvhls_t<41>::nvint_t NVINT41;
typedef nvhls::nvhls_t<42>::nvint_t NVINT42;
typedef nvhls::nvhls_t<43>::nvint_t NVINT43;
typedef nvhls::nvhls_t<44>::nvint_t NVINT44;
typedef nvhls::nvhls_t<45>::nvint_t NVINT45;
typedef nvhls::nvhls_t<46>::nvint_t NVINT46;
typedef nvhls::nvhls_t<47>::nvint_t NVINT47;
typedef nvhls::nvhls_t<48>::nvint_t NVINT48;
typedef nvhls::nvhls_t<49>::nvint_t NVINT49;
typedef nvhls::nvhls_t<50>::nvint_t NVINT50;
typedef nvhls::nvhls_t<51>::nvint_t NVINT51;
typedef nvhls::nvhls_t<52>::nvint_t NVINT52;
typedef nvhls::nvhls_t<53>::nvint_t NVINT53;
typedef nvhls::nvhls_t<54>::nvint_t NVINT54;
typedef nvhls::nvhls_t<55>::nvint_t NVINT55;
typedef nvhls::nvhls_t<56>::nvint_t NVINT56;
typedef nvhls::nvhls_t<57>::nvint_t NVINT57;
typedef nvhls::nvhls_t<58>::nvint_t NVINT58;
typedef nvhls::nvhls_t<59>::nvint_t NVINT59;
typedef nvhls::nvhls_t<60>::nvint_t NVINT60;
typedef nvhls::nvhls_t<61>::nvint_t NVINT61;
typedef nvhls::nvhls_t<62>::nvint_t NVINT62;
typedef nvhls::nvhls_t<63>::nvint_t NVINT63;
typedef nvhls::nvhls_t<64>::nvint_t NVINT64;
typedef nvhls::nvhls_t<128>::nvint_t NVINT128;

#endif
