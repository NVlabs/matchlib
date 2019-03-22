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
// nvhls_message.h
//========================================================================

#ifndef NVHLS_MESSAGE_H_
#define NVHLS_MESSAGE_H_

class nvhls_message {
 public:
  friend inline bool operator==(const nvhls_message& lhs, const nvhls_message& rhs) {
    return false;
  }

  inline friend std::ostream& operator<<(ostream& os, const nvhls_message& rhs)
  {
    os << "<unamed_nvhls_message>";
    return os;
  }

  inline friend void sc_trace(sc_core::sc_trace_file *& tf, const nvhls_message& rhs, const std::string& NAME) {
  }
};

#endif

