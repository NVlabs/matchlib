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

#include <mc_scverify.h>

/**
 * \file match_scverify.h
 * \ingroup NVHLSVerify
 *
 * When using Catapult's scverify CCS_MAIN macro, include this file instead of #include <mc_scverify.h> so that sc_main exists when the design is linked against the systemc library.
 *
 * \par A Simple Example of a testbench.cpp
 * \code
 *      #include <match_scverify.h>
 *
 *      ...
 *
 *     CCS_MAIN(int argc, char *argv[]) 
 *     {
 *        CCS_RETURN(0) ;
 *     }
 * \endcode
 * \par
 */


#if defined(CCS_SYSC) || !defined(CCS_SCVERIFY)
extern "C" int sc_main(int argc, char *argv[]) {
 return -1;
}
#endif
