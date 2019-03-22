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
// nvhls_packet.h
//========================================================================
// Packet and Flit class implementations

#ifndef NVHLS_PACKET_H
#define NVHLS_PACKET_H

#include <systemc.h>
#include <nvhls_types.h>
#include <nvhls_marshaller.h>
#include <nvhls_message.h>

//------------------------------------------------------------------------
// Packet
//------------------------------------------------------------------------
// Packet-id acts as unique identifier for the flits of a packet.
//   NOTE: 0 is default value. Initialize to value!=0.
//
// Dest denotes the packet destination
//
// For Source routing, Dest specifies the "route"

enum RouterType { StoreForward, WormHole };

/**
 * \brief Parameterized implementation of a network packet. 
 * \ingroup nvhls_packet
 *
 * \tparam DataWidth        Width of data field in a packet.
 * \tparam DestWidthPerHop  Number of bits required to encode destination field for one hop in a packet. 
 * \tparam MaxHops          Maximum number of hops. For source routing, this field is used along with DestWidthPerHop field to calculate total width of destination field to be encoded in a packet. (default: 1)
 * \tparam PacketIdWidth    Width of packet id field. PacketID acts a an identified  for every hop in a packet. (default: 0)
 *
 * \par Overview
 * - Packet class is a generalized implementation of a network packet. Packet Format: <PACKET-ID>:<DEST>:<DATA>
 * - PacketID field is optional and  can be removed by setting PacketIdWidth to 0.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_packet.h>
 *
 *      ...
 *      typedef Packet<PACKET_DATAWIDTH,DESTWIDTHPERHOP,MAXHOPS,PACKETIDWIDTH> Packet_t
 *      Packet_t packet;
 *      packet.data = 0xdeadbeef; // Initialize data
 *      packet.dest = 0xde        // Set destination information for data 
 *          ...
 *
 * \endcode
 * \par
 *
 */
template <int DataWidth, int DestWidthPerHop, int MaxHops = 1,
          int PacketIdWidth = 0>
class Packet : public nvhls_message {
 public:
  Packet() { reset(); }

  Packet(const Packet& other) {
    data = other.data;
    dest = other.dest;
    packet_id = other.packet_id;
  }

  Packet& operator=(const Packet& other) {
    data = other.data;
    dest = other.dest;
    packet_id = other.packet_id;
    return *this;
  }

  void reset() {
    data = 0;
    dest = 0;
    packet_id = 0;
  }

  enum {
    data_width = DataWidth,
    dest_width_per_hop = DestWidthPerHop,
    dest_width = DestWidthPerHop * MaxHops,
    packet_id_width = PacketIdWidth,
    width = data_width + dest_width + packet_id_width,
    data_start_bit = 0,
    data_end_bit = data_width - 1,
    dest_start_bit = data_end_bit + 1,
    dest_end_bit = dest_start_bit + dest_width - 1,
    packet_id_start_bit = dest_end_bit + 1,
    packet_id_end_bit = packet_id_start_bit + packet_id_width - 1
  };

  NVUINTW(data_width) data;
  NVUINTW(dest_width) dest;
  NVUINTW(packet_id_width) packet_id;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& data;
    m& dest;
    m& packet_id;
  }
};

//-----------------------------------------------------------------------
// Packet (specialization without ID)
//-----------------------------------------------------------------------
template <int DataWidth, int DestWidthPerHop, int MaxHops>
class Packet<DataWidth, DestWidthPerHop, MaxHops, 0> : public nvhls_message {
 public:
  Packet() { reset(); }

  Packet(const Packet& other) {
    data = other.data;
    dest = other.dest;
  }

  Packet& operator=(const Packet& other) {
    data = other.data;
    dest = other.dest;
    return *this;
  }

  void reset() {
    data = 0;
    dest = 0;
  }

