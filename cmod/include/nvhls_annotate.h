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

#ifndef NVHLS_ANNOTATE_H_
#define NVHLS_ANNOTATE_H_

#include <nvhls_connections_utils.h>

#include <connections/annotate.h>

namespace nvhls {
  void annotate_design(const sc_object &root, std::string base_name = "", std::string input_dir_path = "", std::string output_dir_path = "") {
    Connections::annotate_design(root, base_name, input_dir_path, output_dir_path);
  }
}

#endif // NVHLS_ANNOTATE_H_
