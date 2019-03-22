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

#ifndef __AXIMASTERGATEIF_H__
#define __AXIMASTERGATEIF_H__

#include <nvhls_message.h>
#include <axi/axi4.h>

/**
 * \brief The base type for read or write requests for AxiMasterGate, containing common fields.
 *
 * \tparam Cfg    A valid AXI config.
 */
template <typename Cfg>
struct Request : public nvhls_message {
  public:
  Request() {};
  Request(const Request &rhs) {
    addr = rhs.addr;
    burst = rhs.burst;
    len = rhs.len;
    size = rhs.size;
    cache = rhs.cache;
    auser = rhs.auser;
  };
  
  typedef axi::axi4<Cfg> axi4_;

  typename axi4_::Addr addr;
  typename axi4_::BeatNum len;
  typename axi4_::BeatSize size;
  typename axi4_::Burst burst;
  typename axi4_::Cache cache;
  typename axi4_::AUser auser;

  static const unsigned int width = axi4_::ADDR_WIDTH + axi4_::ALEN_WIDTH +
                                    axi4_::ASIZE_WIDTH + axi4_::BURST_WIDTH +
                                    axi4_::CACHE_WIDTH + axi4_::AUSER_WIDTH;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& addr;
    m& len;
    m& size;
    m& burst;
    m& cache;
    m& auser;
  }

  void copyToAddrPayload(typename axi4_::AddrPayload& payload) const {
    payload.addr = addr;
    payload.burst = burst;
    payload.len = len;
    payload.size = size;
    payload.cache = cache;
    payload.auser = auser;
  };
};

/**
 * \brief The struct for read requests for AxiMasterGate.
 *
 * \tparam Cfg    A valid AXI config.
 */
template <typename Cfg>
struct RdRequest : public Request<Cfg> {};

/**
 * \brief The struct for write requests for AxiMasterGate.
 *
 * \tparam Cfg    A valid AXI config.
 */
template <typename Cfg>
struct WrRequest : public Request<Cfg> {
 public:
  WrRequest() {};
  WrRequest(const WrRequest &rhs) : Request<Cfg>(rhs) {
    data = rhs.data;
    last = rhs.last;
    wuser = rhs.wuser;
  };
  typename Request<Cfg>::axi4_::Data data;
  typename Request<Cfg>::axi4_::WUser wuser;
  typename Request<Cfg>::axi4_::Last last;

  static const unsigned int width =
      Request<Cfg>::width + Request<Cfg>::axi4_::DATA_WIDTH +
      Request<Cfg>::axi4_::LAST_WIDTH + Request<Cfg>::axi4_::WUSER_WIDTH;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& Request<Cfg>::addr;
    m& data;
    m& Request<Cfg>::len;
    m& Request<Cfg>::size;
    m& Request<Cfg>::burst;
    m& Request<Cfg>::cache;
    m& Request<Cfg>::auser;
    m& last;
    m& wuser;
  }
};

/** 
 * \brief The struct for write responses for AxiMasterGate.
 *
 * \tparam Cfg    A valid AXI config.
 */
template <typename Cfg>
struct WrResp : public nvhls_message
{
  typedef axi::axi4<Cfg> axi4_;
  typename axi4_::Resp resp;
  typename axi4_::BUser buser;

  static const unsigned int width = axi4_::RESP_WIDTH + axi4_::BUSER_WIDTH;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& resp;
    m& buser;
  }
};

/** 
 * \brief The struct for read responses for AxiMasterGate.
 *
 * \tparam Cfg    A valid AXI config.
 */
template <typename Cfg>
struct RdResp : public nvhls_message
{
  typedef axi::axi4<Cfg> axi4_;
  typename axi4_::Resp resp;
  typename axi4_::Data data;
  typename axi4_::Last last;
  typename axi4_::RUser ruser;

  static const unsigned int width = axi4_::RESP_WIDTH + axi4_::DATA_WIDTH +
                                    axi4_::LAST_WIDTH + axi4_::RUSER_WIDTH;

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& resp;
    m& data;
    m& last;
    m& ruser;
  }
};

#endif