  enum {
    data_width = DataWidth,
    dest_width_per_hop = DestWidthPerHop,
    dest_width = DestWidthPerHop * MaxHops,
    width = data_width + dest_width,
    data_start_bit = 0,
    data_end_bit = data_width - 1,
    dest_start_bit = data_end_bit + 1,
    dest_end_bit = dest_start_bit + dest_width - 1,
  };

  NVUINTW(data_width) data;
  NVUINTW(dest_width) dest;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& data;
    m& dest;
  }
};

//------------------------------------------------------------------------
// Flit
//------------------------------------------------------------------------
class FlitId2bit;

template <int DataWidth, int DestWidthPerHop, int MaxHops = 1,
          int PacketIdWidth = 0, class FlitId = FlitId2bit,
          RouterType Rtype = StoreForward>
class Flit;

/**
 * \brief Encoding for FlitID. 
 * \ingroup nvhls_packet
 *
 * \par Overview
 * - Flit-id is used to indicate start, intermediate payload, end, and single-flit packet 
 * - Implemented as a class, passed as an argument to Flit class
 * - Encoding: BODY:0, HEAD:1, TAIL:2, SNGL:3 
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_packet.h>
 *
 *      ...
 *      typedef Flit<FLIT_DATAWIDTH,0,0,PACKETIDWIDTH,FLITIDTYPE,WormHole> Flit_t;
 *      Flit_t flit;
 *      flit.flit_id.set(FlitId2bit::HEAD);
 *      ...
 *      flit.flit_id.set(FlitId2bit::BODY);
 *      ...
 *      flit.flit_id.set(FlitId2bit::TAIL);
 *      ...
 *
 * \endcode
 * \par
 *
 */
class FlitId2bit : public nvhls_message {
 public:
  enum { width = 2 };

  typedef enum { BODY = 0, HEAD = 1, TAIL = 2, SNGL = 3 } Encoding;

  bool isHeader() const { return (data[0] == 1); }
  bool isBody() const { return (data == BODY); }
  bool isTail() const { return (data[1] == 1); }
  bool isSingle() const { return (data == SNGL); }

  void set(const Encoding& enc) { data = enc; }
  void reset() { set(BODY); }

  bool operator==(const FlitId2bit& other) const {
    return (data == other.data);
  }
  FlitId2bit& operator=(const FlitId2bit& other) {
    data = other.data;
    return *this;
  }

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& data;
  }

  // Getter and setter providing raw access to the underlying data type
  void set_raw(const NVUINTC(width) a) { data = a; }
  NVUINTC(width) get_raw() { return data; }

  NVUINTC(width) data;

};

#ifndef __SYNTHESIS__
// NOTE: there is no real requirement to have this function templated. The only usage I am aware of it is with T=ostream
// the reason this function was made templated is to avoid linker error. When multiple C files would independently include
// current h file, unless this is a template function it would result a conflict in linkage stage because multiple
// objects will have the implementation of this function. The "real" solution is to move all function bodies into C files,
// and if necessary libraries. The quick and dirty workaround applied here relies on the fact that template functions are
// required to be in h files which forces linker to resolve these kind of conflicts. So by templating this function 
// we avoid linker conflict scenario. Potentially can move it into a library in the future if/once such is created.
template<class T>
T& operator<<(T& os, const FlitId2bit& id) {
  os << id.data;
  return os;
}
#endif
/**
 * \brief Parameterized implementation of a network flit. 
 * \ingroup nvhls_packet
 *
 * \tparam DataWidth        Width of data field in a flit.
 * \tparam DestWidthPerHop  Number of bits required to encode destination field for one hop in a packet. 
 * \tparam MaxHops          Maximum number of hops. For source routing, this field is used along with DestWidthPerHop field to calculate total width of destination field to be encoded in a packet. (default: 1)
 * \tparam PacketIdWidth    Width of packet id field. PacketID acts a an identified  for every hop in a packet. (default: 0)
 *
 * \par Overview
 * - Flit class implements a parameterized network flit.
 * - Flit format varies based on the type of router.
 * - StoreForward router flit format: <FlitID>:<PACKET-ID>:<DEST>:<DATA>
 * - Wormhole router flit format: <FlitID>:<PACKET-ID>:<DATA>. <DEST> is encoded in header flit.
 * - PacketID field is optional and  can be removed by setting PacketIdWidth to 0.
 *
 * \par A Simple Example
 * \code
 *      #include <nvhls_packet.h>
 *
 *      ...
 *      typedef Flit<FLIT_DATAWIDTH,0,0,PACKETIDWIDTH,FLITIDTYPE,WormHole> Flit_t;
 *      Flit_t flit;
 *      flit.data = 0xdeadbeef; // Initialize data
 *      flit.flit_id.set(FlitId2bit::BODY);
 *          ...
 *
 * \endcode
 * \par
 *
 */
