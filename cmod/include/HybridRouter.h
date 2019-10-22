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

#ifndef __HYBRIDROUTER_H__
#define __HYBRIDROUTER_H__

// WormHole router with virtual channel and Multicast support.
//   -Source Routing is assumed.
//   -First flit stores route information.
//   -Multi-cast to 1 remote port and many local ports.
//   -Changing the destination format and route computation can allow multi-cast
//   to all ports without distinction between local and remote ports.
//   -Port numbers: local ports are numbered 0 to L-1, remote ports are numbered
//   L to L+R-1.
//   -1 hot encoding for local destination - send flit to port i if dest[i]=1.
//   -Destination format per hop <Remote Dst> <Local Dst> .
//   -Destination Width per Hop = r_dest_port_width_per_hop +
//   l_dest_port_width_per_hop
//   -Remote destination width is log_num_rports
//   -Local destination width is num_lports
//   -First flit format:  <Dest-N> ... <Dest-2> <Dest-1>
//   -This implementation assumes that virtual channel information is specified
//   at the LSBs of packet id.
//   -Termination condition: if dest of next hop is 0

#include <nvhls_packet.h>
#include <nvhls_vector.h>
#include <Arbiter.h>
#include <hls_globals.h>
#include <fifo.h>
#include <one_hot_to_bin.h>
#include <nvhls_assert.h>
#include <crossbar.h>

