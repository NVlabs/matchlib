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

#ifndef NVHLS_CONNECTIONS_H_
#define NVHLS_CONNECTIONS_H_

#include <nvhls_connections_utils.h>

// Include connections tree nvhls_connections.h
#include <connections/connections.h>

// Include dependencies that we no longer include in Connections' nvhls_connections.h
#include <nvhls_packet.h>
#include <nvhls_assert.h>
#include <nvhls_module.h>
#include <fifo.h>

#include <nvhls_connections_buffered_ports.h>
#include <nvhls_connections_network.h>

#endif // ifndef NVHLS_CONNECTIONS_H_