template <int DataWidth, int DestWidthPerHop, int MaxHops, int PacketIdWidth,
          class FlitId>
class Flit<DataWidth, DestWidthPerHop, MaxHops, PacketIdWidth, FlitId,
           StoreForward> : public nvhls_message {
 public:
  Flit();

  Flit(const Flit& other);

  Flit& operator=(const Flit& other) {
    data = other.data;
    dest = other.dest;
    packet_id = other.packet_id;
    flit_id = other.flit_id;
    return *this;
  }

  void reset();

  enum {
    data_width = DataWidth,
    dest_width_per_hop = DestWidthPerHop,
    dest_width = DestWidthPerHop * MaxHops,
    packet_id_width = PacketIdWidth,
    flit_id_width = FlitId::width,
    width = data_width + dest_width + packet_id_width + flit_id_width,
    data_start_bit = 0,
    data_end_bit = data_width - 1,
    dest_start_bit = data_end_bit + 1,
    dest_end_bit = dest_start_bit + dest_width - 1,
    packet_id_start_bit = dest_end_bit + 1,
    packet_id_end_bit = packet_id_start_bit + packet_id_width - 1,
    flit_id_start_bit = packet_id_end_bit + 1,
    flit_id_end_bit = flit_id_start_bit + flit_id_width - 1
  };

  NVUINTW(data_width) data;
  NVUINTW(dest_width) dest;
  NVUINTW(packet_id_width) packet_id;
  FlitId flit_id;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& data;
    m& dest;
    m& packet_id;
    m& flit_id;
  }
};

template <int DataWidth, int DestWidthPerHop, int MaxHops, int PacketIdWidth,
          class FlitId>
void Flit<DataWidth, DestWidthPerHop, MaxHops, PacketIdWidth, FlitId,
          StoreForward>::reset() {
  data = 0;
  dest = 0;
  packet_id = 0;
  flit_id.reset();
}

template <int DataWidth, int DestWidthPerHop, int MaxHops, int PacketIdWidth,
          class FlitId>
Flit<DataWidth, DestWidthPerHop, MaxHops, PacketIdWidth, FlitId,
     StoreForward>::Flit() {
  reset();
}

template <int DataWidth, int DestWidthPerHop, int MaxHops, int PacketIdWidth,
          class FlitId>
Flit<DataWidth, DestWidthPerHop, MaxHops, PacketIdWidth, FlitId,
     StoreForward>::Flit(const Flit<DataWidth, DestWidthPerHop, MaxHops,
                                    PacketIdWidth, FlitId,
                                    StoreForward>& other) {
  data = other.data;
  dest = other.dest;
  packet_id = other.packet_id;
  flit_id = other.flit_id;
}

//------------------------------------------------------------------------
// WHFlit
//------------------------------------------------------------------------
// Packet-id acts as unique identifier for the flits of a packet
// Flits of a packet take the same route and are always ordered
// Flit-id is used to indicate type of payload : start, data, end

