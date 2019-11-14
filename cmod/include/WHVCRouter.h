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

#ifndef __WHVCROUTER_H__
#define __WHVCROUTER_H__

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

template <int NumLPorts, int NumRports, int NumVchannels, int BufferSize,
          typename FlitType = Flit<64, 0, 0, 0, FlitId2bit, WormHole> >
class WHVCRouterBase : public sc_module {
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

  // Credit registers to store credits for both producer and consumer
  Credit_t credit_recv[num_ports * num_vchannels];
  Credit_t credit_send[num_ports * num_vchannels];

  // Variable to register outputs
  Flit_t flit_out[num_ports];

  // Constructor
  SC_HAS_PROCESS(WHVCRouterBase);
  WHVCRouterBase(sc_module_name name_) : sc_module(name_) {
    SC_THREAD(process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
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
      NVHLS_ASSERT_MSG(credit_recv[i] <= buffersize, "Total_credits_received_cannot_be_larger_than_Buffer_size");
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
          NVHLS_ASSERT_MSG(!ififo.isFull(i * num_vchannels + vcin_tmp), "Input_fifo_is_full");
          ififo.push(inflit[i], i * num_vchannels + vcin_tmp);
        } else {
          NVHLS_ASSERT_MSG(!ififo.isFull(i), "Input_fifo_is_full");
          ififo.push(inflit[i], i);
        }
      }
    }
  }

  void peek_ififo(Flit_t flit_in[num_ports],
                  NVUINTW(log_num_vchannels) vcin[num_ports],
                  NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through the inputs here
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

  void crossbar_traversal(Flit_t flit_in[num_ports], bool is_push[num_ports],
                          NVUINTW(log_num_ports) select_id[num_ports],
                          NVUINTW(log_num_vchannels) vcin[num_ports],
                          NVUINTW(log_num_vchannels) vcout[num_ports]) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through output ports here
      if (is_push[i]) {
        flit_out[i] = flit_in[select_id[i]];
        vcout[i] = vcin[select_id[i]];
      }
    }
  }


  virtual void reset() { }
  virtual void run() { }

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
    reset();

    // Reset credit registers
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      credit_send[i] = 0;
      credit_recv[i] = buffersize;
    }

    #pragma hls_pipeline_init_interval 1
    while (1) {
      wait();
      run();
    }
  }
};

