/*
 * Copyright (c) 2017-2019, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef _AXI_AXI4_H_
#define _AXI_AXI4_H_

#ifdef __SYNTHESIS__
// workaround until connections/connections_utils.h and nvhls_connections_utils.h are cleaned up
#include <connections/connections_utils.h>
#define __CONNECTIONS__CONNECTIONS_UTILS_H_
#define NVHLS_CONNECTIONS_UTILS_H_
#endif

#include "auto_gen_fields.h"

#include <systemc>
#include <nvhls_connections.h>
#include <nvhls_assert.h>
#include <nvhls_message.h>
#include <nvhls_module.h>

#include <UIntOrEmpty.h>

#include <axi/axi4_encoding.h>
#include <axi/axi4_configs.h>


/**
 * \brief The axi namespace contains classes and definitions related to the AXI standard.
 * \ingroup AXI
 */
namespace axi {

/**
 * \brief The base axi4 class parameterized according a valid config.
 * \ingroup AXI
 *
 * \tparam Cfg        A valid AXI config.
 *
 * \par Overview
 * axi4 defines the AXI base class.  Based on the provided Cfg, the bitwidths of
 * the various fields of the AXI specification are defined, and classes and
 * convenience functions can be used to instantiate AXI Connections, wire them
 * together, and use them to implement the AXI protocol.
 * - Each AXI signal is defined as a UIntOrEmpty of an appropriate width, allowing
 * for the presence of 0-width fields when they can be elided entirely.
 * - If useWriteResponses = 0, the B channel is not removed entirely (for
 * implementation convenience), but is reduced to minimum width.
 * - All AW and AR fields are identical, and are combined into a common
 * AddrPayload type.
 *
 */

template <typename Cfg>
class axi4 {
 public:
  typedef AXI4_Encoding Enc;

  enum {
    DATA_WIDTH = Cfg::dataWidth,
    ADDR_WIDTH = Cfg::addrWidth,
    ID_WIDTH = Cfg::idWidth,
    BID_WIDTH = (Cfg::useWriteResponses == 0 ? 0 : Cfg::idWidth),
    ALEN_WIDTH =
        (Cfg::useBurst != 0 ? nvhls::log2_ceil<Cfg::maxBurstSize>::val : 0),
    ASIZE_WIDTH = (Cfg::useVariableBeatSize != 0 ? 3 : 0),
    LAST_WIDTH = (Cfg::useLast != 0 ? 1 : 0),
    CACHE_WIDTH = (Cfg::useCache != 0 ? Enc::ARCACHE::_WIDTH : 0),
    BURST_WIDTH = ((Cfg::useBurst != 0 &&
                    (Cfg::useFixedBurst != 0 || Cfg::useWrapBurst != 0))
                       ? Enc::AXBURST::_WIDTH
                       : 0),
    WSTRB_WIDTH = (Cfg::useWriteStrobes != 0 ? (DATA_WIDTH >> 3) : 0),
    RESP_WIDTH = Enc::XRESP::_WIDTH,
    // TODO - The B channel ought to disappear entirely if useWriteResponses is
    // 0, but that will require substantial refactoring.  For now we leave
    // RESP_WIDTH so there is a data stub in the ready-valid interface.

    AUSER_WIDTH = Cfg::aUserWidth,
    WUSER_WIDTH = Cfg::wUserWidth,
    BUSER_WIDTH = (Cfg::useWriteResponses == 0 ? 0 : Cfg::bUserWidth),
    RUSER_WIDTH = Cfg::rUserWidth,
  };

  typedef NVUINTW(ADDR_WIDTH) Addr;
  typedef NVUINTW(DATA_WIDTH) Data;
  typedef typename nvhls::UIntOrEmpty<ID_WIDTH>::T Id;
  typedef typename nvhls::UIntOrEmpty<BID_WIDTH>::T BId;
  typedef typename nvhls::UIntOrEmpty<ALEN_WIDTH>::T BeatNum;
  typedef typename nvhls::UIntOrEmpty<ASIZE_WIDTH>::T BeatSize;
  typedef typename nvhls::UIntOrEmpty<LAST_WIDTH>::T Last;
  typedef typename nvhls::UIntOrEmpty<WSTRB_WIDTH>::T Wstrb;
  typedef typename nvhls::UIntOrEmpty<CACHE_WIDTH>::T Cache;
  typedef typename nvhls::UIntOrEmpty<BURST_WIDTH>::T Burst;
  typedef NVUINTW(RESP_WIDTH) Resp;

  typedef typename nvhls::UIntOrEmpty<AUSER_WIDTH>::T AUser;
  typedef typename nvhls::UIntOrEmpty<WUSER_WIDTH>::T WUser;
  typedef typename nvhls::UIntOrEmpty<BUSER_WIDTH>::T BUser;
  typedef typename nvhls::UIntOrEmpty<RUSER_WIDTH>::T RUser;