template <int DataWidth, int PacketIdWidth, class FlitId>
class Flit<DataWidth, 0, 0, PacketIdWidth, FlitId, WormHole> : public nvhls_message {
 public:
  Flit();

  Flit(const Flit& other);

  Flit& operator=(const Flit& other) {
    data = other.data;
    packet_id = other.packet_id;
    flit_id = other.flit_id;
    return *this;
  }

  void reset();

  enum {
    data_width = DataWidth,
    packet_id_width = PacketIdWidth,
    flit_id_width = FlitId::width,
    width = data_width + packet_id_width + flit_id_width,
    data_start_bit = 0,
    data_end_bit = data_width - 1,
    packet_id_start_bit = data_end_bit + 1,
    packet_id_end_bit = packet_id_start_bit + packet_id_width - 1,
    flit_id_start_bit = packet_id_end_bit + 1,
    flit_id_end_bit = flit_id_start_bit + flit_id_width - 1
  };

  NVUINTW(data_width) data;
  NVUINTW(packet_id_width) packet_id;
  FlitId flit_id;

  NVUINTW(packet_id_width) get_packet_id() const {
    return packet_id;
  }

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& data;
    m& packet_id;
    m& flit_id;
  }
};

template <int DataWidth, int PacketIdWidth, class FlitId>
void Flit<DataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>::reset() {
  data = 0;
  packet_id = 0;
  flit_id.reset();
}

template <int DataWidth, int PacketIdWidth, class FlitId>
Flit<DataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>::Flit() {
  reset();
}

template <int DataWidth, int PacketIdWidth, class FlitId>
Flit<DataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>::Flit(
    const Flit<DataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>& other) {
  data = other.data;
  packet_id = other.packet_id;
  flit_id = other.flit_id;
}

template <int DataWidth, int PacketIdWidth, class FlitId>
inline std::ostream& operator<<(ostream& os, const Flit<DataWidth, 0, 0, PacketIdWidth, FlitId, WormHole>& flit)
{
    os << "<FlitId, PacketID, Data> <" << flit.flit_id << "," << flit.packet_id << "," << flit.data << ">";
    return os;
}
// WHFlit specialization without packet id
template <int DataWidth, class FlitId>
class Flit<DataWidth, 0, 0, 0, FlitId, WormHole> : public nvhls_message {
 public:
  Flit();

  Flit(const Flit& other);

  Flit& operator=(const Flit& other) {
    data = other.data;
    flit_id = other.flit_id;
    return *this;
  }

  void reset();

  enum {
    data_width = DataWidth,
    flit_id_width = FlitId::width,
    width = data_width + flit_id_width,
    data_start_bit = 0,
    data_end_bit = data_width - 1,
    flit_id_start_bit = data_end_bit + 1,
    flit_id_end_bit = flit_id_start_bit + flit_id_width - 1
  };

  NVUINTW(data_width) data;
  FlitId flit_id;

  int get_packet_id() const {
    return 0;
  }

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& data;
    m& flit_id;
  }
};

template <int DataWidth, class FlitId>
void Flit<DataWidth, 0, 0, 0, FlitId, WormHole>::reset() {
  data = 0;
  flit_id.reset();
}

template <int DataWidth, class FlitId>
Flit<DataWidth, 0, 0, 0, FlitId, WormHole>::Flit() {
  reset();
}

template <int DataWidth, class FlitId>
Flit<DataWidth, 0, 0, 0, FlitId, WormHole>::Flit(
    const Flit<DataWidth, 0, 0, 0, FlitId, WormHole>& other) {
  data = other.data;
  flit_id = other.flit_id;
}

template <int DataWidth, class FlitId>
inline std::ostream& operator<<(ostream& os, const Flit<DataWidth, 0, 0, 0, FlitId, WormHole>& flit)
{
    os << "<FlitId, Data> <" << flit.flit_id << "," << flit.data << ">";
    return os;
}

#endif /*NVHLS_PACKET_H*/