// Specialization for SourceRouting
/**
 * \brief Wormhole Router with virtual channels 
 * \ingroup WHVCRouter
 *
 * \tparam NumLPorts        Number of local ingress/egress ports 
 * \tparam NumRports        Number of remote ports 
 * \tparam NumVchannels     Number of virtual channels
 * \tparam BufferSize       Buffersize of input fifo 
 * \tparam FlitType         Indicates the Flit type 
 * \tparam MaxHops          Indicates Max. number of hops for SourceRouting
 *
 * \par A Simple Example
 * \code
 *      #include <WHVCRouter.h>
 *
 *      ...
 *      
 *        typedef Flit<64, 0, 0, 0, FlitId2bit, WormHole> Flit_t;
 *        WHVCRouter<kNumLPorts, kNumRPorts, kNumVChannels, kBufferSize, Flit_t, kNumMaxHops> router;
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
          typename FlitType, int MaxHops>
class WHVCSourceRouter: public WHVCRouterBase<NumLPorts, NumRports, NumVchannels, BufferSize, FlitType> {
public:
  // Declare constants
  typedef WHVCRouterBase<NumLPorts, NumRports, NumVchannels, BufferSize, FlitType> BaseClass;
  typedef FlitType Flit_t;
  enum {
    num_lports = BaseClass::num_lports,
    num_rports = BaseClass::num_rports,
    num_ports = BaseClass::num_ports,
    num_vchannels = BaseClass::num_vchannels,
    log_num_rports = BaseClass::log_num_rports,
    log_num_ports = BaseClass::log_num_ports,
    log_num_vchannels = BaseClass::log_num_vchannels,
    dest_width_per_hop = log_num_rports + num_lports,
    dest_width = MaxHops * dest_width_per_hop,
  };

  WHVCSourceRouter(sc_module_name name_): BaseClass(name_) {

  }

  Arbiter<num_ports> arbiter[num_ports];

  // Store output destination for each port per virtual channel with 1hot
  // encoding
  NVUINTW(num_ports) out_dest[num_ports][num_vchannels];

  // Variable to block output port until previous push is successful
  NVUINTW(num_ports) out_stall;

  // Variable that indicates if the output port is available to get new packet
  bool is_get_new_packet[num_ports * num_vchannels];

  void inputvc_arbiter(NVUINTW(log_num_vchannels) vcin[num_ports],
                       NVUINTW(num_ports) & in_valid) {
#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through the inputs here
      vcin[i] = 0;
      // Doing static arbitration across input VCs here. VC0 has
      // highest priority. We are assuming that input and output VCs
      // are same.
      if (num_vchannels > 1) {
        NVUINTW(num_vchannels) vcin_valid = 0;
        NVUINTW(log_num_vchannels) vcin_local = 0;
#pragma hls_unroll yes
        for (int j = 0; j < num_vchannels; j++) {
          if ((!this->ififo.isEmpty(i * num_vchannels + j))) {
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
          if (!this->ififo.isEmpty(i)) {
            in_valid[i] = 1;
          }
        }
      }

    }
  }
  void compute_route(Flit_t flit_in[num_ports],
                     NVUINTW(log_num_vchannels) vcin[num_ports],
                     NVUINTW(num_ports) in_valid) {

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through the inputs here
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
    for (int i = 0; i < num_ports; i++) { // Iterating over outputs here
      if (is_push[i] == true || out_stall[i] == 1) {
        bool temp = this->out_port[i].PushNB(this->flit_out[i]);
        if (this->flit_out[i].flit_id.isTail()) {
          is_get_new_packet[i * num_vchannels + vcout[i]] = true;
        } else {
          is_get_new_packet[i * num_vchannels + vcout[i]] = false;
        }
        if (temp == 0)
          out_stall[i] = 1;
        else {
          out_stall[i] = 0;
          this->credit_recv[i * num_vchannels + vcout[i]]--;
        }
        DCOUT(sc_time_stamp()
              << ": " << this->name() << " OutPort " << i
              << " Push success??: " << temp << " Decrementing Credit status "
              << i * num_vchannels + vcout[i] << ": "
              << this->credit_recv[i * num_vchannels + vcout[i]]
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
    for (int i = 0; i < num_ports; i++) { // Iterating through the outputs here
      valid[i] = 0; // valid of ith output is set to 0 here
    }

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through the inputs here
      if (in_valid[i]) {
        bool not_head_flit = (!flit_in[i].flit_id.isHeader());

#pragma hls_unroll yes
        for (int k = 0; k < num_ports;
             k++) { // Iterating through the outputs here
          int out_idx = k * num_vchannels + vcin[i];

          // Set valid bit to 1 for local ports only if credits are available at
          // the output port for the virtual channel chosen
          // also make sure that the output is requested, and that it is not
          // head flit, or output[vc] accepts new headers
          valid[k][i] =
              (((this->credit_recv[out_idx] != 0) && (out_dest[i][vcin[i]][k] == 1) &&
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
    is_multicast = 0; // Store if an input is unicast or multicast
    NVUINTW(log_num_ports) remote_portid[num_ports];
    NVUINTW(log_num_lports) local_portid[num_ports];

#pragma hls_unroll yes
    for (int i = 0; i < num_ports; i++) { // Iterating through the inputs here
#pragma hls_unroll yes
      for (int l = 0; l < num_lports;
           l++) { // Iterating through the outputs here
#pragma hls_unroll yes
        for (int r = num_lports; r < num_ports; r++) {
          if (out_dest[i][vcin[i]][l] && out_dest[i][vcin[i]][r]) {
            is_multicast[i * num_vchannels + vcin[i]] = 1;
            remote_portid[i] = r;
            local_portid[i] = l;
            if (!(valid[l][i] && valid[r][i])) {
              valid[l][i] = 0;
              valid[r][i] = 0; // valid of local port is 1 only is valid of
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
    for (int i = 0; i < num_ports; i++) { // Iterating through the outputs here
      select[i] = 0;
      if (valid[i] != 0) { // There is an input waiting to send to output
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
         i++) { // Iterating through the remote outputs here
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
         i++) { // Iterating through the remote outputs here
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

  void reset() {
    out_stall = 0;
    for (int i = 0; i < num_ports * num_vchannels; i++) {
      is_get_new_packet[i] = true;
    }
  }

  
  void run() {
    this->receive_credit();
    this->fill_ififo();

    Flit_t flit_in[num_ports];
    NVUINTW(log_num_vchannels) vcin[num_ports];
    NVUINTW(num_ports) in_valid = 0;

    // pick a valid vc per input. VC0 has priority.
    // output: vcin[x] - vc selcted for in port x, in_valid - bitmap of
    // inports valid bits
    inputvc_arbiter(vcin, in_valid);

    // read top flit for each selected vc per in port
    // output: flit_in[x] - top flit on selected vc on port x
    this->peek_ififo(flit_in, vcin, in_valid);

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
         i++) { // Iterating through output ports here
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
    for (int i = 0; i < num_ports; i++) { // Iterating over inputs here
      if (is_popfifo[i] == 1) {
        this->ififo.incrHead(i * num_vchannels + vcin[i]);
        // DCOUT(sc_time_stamp() << ": " << name() << " Popped FIFO of port-"
        // << i
        //          << " VC-" << vcin[i] << endl);
        this->credit_send[i * num_vchannels + vcin[i]]++;
        NVHLS_ASSERT_MSG(this->credit_send[i * num_vchannels + vcin[i]] <= BaseClass::buffersize, "Total_credits_cannot_be_larger_than_buffer_size");
      }
    }

    // fill_ififo();
    this->send_credit();
    NVUINTW(log_num_vchannels) vcout[num_ports];

    this->crossbar_traversal(flit_in, is_push, select_id, vcin, vcout);

    // sends out flits to be sent
    // side effect: updating is_get_new_packet[x] when tail goes through port
    // x
    // and out_stall when push_nb fails
    flit_output(is_push, vcout);
  }
};

#endif //__WHVCROUTER_H__