  /**
   * \brief A struct composed of the signals associated with AXI read and write requests.
   */
  struct AddrPayload : public nvhls_message {
    Id id;
    Addr addr;
    Burst burst;
    BeatNum len;    // A*LEN
    BeatSize size;  // A*SIZE
    Cache cache;
    AUser auser;

  AUTO_GEN_FIELD_METHODS(AddrPayload, ( \
     id \
   , addr \
   , burst \
   , len \
   , size \
   , cache \
   , auser \
  ) )
  //


   AddrPayload() {
     if(ID_WIDTH > 0)
       id = 0;
     addr = 0; // NVUINT, ADDR_WIDTH always > 0
     if(ALEN_WIDTH > 0)
       len = 0;
     if(ASIZE_WIDTH > 0)
       size = 0;
     if(BURST_WIDTH > 0)
       burst = 0;
     if(CACHE_WIDTH > 0)
       cache = 0;
     if(AUSER_WIDTH > 0)
       auser = 0;
    }
    
  };

  /**
   * \brief A struct composed of the signals associated with an AXI read
   * response.
   */
  struct ReadPayload : public nvhls_message {
    Id id;
    Data data;
    Resp resp;
    Last last;
    RUser ruser;

  AUTO_GEN_FIELD_METHODS(ReadPayload, ( \
     id \
   , data \
   , resp  \
   , last \
   , ruser \
  ) )
  //


   ReadPayload() {
     if(ID_WIDTH > 0)
       id = 0;
     data = 0; // NVUINT, DATA_WIDTH always > 0 
     resp = 0; // NVUINT, RESP_WIDTH always > 0
     if(LAST_WIDTH > 0)
       last = 0;
     if(RUSER_WIDTH > 0)
       ruser = 0;
   }
  };

  /**
   * \brief A struct composed of the signals associated with an AXI write
   * response.
   */
  struct WRespPayload : public nvhls_message {
    BId id;
    Resp resp;
    BUser buser;

  AUTO_GEN_FIELD_METHODS(WRespPayload, ( \
     id \
   , resp  \
   , buser \
  ) )
  //

   WRespPayload() {
     if(ID_WIDTH > 0)
       id = 0;
     resp = 0; // NVUINT, RESP_WIDTH always > 0
     if(BUSER_WIDTH > 0)
       buser = 0;
    }

  };

  /**
   * \brief A struct composed of the signals associated with AXI write data.
   */
  struct WritePayload : public nvhls_message {
    // no id here!
    Data data;
    Last last;
    Wstrb wstrb;
    WUser wuser;

  AUTO_GEN_FIELD_METHODS(WritePayload, ( \
     data \
   , last  \
   , wstrb \
   , wuser \
  ) )
  //

    WritePayload() {
     data = 0; // NVUINT, DATA_WIDTH always > 0 
     if(LAST_WIDTH > 0)
       last = 0;
     if(WSTRB_WIDTH > 0)
        wstrb = ~0;
     if(WUSER_WIDTH > 0)
       wuser = 0;
    }
  };

  /**
   * \brief The AXI read class.
   *
   * Each Connections implementation contains two ready-valid interfaces, AR for
   * read requests and R for read responses.
   */
  class read {
   public:
    /**
     * \brief The AXI read channel, used for connecting an AXI master and AXI slave.
     */
    template <Connections::connections_port_t PortType = AUTO_PORT>
    class chan {
     public:
     typedef Connections::Combinational<AddrPayload, PortType> ARChan;
     typedef Connections::Combinational<ReadPayload, PortType> RChan;

      ARChan ar;  // master to slave
      RChan r;    // slave to master

      chan(const char *name)
          : ar(nvhls_concat(name, "_ar")), r(nvhls_concat(name, "_r")){};

      // TODO: Implement AXI protocol checker
    }; // read::chan

    /**
     * \brief The AXI read master port.  This port has an AR request channel as output and an R response channel as input.
     */
    template <Connections::connections_port_t PortType = AUTO_PORT>
    class master {
     public:
      typedef Connections::Out<AddrPayload, PortType> ARPort;
      typedef Connections::In<ReadPayload, PortType> RPort;

      ARPort ar;
      RPort r;

      master(const char *name)
          : ar(nvhls_concat(name, "_ar")), r(nvhls_concat(name, "_r")) {}

      void reset() {
        ar.Reset();
        r.Reset();
      }

      ReadPayload query(const AddrPayload &addr) {
        // TODO: add nb version with state
        ar.Push(addr);
        return r.Pop();
      }

      template <class C>
      void operator()(C &c) {
        ar(c.ar);
        r(c.r);
      }
    }; // read::master

