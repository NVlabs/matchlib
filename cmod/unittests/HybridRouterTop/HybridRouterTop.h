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

#ifndef __HYBRIDROUTERTOP_H__
#define __HYBRIDROUTERTOP_H__

#include <systemc.h>
#include <nvhls_connections.h>
#include <nvhls_packet.h>
#include "RouterSpec.h"
#include <HybridRouter.h>

SC_MODULE(HybridRouterTop) {
 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  static const int NUM_LPORTS = spec::noc::numLPorts;
  static const int NUM_RPORTS = spec::noc::numRPorts;
  static const int NUM_VCHANNELS = spec::noc::numVChannels;
  static const int BUFFERSIZE = spec::noc::bufferSize;
  static const int MAX_HOPS = spec::noc::maxHops;
  static const int packetIDWidth = spec::noc::packetIDWidth;
  static const int flitDataWidth = spec::noc::flitDataWidth;

  static const int NoC2Dests = spec::kNumNoC2Routers;
  static const int NoCDests = spec::kNumNoCDests;
  static const int NoCMCastDests = spec::kNumNoCRouters;

  enum {
    NumPorts = NUM_LPORTS + NUM_RPORTS,
    NumCredits = (NUM_LPORTS + NUM_RPORTS) * NUM_VCHANNELS
  };
  typedef Flit<flitDataWidth, 0, 0, packetIDWidth, FlitId2bit, WormHole> Flit_t;
  typedef HybridRouter<NUM_LPORTS, NUM_RPORTS, BUFFERSIZE, Flit_t, NoC2Dests,
                       NoCDests, NoCMCastDests, spec::noc::maxPktSize> Router;

  Router router;
  typedef Router::NoCRouteLUT_t NoCRouteLUT_t;
  typedef Router::NoC2RouteLUT_t NoC2RouteLUT_t;
  typedef Router::noc2_id_t noc2_id_t;
  typedef spec::noc::Credit_t Credit_t;
  typedef spec::noc::Credit_ret_t Credit_ret_t;
  Connections::In<Flit_t> in_port[NumPorts];
  Connections::Out<Flit_t> out_port[NumPorts];

  // Jtag
  static const int NOC_DEST_WIDTH = Router::NoC_dest_width;
  static const int NOC2_DEST_WIDTH = Router::NoC2_dest_width;
  static const int NOC_UCAST_DEST_WIDTH = Router::NoC_ucast_dest_width;
  static const int NOC2_UCAST_DEST_WIDTH = Router::NoC2_ucast_dest_width;
  static const int NOC_MCAST_DEST_WIDTH = Router::NoC_mcast_dest_width;
  static const int NOC2_MCAST_DEST_WIDTH = Router::NoC2_mcast_dest_width;

  sc_in<NoCRouteLUT_t> NoCRouteLUT_jtag[NOC_DEST_WIDTH];
  sc_in<NoC2RouteLUT_t> NoC2RouteLUT_jtag[NOC2_DEST_WIDTH];
  sc_in<NVUINTC(spec::kNoC2DestWidth)> noc2_id_jtag;

  SC_HAS_PROCESS(HybridRouterTop);
  HybridRouterTop(sc_module_name name)
      : sc_module(name), clk("clk"), rst("rst"), router("router") {
    router.clk(clk);
    router.rst(rst);

    for (int i = 0; i < NOC_DEST_WIDTH; i++)
      router.NoCRouteLUT_jtag[i](NoCRouteLUT_jtag[i]);
    for (int i = 0; i < NOC2_DEST_WIDTH; i++)
      router.NoC2RouteLUT_jtag[i](NoC2RouteLUT_jtag[i]);
    router.noc2_id_jtag(noc2_id_jtag);

    for (int i = 0; i < NumPorts; i++) {
      router.in_port[i](in_port[i]);
      router.out_port[i](out_port[i]);
    }
  }
};

#endif
