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

#include <systemc>
#include <nvhls_connections.h>
#include <nvhls_assert.h>
#include <nvhls_message.h>
#include <nvhls_module.h>
#include <axi/axi4_encoding.h>
#include <axi/axi4_configs.h>

///////////////////
// This section should be moved into marshaller

/**
 * \brief The EmptyField class is used for rudimentary support for members of a struct that can be configured to have zero width.
 */
struct EmptyField : public nvhls_message {
  template <typename T>
  EmptyField operator=(T const &) {
    NVHLS_ASSERT_MSG(true,"EmptyField_should_never_be_assigned_or_accessed");   // If an assignment actually occurs during runtime
                                                                                // you've done something wrong
    return EmptyField();
  }
  uint64 to_uint64() { return 0; }
  const sc_bit operator[](std::size_t idx) const { return static_cast<sc_bit>(0); }
};

/* Operator << for EmptyField. */
inline std::ostream &operator<<(ostream &os, const EmptyField &empty) {
  return os << "EMPTYFIELD";
}

/* Operator & for Marshaller and EmptyField. */
template <unsigned int Size>
Marshaller<Size> &operator&(Marshaller<Size> &m, EmptyField &rhs) {
  return m;  // just do nothing
}

/**
 * \brief A class to determine whether to instantiate an sc_bv or an EmptyField.
 */
template <bool, int W>
class BitFieldwCheck;

/**
 * \brief Template specialization to instantiate an sc_bv if the width of the BitField is greater than 0.
 */
template <int W>
class BitFieldwCheck<true, W> {
 public:
  typedef sc_bv<W> T;
};

/**
 * \brief Template specialization to instantiate an EmptyField if the width of the BitField is 0.
 */
template <int W>
class BitFieldwCheck<false, W> {
 public:
  typedef EmptyField T;
};

/**
 * \brief The BitField class is used to define a bitvector that can have a bitwidth of 0.
 *
 * \tparam axiCfg     The width of the bitfield (can be 0).
 *
 * \par Overview
 * BitField allows fields to be elided entirely from a struct in the case that they are not used.
 * If W=0, an EmptyField is instantiated instead of an sc_bv.
 * EmptyField is ignored by the Marshaller and overloads some common operators so that
 * code using it does not need to be special-cased for zero-width parameterizations.
 */
template <int W>
class BitField {
 public:
  typedef typename BitFieldwCheck<(W > 0), W>::T T;
};

//////////////////

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
 * - Each AXI signal is defined as a BitField of an appropriate width, allowing
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

  typedef sc_bv<ADDR_WIDTH> Addr;
  typedef sc_bv<DATA_WIDTH> Data;
  typedef typename BitField<ID_WIDTH>::T Id;
  typedef typename BitField<BID_WIDTH>::T BId;
  typedef typename BitField<ALEN_WIDTH>::T BeatNum;
  typedef typename BitField<ASIZE_WIDTH>::T BeatSize;
  typedef typename BitField<LAST_WIDTH>::T Last;
  typedef typename BitField<WSTRB_WIDTH>::T Wstrb;
  typedef typename BitField<CACHE_WIDTH>::T Cache;
  typedef typename BitField<BURST_WIDTH>::T Burst;
  typedef sc_bv<RESP_WIDTH> Resp;

  typedef typename BitField<AUSER_WIDTH>::T AUser;
  typedef typename BitField<WUSER_WIDTH>::T WUser;
  typedef typename BitField<BUSER_WIDTH>::T BUser;
  typedef typename BitField<RUSER_WIDTH>::T RUser;

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

    static const unsigned int width = ADDR_WIDTH + ID_WIDTH + ALEN_WIDTH +
                                      ASIZE_WIDTH + BURST_WIDTH + CACHE_WIDTH +
                                      AUSER_WIDTH;

    template <unsigned int Size>
    void Marshall(Marshaller<Size> &m) {
      m &id;
      m &addr;
      m &len;
      m &size;
      m &burst;
      m &cache;
      m &auser;
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

    static const unsigned int width =
        DATA_WIDTH + RESP_WIDTH + ID_WIDTH + LAST_WIDTH + RUSER_WIDTH;

    template <unsigned int Size>
    void Marshall(Marshaller<Size> &m) {
      m &id;
      m &data;
      m &resp;
      m &last;
      m &ruser;
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

    static const unsigned int width = RESP_WIDTH + BID_WIDTH + BUSER_WIDTH;

    template <unsigned int Size>
    void Marshall(Marshaller<Size> &m) {
      m &id;
      m &resp;
      m &buser;
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

    static const unsigned int width =
        DATA_WIDTH + LAST_WIDTH + WSTRB_WIDTH + WUSER_WIDTH;

    template <unsigned int Size>
    void Marshall(Marshaller<Size> &m) {
      m &data;
      m &last;
      m &wstrb;
      m &wuser;
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
    class chan {
     public:
      typedef Connections::Combinational<AddrPayload> ARChan;
      typedef Connections::Combinational<ReadPayload> RChan;

      ARChan ar;  // master to slave
      RChan r;    // slave to master

      chan(const char *name)
          : ar(nvhls_concat(name, "_ar")), r(nvhls_concat(name, "_r")){};

      // TODO: Implement AXI protocol checker
    }; // read::chan

    /**
     * \brief The AXI read master port.  This port has an AR request channel as output and an R response channel as input.
     */
    class master {
     public:
      typedef Connections::Out<AddrPayload> ARPort;
      typedef Connections::In<ReadPayload> RPort;

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
    class slave {
     public:
      typedef Connections::In<AddrPayload> ARPort;
      typedef Connections::Out<ReadPayload> RPort;

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
    class chan {
     public:
      typedef Connections::Combinational<AddrPayload> AWChan;
      typedef Connections::Combinational<WritePayload> WChan;
      typedef Connections::Combinational<WRespPayload> BChan;

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
    class master {
     public:
      typedef Connections::Out<AddrPayload> AWPort;
      typedef Connections::Out<WritePayload> WPort;
      typedef Connections::In<WRespPayload> BPort;

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
    class slave {
     public:
      typedef Connections::In<AddrPayload> AWPort;
      typedef Connections::In<WritePayload> WPort;
      typedef Connections::Out<WRespPayload> BPort;

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
