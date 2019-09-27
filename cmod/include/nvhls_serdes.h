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
//========================================================================
// nvhls_serdes.h
//========================================================================
// Implements the Serializer and Deserializer classes

#ifndef NVHLS_SERDES_H
#define NVHLS_SERDES_H

#include <systemc.h>

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_packet.h>
#include <nvhls_connections.h>
#include <hls_globals.h>
#include <fifo.h>
//------------------------------------------------------------------------
// serializer for store-forward router
//------------------------------------------------------------------------
// Converts a Packet into sequence of Flits
// Flits have flit_id, packet_id, data and dest field
// Packet has packet_id, data and dest field
/**
 * \brief serializer for store-forward router 
 * \ingroup SerDes
 *
 * \tparam packet_t       PacketType 
 * \tparam flit_t         FlitType 
 * \tparam Rtype          RouterType 
 *
 * \par Overview
 * Converts a Packet into sequence of Flits.
 * 
 * \par A Simple Example
 * \code
 *      #include <nvhls_serdes.h>
 *
 *      ...
 *      serializer<packet_t, flit_t, StoreForward> serializer_inst;
 *      Connections::In<Packet_t>    in_packet;
 *      Connections::Out<Flit_t>     out_flit;

 *      ...
 *          serializer_inst.clk(clk);
 *          serializer_inst.rst(rst);
 *          serializer_inst.in_packet(in_packet);
 *          serializer_inst.out_flit(out_flit);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename packet_t, typename flit_t, RouterType Rtype = StoreForward>
class serializer : public sc_module {
 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  Connections::In<packet_t> in_packet;
  Connections::Out<flit_t> out_flit;
  static const int header_data_width = flit_t::data_width - packet_t::dest_width;
  static const int num_flits = (((packet_t::data_width-header_data_width) % flit_t::data_width) == 0) ? ((packet_t::data_width-header_data_width) / flit_t::data_width) : ((packet_t::data_width-header_data_width) / flit_t::data_width+1) ;
  enum { width = 0 };
  void Process();

  SC_HAS_PROCESS(serializer);
  serializer(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        in_packet("in_packet"),
        out_flit("out_flit") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  };
};

template <typename packet_t, typename flit_t, RouterType Rtype>
void serializer<packet_t, flit_t, Rtype>::Process() {
  in_packet.Reset();
  out_flit.Reset();
  wait();

  while (1) {
    packet_t packet_reg;
    flit_t flit_reg;
    NVUINTW(packet_t::data_width) data;

    if (in_packet.PopNB(packet_reg)) {
      // Assuming packet_t::data_width is multiple of flit_t::data_width
      flit_reg.dest = packet_reg.dest;
      flit_reg.packet_id = packet_reg.packet_id;
      data = packet_reg.data;
      for (int i = 0; i < num_flits; i++) {
        if (num_flits == 1) {
          flit_reg.flit_id.set(FlitId2bit::SNGL);
        } else if (i == 0) {
          flit_reg.flit_id.set(FlitId2bit::HEAD);
        } else if (i == num_flits - 1) {
          flit_reg.flit_id.set(FlitId2bit::TAIL);
        } else {
          flit_reg.flit_id.set(FlitId2bit::BODY);
        }
        flit_reg.data =
            nvhls::get_slc<flit_t::data_width>(data, i * flit_t::data_width);
        out_flit.Push(flit_reg);
      }
    }
    wait();
  }
}

//------------------------------------------------------------------------
// serializer specialization for WormHole router
//------------------------------------------------------------------------
// Converts a Packet into sequence of Flits
// Flits have flit_id, packet_id, and data field
// Packet has packet_id, data and dest field
/**
 * \brief serializer for WormHole router 
 * \ingroup SerDes
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_serdes.h>
 *
 *      ...
 *      serializer<packet_t, flit_t, WormHole> serializer_inst;
 *      Connections::In<Packet_t>    in_packet;
 *      Connections::Out<Flit_t>     out_flit;

 *      ...
 *          serializer_inst.clk(clk);
 *          serializer_inst.rst(rst);
 *          serializer_inst.in_packet(in_packet);
 *          serializer_inst.out_flit(out_flit);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <int PacketDataWidth, int DestWidthPerHop, int MaxHops,
          int PacketIdWidth, int FlitDataWidth, class FlitId>
class serializer<
    Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth>,
    Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>, WormHole>
    : public sc_module {
  typedef Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth>
      packet_t;
  typedef Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole> flit_t;

 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  static const int header_data_width = flit_t::data_width - packet_t::dest_width;
  // num_flits indicates number of data flits. route flits are not counted
  static const int num_flits = (((packet_t::data_width-header_data_width) % flit_t::data_width) == 0) ? ((packet_t::data_width-header_data_width) / flit_t::data_width) : ((packet_t::data_width-header_data_width) / flit_t::data_width+1) ;
  static const int log_num_flits = nvhls::index_width<num_flits+1>::val;
  Connections::In<packet_t> in_packet;
  Connections::Out<flit_t> out_flit;
  enum { width = 0 };

  void Process() {
    in_packet.Reset();
    out_flit.Reset();
    NVUINTW(log_num_flits) num = 0;
    packet_t packet_reg;
    wait();
    while (1) {
      flit_t flit_reg;
      if (num == 0) {
        packet_reg = in_packet.Pop();
        flit_reg.packet_id = packet_reg.packet_id;
        flit_reg.data = packet_reg.dest;
        NVUINTW(header_data_width) header_data = nvhls::get_slc<header_data_width>(packet_reg.data, 0);
        flit_reg.data = nvhls::set_slc(flit_reg.data, header_data, packet_t::dest_width);
        flit_reg.flit_id.set(
            FlitId2bit::HEAD);  // Destination is sent in first flit
        num++;
      } else {
        flit_reg.packet_id = packet_reg.packet_id;
        if (num == num_flits) {
          flit_reg.flit_id.set(FlitId2bit::TAIL);
          int num_minus_one = num - 1;
          flit_reg.data = nvhls::get_slc(
              packet_reg.data, packet_t::data_width - 1, num_minus_one * flit_t::data_width + header_data_width);
          num = 0;
        } else {
          flit_reg.flit_id.set(FlitId2bit::BODY);
          int num_minus_one = num - 1;
        flit_reg.data = nvhls::get_slc<flit_t::data_width>(
              packet_reg.data, num_minus_one * flit_t::data_width + header_data_width);
          num++;
        }
      }
      out_flit.Push(flit_reg);
      wait();
    }
  }

  SC_HAS_PROCESS(serializer);
  serializer(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        in_packet("in_packet"),
        out_flit("out_flit") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

//------------------------------------------------------------------------
// serializer specialization for WormHole router without packet-id
//------------------------------------------------------------------------
// Converts a Packet into sequence of Flits
/**
 * \brief serializer for WormHole router without packet-id in flits 
 * \ingroup SerDes
 *
 *
 */

