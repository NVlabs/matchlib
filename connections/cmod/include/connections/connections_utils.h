/*
 * Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
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
//========================================================================
// connections_utils.h
//
// Define macros and typedefs to use within Connections. May be tool or
// library dependent.
//========================================================================

#ifndef __CONNECTIONS__CONNECTIONS_UTILS_H_
#define __CONNECTIONS__CONNECTIONS_UTILS_H_

#include <systemc>

/**
 * \def CONNECTIONS_ASSERT_MSG(x, msg)
 * \ingroup Assertions
 * Synthesizable assertion to check \a x and print \a msg if assertion fails. It will be synthesized by Catapult HLS tool to either psl or ovl assertions in RTL depending on HLS tool settings 
 * \par A Simple Example
 * \code
 *      #include <connections_utils.h>
 *
 *      ...
 *      while(1) {
 *        if (in.PopNB(in_var)) {
 *          CONNECTIONS_ASSERT_MSG(in_var!=0, "Input is Zero"); // Assert that input is never 0
 *        ...
 *        }
 *      }
 * \endcode
 * \par
 */

#ifdef HLS_CATAPULT
#include <ac_assert.h>

  #ifndef CONNECTIONS_ASSERT_MSG
#define CONNECTIONS_ASSERT_MSG(X,MSG) \
     if (!(X)) { \
       CONNECTIONS_COUT("Assertion Failed. " << MSG << endl);  \
     }\
     assert(X);
  #endif // ifndef CONNECTIONS_ASSERT_MSG

#else // ifdef HLS_CATAPULT

  #ifndef __SYNTHESIS__
    #ifndef CONNECTIONS_ASSERT_MSG
    #define CONNECTIONS_ASSERT_MSG(X,MSG)  \
     if (!(X)) { \
       CONNECTIONS_COUT("Assertion Failed. " << MSG << endl);  \
     } \
     sc_assert(X);
    #endif // ifndef CONNECTIONS_ASSERT_MSG
  #else
    #ifndef CONNECTIONS_ASSERT_MSG
    #define CONNECTIONS_ASSERT_MSG(X,MSG) ((void)0);
    #endif // ifndef CONNECTIONS_ASSERT_MSG
  #endif

#endif // ifdef HLS_CATAPULT


/**
 * \def CONNECTIONS_SIM_ONLY_ASSERT_MSG(x, msg)
 * \ingroup Assertions
 * Non-synthesizable assertion to check \a x and print \a msg if assertion fails.
 * \par A Simple Example
 * \code
 *      #include <connections_utils.h>
 *
 *      ...
 *      while(1) {
 *        if (in.PopNB(in_var)) {
 *          CONNECTIONS_SIM_ONLY_ASSERT_MSG(in_var!=0, "Input is Zero"); // Assert that input is never 0
 *        ...
 *        }
 *      }
 * \endcode
 * \par
 */
#ifndef CONNECTIONS_SIM_ONLY_ASSERT_MSG
#ifndef __SYNTHESIS__
#define CONNECTIONS_SIM_ONLY_ASSERT_MSG(X,MSG)	\
  if (!(X)) {						       \
    CONNECTIONS_COUT("Assertion Failed. " << MSG << endl);     \
  }							       \
  sc_assert(X);
#else
#define CONNECTIONS_SIM_ONLY_ASSERT_MSG(X,MSG) ((void)0);
#endif // ifdef HLS_CATAPULT
#endif // ifndef CONNECTIONS_SIM_ONLY_ASSERT_MSG


// Define preferred debug mechanism.
// FIXME: Change this to not support << but be a string instead, so can be pushed over to
// sc_report as well.
#ifndef CONNECTIONS_COUT
#if __SYNTHESIS__
   #define CONNECTIONS_COUT(x) ((void)0);
#else
   #define CONNECTIONS_COUT(x) cout << x
#endif
#endif // ifndef CONNECTIONS_COUT

/**
 * \brief CONNECTIONS_CONCAT define: Concatenate two strings, separate with an underscore.
 * \ingroup connections_module
 *
 * Useful for concatenating two strings to derive a module name.
 *
 * Catapult synthesis in C++11 mode currently doesn't support + operators on
 * strings, so only s2 is used.
 */
#ifndef CONNECTIONS_CONCAT

#if defined(__SYNTHESIS__) && defined(HLS_CATAPULT) && __cplusplus >= 201103L
#define CONNECTIONS_CONCAT(s1,s2) (std::string(s2)).c_str()
#else
#define CONNECTIONS_CONCAT(s1,s2) (std::string(s1) + "_" + std::string(s2)).c_str()
#endif

#endif // ifndef CONNECTIONS_CONCAT

namespace Connections {

  // Intentionally empty for now. Placeholder for any classes that we may want to override later. (e.g. Fifo for BufferedPorts)

}  // namespace Connections

#endif  // __CONNECTIONS__CONNECTIONS_UTILS_H_

