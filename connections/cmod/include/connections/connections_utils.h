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

// Define the preferred assert mechanism for Connnections library.
#ifndef CONNECTIONS_ASSERT

#ifdef HLS_CATAPULT
#include <ac_assert.h>
#define CONNECTIONS_ASSERT(X) assert(X);
#else // ifdef HLS_CATAPULT
  #ifndef __SYNTHESIS__
    #define CONNECTIONS_ASSERT(X) sc_assert(X);
  #else
    #define CONNECTIONS_ASSERT(X) ((void)0);
  #endif
#endif // ifdef HLS_CATAPULT

#endif // ifndef CONNECTIONS_ASSERT

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

// Sim only assetion.
#ifndef CONNECTIONS_SIM_ASSERT

#ifndef __SYNTHESIS__
#define CONNECTIONS_SIM_ASSERT(x) assert(x);
#else 
#define CONNECTIONS_SIM_ASSERT(x) ((void)0);
#endif

#endif // ifndef CONNECTIONS_SIM_ASSERT

/**
 * \brief CONNECTIONS_CONCAT define: Concatenate two strings, separate with an underscore.
 * \ingroup connections_module
 *
 * Useful for concatenating two strings to derive a module name.
 */
#ifndef CONNECTIONS_CONCAT

#define CONNECTIONS_CONCAT(s1,s2) (std::string(s1) + "_" + std::string(s2)).c_str()

#endif // ifndef CONNECTIONS_CONCAT

namespace Connections {

  // Intentionally empty for now. Placeholder for any classes that we may want to override later. (e.g. Fifo for BufferedPorts)

}  // namespace Connections

#endif  // __CONNECTIONS__CONNECTIONS_UTILS_H_

