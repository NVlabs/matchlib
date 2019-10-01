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

#ifndef NVHLS_CONNECTIONS_UTILS_H_
#define NVHLS_CONNECTIONS_UTILS_H_

#include <nvhls_assert.h>

// Map CONNECTIONS_ASSERT_MSG onto NVHLS_ASSERT_MSG to support CTC_SKIP_ASSER
#define CONNECTIONS_ASSERT_MSG(X,MSG) NVHLS_ASSERT_MSG(X,MSG)
#define CONNECTIONS_SIM_ONLY_ASSERT_MSG(X,MSG) CMOD_ASSERT_MSG(X,MSG)


#endif // ifndef NVHLS_CONNECTIONS_UTILS_H_
