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
// Use these macros to print debugging information which gets compiled out when
// synthesizing.

#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

// the following is to avoid "statement has no effect"
// during compilation when CTC coverage is disabled.
// When coverage is enabled, these warnings are present.
#ifdef COV_ENABLE
   #define CTC_SKIP_CDCOUT "CTC SKIP";
   #define CTC_ENDSKIP_CDCOUT "CTC ENDSKIP";
#else
   #define CTC_SKIP_CDCOUT ((void)"CTC SKIP");
   #define CTC_ENDSKIP_CDCOUT ((void)"CTC ENDSKIP");
#endif

#define DISABLED_PRINT_STMT() \
  do {                        \
  } while (0)
/**
 * \def DCOUT(x)
 * \ingroup DebugPrint
 * Debug print statements to print \a x. It is enabled in SystemC/C++ simulation but disabled in HLS as they are non-synthesizable. 
 * \par A Simple Example
 * \code
 *      #include <hls_globals.h>
 *
 *      ...
 *      DCOUT("Starting SystemC simulation\n"); // Always prints in SystemC/C++ simulation
 *
 * \endcode
 * \par
 */
/**
 * \def CDCOUT(x,y)
 * \ingroup DebugPrint
 * Debug print statements to print \a x if DEBUG_LEVEL set during compilation is greater than or equal to \a y. This is useful is we want to limit print statements during debugging. It is also enabled in SystemC/C++ simulation but disabled in HLS as they are non-synthesizable. 
 * \par A Simple Example
 * \code
 *      #include <hls_globals.h>
 *
 *      ...
 *      CDCOUT("Starting SystemC simulation with debug level:" << y << endl); // Prints if DEBUG_LEVEL >= y 
 *
 * \endcode
 * \par
 */

#if __SYNTHESIS__
   #define DCOUT(x) DISABLED_PRINT_STMT()
   #define CDCOUT(x,y) DISABLED_PRINT_STMT()
#else
   #define DCOUT(x) cout << x
   #define CDCOUT(x,y) \
      CTC_SKIP_CDCOUT \
         if (DEBUG_LEVEL >= y) cout << x; \
      CTC_ENDSKIP_CDCOUT
#endif