template <int PacketDataWidth, int DestWidthPerHop, int MaxHops,
          int FlitDataWidth, class FlitId>
class serializer<Packet<PacketDataWidth, DestWidthPerHop, MaxHops, 0>,
                 Flit<FlitDataWidth, 0, 0, 0, FlitId, WormHole>, WormHole>
    : public sc_module {
  typedef Packet<PacketDataWidth, DestWidthPerHop, MaxHops, 0> packet_t;
  typedef Flit<FlitDataWidth, 0, 0, 0, FlitId, WormHole> flit_t;

 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  // num_flits indicates number of data flits. route flits are not counted
  static const int num_flits = ((packet_t::data_width % flit_t::data_width) == 0) ? (packet_t::data_width / flit_t::data_width) : (packet_t::data_width / flit_t::data_width+1) ;
  Connections::In<packet_t> in_packet;
  Connections::Out<flit_t> out_flit;
  enum { width = 0 };

  void Process() {
    in_packet.Reset();
    out_flit.Reset();
    wait();
    while (1) {
      packet_t packet_reg;
      flit_t flit_reg;

      if (in_packet.PopNB(packet_reg)) {
        flit_reg.data = packet_reg.dest;
        flit_reg.flit_id.set(
            FlitId2bit::HEAD);  // Destination is sent in first flit
        out_flit.Push(flit_reg);
#pragma unroll yes
        for (int i = 0; i < num_flits; i++) {
          // Single packet flit is not possible. We have atleast 1 route flit
          // and 1 data flit. Flit_id cannot be 3
          if (i == num_flits - 1) {
            flit_reg.flit_id.set(FlitId2bit::TAIL);
          } else {
            flit_reg.flit_id.set(FlitId2bit::BODY);
          }
          flit_reg.data = nvhls::get_slc<flit_t::data_width>(
              packet_reg.data, i * flit_t::data_width);
          out_flit.Push(flit_reg);
        }
      }
      wait();
    }
  }

  SC_HAS_PROCESS(serializer);
  serializer(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        in_packet("in_packet"),
        out_flit("out_flit") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

//------------------------------------------------------------------------
// deserializer for store-forward router
//------------------------------------------------------------------------
// Converts a sequence of Flits into a packet
// Flits have flit_id, packet_id, data and dest field
// Packet has packet_id, data and dest field
/**
 * \brief Deserializer for store-forward router 
 * \ingroup SerDes
 *
 * \tparam packet_t       PacketType 
 * \tparam flit_t         FlitType 
 * \tparam buffersize     BufferSize 
 * \tparam Rtype          RouterType 
 *
 * \par Overview
 * Converts a sequence of flits into a packet
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_serdes.h>
 *
 *      ...
 *      deserializer<packet_t, flit_t, buffersize, StoreForward> deserializer_inst;
 *      Connections::In<Packet_t>    in_packet;
 *      Connections::Out<Flit_t>     out_flit;

 *      ...
 *          deserializer_inst.clk(clk);
 *          deserializer_inst.rst(rst);
 *          deserializer_inst.in_packet(in_packet);
 *          deserializer_inst.out_flit(out_flit);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename packet_t, typename flit_t, int buffersize,
          RouterType Rtype = StoreForward>
class deserializer : public sc_module {
 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  Connections::Out<packet_t> out_packet;
  Connections::In<flit_t> in_flit;
  enum { width = 0 };
  flit_t flit_reg;
  // Buffer to store flits from different packets
  packet_t buffer[buffersize];
  static const int logbufsize = (nvhls::index_width<buffersize>::val);
  typedef NVUINTW(logbufsize) BuffIdx;

  // FIFO to store buffer idxes that are available for a new packet
  FIFO<BuffIdx, buffersize> fifo;

  static const int num_flits = packet_t::data_width / flit_t::data_width;
  static const int log_num_flits =
      (nvhls::index_width<num_flits>::val);
  NVUINTW(log_num_flits)
  num_flits_received[buffersize];  // Stores the number of flits received for
                                   // every packet
  void Process();

  SC_HAS_PROCESS(deserializer);
  deserializer(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        out_packet("out_packet"),
        in_flit("in_flit") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  };
};

template <typename packet_t, typename flit_t, int buffersize, RouterType Rtype>
void deserializer<packet_t, flit_t, buffersize, Rtype>::Process() {
  out_packet.Reset();
  in_flit.Reset();
// Reset fifo
#pragma hls_unroll yes
  for (int i = 0; i < buffersize; i++) {
    if (!fifo.isFull()) {
      fifo.push(i);
    }
  }

  wait();

  int packet_pos;  // Indicates position in the buffer where flit is being
                   // inserted
  while (1) {
    if (in_flit.PopNB(flit_reg)) {
      // First flit or Single-flit packet. Allocate new buffer and set valid to
      // 1
      if (flit_reg.flit_id.isHeader()) {
        NVHLS_ASSERT_MSG(!fifo.isEmpty(), "FIFO_empty");
        packet_pos = fifo.pop();
        buffer[packet_pos].packet_id = flit_reg.packet_id;
        buffer[packet_pos].dest = flit_reg.dest;
        buffer[packet_pos].data =
            nvhls::set_slc(buffer[packet_pos].data, flit_reg.data, 0);
        num_flits_received[packet_pos] = 1;
      } else {
// Search for buffer storing the packet and insert flit in appropriate position.
// Assuming Flits arrive in-order
#pragma hls_unroll yes
        for (int i = 0; i < buffersize; i++) {
          BuffIdx head = fifo.get_head();
          BuffIdx tail = fifo.get_tail();
          // Check if buffer entry is valid
          if((((tail < head) && ((i >= tail) && (i < head))) ||
              ((head < tail) && ((i >= tail) || (i < head))) || fifo.isFull()) && (!fifo.isEmpty()))  {
            if (buffer[i].packet_id == flit_reg.packet_id) {
              buffer[i].data =
                  nvhls::set_slc(buffer[i].data, flit_reg.data,
                                 num_flits_received[i] * flit_t::data_width);
              num_flits_received[i]++;
              packet_pos = i;
            }
          }
        }
      }
      // Last flit or Single-flit packet. Write data out. Assuming all packet
      // and flit sizes are same in the network
      if (flit_reg.flit_id.isTail()) {
        out_packet.Push(buffer[packet_pos]);
        // Release buffer and insert to fifo
        NVHLS_ASSERT_MSG(!fifo.isFull(), "FIFO_full");
        fifo.push(packet_pos);
      }
    }
    wait();
  }
};

//------------------------------------------------------------------------
// deserializer specialization for Wormhole Router
//------------------------------------------------------------------------
// Converts a Packet into sequence of Flits
// Flits have flit_id, packet_id, data and dest field
// Packet has packet_id, data and dest field
/**
 * \brief Deserializer for Wormhole router 
 * \ingroup SerDes
 *
 * \tparam packet_t       PacketType 
 * \tparam flit_t         FlitType 
 * \tparam buffersize     BufferSize 
 * \tparam Rtype          RouterType 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_serdes.h>
 *
 *      ...
 *      deserializer<packet_t, flit_t, buffersize, Wormhole> deserializer_inst;
 *      Connections::In<Packet_t>    in_packet;
 *      Connections::Out<Flit_t>     out_flit;

 *      ...
 *          deserializer_inst.clk(clk);
 *          deserializer_inst.rst(rst);
 *          deserializer_inst.in_packet(in_packet);
 *          deserializer_inst.out_flit(out_flit);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <int PacketDataWidth, int DestWidthPerHop, int MaxHops,
          int PacketIdWidth, int FlitDataWidth, class FlitId, int buffersize>
class deserializer<
    Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth>,
    Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>, buffersize,
    WormHole> : public sc_module {
  typedef Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth>
      packet_t;
  typedef Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole> flit_t;

 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  Connections::Out<packet_t> out_packet;
  Connections::In<flit_t> in_flit;
  enum { width = 0 };
  // Buffer to store packets
  packet_t buffer[buffersize];
  static const int logbufsize = (nvhls::index_width<buffersize>::val);
  typedef NVUINTW(logbufsize) BuffIdx;

  // FIFO to store buffer idxes that are available for a new packet
  FIFO<BuffIdx, buffersize> fifo;

  static const int num_flits = packet_t::data_width / flit_t::data_width;
  static const int log_num_flits = (nvhls::index_width<num_flits>::val);
  NVUINTW(log_num_flits)
  num_flits_received[buffersize];  // Stores the number of flits received for
                                   // every packet
  void Process() {
    out_packet.Reset();
    in_flit.Reset();
// Reset fifo
#pragma hls_unroll yes
    for (int i = 0; i < buffersize; i++) {
      if (!fifo.isFull()) {
        fifo.push(i);
      }
    }

    wait();

    int packet_pos;  // Indicates position in the buffer where flit is being
                     // inserted
    while (1) {
      flit_t flit_reg;
      if (in_flit.PopNB(flit_reg)) {
        // First flit or Single-flit packet. Allocate new buffer and set valid
        // to 1
        if (flit_reg.flit_id.isHeader()) {
          NVHLS_ASSERT_MSG(!fifo.isEmpty(), "FIFO_empty");
          packet_pos = fifo.pop();
          buffer[packet_pos].packet_id = flit_reg.packet_id;
          buffer[packet_pos].dest = flit_reg.data;
          num_flits_received[packet_pos] = 0;
        } else {
// Search for buffer storing the packet and insert flit in appropriate position.
// Assuming Flits arrive in-order
#pragma hls_unroll yes
          for (int i = 0; i < buffersize; i++) {
            // Check if buffer entry is valid
            BuffIdx head = fifo.get_head();
            BuffIdx tail = fifo.get_tail();
            if((((tail < head) && ((i >= tail) && (i < head))) ||
              ((head < tail) && ((i >= tail) || (i < head))) || fifo.isFull()) && (!fifo.isEmpty()))  {
              if (buffer[i].packet_id == flit_reg.packet_id) {
                buffer[i].data =
                    nvhls::set_slc(buffer[i].data, flit_reg.data,
                                   num_flits_received[i] * flit_t::data_width);
                num_flits_received[i]++;
                packet_pos = i;
              }
            }
          }
        }
        // Last flit or Single-flit packet. Write data out. 
        if (flit_reg.flit_id.isTail()) {
          out_packet.Push(buffer[packet_pos]);
          // Release buffer
          NVHLS_ASSERT_MSG(!fifo.isFull(), "FIFO_full");
          fifo.push(packet_pos);
        }
      }
      wait();
    }
  }

  SC_HAS_PROCESS(deserializer);
  deserializer(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        out_packet("out_packet"),
        in_flit("in_flit") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

//------------------------------------------------------------------------
// deserializer (specialization for Wormhole flit without input buffer)
//------------------------------------------------------------------------
/**
 * \brief Deserializer for Wormhole router without input buffer 
 * \ingroup SerDes
 *
 * \tparam packet_t       PacketType 
 * \tparam flit_t         FlitType 
 * \tparam Rtype          RouterType 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_serdes.h>
 *
 *      ...
 *      deserializer<packet_t, flit_t, 0, Wormhole> deserializer_inst;
 *      Connections::In<Packet_t>    in_packet;
 *      Connections::Out<Flit_t>     out_flit;

 *      ...
 *          deserializer_inst.clk(clk);
 *          deserializer_inst.rst(rst);
 *          deserializer_inst.in_packet(in_packet);
 *          deserializer_inst.out_flit(out_flit);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <int PacketDataWidth, int DestWidthPerHop, int MaxHops,
          int PacketIdWidth, int FlitDataWidth, class FlitId>
class deserializer<
    Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth>,
    Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>, 0,
    WormHole> : public sc_module {
  typedef Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth> packet_t;
  typedef Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole> flit_t;

 public:
  sc_in_clk clk;
  sc_in<bool> rst;

  Connections::Out<packet_t> out_packet;
  Connections::In<flit_t> in_flit;
  enum { width = 0 };
  // Buffer to store packets.
  packet_t buffer;
  static const int header_data_width = flit_t::data_width - packet_t::dest_width;
  // num_flits indicates number of data flits. route flits are not counted. 
  static const int num_flits = (((packet_t::data_width-header_data_width) % flit_t::data_width) == 0) ? ((packet_t::data_width-header_data_width) / flit_t::data_width) : ((packet_t::data_width-header_data_width) / flit_t::data_width+1) ;
  static const int log_num_flits = nvhls::index_width<num_flits>::val;
  NVUINTW(log_num_flits) num_flits_received;  // Stores the number of flits received for every packet
  void Process();

  SC_HAS_PROCESS(deserializer);
  deserializer(sc_module_name name)
      : sc_module(name),
        clk("clk"),
        rst("rst"),
        out_packet("out_packet"),
        in_flit("in_flit") {
    SC_THREAD(Process);
    sensitive << clk.pos();
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  };
};

template <int PacketDataWidth, int DestWidthPerHop, int MaxHops,
          int PacketIdWidth, int FlitDataWidth, class FlitId>
void deserializer<
    Packet<PacketDataWidth, DestWidthPerHop, MaxHops, PacketIdWidth>,
    Flit<FlitDataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>, 0,
    WormHole>::Process() {
  out_packet.Reset();
  in_flit.Reset();
  num_flits_received = 0;
  buffer.data = 0;
  wait();

  while (1) {
    flit_t flit_reg;
    if (in_flit.PopNB(flit_reg)) {
      // First flit
      if (flit_reg.flit_id.isHeader()) {
        buffer.dest = static_cast<NVUINTW(packet_t::dest_width)> (flit_reg.data);
        buffer.data = nvhls::get_slc<header_data_width>(flit_reg.data, packet_t::dest_width);
        num_flits_received = 0;
      } else {
        buffer.data |= (static_cast<NVUINTW(PacketDataWidth)>(flit_reg.data) << num_flits_received * flit_t::data_width + header_data_width);
        num_flits_received++;
      }
      // Last flit or Single-flit packet. Write data out. 
      if (flit_reg.flit_id.isTail()) {
        out_packet.Push(buffer);
        num_flits_received = 0;
        buffer.data = 0;
      }
    }
    wait();
  }
}

#endif /*NVHLS_SERDES_H*/