    /**
     * \brief The AXI read slave port.  This port has an AR request channel as input and an R response channel as output.
     */
    template <Connections::connections_port_t PortType = AUTO_PORT>
    class slave {
     public:
      typedef Connections::In<AddrPayload, PortType> ARPort;
      typedef Connections::Out<ReadPayload, PortType> RPort;

      ARPort ar;
      RPort r;

      slave(const char *name)
          : ar(nvhls_concat(name, "_ar")), r(nvhls_concat(name, "_r")) {}

      void reset() {
        ar.Reset();
        r.Reset();
      }

      AddrPayload aread() { return ar.Pop(); }

      bool nb_aread(AddrPayload &addr) { return ar.PopNB(addr); }

      void rwrite(const ReadPayload &data) { r.Push(data); }

      bool nb_rwrite(const ReadPayload &data) { return r.PushNB(data); }

      template <class C>
      void operator()(C &c) {
        ar(c.ar);
        r(c.r);
      }
    }; // read::slave
  }; // read

  /**
   * \brief The AXI write class.
   *
   * Each Connections implementation contains three ready-valid interfaces: AW
   * for write requests, W for write data, and B for write responses.
   */
  class write {
   public:
    /**
     * \brief The AXI write channel, used for connecting an AXI master and AXI slave.
     */
    template <Connections::connections_port_t PortType = AUTO_PORT>
    class chan {
     public:
     typedef Connections::Combinational<AddrPayload, PortType> AWChan;
     typedef Connections::Combinational<WritePayload, PortType> WChan;
     typedef Connections::Combinational<WRespPayload, PortType> BChan;

      AWChan aw;  // master to slave
      WChan w;    // master to slave
      BChan b;    // slave to master

      chan(const char *name)
          : aw(nvhls_concat(name, "_aw")),
            w(nvhls_concat(name, "_w")),
            b(nvhls_concat(name, "_b")){};

      // TODO: Implement AXI protocol checker
    };  // write::chan

    /**
     * \brief The AXI write master port.  This port has AW and W request channels as outputs and a B response channel as input.
     */
    template <Connections::connections_port_t PortType = AUTO_PORT>
    class master {
     public:
      typedef Connections::Out<AddrPayload, PortType> AWPort;
      typedef Connections::Out<WritePayload, PortType> WPort;
      typedef Connections::In<WRespPayload, PortType> BPort;

      AWPort aw;
      WPort w;
      BPort b;

      master(const char *name)
          : aw(nvhls_concat(name, "_aw")),
            w(nvhls_concat(name, "_w")),
            b(nvhls_concat(name, "_b")) {}

      void reset() {
        aw.Reset();
        w.Reset();
        b.Reset();
      }

      WRespPayload write(const AddrPayload &addr, const WritePayload &data) {
        // TODO: add nb version with state
        aw.Push(addr);
        w.Push(data);
        return b.Pop();
      }

      template <class C>
      void operator()(C &c) {
        aw(c.aw);
        w(c.w);
        b(c.b);
      }
    };  // write::master

    /**
     * \brief The AXI write slave port.  This port has AW and W request channels as inputs and a B response channel as output.
     */
    template <Connections::connections_port_t PortType = AUTO_PORT>
    class slave {
     public:
      typedef Connections::In<AddrPayload, PortType> AWPort;
      typedef Connections::In<WritePayload, PortType> WPort;
      typedef Connections::Out<WRespPayload, PortType> BPort;

      AWPort aw;
      WPort w;
      BPort b;

      bool got_waddr;
      AddrPayload stored_waddr;

      slave(const char *name)
          : aw(nvhls_concat(name, "_aw")),
            w(nvhls_concat(name, "_w")),
            b(nvhls_concat(name, "_b")),
            got_waddr(false) {}

      void reset() {
        aw.Reset();
        w.Reset();
        b.Reset();
      }

      void wread(AddrPayload &addr, WritePayload &data) {
        addr = aw.Pop();
        data = w.Pop();
      }

      bool nb_wread(AddrPayload &addr, WritePayload &data) {
        if (!got_waddr) {
          if (!aw.PopNB(addr)) {
            return false;
          } else {
            got_waddr = true;
            stored_waddr = addr;
          }
        } else {
          addr = stored_waddr;
        }

        if (w.PopNB(data)) {
          got_waddr = false;
          return true;
        } else {
          return false;
        }
      }

      void bwrite(const WRespPayload &resp) { b.Push(resp); }

      bool nb_bwrite(const WRespPayload &resp) { return b.PushNB(resp); }

      template <class C>
      void operator()(C &c) {
        aw(c.aw);
        w(c.w);
        b(c.b);
      }
    }; // write::slave
  }; // write
}; // axi4
}; // axi

#endif