enum WHRouting { SourceRouting, LUTRouting };
/**
 * \brief Wormhole Router with virtual channels
 * \ingroup WHVCRouter
 *
 * \tparam NumLPorts        Number of local ingress/egress ports
 * \tparam NumRports        Number of remote ports
 * \tparam NumVchannels     Number of virtual channels
 * \tparam BufferSize       Buffersize of input fifo
 * \tparam WHRouting        Type of routing (SourceRouting/LUTRouting)
 * \tparam NoC2Dests         Indicates the destination for hierarchical LUT routing (1st level)
 * \tparam NoCDests         Indicates the destination for hierarchical LUT routing (2nd level)
 * \tparam NoCMCastDests    Indicates the multi-cast destination for hierarchical LUT routing
 * \tparam FlitType         Indicates the Flit type
 *
 * \par A Simple Example
 * \code
 *      #include <WHVCRouter.h>
 *
 *      ...
 *
 *        typedef WHVCRouter<kNumLPorts, kNumRPorts, kNumVChannels, kBufferSize, kNumMaxHops, SourceRouting>::Flit_t Flit_t;
 *        WHVCRouter<kNumLPorts, kNumRPorts, kNumVChannels, kBufferSize, kNumMaxHops, SourceRouting, 0, 0, 0, Flit_t> router;
 *
 *        Connections::In<Flit_t> in_port[kNumPorts];
 *        Connections::Out<Flit_t> out_port[kNumPorts];
 *        Connections::In<Credit_t> in_credit[kNumCredits];
 *        Connections::Out<Credit_t> out_credit[kNumCredits];
 *
 *        for (int i = 0; i < kNumPorts; i++) {
 *          router.in_port[i](in_port[i]);
 *          router.out_port[i](out_port[i]);
 *          for (int j = 0; j < kNumVChannels; j++) {
 *            router.in_credit[i * kNumVChannels + j](
 *                in_credit[i * kNumVChannels + j]);
 *            router.out_credit[i * kNumVChannels + j](
 *                out_credit[i * kNumVChannels + j]);
 *          }
 *        }
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <int NumLPorts, int NumRports, int NumVchannels, int BufferSize,
          int MaxHops = 16, WHRouting = SourceRouting, int NoC2Dests = 0,
          int NoCDests = 0, int NoCMCastDests = 0,
          typename FlitType = Flit<64, 0, 0, 0, FlitId2bit, WormHole>,
          int MaxPacketSize = 0>
class WHVCRouter : public sc_module {};

template <int NumLPorts, int NumRports, int NumVchannels, int BufferSize,
          int MaxHops, typename FlitType>
class WHVCRouter<NumLPorts, NumRports, NumVchannels, BufferSize, MaxHops,
                 SourceRouting, 0, 0, 0, FlitType> : public sc_module {
 public:
  typedef FlitType Flit_t;
  // Declare constants
  enum {
    num_lports = NumLPorts,
    num_rports = NumRports,
    num_ports = num_lports + num_rports,
    num_vchannels = NumVchannels,
    log_num_vchannels = nvhls::index_width<num_vchannels>::val,
    log_num_lports = nvhls::index_width<num_lports>::val,
    log_num_rports = nvhls::index_width<num_rports>::val,
    log_num_ports = nvhls::index_width<num_ports>::val,
    dest_width_per_hop = log_num_rports + num_lports,
    dest_width = MaxHops * dest_width_per_hop,
    buffersize = BufferSize,
    log_buffersize = nvhls::index_width<buffersize>::val,
    log_buffersizeplus1 = nvhls::index_width<buffersize + 1>::val
  };
  typedef NVUINTW(log_buffersizeplus1) Credit_t;
  typedef NVUINTW(1) Credit_ret_t;

  // Input / Output ports
  Connections::In<Flit_t> in_port[num_ports];
  Connections::Out<Flit_t> out_port[num_ports];

  // Credits
  // Send out credit when buffer is popped
  // Increment local credit when credits are read
  // 1 credit counter per output port
  Connections::In<Credit_ret_t> in_credit[num_ports * num_vchannels];
  Connections::Out<Credit_ret_t> out_credit[num_ports * num_vchannels];
  sc_in_clk clk;
  sc_in<bool> rst;

  // Input FIFO Buffer
  FIFO<Flit_t, buffersize, num_ports * num_vchannels> ififo;

  Arbiter<num_ports> arbiter[num_ports];

  // Credit registers to store credits for both producer and consumer
  Credit_t credit_recv[num_ports * num_vchannels];
  Credit_t credit_send[num_ports * num_vchannels];

  // Store output destination for each port per virtual channel with 1hot
  // encoding
  NVUINTW(num_ports) out_dest[num_ports][num_vchannels];

  // Variable to register outputs
  Flit_t flit_out[num_ports];

  // Variable to block output port until previous push is successful
  NVUINTW(num_ports) out_stall;

  // Variable that indicates if the output port is available to get new packet
  bool is_get_new_packet[num_ports * num_vchannels];

  // Constructor
  SC_HAS_PROCESS(WHVCRouter);
  WHVCRouter(sc_module_name name_) : sc_module(name_) {
    SC_THREAD(process);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }

  // Function to receive credits from the consumers
  void receive_credit() {
    Credit_ret_t credit_in;
#pragma hls_unroll yes
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      if (in_credit[i].PopNB(credit_in)) {
        credit_recv[i] = credit_recv[i] + credit_in;
        // DCOUT(sc_time_stamp()  << ": " << name()
        //          << " Read credit from credit port-"
        //          << i << " " << credit_recv[i] << endl);
      }
      NVHLS_ASSERT(credit_recv[i] <= buffersize);
    }
  }

  void send_credit() {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      if (credit_send[i] > 0) {
        Credit_ret_t credit_out = 1;
        bool temp = out_credit[i].PushNB(credit_out);
        if (temp) {
          credit_send[i]--;
        }
        // DCOUT(sc_time_stamp() << ": " << name() << " Returning credit to port
        // "
        //      << i << " credit_send=" << credit_send[i]
        //      << " Success: " << temp << endl);
      }
    }
  }

  void fill_ififo() {
    // Read input port if data is available and fill input buffers
    Flit_t inflit[num_ports];
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
      if (in_port[i].PopNB(inflit[i])) {
        DCOUT(sc_time_stamp() << ": " << name() << " Read input from port-" << i
                              << endl);
        if (num_vchannels > 1) {
          NVUINTW(log_num_vchannels) vcin_tmp = inflit[i].get_packet_id();
          NVHLS_ASSERT(!ififo.isFull(i * num_vchannels + vcin_tmp));
          ififo.push(inflit[i], i * num_vchannels + vcin_tmp);
        } else {
          NVHLS_ASSERT(!ififo.isFull(i));
          ififo.push(inflit[i], i);
        }
      }
    }
  }

  void inputvc_arbiter(NVUINTW(log_num_vchannels) vcin[num_ports],
                       NVUINTW(num_ports) & in_valid) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      vcin[i] = 0;
      // Doing static arbitration across input VCs here. VC0 has
      // highest priority. We are assuming that input and output VCs
      // are same.
      if (num_vchannels > 1) {
        NVUINTW(num_vchannels) vcin_valid = 0;
        NVUINTW(log_num_vchannels) vcin_local = 0;
#pragma hls_unroll yes
        for (int j = 0; j < num_vchannels; j++) {
          if ((!ififo.isEmpty(i * num_vchannels + j))) {
            vcin_valid[num_vchannels - j - 1] = 1;
            in_valid[i] = 1;
          }
        }
        if (in_valid[i] == 1) {
          vcin_local =
              nvhls::leading_ones<num_vchannels, NVUINTW(num_vchannels),
                                  NVUINTW(log_num_vchannels)>(vcin_valid);
          vcin[i] = num_vchannels - vcin_local - 1;
        }
      } else {
#pragma hls_unroll yes
        for (int i = 0; i < num_ports; i++) {
          vcin[i] = 0;
          if (!ififo.isEmpty(i)) {
            in_valid[i] = 1;
          }
        }
      }
    }
  }

  void peek_ififo(Flit_t flit_in[num_ports],
                  NVUINTW(log_num_vchannels) vcin[num_ports],
                  NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      // FIFO Read
      if (in_valid[i]) {
        flit_in[i] = ififo.peek(i * num_vchannels + vcin[i]);
        DCOUT(sc_time_stamp()
              << ": " << name() << hex << " Read from FIFO:"
              << i * num_vchannels + vcin[i] << " Flit< " << flit_in[i].flit_id
              << "," << flit_in[i].data.to_uint64() << ">" << dec << endl);
      }
    }
  }

  void compute_route(Flit_t flit_in[num_ports],
                     NVUINTW(log_num_vchannels) vcin[num_ports],
                     NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        // Route Computation: For source routing just copy from header flit
        if (flit_in[i].flit_id.isHeader()) {
          NVUINTW(dest_width)
          route = nvhls::get_slc<dest_width>(flit_in[i].data, 0);
          NVUINTW(dest_width_per_hop)
          out_dest_local = nvhls::get_slc<dest_width_per_hop>(route, 0);
          NVUINTW(dest_width) next_dests_local = route >> dest_width_per_hop;
          flit_in[i].data =
              nvhls::set_slc(flit_in[i].data, next_dests_local, 0);
          NVUINTW(num_lports)
          ldest = nvhls::get_slc<num_lports>(out_dest_local, 0);
          NVUINTW(log_num_rports)
          rdest = nvhls::get_slc<log_num_rports>(out_dest_local, num_lports);
          NVUINTW(num_rports) rdest_1hot = 0;
          if (next_dests_local != 0) {
            rdest_1hot[rdest] = 1;
          }
          out_dest[i][vcin[i]] =
              (static_cast<NVUINTW(num_ports)>(rdest_1hot) << num_lports) |
              ldest;
        }
      }
    }
  }

  void flit_output(bool is_push[num_ports],
                   NVUINTW(log_num_vchannels) vcout[num_ports]) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating over outputs here
      if (is_push[i] == true || out_stall[i] == 1) {
        bool temp = out_port[i].PushNB(flit_out[i]);
        if (flit_out[i].flit_id.isTail()) {
          is_get_new_packet[i * num_vchannels + vcout[i]] = true;
        } else {
          is_get_new_packet[i * num_vchannels + vcout[i]] = false;
        }
        if (temp == 0)
          out_stall[i] = 1;
        else {
          out_stall[i] = 0;
          credit_recv[i * num_vchannels + vcout[i]]--;
        }
        DCOUT(sc_time_stamp()
              << ": " << name() << " OutPort " << i
              << " Push success??: " << temp << " Decrementing Credit status "
              << i * num_vchannels + vcout[i] << ": "
              << credit_recv[i * num_vchannels + vcout[i]]
              << " [Router] Out_stall: " << out_stall[i] << endl);
      }
    }
  }

  void arbitration(Flit_t flit_in[num_ports], NVUINTW(num_ports) in_valid,
                   NVUINTW(log_num_vchannels) vcin[num_ports],
                   NVUINTW(num_ports) valid[num_ports],
                   NVUINTW(num_ports) select[num_ports],
                   NVUINTW(log_num_ports) select_id[num_ports]) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      valid[i] = 0;  // valid of ith output is set to 0 here
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        bool not_head_flit = (!flit_in[i].flit_id.isHeader());

#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) {  // Iterating through the outputs here
          int out_idx = k * num_vchannels + vcin[i];

          // Set valid bit to 1 for local ports only if credits are available at
          // the output port for the virtual channel chosen
          // also make sure that the output is requested, and that it is not
          // head flit, or output[vc] accepts new headers
          valid[k][i] =
              (((credit_recv[out_idx] != 0) && (out_dest[i][vcin[i]][k] == 1) &&
                (is_get_new_packet[out_idx] || not_head_flit) &&
                (out_stall[k] == 0))
                   ? 1
                   : 0);
          // DCOUT(sc_time_stamp() << ": " << name() << " Input Port:" << i << "
          // Output port: " << k << " VC: " << vcin[i] << " valid: "<<
          // valid[k][i] << " OutStall: " << out_stall[k] << " Credit: " <<
          // credit_recv[out_idx] << " GetnewPacket: " <<
          // is_get_new_packet[out_idx] << hex << " Flit " << " <" <<
          // flit_in[i].flit_id << "," << flit_in[i].data.to_uint64() << dec <<
          // ">" << endl);
        }
      }
    }

#ifdef ENABLE_MULTICAST
    // For Multicast, supress local output valid  if remote output is invalid
    NVUINTW((num_ports * num_vchannels))
    is_multicast = 0;  // Store if an input is unicast or multicast
    NVUINTW(log_num_ports) remote_portid[num_ports];
    NVUINTW(log_num_lports) local_portid[num_ports];

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
#pragma hls_unroll yes
      for (int l = 0; l < num_lports;
           l++) {  // Iterating through the outputs here
#pragma hls_unroll yes
        for (int r = num_lports; r < num_ports; r++) {
          if (out_dest[i][vcin[i]][l] && out_dest[i][vcin[i]][r]) {
            is_multicast[i * num_vchannels + vcin[i]] = 1;
            remote_portid[i] = r;
            local_portid[i] = l;
            if (!(valid[l][i] && valid[r][i])) {
              valid[l][i] = 0;
              valid[r][i] = 0;  // valid of local port is 1 only is valid of
                                // remote port is 1
              // DCOUT(sc_time_stamp() << ": " << name() << " Input Port:" << i
              // << " Local port: " << l << " Remote port: " << r << " VC: " <<
              // vcin[i] << " valid: "<< valid[l][i] << " Multicast: " <<
              // is_multicast[i*num_vchannels + vcin[i]] << endl);
            }
          }
        }
      }
    }
#endif

// Arbitrate for output port if it is header flit
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      select[i] = 0;
      if (valid[i] != 0) {  // There is an input waiting to send to output
                            // i, credits are available at output
        select[i] = arbiter[i].pick(valid[i]);

        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
        // DCOUT(sc_time_stamp() << ": " << name() << hex << " Output Port:" <<
        // i
        //          << " Valid: " << valid[i].to_uint64()
        //          << " Select : " << select[i].to_int64() << dec << "
        //          Multicast: " <<
        //          is_multicast[(select_id[i]*num_vchannels+vcin[select_id[i]])]
        //          << endl);
      }
    }

#ifdef ENABLE_MULTICAST
// For Multicast, set remote select = local select
#pragma hls_unroll yes
    for (int i = 0; i < num_lports;
         i++) {  // Iterating through the remote outputs here
      if (select[i] != 0 &&
          is_multicast[(select_id[i] * num_vchannels + vcin[select_id[i]])] &&
          valid[remote_portid[select_id[i]]][select_id[i]]) {
        select[remote_portid[select_id[i]]] = select[i];
        select_id[remote_portid[select_id[i]]] = select_id[i];
        // DCOUT(sc_time_stamp() << ": " << name() << " Output Port:" << i << "
        // Input port: " << select_id[i] << " Remote port: " <<
        // remote_portid[select_id[i]] << " VC: " << vcin[select_id[i]] << "
        // Remote Select: " << select[remote_portid[select_id[i]]] << " Local
        // Select: " << select[i] << endl);
      }
    }
// For Multicast, if remote port is selected and not local port, reset remote
// port
#pragma hls_unroll yes
    for (int i = num_lports; i < num_ports;
         i++) {  // Iterating through the remote outputs here
      if (select[i] != 0 &&
          is_multicast[(select_id[i] * num_vchannels + vcin[select_id[i]])] &&
          (valid[i][select_id[local_portid[select_id[i]]]] == 0)) {
        select[i] = 0;
        // DCOUT("Set Select = 0 for Output port: " << i << endl);
        // DCOUT(sc_time_stamp() << ": " << name() << " Output Port:" << i << "
        // Input port: " << select_id[i] << " Remote port: " <<
        // remote_portid[select_id[i]] << " VC: " << vcin[select_id[i]] << "
        // Remote Select: " << select[i] << " Local Select: " <<
        // select[local_portid[select_id[i]]] << endl);
      }
    }
#endif
  }

  void crossbar_traversal(Flit_t flit_in[num_ports], bool is_push[num_ports],
                          NVUINTW(log_num_ports) select_id[num_ports],
                          NVUINTW(log_num_vchannels) vcin[num_ports],
                          NVUINTW(log_num_vchannels) vcout[num_ports]) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through output ports
      if (is_push[i]) {
        flit_out[i] = flit_in[select_id[i]];
        vcout[i] = vcin[select_id[i]];
      }
    }
  }

  void process() {
    for (int i = 0; i < num_ports; i++)
      in_port[i].Reset();
    for (int i = 0; i < num_ports; i++)
      out_port[i].Reset();
    for (int i = 0; i < num_ports * num_vchannels; i++)
      out_credit[i].Reset();
    for (int i = 0; i < num_ports * num_vchannels; i++)
      in_credit[i].Reset();
    ififo.reset();
    out_stall = 0;

    // Reset credit registers
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      credit_send[i] = 0;
      credit_recv[i] = buffersize;
    }

    for (int i = 0; i < num_ports * num_vchannels; i++) {
      is_get_new_packet[i] = true;
    }

    while (1) {
      wait();
      receive_credit();
      fill_ififo();

      Flit_t flit_in[num_ports];
      NVUINTW(log_num_vchannels) vcin[num_ports];
      NVUINTW(num_ports) in_valid = 0;

      // pick a valid vc per input. VC0 has priority.
      // output: vcin[x] - vc selcted for in port x, in_valid - bitmap of
      // inports valid bits
      inputvc_arbiter(vcin, in_valid);

      // read top flit for each selected vc per in port
      // output: flit_in[x] - top flit on selected vc on port x
      peek_ififo(flit_in, vcin, in_valid);

      // for each selected input, in case of valid header flit only, update
      // out_dest[i][vcin[i]]
      // side effect updating out_dest[x][vc] for each input port x with output
      // port bitmap
      compute_route(flit_in, vcin, in_valid);

      // Variable to hold valid input requests. Set valid[i][j] = 1 if input j
      // is sending flit to output i
      NVUINTW(num_ports) valid[num_ports], select[num_ports];
      NVUINTW(log_num_ports) select_id[num_ports];

      // arbitrate for any top header flits on selected vcs
      // outputs:
      // valid[x] - bitmap of valid input ports to address output port x. valid
      // means there is credit on output port(and vc), and no other packet in
      // flight to that output vc
      // select[x] - bitmap of selected input port (one hot) for output port x
      // select_id[x] - selected input port for output port x
      arbitration(flit_in, in_valid, vcin, valid, select, select_id);

      // decide which port is going to push data out
      // side effect updating is_push[x] to indicate that output port x is going
      // to push a flit out
      NVUINTW(num_ports) is_popfifo = 0;
      bool is_push[num_ports];
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) { // Iterating through output ports
        is_push[i] = false;
        if (select[i] != 0) {
          is_popfifo[select_id[i]] = 1;
          is_push[i] = true;
          // DCOUT(sc_time_stamp() << ": " << name() << " Port:" << i << "
          // Select: "
          //          << select[i] << " select_id " << select_id[i]
          //          << " Valid:" << valid[i] << "PopFifo: " <<
          //          is_popfifo[select_id[i]] << endl);
        }
      }

// pop input fifos and prepare credits to be returned to sources
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {  // Iterating over inputs here
        if (is_popfifo[i] == 1) {
          ififo.incrHead(i * num_vchannels + vcin[i]);
          // DCOUT(sc_time_stamp() << ": " << name() << " Popped FIFO of port-"
          // << i
          //          << " VC-" << vcin[i] << endl);
          credit_send[i * num_vchannels + vcin[i]]++;
          NVHLS_ASSERT(credit_send[i * num_vchannels + vcin[i]] <= buffersize);
        }
      }

      // fill_ififo();
      send_credit();
      NVUINTW(log_num_vchannels) vcout[num_ports];

      crossbar_traversal(flit_in, is_push, select_id, vcin, vcout);

      // sends out flits to be sent
      // side effect: updating is_get_new_packet[x] when tail goes through port
      // x
      // and out_stall when push_nb fails
      flit_output(is_push, vcout);
    }
  }
};

template <int NumLPorts, int NumRports, int NumVchannels, int BufferSize,
          int MaxHops, typename FlitType, int NoC2Dests, int NoCDests,
          int NoCMCastDests>
class WHVCRouter<NumLPorts, NumRports, NumVchannels, BufferSize, MaxHops,
                 LUTRouting, NoC2Dests, NoCDests, NoCMCastDests, FlitType>
    : public sc_module {
 public:
  typedef FlitType Flit_t;
  // Declare constants
  enum {
    num_lports = NumLPorts,
    num_rports = NumRports,
    num_ports = num_lports + num_rports,
    num_vchannels = NumVchannels,
    log_num_vchannels = nvhls::index_width<num_vchannels>::val,
    log_num_lports = nvhls::index_width<num_lports>::val,
    log_num_rports = nvhls::index_width<num_rports>::val,
    log_num_ports = nvhls::index_width<num_ports>::val,
    NoC_dest_width = NoCDests,
    NoC2_dest_width = NoC2Dests,
    NoC_ucast_dest_width = nvhls::index_width<NoC_dest_width>::val,
    NoC2_ucast_dest_width = nvhls::index_width<NoC2_dest_width>::val,
    NoC_mcast_dest_width = NoCMCastDests,
    NoC2_mcast_dest_width = NoC2_dest_width,
    log_NoC_dest_width = nvhls::index_width<NoC_dest_width>::val,
    log_NoC2_dest_width = nvhls::index_width<NoC2_dest_width>::val,
    ucast_dest_width = NoC_ucast_dest_width + NoC2_ucast_dest_width,
    mcast_dest_width = NoC_mcast_dest_width + NoC2_mcast_dest_width,
    buffersize = BufferSize,
    log_buffersize = nvhls::index_width<buffersize>::val,
    log_buffersizeplus1 = nvhls::index_width<buffersize + 1>::val

  };
  static const int kDebugLevel = 4;
  typedef NVUINTW(NoC2_ucast_dest_width) noc2_id_t;
  sc_in<noc2_id_t> noc2_id_jtag;
  noc2_id_t noc2_id;
  const int part_id;
  NVUINTW(NoC2_mcast_dest_width) noc2_noc2_mask;
  NVUINTW(num_ports) NoCRouteLUT[NoC_dest_width];
  NVUINTW(num_ports) NoC2RouteLUT[NoC2_dest_width];
  typedef NVUINTW(log_buffersizeplus1) Credit_t;
  typedef NVUINTW(1) Credit_ret_t;

  // Configuration of route
  // typedef NVUINTW(num_ports) NoCRouteLUT_t[NoC_dest_width];
  typedef NVUINTW(num_ports) NoCRouteLUT_t;
  typedef NVUINTW(num_ports) NoC2RouteLUT_t;
  sc_in<NoCRouteLUT_t> NoCRouteLUT_jtag[NoC_dest_width];
  sc_in<NoC2RouteLUT_t> NoC2RouteLUT_jtag[NoC2_dest_width];

  // Input / Output ports
  Connections::In<Flit_t> in_port[num_ports];
  Connections::OutBuffered<Flit_t, 1> out_port[num_ports];

  // Credits
  // Send out credit when buffer is popped
  // Increment local credit when credits are read
  // 1 credit counter per output port
  Connections::In<Credit_ret_t> in_credit[num_ports * num_vchannels];
  Connections::Out<Credit_ret_t> out_credit[num_ports * num_vchannels];
  sc_in_clk clk;
  sc_in<bool> rst;

  // Input FIFO Buffer
  FIFO<Flit_t, buffersize, num_ports * num_vchannels> ififo;

  Arbiter<num_ports> arbiter[num_ports];

  // Credit registers to store credits for both producer and consumer
  Credit_t credit_recv[num_ports * num_vchannels];
  Credit_t credit_send[num_ports * num_vchannels];

  // Store output destination for each port per virtual channel with 1hot
  // encoding
  NVUINTW(num_ports) out_dest[num_ports][num_vchannels];
  NVUINTW(num_ports) is_mcast;
  NVUINTW(mcast_dest_width)
  mcast_mask_local[num_ports];  // store mask for each output, if local
                                // destination.
  NVUINTW(mcast_dest_width) mcast_mask_remote[num_ports];
                                // store mask for each output, if remote
                                // destination.
  NVUINTW(num_ports) is_local_mask[num_ports]; 
                                // indicates if a given input-output
                                // pair is intended for a local
                                // destination for masking purpose
                                // (if false then remote)

  // Variable that indicates if the output port is available to get new packet
  bool is_get_new_packet[num_ports * num_vchannels];

  // Constructor
  SC_HAS_PROCESS(WHVCRouter);
  WHVCRouter(sc_module_name name_)
      : sc_module(name_),
        part_id(7)  // x = 2, y=1. 5 columns and 4 rows
  {
    SC_THREAD(process);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }

  // Function to receive credits from the consumers
  void receive_credit() {
    Credit_ret_t credit_in;
#pragma hls_unroll yes
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      if (in_credit[i].PopNB(credit_in)) {
        credit_recv[i] = credit_recv[i] + credit_in;
        CDCOUT(sc_time_stamp() << ": " << name()
                               << " Read credit from credit port-" << i << " "
                               << credit_recv[i] << endl,
               kDebugLevel);
      }
      NVHLS_ASSERT(credit_recv[i] <= buffersize);
    }
  }

  void send_credit() {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      if (credit_send[i] > 0) {
        Credit_ret_t credit_out = 1;
        bool temp = out_credit[i].PushNB(credit_out);
        if (temp) {
          credit_send[i]--;
        }
        CDCOUT(sc_time_stamp() << ": " << name() << " Returning credit to port "
                               << i << " credit_send=" << credit_send[i]
                               << " Success: " << temp << endl,
               kDebugLevel);
      }
    }
  }

  void fill_ififo() {
    // Read input port if data is available and fill input buffers
    Flit_t inflit[num_ports];
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
      if (in_port[i].PopNB(inflit[i])) {
        CDCOUT(sc_time_stamp() << ": " << name() << " Read input from port-"
                               << i << " " << inflit[i] << endl,
               kDebugLevel);
        if (num_vchannels > 1) {
          NVUINTW(log_num_vchannels) vcin_tmp = inflit[i].get_packet_id();
          NVHLS_ASSERT(!ififo.isFull(i * num_vchannels + vcin_tmp));
          ififo.push(inflit[i], i * num_vchannels + vcin_tmp);
        } else {
          NVHLS_ASSERT(!ififo.isFull(i));
          ififo.push(inflit[i], i);
        }
      }
    }
  }

  void inputvc_arbiter(NVUINTW(log_num_vchannels) vcin[num_ports],
                       NVUINTW(num_ports) & in_valid) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      vcin[i] = 0;
      // Doing static arbitration across input VCs here. VC0 has
      // highest priority. We are assuming that input and output VCs
      // are same.
      if (num_vchannels > 1) {
        NVUINTW(num_vchannels) vcin_valid = 0;
        NVUINTW(log_num_vchannels) vcin_local = 0;
#pragma hls_unroll yes
        for (int j = 0; j < num_vchannels; j++) {
          if ((!ififo.isEmpty(i * num_vchannels + j))) {
            vcin_valid[num_vchannels - j - 1] = 1;
            in_valid[i] = 1;
          }
        }
        if (in_valid[i] == 1) {
          vcin_local =
              nvhls::leading_ones<num_vchannels, NVUINTW(num_vchannels),
                                  NVUINTW(log_num_vchannels)>(vcin_valid);
          vcin[i] = num_vchannels - vcin_local - 1;
        }
      } else {
        if (!ififo.isEmpty(i)) {
          in_valid[i] = 1;
        }
      }
    }
  }

  void peek_ififo(Flit_t flit_in[num_ports],
                  NVUINTW(log_num_vchannels) vcin[num_ports],
                  NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      // FIFO Read
      if (in_valid[i]) {
        flit_in[i] = ififo.peek(i * num_vchannels + vcin[i]);
        CDCOUT(sc_time_stamp()
                   << ": " << name() << hex
                   << " Read from FIFO:" << i * num_vchannels + vcin[i]
                   << " Flit< " << flit_in[i].flit_id << ","
                   << flit_in[i].data.to_uint64() << ">" << dec << endl,
               kDebugLevel);
      }
    }
  }

  void compute_mcast_route(Flit_t flit_in[num_ports],
                           NVUINTW(log_num_vchannels) vcin[num_ports],
                           NVUINTW(log_num_ports) i) {  // i is input port

    NVUINTW(mcast_dest_width)
    route = nvhls::get_slc<mcast_dest_width>(flit_in[i].data, 0);
    NVUINTW(NoC2_mcast_dest_width)
    NoC2_out_dest_local = nvhls::get_slc<NoC2_mcast_dest_width>(route, 0);
    NVUINTW(NoC_mcast_dest_width)
    NoC_out_dest_local =
        nvhls::get_slc<NoC_mcast_dest_width>(route, NoC2_mcast_dest_width);
    bool is_local_noc2 = NoC2_out_dest_local[noc2_id];
    bool is_remote_noc2 = ((NoC2_out_dest_local & (~noc2_noc2_mask)) != 0);
    NVUINTW(num_ports) noc_dest_local[NoC_mcast_dest_width];
    NVUINTW(num_ports) noc2_dest_local[NoC2_mcast_dest_width];
#pragma hls_unroll yes
    for (int j = 0; j < NoC_mcast_dest_width; j++) {
      if (is_local_noc2 && (NoC_out_dest_local[j] == 1)) {
        noc_dest_local[j] = NoCRouteLUT[j];
      } else {
        noc_dest_local[j] = 0;
      }
    }

#pragma hls_unroll yes
    for (int j = 0; j < NoC2_mcast_dest_width; j++) {
      if (is_remote_noc2 && (NoC2_out_dest_local[j] == 1)) {
        noc2_dest_local[j] = NoC2RouteLUT[j];
      } else {
        noc2_dest_local[j] = 0;
      }
    }

    is_local_mask[i] = 0;
#pragma hls_unroll yes
    for (int j = 0; j < NoC_mcast_dest_width; j++) {
      is_local_mask[i] |= noc_dest_local[j];
    }
    out_dest[i][vcin[i]] = is_local_mask[i];

#pragma hls_unroll yes
    for (int j = 0; j < NoC2_mcast_dest_width; j++) {
      out_dest[i][vcin[i]] |= noc2_dest_local[j];
    }

    CDCOUT(sc_time_stamp() << ": " << name() << hex << " Mcast OutDest[" << i
                           << "] " << out_dest[i][vcin[i]].to_uint64()
                           << " IS-Remote-NoC2: " << is_remote_noc2
                           << " Route: " << route.to_uint64() << endl,
           kDebugLevel);
  }

  void compute_ucast_route(Flit_t flit_in[num_ports],
                           NVUINTW(log_num_vchannels) vcin[num_ports],
                           NVUINTW(log_num_ports) i) {

    NVUINTW(ucast_dest_width)
    route = nvhls::get_slc<ucast_dest_width>(flit_in[i].data, 0);
    NVUINTW(NoC2_ucast_dest_width)
    NoC2_out_dest_local =
        nvhls::get_slc<NoC2_ucast_dest_width>(route, NoC_ucast_dest_width);
    NVUINTW(NoC_ucast_dest_width)
    NoC_out_dest_local = nvhls::get_slc<NoC_ucast_dest_width>(route, 0);
    bool is_local_noc2 = (NoC2_out_dest_local == noc2_id);
    bool is_remote_noc2 = !is_local_noc2;

    if (is_local_noc2) {
      out_dest[i][vcin[i]] = NoCRouteLUT[NoC_out_dest_local];
    } else {
      out_dest[i][vcin[i]] = NoC2RouteLUT[NoC2_out_dest_local];
    }

    CDCOUT(sc_time_stamp() << ": " << name() << hex << " Ucast OutDest[" << i
                           << "] " << out_dest[i][vcin[i]]
                           << " IS-Remote-NoC2: " << is_remote_noc2
                           << " NoC Dest: " << NoC_out_dest_local
                           << " NoC2 Dest: " << NoC2_out_dest_local
                           << " Route: " << route.to_uint64() << endl,
           kDebugLevel);
  }

  void compute_route(Flit_t flit_in[num_ports],
                     NVUINTW(log_num_vchannels) vcin[num_ports],
                     NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        if (flit_in[i].flit_id.isHeader()) {
          is_mcast[i] = flit_in[i].data[mcast_dest_width];
          if (is_mcast[i]) {
            compute_mcast_route(flit_in, vcin, i);
          } else {
            compute_ucast_route(flit_in, vcin, i);
          }
        }
      }
    }
  }

  void flit_output(bool is_push[num_ports],
                   NVUINTW(log_num_vchannels) vcout[num_ports],
                   Flit_t flit_out[num_ports]) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating over outputs here
      if (is_push[i] == true) {
        if (flit_out[i].flit_id.isTail()) {
          is_get_new_packet[i * num_vchannels + vcout[i]] = true;
        } else {
          is_get_new_packet[i * num_vchannels + vcout[i]] = false;
        }
        credit_recv[i * num_vchannels + vcout[i]]--;
        CDCOUT(sc_time_stamp()
                   << ": " << name() << " OutPort " << i << " Push success??: "
                   << out_port[i].Full() << " Decrementing Credit status "
                   << i * num_vchannels + vcout[i] << ": "
                   << credit_recv[i * num_vchannels + vcout[i]]
                   << " [Router] Out FiFO full?: " << (out_port[i].Full())
                   << endl,
               kDebugLevel);
      }
      out_port[i].TransferNB();
    }
  }

  void arbitration(Flit_t flit_in[num_ports], NVUINTW(num_ports) in_valid,
                   NVUINTW(log_num_vchannels) vcin[num_ports],
                   NVUINTW(num_ports) valid[num_ports],
                   NVUINTW(num_ports) select[num_ports],
                   NVUINTW(log_num_ports) select_id[num_ports]) {

    NVUINTW(num_ports)
    valid_transpose[num_ports], select_transpose[num_ports],
        mcast_valid_transpose[num_ports];
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      valid[i] = 0;             // valid of ith output is set to 0 here
      valid_transpose[i] = 0;   // valid_transpose of ith input is set to 0 here
      select[i] = 0;            // select of ith output is set to 0 here
      select_transpose[i] = 0;  // select_transpose of ith input
                                // is set to 0 here
      mcast_valid_transpose[i] = 0;   // valid_transpose of ith input
                                      //is set to 0 here
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        bool not_head_flit = (!flit_in[i].flit_id.isHeader());
#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) {  // Iterating through the outputs here
          int out_idx = k * num_vchannels + vcin[i];
          // Set valid bit to 1 for local ports only if credits are available at
          // the output port for the virtual channel chosen
          // also make sure that the output is requested, and that it is not
          // head flit, or output[vc] accepts new headers
          valid_transpose[i][k] =
              (((!is_mcast[i]) && (credit_recv[out_idx] != 0) &&
                (out_dest[i][vcin[i]][k] == 1) &&
                (is_get_new_packet[out_idx] || not_head_flit) &&
                (!out_port[k].Full()))
                   ? 1
                   : 0);
          mcast_valid_transpose[i][k] =
              (((is_mcast[i]) && (credit_recv[out_idx] != 0) &&
                (out_dest[i][vcin[i]][k] == 1) &&
                (is_get_new_packet[out_idx] || not_head_flit) &&
                (!out_port[k].Full()))
                   ? 1
                   : 0);
          CDCOUT(sc_time_stamp()
                     << ": " << name() << " Input Port:" << i
                     << " Output port: " << k << " VC: " << vcin[i]
                     << " valid: " << valid_transpose[i][k]
                     << " OutStall: " << (!out_port[k].Full())
                     << " Credit: " << credit_recv[out_idx] << " GetnewPacket: "
                     << is_get_new_packet[out_idx] << hex << " Flit "
                     << " <" << flit_in[i].flit_id << ","
                     << flit_in[i].data.to_uint64() << dec << ">" << endl,
                 kDebugLevel);
        }
      }
    }

// For Multicast, supress valid  if some of output ports are invalid
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (mcast_valid_transpose[i] ^ out_dest[i][vcin[i]]) {  //
        mcast_valid_transpose[i] = 0;
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {   // Iterating through the inputs here
      if (mcast_valid_transpose[i] != 0) {  //
        select_transpose[i] = mcast_valid_transpose[i];
#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) {  // Iterating through the outputs here
          select[k][i] = select_transpose[i][k];
        }
// Update valid_transpose if the output is already selected
#pragma hls_unroll yes
        for (int j = 0; j < num_ports; j++) {  // inputs
          if (j >= i + 1) {
#pragma hls_unroll yes
            for (int k = 0; k < num_ports; k++) {  // outputs
              if (mcast_valid_transpose[j][k] && (select[k] != 0)) {
                mcast_valid_transpose[j] = 0;
              }
            }
          }
        }
        // break;  // If timing is an issue, process only one mcast request at a
        // time using break. Remove the j,k nested loops about
      }
    }

// Transpose to form valid matrix
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
#pragma hls_unroll yes
      for (int j = 0; j < num_ports; j++) {
        valid[i][j] = valid_transpose[j][i];
        // select[i][j] = select_transpose[j][i];
      }
      CDCOUT(i << " Select: " << select[i] << " Valid: " << valid[i] << endl,
             kDebugLevel);
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      if ((select[i] != 0)) {              //
        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
      }
    }

// Arbitrate for output port if it is header flit
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      if ((valid[i] != 0) &&
          (select[i] == 0)) {  // There is an input waiting to send to output
                               // i, credits are available at
                               // output, and its input is not
                               // yet assigned by multicast
        select[i] = arbiter[i].pick(valid[i]);
        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
        CDCOUT(sc_time_stamp() << ": " << name() << hex << " Output Port:" << i
                               << " Valid: " << valid[i].to_uint64()
                               << " Select : " << select[i].to_int64() << dec
                               << " Multicast: " << is_mcast[select_id[i]]
                               << endl,
               kDebugLevel);
      }
    }
  }

  void crossbar_traversal(Flit_t flit_in[num_ports], bool is_push[num_ports],
                          NVUINTW(log_num_ports) select_id[num_ports],
                          NVUINTW(log_num_vchannels) vcin[num_ports],
                          NVUINTW(log_num_vchannels) vcout[num_ports],
                          Flit_t flit_out[num_ports]) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through output ports
      if (is_push[i]) {
        flit_out[i] = flit_in[select_id[i]];
        vcout[i] = vcin[select_id[i]];
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through output ports
      if (is_push[i]) {
        NVUINTW(mcast_dest_width)
        route = nvhls::get_slc<mcast_dest_width>(flit_out[i].data, 0);
        if (flit_out[i].flit_id.isHeader() &&
            is_mcast[select_id[i]]) {  // Mask the route in header
          if (is_local_mask[select_id[i]][i]) {
            route &= mcast_mask_local[i];
          } else {
            route &= mcast_mask_remote[i];
          }
        }
        Flit_t flit_local = flit_out[i];
        flit_local.data = nvhls::set_slc(flit_local.data, route, 0);
        out_port[i].Push(flit_local);
      }
    }
  }

  void process() {
    for (int i = 0; i < num_ports; i++)
      in_port[i].Reset();
    for (int i = 0; i < num_ports; i++)
      out_port[i].Reset();
    for (int i = 0; i < num_ports * num_vchannels; i++)
      out_credit[i].Reset();
    for (int i = 0; i < num_ports * num_vchannels; i++)
      in_credit[i].Reset();
    ififo.reset();
    is_mcast = 0;
    // Reset credit registers
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      credit_send[i] = 0;
      credit_recv[i] = buffersize;
    }

    for (int i = 0; i < num_ports * num_vchannels; i++) {
      is_get_new_packet[i] = true;
    }

#pragma hls_unroll yes
    for (int i = 0; i < NoC_dest_width; i++) {
      NoCRouteLUT[i] = NoCRouteLUT_jtag[i].read();
    }
    noc2_id = noc2_id_jtag.read();
    noc2_noc2_mask = 0;
    noc2_noc2_mask[noc2_id] = 1;
#pragma hls_unroll yes
    for (int i = 0; i < NoC2_dest_width; i++) {
      NoC2RouteLUT[i] = NoC2RouteLUT_jtag[i].read();
    }

// Precompute masks
#pragma hls_unroll yes
    for (int k = 0; k < num_ports; k++) {
      // Init masks to all 1's
      mcast_mask_local[k] = 0;
      mcast_mask_remote[k] = 0;

// Precompute for local destinations
#pragma hls_unroll yes
      for (int j = 0; j < NoC_mcast_dest_width; j++) {
        NVUINTW(num_ports) NoCRouteLUT_tmp = NoCRouteLUT[j];
        if (NoCRouteLUT_tmp[k])
          mcast_mask_local[k][j + NoC2_mcast_dest_width] = 1;
      }
#pragma hls_unroll yes
      for (int j = 0; j < NoC2_mcast_dest_width; j++) {
        mcast_mask_local[k][j] = 1;
      }
// Precompute for remote destinations
#pragma hls_unroll yes
      for (int j = 0; j < NoC2_mcast_dest_width; j++) {
        NVUINTW(num_ports) NoC2RouteLUT_tmp = NoC2RouteLUT[j];
        if (NoC2RouteLUT_tmp[k])
          mcast_mask_remote[k][j] = 1;
      }
#pragma hls_unroll yes
      for (int j = 0; j < NoC_mcast_dest_width; j++) {
        mcast_mask_remote[k][j + NoC2_mcast_dest_width] = 1;
      }
    }

    while (1) {
      wait();
      receive_credit();
      // fill_ififo();
      Flit_t flit_in[num_ports];
      NVUINTW(log_num_vchannels) vcin[num_ports];
      NVUINTW(num_ports) in_valid = 0;

      // pick a valid vc per input. VC0 has priority.
      // output: vcin[x] - vc selcted for in port x, in_valid - bitmap of
      // inports valid bits
      inputvc_arbiter(vcin, in_valid);

      // read top flit for each selected vc per in port
      // output: flit_in[x] - top flit on selected vc on port x
      peek_ififo(flit_in, vcin, in_valid);

      // for each selected input, in case of valid header flit only, update
      // out_dest[i][vcin[i]]
      // side effect updating out_dest[x][vc] for each input port x with output
      // port bitmap
      compute_route(flit_in, vcin, in_valid);

      // Variable to hold valid input requests. Set valid[i][j] = 1 if input j
      // is sending flit to output i
      NVUINTW(num_ports) valid[num_ports], select[num_ports];
      NVUINTW(log_num_ports) select_id[num_ports];

      // arbitrate for any top header flits on selected vcs
      // outputs:
      // valid[x] - bitmap of valid input ports to address output port x. valid
      // means there is credit on output port(and vc), and no other packet in
      // flight to that output vc
      // select[x] - bitmap of selected input port (one hot) for output port x
      // select_id[x] - selected input port for output port x
      arbitration(flit_in, in_valid, vcin, valid, select, select_id);

      // decide which port is going to push data out
      // side effect updating is_push[x] to indicate that output port x is going
      // to push a flit out
      NVUINTW(num_ports) is_popfifo = 0;
      bool is_push[num_ports];
#pragma hls_unroll yes
      for (int i = 0; i < num_ports;
           i++) {  // Iterating through output ports here
        is_push[i] = false;
        if (select[i] != 0) {
          is_popfifo[select_id[i]] = 1;
          is_push[i] = true;
          CDCOUT(sc_time_stamp() << ": " << name() << " Port:" << i
                                 << " Select: " << select[i] << " select_id "
                                 << select_id[i] << " Valid:" << valid[i]
                                 << " PopFifo: " << is_popfifo[select_id[i]]
                                 << endl,
                 kDebugLevel);
        }
      }

// pop input fifos and prepare credits to be returned to sources
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {  // Iterating over inputs here
        if (is_popfifo[i] == 1) {
          ififo.incrHead(i * num_vchannels + vcin[i]);
          CDCOUT(sc_time_stamp() << ": " << name() << " Popped FIFO of port-"
                                 << i << " VC-" << vcin[i] << endl,
                 kDebugLevel);
          credit_send[i * num_vchannels + vcin[i]]++;
          NVHLS_ASSERT(credit_send[i * num_vchannels + vcin[i]] <= buffersize);
        }
      }

      fill_ififo();
      send_credit();
      NVUINTW(log_num_vchannels) vcout[num_ports];
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {
        vcout[i] = 0;
      }
      Flit_t flit_out[num_ports];
      crossbar_traversal(flit_in, is_push, select_id, vcin, vcout, flit_out);

      // sends out flits to be sent
      // side effect: updating is_get_new_packet[x] when tail goes through port
      // x
      flit_output(is_push, vcout, flit_out);
    }
  }
};

template <int NumLPorts, int NumRports, int BufferSize, int MaxHops,
          typename FlitType, int NoC2Dests, int NoCDests, int NoCMCastDests,
          int MaxPacketSize>
class WHVCRouter<NumLPorts, NumRports, 1, BufferSize, MaxHops, LUTRouting,
                 NoC2Dests, NoCDests, NoCMCastDests, FlitType, MaxPacketSize>
    : public sc_module {
 public:
  typedef FlitType Flit_t;
  // Declare constants
  enum {
    num_lports = NumLPorts,
    num_rports = NumRports,
    num_ports = num_lports + num_rports,
    num_vchannels = 1,
    log_num_vchannels = nvhls::index_width<num_vchannels>::val,
    log_num_lports = nvhls::index_width<num_lports>::val,
    log_num_rports = nvhls::index_width<num_rports>::val,
    log_num_ports = nvhls::index_width<num_ports>::val,
    NoC_dest_width = NoCDests,
    NoC2_dest_width = NoC2Dests,
    NoC_ucast_dest_width = nvhls::index_width<NoC_dest_width>::val,
    NoC2_ucast_dest_width = nvhls::index_width<NoC2_dest_width>::val,
    NoC_mcast_dest_width = NoCMCastDests,
    NoC2_mcast_dest_width = NoC2_dest_width,
    log_NoC_dest_width = nvhls::index_width<NoC_dest_width>::val,
    log_NoC2_dest_width = nvhls::index_width<NoC2_dest_width>::val,
    ucast_dest_width = NoC_ucast_dest_width + NoC2_ucast_dest_width,
    mcast_dest_width = NoC_mcast_dest_width + NoC2_mcast_dest_width,
    buffersize = BufferSize,
    log_buffersize = nvhls::index_width<buffersize>::val,
    max_packet_size = MaxPacketSize
  };
  static const int kDebugLevel = 4;
  typedef NVUINTW(NoC2_ucast_dest_width) noc2_id_t;
  sc_in<noc2_id_t> noc2_id_jtag;
  noc2_id_t noc2_id;
  NVUINTW(NoC2_mcast_dest_width) noc2_noc2_mask;
  NVUINTW(num_ports) NoCRouteLUT[NoC_dest_width];
  NVUINTW(num_ports) NoC2RouteLUT[NoC2_dest_width];

  // Jtag of route
  // typedef NVUINTW(num_ports) NoCRouteLUT_t[NoC_dest_width];
  typedef NVUINTW(num_ports) NoCRouteLUT_t;
  typedef NVUINTW(num_ports) NoC2RouteLUT_t;
  sc_in<NoCRouteLUT_t> NoCRouteLUT_jtag[NoC_dest_width];
  sc_in<NoC2RouteLUT_t> NoC2RouteLUT_jtag[NoC2_dest_width];

  // Input / Output ports
  Connections::In<Flit_t> in_port[num_ports];
  // Connections::OutBuffered<Flit_t, buffersize> out_port[num_ports];
  nvhls::nv_array<Connections::OutBuffered<Flit_t, buffersize>, num_ports>
      out_port;

  sc_in_clk clk;
  sc_in<bool> rst;

  // Input FIFO Buffer
  FIFO<Flit_t, 2, num_ports * num_vchannels> ififo;

  Arbiter<num_ports> arbiter[num_ports];

  // Store output destination for each port per virtual channel with 1hot
  // encoding
  NVUINTW(num_ports) out_dest[num_ports][num_vchannels];
  NVUINTW(num_ports) is_mcast;
  NVUINTW(mcast_dest_width)
  mcast_mask_local[num_ports];  // store mask for each output, if local
                                // destination.
  NVUINTW(mcast_dest_width) mcast_mask_remote[num_ports]; 
                                // store mask for each output,
                                // if remote destination.
  NVUINTW(num_ports) is_local_mask[num_ports];  
                                // indicates if a given input-output
                                // pair is intended for a local
                                // destination for masking purpose
                                // (if false then remote)

  // Variable that indicates if the output port is available to get new packet
  bool is_get_new_packet[num_ports * num_vchannels];

  // Constructor
  SC_HAS_PROCESS(WHVCRouter);
  WHVCRouter(sc_module_name name_) : sc_module(name_), out_port("out_port") {
    SC_THREAD(process);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }

  void fill_ififo() {
    // Read input port if data is available and fill input buffers
    Flit_t inflit[num_ports];
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
      if (!ififo.isFull(i)) {
        if (in_port[i].PopNB(inflit[i])) {
          CDCOUT(sc_time_stamp() << ": " << name()
                                 << " Read input from port-" << i << " "
                                 << inflit[i] << endl,
                 kDebugLevel);
          ififo.push(inflit[i], i);
        }
      }
    }
  }

  void inputvc_arbiter(NVUINTW(log_num_vchannels) vcin[num_ports],
                       NVUINTW(num_ports) & in_valid) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      vcin[i] = 0;
      // Doing static arbitration across input VCs here. VC0 has
      // highest priority. We are assuming that input and output VCs
      // are same.
      if (num_vchannels > 1) {
        NVUINTW(num_vchannels) vcin_valid = 0;
        NVUINTW(log_num_vchannels) vcin_local = 0;
#pragma hls_unroll yes
        for (int j = 0; j < num_vchannels; j++) {
          if ((!ififo.isEmpty(i * num_vchannels + j))) {
            vcin_valid[num_vchannels - j - 1] = 1;
            in_valid[i] = 1;
          }
        }
        if (in_valid[i] == 1) {
          vcin_local =
              nvhls::leading_ones<num_vchannels, NVUINTW(num_vchannels),
                                  NVUINTW(log_num_vchannels)>(vcin_valid);
          vcin[i] = num_vchannels - vcin_local - 1;
        }
      } else {
        if (!ififo.isEmpty(i)) {
          in_valid[i] = 1;
        }
      }
    }
  }

  void peek_ififo(Flit_t flit_in[num_ports],
                  NVUINTW(log_num_vchannels) vcin[num_ports],
                  NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      // FIFO Read
      if (in_valid[i]) {
        flit_in[i] = ififo.peek(i * num_vchannels + vcin[i]);
        CDCOUT(sc_time_stamp()
                   << ": " << name() << hex
                   << " Read from FIFO:" << i * num_vchannels + vcin[i]
                   << " Flit< " << flit_in[i].flit_id << ","
                   << flit_in[i].data.to_uint64() << ">" << dec << endl,
               kDebugLevel);
      }
    }
  }

  void compute_mcast_route(Flit_t flit_in[num_ports],
                           NVUINTW(log_num_vchannels) vcin[num_ports],
                           NVUINTW(log_num_ports) i) {  // i is input port

    NVUINTW(mcast_dest_width)
    route = nvhls::get_slc<mcast_dest_width>(flit_in[i].data, 0);
    NVUINTW(NoC2_mcast_dest_width)
    NoC2_out_dest_local = nvhls::get_slc<NoC2_mcast_dest_width>(route, 0);
    NVUINTW(NoC_mcast_dest_width)
    NoC_out_dest_local =
        nvhls::get_slc<NoC_mcast_dest_width>(route, NoC2_mcast_dest_width);
    bool is_local_noc2 = NoC2_out_dest_local[noc2_id];
    bool is_remote_noc2 = ((NoC2_out_dest_local & (~noc2_noc2_mask)) != 0);
    NVUINTW(num_ports) noc_dest_local[NoC_mcast_dest_width];
    NVUINTW(num_ports) noc2_dest_local[NoC2_mcast_dest_width];
#pragma hls_unroll yes
    for (int j = 0; j < NoC_mcast_dest_width; j++) {
      if (is_local_noc2 && (NoC_out_dest_local[j] == 1)) {
        noc_dest_local[j] = NoCRouteLUT[j];
      } else {
        noc_dest_local[j] = 0;
      }
    }

#pragma hls_unroll yes
    for (int j = 0; j < NoC2_mcast_dest_width; j++) {
      if (is_remote_noc2 && (NoC2_out_dest_local[j] == 1)) {
        noc2_dest_local[j] = NoC2RouteLUT[j];
      } else {
        noc2_dest_local[j] = 0;
      }
    }

    is_local_mask[i] = 0;
#pragma hls_unroll yes
    for (int j = 0; j < NoC_mcast_dest_width; j++) {
      is_local_mask[i] |= noc_dest_local[j];
    }
    out_dest[i][vcin[i]] = is_local_mask[i];

#pragma hls_unroll yes
    for (int j = 0; j < NoC2_mcast_dest_width; j++) {
      out_dest[i][vcin[i]] |= noc2_dest_local[j];
    }

    CDCOUT(sc_time_stamp() << ": " << name() << hex << " Mcast OutDest[" << i
                           << "] " << out_dest[i][vcin[i]].to_uint64()
                           << " IS-Remote-NoC2: " << is_remote_noc2
                           << " Route: " << route.to_uint64() << endl,
           kDebugLevel);
  }

  void compute_ucast_route(Flit_t flit_in[num_ports],
                           NVUINTW(log_num_vchannels) vcin[num_ports],
                           NVUINTW(log_num_ports) i) {

    NVUINTW(ucast_dest_width)
    route = nvhls::get_slc<ucast_dest_width>(flit_in[i].data, 0);
    NVUINTW(NoC2_ucast_dest_width)
    NoC2_out_dest_local =
        nvhls::get_slc<NoC2_ucast_dest_width>(route, NoC_ucast_dest_width);
    NVUINTW(NoC_ucast_dest_width)
    NoC_out_dest_local = nvhls::get_slc<NoC_ucast_dest_width>(route, 0);
    bool is_local_noc2 = (NoC2_out_dest_local == noc2_id);
    bool is_remote_noc2 = !is_local_noc2;

    if (is_local_noc2) {
      out_dest[i][vcin[i]] = NoCRouteLUT[NoC_out_dest_local];
    } else {
      out_dest[i][vcin[i]] = NoC2RouteLUT[NoC2_out_dest_local];
    }

    CDCOUT(sc_time_stamp() << ": " << name() << hex << " Ucast OutDest[" << i
                           << "] " << out_dest[i][vcin[i]]
                           << " IS-Remote-NoC2: " << is_remote_noc2
                           << " NoC Dest: " << NoC_out_dest_local
                           << " NoC2 Dest: " << NoC2_out_dest_local
                           << " Route: " << route.to_uint64() << endl,
           kDebugLevel);
  }

  void compute_route(Flit_t flit_in[num_ports],
                     NVUINTW(log_num_vchannels) vcin[num_ports],
                     NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        if (flit_in[i].flit_id.isHeader()) {
          is_mcast[i] = flit_in[i].data[mcast_dest_width];
          if (is_mcast[i]) {
            compute_mcast_route(flit_in, vcin, i);
          } else {
            compute_ucast_route(flit_in, vcin, i);
          }
        }
      }
    }
  }

  void flit_output(bool is_push[num_ports],
                   NVUINTW(log_num_vchannels) vcout[num_ports],
                   Flit_t flit_out[num_ports]) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating over outputs here
      if (is_push[i] == true) {
        if (flit_out[i].flit_id.isTail()) {
          is_get_new_packet[i * num_vchannels + vcout[i]] = true;
        } else {
          is_get_new_packet[i * num_vchannels + vcout[i]] = false;
        }
        CDCOUT(sc_time_stamp() << ": " << name() << " OutPort " << i
                               << " [Router] Out FiFO full?: "
                               << (out_port[i].Full()) << endl,
               kDebugLevel);
      }
      // out_port[i].TransferNB();
    }
  }

  void arbitration(Flit_t flit_in[num_ports], NVUINTW(num_ports) in_valid,
                   NVUINTW(num_ports) out_port_notfull,
                   NVUINTW(log_num_vchannels) vcin[num_ports],
                   NVUINTW(num_ports) valid[num_ports],
                   NVUINTW(num_ports) select[num_ports],
                   NVUINTW(log_num_ports) select_id[num_ports]) {

    NVUINTW(num_ports)
    valid_transpose[num_ports], select_transpose[num_ports],
        mcast_valid_transpose[num_ports];
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      valid[i] = 0;            // valid of ith output is set to 0 here
      valid_transpose[i] = 0;  // valid_transpose of ith input is set to 0 here
      select[i] = 0;           // select of ith output is set to 0 here
      select_transpose[i] = 0;
                  // select_transpose of ith input is set to 0 here
      mcast_valid_transpose[i] = 0;
                  // valid_transpose of ith input is set to 0 here
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        bool not_head_flit = (!flit_in[i].flit_id.isHeader());
#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) {  // Iterating through the outputs here
          int out_idx = k * num_vchannels + vcin[i];
          // Set valid bit to 1 for local ports only if credits are available at
          // the output port for the virtual channel chosen
          // also make sure that the output is requested, and that it is not
          // head flit, or output[vc] accepts new headers
          valid_transpose[i][k] =
              (((!is_mcast[i]) && (out_dest[i][vcin[i]][k] == 1) &&
                (is_get_new_packet[out_idx] || not_head_flit) &&
                (out_port_notfull[k]))
                   ? 1
                   : 0);
          mcast_valid_transpose[i][k] =
              (((is_mcast[i]) && (out_dest[i][vcin[i]][k] == 1) &&
                (is_get_new_packet[out_idx] || not_head_flit) &&
                (out_port_notfull[k]))
                   ? 1
                   : 0);
          CDCOUT(sc_time_stamp()
                     << ": " << name() << " Input Port:" << i
                     << " Output port: " << k << " VC: " << vcin[i]
                     << " valid: " << valid_transpose[i][k]
                     << " OutStall: " << (out_port_notfull[k])
                     << " GetnewPacket: " << is_get_new_packet[out_idx] << hex
                     << " Flit "
                     << " <" << flit_in[i].flit_id << ","
                     << flit_in[i].data.to_uint64() << dec << ">" << endl,
                 kDebugLevel);
        }
      }
    }

// For Multicast, supress valid  if some of output ports are invalid
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (mcast_valid_transpose[i] ^ out_dest[i][vcin[i]]) {  //
        mcast_valid_transpose[i] = 0;
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {   // Iterating through the inputs here
      if (mcast_valid_transpose[i] != 0) {  //
        select_transpose[i] = mcast_valid_transpose[i];
#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) {  // Iterating through the outputs here
          select[k][i] = select_transpose[i][k];
        }
// Update valid_transpose if the output is already selected
#pragma hls_unroll yes
        for (int j = 0; j < num_ports; j++) {  // inputs
          if (j >= i + 1) {
#pragma hls_unroll yes
            for (int k = 0; k < num_ports; k++) {  // outputs
              if (mcast_valid_transpose[j][k] && (select[k] != 0)) {
                mcast_valid_transpose[j] = 0;
              }
            }
          }
        }
        // break;  // If timing is an issue, process only one mcast request at a
        // time using break. Remove the j,k nested loops about
      }
    }

// Transpose to form valid matrix
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
#pragma hls_unroll yes
      for (int j = 0; j < num_ports; j++) {
        valid[i][j] = valid_transpose[j][i];
        // select[i][j] = select_transpose[j][i];
      }
      CDCOUT(i << " Select: " << select[i] << " Valid: " << valid[i] << endl,
             kDebugLevel);
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      if ((select[i] != 0)) {              //
        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
      }
    }

// Arbitrate for output port if it is header flit
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      if ((valid[i] != 0) &&
          (select[i] == 0)) {  // There is an input waiting to send to output
                               // i, credits are available at
                               // output, and its input is not
                               // yet assigned by multicast
        select[i] = arbiter[i].pick(valid[i]);
        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
        CDCOUT(sc_time_stamp() << ": " << name() << hex << " Output Port:" << i
                               << " Valid: " << valid[i].to_uint64()
                               << " Select : " << select[i].to_int64() << dec
                               << " Multicast: " << is_mcast[select_id[i]]
                               << endl,
               kDebugLevel);
      }
    }
  }

  void crossbar_traversal(Flit_t flit_in[num_ports], bool is_push[num_ports],
                          NVUINTW(log_num_ports) select_id[num_ports],
                          NVUINTW(log_num_vchannels) vcin[num_ports],
                          NVUINTW(log_num_vchannels) vcout[num_ports],
                          Flit_t flit_out[num_ports]) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through output ports
      if (is_push[i]) {
        flit_out[i] = flit_in[select_id[i]];
        vcout[i] = vcin[select_id[i]];
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through output ports
      if (is_push[i]) {
        NVUINTW(mcast_dest_width)
        route = nvhls::get_slc<mcast_dest_width>(flit_out[i].data, 0);
        if (flit_out[i].flit_id.isHeader() &&
            is_mcast[select_id[i]]) {  // Mask the route in header
          if (is_local_mask[select_id[i]][i]) {
            route &= mcast_mask_local[i];
          } else {
            route &= mcast_mask_remote[i];
          }
        }
        Flit_t flit_local = flit_out[i];
        flit_local.data = nvhls::set_slc(flit_local.data, route, 0);
        out_port[i].Push(flit_local);
      }
    }
  }

  void process() {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++)
      in_port[i].Reset();
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++)
      out_port[i].Reset();
    ififo.reset();
    is_mcast = 0;

    for (int i = 0; i < num_ports * num_vchannels; i++) {
      is_get_new_packet[i] = true;
    }
    noc2_noc2_mask = 0;

    wait();
#pragma hls_unroll yes
    for (int i = 0; i < NoC_dest_width; i++) {
      NoCRouteLUT[i] = NoCRouteLUT_jtag[i].read();
    }
    noc2_id = noc2_id_jtag.read();
    noc2_noc2_mask[noc2_id] = 1;
#pragma hls_unroll yes
    for (int i = 0; i < NoC2_dest_width; i++) {
      NoC2RouteLUT[i] = NoC2RouteLUT_jtag[i].read();
    }

// Precompute masks
#pragma hls_unroll yes
    for (int k = 0; k < num_ports; k++) {
      // Init masks to all 1's
      mcast_mask_local[k] = 0;
      mcast_mask_remote[k] = 0;

// Precompute for local destinations
#pragma hls_unroll yes
      for (int j = 0; j < NoC_mcast_dest_width; j++) {
        NVUINTW(num_ports) NoCRouteLUT_tmp = NoCRouteLUT[j];
        if (NoCRouteLUT_tmp[k])
          mcast_mask_local[k][j + NoC2_mcast_dest_width] = 1;
      }
#pragma hls_unroll yes
      for (int j = 0; j < NoC2_mcast_dest_width; j++) {
        mcast_mask_local[k][j] = 1;
      }
// Precompute for remote destinations
#pragma hls_unroll yes
      for (int j = 0; j < NoC2_mcast_dest_width; j++) {
        NVUINTW(num_ports) NoC2RouteLUT_tmp = NoC2RouteLUT[j];
        if (NoC2RouteLUT_tmp[k])
          mcast_mask_remote[k][j] = 1;
      }
#pragma hls_unroll yes
      for (int j = 0; j < NoC_mcast_dest_width; j++) {
        mcast_mask_remote[k][j + NoC2_mcast_dest_width] = 1;
      }
    }

    while (1) {
      wait();
      Flit_t flit_in[num_ports];
      NVUINTW(log_num_vchannels) vcin[num_ports];
      NVUINTW(num_ports) in_valid = 0;
      NVUINTW(num_ports) out_port_notfull = 0;
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {
        out_port[i].TransferNB();
        out_port_notfull[i] = !out_port[i].Full();
      }
      fill_ififo();
      // pick a valid vc per input. VC0 has priority.
      // output: vcin[x] - vc selcted for in port x, in_valid - bitmap of
      // inports valid bits
      inputvc_arbiter(vcin, in_valid);

      // read top flit for each selected vc per in port
      // output: flit_in[x] - top flit on selected vc on port x
      peek_ififo(flit_in, vcin, in_valid);

      // for each selected input, in case of valid header flit only, update
      // out_dest[i][vcin[i]]
      // side effect updating out_dest[x][vc] for each input port x with output
      // port bitmap
      compute_route(flit_in, vcin, in_valid);

      // Variable to hold valid input requests. Set valid[i][j] = 1 if input j
      // is sending flit to output i
      NVUINTW(num_ports) valid[num_ports], select[num_ports];
      NVUINTW(log_num_ports) select_id[num_ports];

      // arbitrate for any top header flits on selected vcs
      // outputs:
      // valid[x] - bitmap of valid input ports to address output port x. valid
      // means there is credit on output port(and vc), and no other packet in
      // flight to that output vc
      // select[x] - bitmap of selected input port (one hot) for output port x
      // select_id[x] - selected input port for output port x
      arbitration(flit_in, in_valid, out_port_notfull, vcin, valid, select,
                  select_id);

      // decide which port is going to push data out
      // side effect updating is_push[x] to indicate that output port x is going
      // to push a flit out
      NVUINTW(num_ports) is_popfifo = 0;
      bool is_push[num_ports];
#pragma hls_unroll yes
      for (int i = 0; i < num_ports;
           i++) {  // Iterating through output ports here
        is_push[i] = false;
        if (select[i] != 0) {
          is_popfifo[select_id[i]] = 1;
          is_push[i] = true;
          CDCOUT(sc_time_stamp() << ": " << name() << " Port:" << i
                                 << " Select: " << select[i] << " select_id "
                                 << select_id[i] << " Valid:" << valid[i]
                                 << " PopFifo: " << is_popfifo[select_id[i]]
                                 << endl,
                 kDebugLevel);
        }
      }

// pop input fifos and prepare credits to be returned to sources
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {  // Iterating over inputs here
        if (is_popfifo[i] == 1) {
          ififo.incrHead(i * num_vchannels + vcin[i]);
          CDCOUT(sc_time_stamp() << ": " << name() << " Popped FIFO of port-"
                                 << i << " VC-" << vcin[i] << endl,
                 kDebugLevel);
        }
      }

      // fill_ififo();
      NVUINTW(log_num_vchannels) vcout[num_ports];
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {
        vcout[i] = 0;
      }
      Flit_t flit_out[num_ports];
      crossbar_traversal(flit_in, is_push, select_id, vcin, vcout, flit_out);

      // sends out flits to be sent
      // side effect: updating is_get_new_packet[x] when tail goes through port
      // x
      flit_output(is_push, vcout, flit_out);
    }
  }
};

// Hybrid Router Design for RC18
// Unicast packets follow WormHole Routing
// Multicast packets follow CutThrough Routing
// Supports only one VC
// Output Buffered. No credits
template <int NumLPorts, int NumRports, int BufferSize, typename FlitType,
          int NoC2Dests, int NoCDests, int NoCMCastDests, int MaxPacketSize>
class HybridRouter : public sc_module {
 public:
  typedef FlitType Flit_t;
  // Declare constants
  enum {
    num_lports = NumLPorts,
    num_rports = NumRports,
    num_ports = num_lports + num_rports,
    num_vchannels = 1,
    log_num_vchannels = nvhls::index_width<num_vchannels>::val,
    log_num_lports = nvhls::index_width<num_lports>::val,
    log_num_rports = nvhls::index_width<num_rports>::val,
    log_num_ports = nvhls::index_width<num_ports>::val,
    NoC_dest_width = NoCDests,
    NoC2_dest_width = NoC2Dests,
    NoC_ucast_dest_width = nvhls::index_width<NoC_dest_width>::val,
    NoC2_ucast_dest_width = nvhls::index_width<NoC2_dest_width>::val,
    NoC_mcast_dest_width = NoCMCastDests,
    NoC2_mcast_dest_width = NoC2_dest_width,
    log_NoC_dest_width = nvhls::index_width<NoC_dest_width>::val,
    log_NoC2_dest_width = nvhls::index_width<NoC2_dest_width>::val,
    ucast_dest_width = NoC_ucast_dest_width + NoC2_ucast_dest_width,
    mcast_dest_width = NoC_mcast_dest_width + NoC2_mcast_dest_width,
    buffersize = BufferSize,
    log_buffersize = nvhls::index_width<buffersize>::val,
    max_packet_size = MaxPacketSize
  };
  static const int kDebugLevel = 4;
  typedef NVUINTW(NoC2_ucast_dest_width) noc2_id_t;
  sc_in<noc2_id_t> noc2_id_jtag;
  noc2_id_t noc2_id;
  NVUINTW(num_ports) NoCRouteLUT[NoC_dest_width];
  NVUINTW(num_ports) NoC2RouteLUT[NoC2_dest_width];

  // Jtag of route
  typedef NVUINTW(num_ports) NoCRouteLUT_t;
  typedef NVUINTW(num_ports) NoC2RouteLUT_t;
  sc_in<NoCRouteLUT_t> NoCRouteLUT_jtag[NoC_dest_width];
  sc_in<NoC2RouteLUT_t> NoC2RouteLUT_jtag[NoC2_dest_width];

  // Input / Output ports
  Connections::InBuffered<Flit_t, 1> in_port[num_ports];
  // Connections::OutBuffered<Flit_t, buffersize> out_port[num_ports];
  nvhls::nv_array<Connections::OutBuffered<Flit_t, 1>, num_ports> out_port;

  sc_in_clk clk;
  sc_in<bool> rst;

  FIFO<Flit_t, buffersize, num_ports> ofifo;
  Arbiter<num_ports> arbiter[num_ports];

  // Store output destination for each port per virtual channel with 1hot
  // encoding
  NVUINTW(num_ports) out_dest[num_ports];
  NVUINTW(num_ports) is_mcast;
  NVUINTW(mcast_dest_width)
  mcast_mask_local[num_ports];  // store mask for each output, if local
                                // destination.
  NVUINTW(mcast_dest_width) mcast_mask_remote[num_ports];  
                                // store mask for each output, if remote
                                // destination.
  NVUINTW(num_ports) is_local_mask[num_ports];
                                // indicates if a given input-output
                                // pair is intended for a local
                                // destination for masking purpose
                                // (if false then remote)

  // Variable that indicates if the output port is available to get new packet
  bool is_get_new_packet[num_ports];

  // Store no. of empty slots in output fifo
  NVUINTW(num_ports) ofifo_is_num_available;
  NVUINTW(num_ports) ofifo_not_full;
  NVUINTW(num_ports) in_valid;

  // Constructor
  SC_HAS_PROCESS(HybridRouter);
  HybridRouter(sc_module_name name_) : sc_module(name_), out_port("out_port") {
    SC_THREAD(process);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }

  void fill_ififo() {
// Read input port if data is available and fill input buffers
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
      in_port[i].TransferNB();
    }
  }

  void inputvc_arbiter() {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      in_valid[i] = !in_port[i].Empty();
    }
  }

  void peek_ififo(Flit_t flit_in[num_ports]) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      // FIFO Read
      if (in_valid[i]) {
        flit_in[i] = in_port[i].Peek();
        CDCOUT(sc_time_stamp()
                   << ": " << name() << hex << " Read from FIFO:" << i
                   << " Flit< " << flit_in[i].flit_id << ","
                   << flit_in[i].data.to_uint64() << ">" << dec << endl,
               kDebugLevel);
      }
    }
  }

  void compute_mcast_route(Flit_t flit_in[num_ports],
                           NVUINTW(log_num_ports) i) {  // i is input port

    NVUINTW(NoC2_mcast_dest_width) noc2_noc2_mask;
    noc2_noc2_mask = ~0;
    noc2_noc2_mask[noc2_id] = 0;
    NVUINTW(mcast_dest_width)
    route = nvhls::get_slc<mcast_dest_width>(flit_in[i].data, 0);
    NVUINTW(NoC2_mcast_dest_width)
    NoC2_out_dest_local = nvhls::get_slc<NoC2_mcast_dest_width>(route, 0);
    NVUINTW(NoC_mcast_dest_width)
    NoC_out_dest_local =
        nvhls::get_slc<NoC_mcast_dest_width>(route, NoC2_mcast_dest_width);
    bool is_local_noc2 = NoC2_out_dest_local[noc2_id];
    bool is_remote_noc2 = ((NoC2_out_dest_local & (noc2_noc2_mask)) != 0);
    NVUINTW(num_ports) noc_dest_local[NoC_mcast_dest_width];
    NVUINTW(num_ports) noc2_dest_local[NoC2_mcast_dest_width];
#pragma hls_unroll yes
    for (int j = 0; j < NoC_mcast_dest_width; j++) {
      if (is_local_noc2 && (NoC_out_dest_local[j] == 1)) {
        noc_dest_local[j] = NoCRouteLUT[j];
      } else {
        noc_dest_local[j] = 0;
      }
    }

#pragma hls_unroll yes
    for (int j = 0; j < NoC2_mcast_dest_width; j++) {
      if (is_remote_noc2 && (NoC2_out_dest_local[j] == 1)) {
        noc2_dest_local[j] = NoC2RouteLUT[j];
      } else {
        noc2_dest_local[j] = 0;
      }
    }

    is_local_mask[i] = 0;
#pragma hls_unroll yes
    for (int j = 0; j < NoC_mcast_dest_width; j++) {
      is_local_mask[i] |= noc_dest_local[j];
    }
    out_dest[i] = is_local_mask[i];

#pragma hls_unroll yes
    for (int j = 0; j < NoC2_mcast_dest_width; j++) {
      out_dest[i] |= noc2_dest_local[j];
    }

    CDCOUT(sc_time_stamp() << ": " << name() << hex << " Mcast OutDest[" << i
                           << "] " << out_dest[i].to_uint64()
                           << " IS-Remote-NoC2: " << is_remote_noc2
                           << " Route: " << route.to_uint64() << endl,
           kDebugLevel);
  }

  void compute_ucast_route(Flit_t flit_in[num_ports],
                           NVUINTW(log_num_ports) i) {

    NVUINTW(ucast_dest_width)
    route = nvhls::get_slc<ucast_dest_width>(flit_in[i].data, 0);
    NVUINTW(NoC2_ucast_dest_width)
    NoC2_out_dest_local =
        nvhls::get_slc<NoC2_ucast_dest_width>(route, NoC_ucast_dest_width);
    NVUINTW(NoC_ucast_dest_width)
    NoC_out_dest_local = nvhls::get_slc<NoC_ucast_dest_width>(route, 0);
    bool is_local_noc2 = (NoC2_out_dest_local == noc2_id);
    bool is_remote_noc2 = !is_local_noc2;

    if (is_local_noc2) {
      out_dest[i] = NoCRouteLUT[NoC_out_dest_local];
    } else {
      out_dest[i] = NoC2RouteLUT[NoC2_out_dest_local];
    }

    CDCOUT(sc_time_stamp() << ": " << name() << hex << " Ucast OutDest[" << i
                           << "] " << out_dest[i]
                           << " IS-Remote-NoC2: " << is_remote_noc2
                           << " NoC Dest: " << NoC_out_dest_local
                           << " NoC2 Dest: " << NoC2_out_dest_local
                           << " Route: " << route.to_uint64() << endl,
           kDebugLevel);
  }

  void compute_route(Flit_t flit_in[num_ports]) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        if (flit_in[i].flit_id.isHeader()) {
          is_mcast[i] = flit_in[i].data[mcast_dest_width];
          if (is_mcast[i]) {
            compute_mcast_route(flit_in, i);
          } else {
            compute_ucast_route(flit_in, i);
          }
        }
      }
    }
  }

  void flit_output(bool is_push[num_ports], Flit_t flit_out[num_ports]) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating over outputs here
      out_port[i].TransferNB();
      if (!ofifo.isEmpty(i) && !out_port[i].Full()) {
        out_port[i].Push(ofifo.pop(i));
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating over outputs here
      if (is_push[i] == true) {
        ofifo.push(flit_out[i], i);
        if (flit_out[i].flit_id.isTail()) {
          is_get_new_packet[i] = true;
        } else {
          is_get_new_packet[i] = false;
        }
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
      ofifo_not_full[i] = !ofifo.isFull(i);
      ofifo_is_num_available[i] = (ofifo.NumAvailable(i) >= max_packet_size);
    }
  }

  void arbitration(Flit_t flit_in[num_ports],
                   NVUINTW(num_ports) valid[num_ports],
                   NVUINTW(num_ports) select[num_ports],
                   NVUINTW(log_num_ports) select_id[num_ports]) {

    NVUINTW(num_ports)
    valid_transpose[num_ports], select_transpose[num_ports],
        mcast_valid_transpose[num_ports];
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      valid[i] = 0;            // valid of ith output is set to 0 here
      valid_transpose[i] = 0;  // valid_transpose of ith input is set to 0 here
      select[i] = 0;           // select of ith output is set to 0 here
      select_transpose[i] = 0;
                    // select_transpose of ith input is set to 0 here
      mcast_valid_transpose[i] = 0;
                    // valid_transpose of ith input is set to 0 here
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (in_valid[i]) {
        bool not_head_flit = (!flit_in[i].flit_id.isHeader());
#pragma hls_unroll yes
        for (int k = 0; k < num_ports; k++) {  // Iterating through the outputs
          // Set valid bit to 1 for local ports only if credits are available at
          // the output port for the virtual channel chosen
          // also make sure that the output is requested, and that it is not
          // head flit, or output[vc] accepts new headers
          valid_transpose[i][k] =
              (((!is_mcast[i]) && (out_dest[i][k] == 1) &&
                (is_get_new_packet[k] || not_head_flit) && (ofifo_not_full[k]))
                   ? 1
                   : 0);
          mcast_valid_transpose[i][k] =
              (((is_mcast[i]) && (out_dest[i][k] == 1) &&
                ((is_get_new_packet[k] && ofifo_is_num_available[k]) ||
                 (not_head_flit && ofifo_not_full[k])))
                   ? 1
                   : 0);
          CDCOUT(sc_time_stamp() << ": " << name() << " Input Port:" << i
                                 << " Output port: " << k
                                 << " valid: " << valid_transpose[i][k]
                                 << " OutStall: " << (ofifo_not_full[k])
                                 << " GetnewPacket: " << is_get_new_packet[k]
                                 << hex << " Flit "
                                 << " <" << flit_in[i].flit_id << ","
                                 << flit_in[i].data.to_uint64() << dec << ">"
                                 << endl,
                 kDebugLevel);
        }
      }
    }

// For Multicast, supress valid  if some of output ports are invalid
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the inputs here
      if (mcast_valid_transpose[i] ^ out_dest[i]) {  //
        mcast_valid_transpose[i] = 0;
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {   // Iterating through the inputs here
      if (mcast_valid_transpose[i] != 0) {  //
        select_transpose[i] = mcast_valid_transpose[i];
#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) {  // Iterating through the outputs here
          select[k][i] = select_transpose[i][k];
        }
// Update valid_transpose if the output is already selected
#pragma hls_unroll yes
        for (int j = 0; j < num_ports; j++) {  // inputs
          if (j >= i + 1) {
#pragma hls_unroll yes
            for (int k = 0; k < num_ports; k++) {  // outputs
              if (mcast_valid_transpose[j][k] && (select[k] != 0)) {
                mcast_valid_transpose[j] = 0;
              }
            }
          }
        }
      }
    }

// Transpose to form valid matrix
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
#pragma hls_unroll yes
      for (int j = 0; j < num_ports; j++) {
        valid[i][j] = valid_transpose[j][i];
        // select[i][j] = select_transpose[j][i];
      }
      CDCOUT(i << " Select: " << select[i] << " Valid: " << valid[i] << endl,
             kDebugLevel);
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      if ((select[i] != 0)) {              //
        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
      }
    }

// Arbitrate for output port if it is header flit
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through the outputs here
      if ((valid[i] != 0) &&
          (select[i] == 0)) {  // There is an input waiting to send to output
                               // i, credits are available at
                               // output, and its input is not
                               // yet assigned by multicast
        select[i] = arbiter[i].pick(valid[i]);
        NVUINTW(num_ports) select_temp = select[i];
        NVUINTW(log_num_ports) select_id_temp;
        one_hot_to_bin<num_ports, log_num_ports>(select_temp, select_id_temp);
        select_id[i] = select_id_temp;
        CDCOUT(sc_time_stamp() << ": " << name() << hex << " Output Port:" << i
                               << " Valid: " << valid[i].to_uint64()
                               << " Select : " << select[i].to_int64() << dec
                               << " Multicast: " << is_mcast[select_id[i]]
                               << endl,
               kDebugLevel);
      }
    }
  }

  void crossbar_traversal(Flit_t flit_in[num_ports], bool is_push[num_ports],
                          NVUINTW(log_num_ports) select_id[num_ports],
                          Flit_t flit_out[num_ports]) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through output ports
      if (is_push[i]) {
        flit_out[i] = flit_in[select_id[i]];
      }
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {  // Iterating through output ports
      if (is_push[i]) {
        NVUINTW(mcast_dest_width)
        route = nvhls::get_slc<mcast_dest_width>(flit_out[i].data, 0);
        if (flit_out[i].flit_id.isHeader() &&
            is_mcast[select_id[i]]) {  // Mask the route in header
          if (is_local_mask[select_id[i]][i]) {
            route &= mcast_mask_local[i];
          } else {
            route &= mcast_mask_remote[i];
          }
        }
        flit_out[i].data = nvhls::set_slc(flit_out[i].data, route, 0);
      }
    }
  }

  void compute_masks() {
#pragma hls_unroll yes
    for (int k = 0; k < num_ports; k++) {
      // Init masks to all 1's
      mcast_mask_local[k] = 0;
      mcast_mask_remote[k] = 0;

// Precompute for local destinations
#pragma hls_unroll yes
      for (int j = 0; j < NoC_mcast_dest_width; j++) {
        NVUINTW(num_ports) NoCRouteLUT_tmp = NoCRouteLUT[j];
        if (NoCRouteLUT_tmp[k])
          mcast_mask_local[k][j + NoC2_mcast_dest_width] = 1;
      }
#pragma hls_unroll yes
      for (int j = 0; j < NoC2_mcast_dest_width; j++) {
        if (j == noc2_id)
          mcast_mask_local[k][j] = 1;
      }
// Precompute for remote destinations
#pragma hls_unroll yes
      for (int j = 0; j < NoC2_mcast_dest_width; j++) {
        NVUINTW(num_ports) NoC2RouteLUT_tmp = NoC2RouteLUT[j];
        if (NoC2RouteLUT_tmp[k] && j != noc2_id)
          mcast_mask_remote[k][j] = 1;
      }
#pragma hls_unroll yes
      for (int j = 0; j < NoC_mcast_dest_width; j++) {
        mcast_mask_remote[k][j + NoC2_mcast_dest_width] = 1;
      }
    }
  }

  void jtag_read() {
#pragma hls_unroll yes
    for (int i = 0; i < NoC_dest_width; i++) {
      NoCRouteLUT[i] = NoCRouteLUT_jtag[i].read();
    }
    noc2_id = noc2_id_jtag.read();
#pragma hls_unroll yes
    for (int i = 0; i < NoC2_dest_width; i++) {
      NoC2RouteLUT[i] = NoC2RouteLUT_jtag[i].read();
    }
  }

  void process() {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++)
      in_port[i].Reset();
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++)
      out_port[i].Reset();
    ofifo.reset();
    // ofifo.fifo_body.reset();
    // ofifo.fifo_body = mem_array_sep<Flit_t, buffersize*num_ports,
    // num_ports>();
    is_mcast = 0;

    for (int i = 0; i < num_ports; i++) {
      is_get_new_packet[i] = true;
      out_dest[i] = 0;
      is_local_mask[i] = 0;
    }
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) {
      ofifo_not_full[i] = 1;
      ofifo_is_num_available[i] = 1;
    }
    wait();
    jtag_read();

    // Precompute masks
    compute_masks();

    while (1) {

      //#pragma hls_unroll yes
      //    for (int i = 0; i < num_ports; i++) {
      //      ofifo_not_full[i] = !ofifo.isFull(i);
      //      ofifo_is_num_available[i] = (ofifo.NumAvailable(i) >=
      //      max_packet_size);
      //    }

      Flit_t flit_in[num_ports];

      // pick a valid vc per input. VC0 has priority.
      // output: in_valid - bitmap of
      // inports valid bits
      inputvc_arbiter();

      // read top flit for each selected vc per in port
      // output: flit_in[x] - top flit on selected vc on port x
      peek_ififo(flit_in);

      // for each selected input, in case of valid header flit only, update
      // out_dest[i]
      // side effect updating out_dest[x] for each input port x with output
      // port bitmap
      compute_route(flit_in);

      // Variable to hold valid input requests. Set valid[i][j] = 1 if input j
      // is sending flit to output i
      NVUINTW(num_ports) valid[num_ports], select[num_ports];
      NVUINTW(log_num_ports) select_id[num_ports];

      // arbitrate for any top header flits on selected vcs
      // outputs:
      // valid[x] - bitmap of valid input ports to address output port x. valid
      // means there is credit on output port(and vc), and no other packet in
      // flight to that output vc
      // select[x] - bitmap of selected input port (one hot) for output port x
      // select_id[x] - selected input port for output port x
      arbitration(flit_in, valid, select, select_id);

      // decide which port is going to push data out
      // side effect updating is_push[x] to indicate that output port x is going
      // to push a flit out
      NVUINTW(num_ports) is_popfifo = 0;
      bool is_push[num_ports];
#pragma hls_unroll yes
      for (int i = 0; i < num_ports;
           i++) {  // Iterating through output ports here
        is_push[i] = false;
        if (select[i] != 0) {
          is_popfifo[select_id[i]] = 1;
          is_push[i] = true;
          CDCOUT(sc_time_stamp() << ": " << name() << " Port:" << i
                                 << " Select: " << select[i] << " select_id "
                                 << select_id[i] << " Valid:" << valid[i]
                                 << " PopFifo: " << is_popfifo[select_id[i]]
                                 << endl,
                 kDebugLevel);
        }
      }

// pop input fifos and prepare credits to be returned to sources
#pragma hls_unroll yes
      for (int i = 0; i < num_ports; i++) {  // Iterating over inputs here
        if (is_popfifo[i] == 1) {
          in_port[i].IncrHead();
          CDCOUT(sc_time_stamp() << ": " << name() << " Popped FIFO of port-"
                                 << i << endl,
                 kDebugLevel);
        }
      }

      Flit_t flit_out[num_ports];
      crossbar_traversal(flit_in, is_push, select_id, flit_out);

      // sends out flits to be sent
      // side effect: updating is_get_new_packet[x] when tail goes through port
      // x
      flit_output(is_push, flit_out);
      fill_ififo();
      wait();
    }
  }
};

#endif  //__WHVCROUTER_H__
