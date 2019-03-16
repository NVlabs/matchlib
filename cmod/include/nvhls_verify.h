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

/**
 * \file nvhls_verify.h
 * \ingroup NVHLSVerify
 *
 * Replacement for Catapult's mc_verify.h, including their CCS_DESIGN() macros. NVHLS_VERIFY_BLOCKS list of units to be supported for co-simulation by NVHLS_DESIGN() And NVHLS_DESIGN_IN_CHIP() should be specified before including nvhls_verify.h. Only need to include the top-level units that will be directly under SystemC in co-simulation. Again, it's important to specify this immediately _before_ including nvhls_verify, because there may be multiple calls to nvhls_verify.h within a full design. Do not include any intermediate header files or specify any other defines in between.
 *
 * \par A Simple Example
 * \code
 *      #define NVHLS_VERIFY_BLOCKS (UnitA)(UnitB)(UnitC)
 *      #include <nvhls_verify.h>
 *
 *      ...
 *      NVHLS_DESIGN(UnitA) my_unita; // Instance
 *      NVHLS_DESIGN_IN_CHIP(UnitA)* my_unita_ptr = new NVHLS_DESIGN_IN_CHIP(UnitA)(); // Pointer and dynamic allocation
 *
 * \endcode
 * \par
 */
/**
 * \def NVHLS_DESIGN(X)
 * \ingroup NVHLSVerify
 * Debug Specifies a unit that is the device-under-test within a testbench. This will be the DUT for SCVerify, and is only supported with SystemC. Use this macro within a testbench, and use the NVHLS_DESIGN_IN_CHIP() if it is within the design files.
 *
 * List of units to be supported for co-simulation by NVHLS_DESIGN() And NVHLS_DESIGN_IN_CHIP() should be specified in NVHLS_VERIFY_BLOCKS. Only need to include the top-level units that will be directly under SystemC in co-simulation. It's important to specify this immediately _before_ including nvhls_verify. Do not include any intermediate header files or specify any other defines in between.
 *
 * \par A Simple Example
 * \code
 *      #define NVHLS_VERIFY_BLOCKS (UnitA)(UnitB)(UnitC)
 *      #include <nvhls_verify.h>
 *
 *      ...
 *      NVHLS_DESIGN(UnitA) my_unita; // Instance
 *      NVHLS_DESIGN(UnitA)* my_unita_ptr = new NVHLS_DESIGN(UnitA)(); // Pointer and dynamic allocation
 *
 * \endcode
 * \par
 */
/**
 * \def NVHLS_DESIGN_IN_CHIP(X)
 * \ingroup NVHLSVerify
 * Debug Specifies a unit that could be switched between SystemC and RTL in co-simulation. Use this macro within the design files, and use the NVHLS_DESIGN() macro if it is within the testbench.
 *
 * List of units to be supported for co-simulation by NVHLS_DESIGN() And NVHLS_DESIGN_IN_CHIP() should be specified in NVHLS_VERIFY_BLOCKS. Only need to include the top-level units that will be directly under SystemC in co-simulation. It's important to specify this immediately _before_ including nvhls_verify. Do not include any intermediate header files or specify any other defines in between.
 *
 * \par A Simple Example
 * \code
 *      #define NVHLS_VERIFY_BLOCKS (UnitA)(UnitB)(UnitC)
 *      #include <nvhls_verify.h>
 *
 *      ...
 *      NVHLS_DESIGN_IN_CHIP(UnitA) my_unita; // Instance
 *      NVHLS_DESIGN_IN_CHIP(UnitA)* my_unita_ptr = new NVHLS_DESIGN_IN_CHIP(UnitA)(); // Pointer and dynamic allocation
 *
 * \endcode
 * \par
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !BOOST_PP_IS_ITERATING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <mc_scverify.h>  //needed for scverify to work, and still needed in vcsmx for the wrapper

// If we're not running synthesis, rerun preprocessor regardless of
// simulation type.
#ifndef __SYNTHESIS__
#include <boost/preprocessor.hpp>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define BOOST_PP_FILENAME_1 "nvhls_verify.h"
#define BOOST_PP_ITERATION_LIMITS \
  (0, BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(NVHLS_VERIFY_BLOCKS)))

#include BOOST_PP_ITERATE()

#endif // ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif

/////////////////////////////////////////////////////////////////////////////
// If this is running in CCS_SCVERIFY (instead of standalone g++ or vcsmx)
#ifndef NVHLS_VERIFY_ISVCSMX

// This section just defines a shortcut macro to be more consistent with how
// SCVERIFY CCS_DESIGN worked
#ifndef __NVHLS_VERIFY__H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __NVHLS_VERIFY__H
#endif // ifndef DOXYGEN_SHOULD_SKIP_THIS

#define NVHLS_DESIGN(X) CCS_DESIGN(X) /**< \hideinitializer */

// If synthesis, be don't use typedef
#ifdef __SYNTHESIS__
#define NVHLS_DESIGN_IN_CHIP(X) X /**< \hideinitializer */
#else
#define NVHLS_DESIGN_IN_CHIP(X) NVHLS_DESIGN_##X /**< \hideinitializer */
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
#else  // NVHLS_VERIFY_ISVCSMX

// We are running the nvhls_verify stand_sim flow.

/* #include <boost/preprocessor.hpp> */

/* #define BOOST_PP_FILENAME_1 "nvhls_verify.h" */
/* #define BOOST_PP_ITERATION_LIMITS \ */
/*   (0, BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(NVHLS_VERIFY_BLOCKS))) */

/* #include BOOST_PP_ITERATE() */

// This section just defines a shortcut macro to be more consistent with how
// SCVERIFY CCS_DESIGN worked
#ifndef __NVHLS_VERIFY__H
#define __NVHLS_VERIFY__H

#define NVHLS_DESIGN(X) NVHLS_DESIGN_##X
#define NVHLS_DESIGN_IN_CHIP(X) NVHLS_DESIGN_##X

#endif

#endif  // NVHLS_VERIFY_ISVCSMX
/////////////////////////////////////////////////////////////////////////////

// Undefine this so that nested uses of nvhls_verify.h doesn't throw warnings
#undef NVHLS_VERIFY_BLOCKS

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else  // !BOOST_PP_IS_ITERATING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CURRENT BOOST_PP_SEQ_ELEM(BOOST_PP_ITERATION(), NVHLS_VERIFY_BLOCKS)
#define CURRENT_TYPE BOOST_PP_CAT(NVHLS_DESIGN_, CURRENT)
#define CURRENT_WRAP_DEF BOOST_PP_CAT(BOOST_PP_CAT(__WRAP_, CURRENT), __)
#define CURRENT_WRAP_CLASS BOOST_PP_CAT(CURRENT, _vcsmx_wrapper)
//#define CURRENT_MAX BOOST_PP_CAT(CURRENT, _MAX)
//#define FOO         BOOST_PP_CAT(NVHLS_DESIGN_, CURRENT)

#if CURRENT_WRAP_DEF && defined(NVHLS_VERIFY_ISVCSMX)

// If this will be a Verilog module in vcsmx, include the wrapper and typedef
// it.
#include BOOST_PP_STRINGIZE(BOOST_PP_CAT(CURRENT, _wrapper.h))
typedef CURRENT_WRAP_CLASS CURRENT_TYPE;

#else

// If it is the normal module name.
typedef CURRENT CURRENT_TYPE;

#endif

// Leave no trace
#undef CURRENT
#undef CURRENT_TYPE
#undef CURRENT_WRAP_DEF
#undef CURRENT_WRAP_CLASS

#endif  // !BOOST_PP_IS_ITERATING
