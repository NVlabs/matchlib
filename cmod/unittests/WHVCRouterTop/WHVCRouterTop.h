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

#ifndef __WHVCROUTERTOP_H__
#define __WHVCROUTERTOP_H__

#include <systemc.h>
#include <nvhls_connections.h>
#include <nvhls_packet.h>
#include <WHVCRouter.h>

SC_MODULE(WHVCRouterTop) {
 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  enum { 
    kNumVChannels = 1,
    kBufferSize = 8,
    kFlitDataWidth = 64,
    kFlitIDWidth = 2,
    kNumLPorts = 1,
    kNumRPorts = 4,
    kNumMaxHops = 16,
    kLogBufferSize = nvhls::index_width<kBufferSize+1>::val,
    kNumPorts = kNumLPorts + kNumRPorts,
    kNumCredits = (kNumLPorts + kNumRPorts)*kNumVChannels
  };
  typedef NVUINTC(kLogBufferSize) Credit_t;
  typedef NVUINTC(1) Credit_ret_t;

  typedef Flit<64, 0, 0, 0, FlitId2bit, WormHole> Flit_t;
  WHVCSourceRouter<kNumLPorts, kNumRPorts, kNumVChannels, kBufferSize, Flit_t, kNumMaxHops> router;

  Connections::In<Flit_t> in_port[kNumPorts];
  Connections::Out<Flit_t> out_port[kNumPorts];
  Connections::In<Credit_ret_t> in_credit[kNumCredits];
  Connections::Out<Credit_ret_t> out_credit[kNumCredits];

  SC_HAS_PROCESS(WHVCRouterTop);
  WHVCRouterTop(sc_module_name name)
      : sc_module(name), clk("clk"), rst("rst"), router("router") {
    router.clk(clk);
    router.rst(rst);

    for (int i = 0; i < kNumPorts; i++) {
      router.in_port[i](in_port[i]);
      router.out_port[i](out_port[i]);
      for (int j = 0; j < kNumVChannels; j++) {
        router.in_credit[i * kNumVChannels + j](
            in_credit[i * kNumVChannels + j]);
        router.out_credit[i * kNumVChannels + j](
            out_credit[i * kNumVChannels + j]);
      }
    }
  }
};

#endif
