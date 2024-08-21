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

#ifndef NVHLS_ASSERT_H
#define NVHLS_ASSERT_H

#include <systemc.h>
#include <hls_globals.h>

// the following is to avoid "statement has no effect"
// during compilation when CTC coverage is disabled.
// When coverage is enabled, these warnings are present.
#ifdef COV_ENABLE
   #define CTC_SKIP_ASSERT "CTC SKIP";
   #define CTC_ENDSKIP_ASSERT "CTC ENDSKIP";
#else
   #define CTC_SKIP_ASSERT ((void)"CTC SKIP");
   #define CTC_ENDSKIP_ASSERT ((void)"CTC ENDSKIP");
#endif

// Some common phrases
#define __NVHLS_ASSERT(X) CTC_SKIP_ASSERT assert(X); CTC_ENDSKIP_ASSERT
#define __NVHLS_ASSERT_SC(X) CTC_SKIP_ASSERT sc_assert(X); CTC_ENDSKIP_ASSERT
#define __NVHLS_ASSERT_NULL CTC_SKIP_ASSERT ((void)0); CTC_ENDSKIP_ASSERT
#define __NVHLS_ASSERT_SC_REPORT_ERROR(X,MSG)                     \
  if (!(X)) {                                                     \
    DCOUT("Assertion failed. " << MSG);            \
    SC_REPORT_ERROR("NVHLS_ASSERT", "Assertion failed.");  \
  }

/**
 * \def NVHLS_ASSERT(x)
 * \ingroup Assertions
 * Synthesizable assertion to check \a x. It will be synthesized by Catapult HLS tool to either psl or ovl assertions in RTL depending on HLS tool settings. If HLS_CATAPULT flag is not set, then assertions are enabled only in SystemC/C++ simulation and not synthesized to RTL.
 * \par A Simple Example
 * \code
 *      #include <nvhls_assert.h>
 *
 *      ...
 *      while(1) {
 *        if (in.PopNB(in_var)) {
 *          NVHLS_ASSERT(in_var!=0); // Assert that input is never 0
 *        ...
 *        }
 *      }
 * \endcode
 * \par
 */

#ifdef NVHLS_CONTINUE_ON_ASSERT
  #define NVHLS_ASSERT(X) __NVHLS_ASSERT_SC_REPORT_ERROR(X,"")
#else
  #ifdef HLS_CATAPULT
    #include <ac_assert.h>
    #define NVHLS_ASSERT(X) __NVHLS_ASSERT(X)
  #else
    #ifndef __SYNTHESIS__
      #define NVHLS_ASSERT(X) __NVHLS_ASSERT_SC(X)
    #else
      #define NVHLS_ASSERT(X) __NVHLS_ASSERT_NULL
    #endif
  #endif
#endif


/**
 * \def CMOD_ASSERT(x)
 * \ingroup Assertions
 * Non-synthesizable assertion to check \a x. It will be checked only in C++ simulation and not synthesized by HLS tool.
 * \par A Simple Example
 * \code
 *      #include <nvhls_assert.h>
 *
 *      ...
 *      while(1) {
 *        if (in.PopNB(in_var)) {
 *          CMOD_ASSERT(in_var!=0); // Assert that input is never 0
 *        ...
 *        }
 *      }
 * \endcode
 * \par
 */

#ifndef __SYNTHESIS__
  #ifdef NVHLS_CONTINUE_ON_ASSERT
    #define CMOD_ASSERT(X) __NVHLS_ASSERT_SC_REPORT_ERROR(X,"")
  #else
    #define CMOD_ASSERT(X) __NVHLS_ASSERT(X)
  #endif
#else
  #define CMOD_ASSERT(X) __NVHLS_ASSERT_NULL
#endif

/**
 * \def NVHLS_ASSERT_MSG(x, msg)
 * \ingroup Assertions
 * Synthesizable assertion to check \a x and print \a msg if assertion fails. It will be synthesized by Catapult HLS tool to either psl or ovl assertions in RTL depending on HLS tool settings. If HLS_CATAPULT flag is not set, then assertions are enabled only in SystemC/C++ simulation and not synthesized to RTL.
 * \par A Simple Example
 * \code
 *      #include <nvhls_assert.h>
 *
 *      ...
 *      while(1) {
 *        if (in.PopNB(in_var)) {
 *          NVHLS_ASSERT_MSG(in_var!=0, "Input is Zero"); // Assert that input is never 0
 *        ...
 *        }
 *      }
 * \endcode
 * \par
 */

#ifdef NVHLS_CONTINUE_ON_ASSERT
  #define NVHLS_ASSERT_MSG(X,MSG) __NVHLS_ASSERT_SC_REPORT_ERROR(X,MSG)
#else
  #ifdef HLS_CATAPULT
    #include <ac_assert.h>
    #define NVHLS_ASSERT_MSG(X,MSG) __NVHLS_ASSERT(X && MSG)
  #else
    #ifndef __SYNTHESIS__
      #define NVHLS_ASSERT_MSG(X,MSG) __NVHLS_ASSERT_SC(X && MSG)
    #else
      #define NVHLS_ASSERT_MSG(X,MSG) __NVHLS_ASSERT_NULL
    #endif
  #endif
#endif


/**
 * \def CMOD_ASSERT_MSG(x,msg)
 * \ingroup Assertions
 * Non-synthesizable assertion to check \a x and print \a msg if assertion fails. It will be checked only in C++ simulation and not synthesized by HLS tool.
 * \par A Simple Example
 * \code
 *      #include <nvhls_assert.h>
 *
 *      ...
 *      while(1) {
 *        if (in.PopNB(in_var)) {
 *          CMOD_ASSERT_MSG(in_var!=0, "Input is Zero"); // Assert that input is never 0
 *        ...
 *        }
 *      }
 * \endcode
 * \par
 */

#ifndef __SYNTHESIS__
  #ifdef NVHLS_CONTINUE_ON_ASSERT
    #define CMOD_ASSERT_MSG(X,MSG) __NVHLS_ASSERT_SC_REPORT_ERROR(X,MSG)
  #else
    #define CMOD_ASSERT_MSG(X,MSG)                          \
      if (!(X)) {                                           \
        DCOUT("Error: Assertion failed. " << MSG << endl);  \
      }                                                     \
      __NVHLS_ASSERT(X)
  #endif
#else
  #define CMOD_ASSERT_MSG(X,MSG) __NVHLS_ASSERT_NULL
#endif


#endif
