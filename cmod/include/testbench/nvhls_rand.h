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

#ifndef __NVHLS_RAND__
#define __NVHLS_RAND__

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <mem_array.h>
#include <TypeToBits.h>

namespace nvhls {
/**
 * \brief Set random seed 
 * \ingroup set_random_seed
 * 
 * \par Overview
 *      A function to set the random seed in the testbench. The seed is set based on the following prioritization:
 *        -# The setting of the RAND_SEED environment variable at runtime
 *        -# The setting of the RAND_SEED CFLAG at compile time (or via #define in a source file)
 *        -# A random seed based on the unix timestamp at runtime
 *
 * \par A Simple Example
 * \code
 *   #include <nvhls_rand.h>
 *
 *   int sc_main(int argc, char *argv[]) {
 *       nvhls::set_random_seed();
 *       testbench tb("tb");
 *       sc_report_handler::set_actions(SC_ERROR, SC_DISPLAY);
 *       sc_start();
 *       bool rc = (sc_report_handler::get_count(SC_ERROR) > 0);
 *       if (rc)
 *         DCOUT("TESTBENCH FAIL" << endl);
 *       else
 *         DCOUT("TESTBENCH PASS" << endl);
 *       return rc;
 *   };
 *      
 * \endcode
 * \par
 *
 */

inline int set_random_seed() {
  unsigned int seed = time(NULL);
#ifdef RAND_SEED  
  seed = (RAND_SEED);
#endif
  const char* env_rand_seed = std::getenv("RAND_SEED");
  if (env_rand_seed != NULL) seed = atoi(env_rand_seed);
  srand(seed);
  cout << "================================" << endl;
  cout << dec << "SETTING RANDOM SEED = " << seed << endl;
  cout << "================================" << endl;
  return seed;
}

/**
 * \brief Generate Random payload of any type 
 * \ingroup gen_random_payload
 * 
 * \par Overview
 *      Function to generate a random payload for desired type.
 *
 * \par A Simple Example
 * \code
 *   #include <nvhls_rand.h>
 *  
 *   ...
 *   payload_t data = nvhls::gen_random_payload<payload_t>();
 *      
 * \endcode
 * \par
 *
 */
template < typename Payload>
Payload gen_random_payload() {
  Payload payload;
  NVUINTW(Payload::width) random = rand();
  int num = Payload::width/32 + 1;
  for (int i = 0; i < num; i++) {
    random = (random << 32) + rand();
  }
  sc_lv<Payload::width> payload_lv = TypeToBits<NVUINTW(Payload::width) > (random);
  payload = BitsToType<Payload> (payload_lv);
  return payload;
}
/**
 * \brief Generate Random integer value of desired width
 * \ingroup get_rand
 * 
 * \par Overview
 *      Function to generate a random value of desired width
 *
 * \par A Simple Example
 * \code
 *   #include <nvhls_rand.h>
 *  
 *   ...
 *   NVUINTW(width) data = nvhls::gen_rand<width>();
 *      
 * \endcode
 * \par
 *
 */
template < int bitwidth>
NVUINTW(bitwidth) get_rand() {
  NVUINTW(bitwidth) random = rand();
  int num = bitwidth/32 + 1;
  for (int i = 0; i < num; i++) {
    random = (random << 32) + rand();
  }
  return random;
}

}
#endif
