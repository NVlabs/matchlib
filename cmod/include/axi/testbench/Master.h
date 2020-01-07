/*
 * Copyright (c) 2017-2020, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef __AXI_T_MASTER__
#define __AXI_T_MASTER__

#include <systemc.h>
#include <ac_reset_signal_is.h>

#include <axi/axi4.h>
#include <nvhls_connections.h>
#include <hls_globals.h>

#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>
#include <boost/assert.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <algorithm>

/**
 * \brief An example config for the AXI master.
 * 
 * \par The following constants must be defined:
 *
 * - numWrites: The number of writes to perform.
 * - numReads: The number of reads to perform.
 * - readDelay: The number of cycles to wait after a write to an address before permitting reads to that address.  (If write responses are supported, this can safely be set to 0.)
 * - addrBoundLower: The lower bound of generated addresses.
 * - addrBoundUpper: The upper bound of generated addresses.
 * - seed: The random seed.
 */
struct masterCfg {
  enum {
    numWrites = 100,
    numReads = 100,
    readDelay = 0,
    seed = 0,
  };
  // enum only supports 32-bit types, so use static 64-bit types as needed (e.g., for >32bit addresses)
  static const uint64_t addrBoundLower = 0;
  static const uint64_t addrBoundUpper = 0xFFFFFFFF;
};

/**
 * \brief An AXI master that generates random traffic for use in a testbench.
 * \ingroup AXI
 *
 * \tparam axiCfg                   A valid AXI config.
 * \tparam cfg                      A valid config for the master (such as masterCfg).
 *
 * \par Overview
 * Master is an AXI master block for use in testbenches.  It generates read and write requests and checks the responses.  The block supports AXI configurations with and without burst mode, write strobes, and write responses.  The block enforces the following via assertion:
 * - Master may not issue a read to an address it has not yet written to.
 * - Read response data must match the most recent write data to the same address.
 * - Master may not issue writes with no strobe bits asserted to an address and then read from that address.
 * - Responses should assert the AXI "OKAY" response code.
 * - The number of read (write) responses should not exceed the number of read (write) requests.
 *
 * In addition, the block asserts a done signal only when each request has received a corresponding response.  The Master generates random requests according to the following algorithm:
 * - Writes are generated to an address randomly selected from the valid range.  20% of writes have burst length greater than one; these writes randomly select a burst length from the valid range.  20% of write data beats have nonuniform strobes; the strobe bits of these write data beats are randomly set.  Some data bits are generated via a hash of the address, while others are randomly generated.  The data associated with a given address is stored locally so reads to that address can be validated later.
 * - Every AXI address that is written to (including multiple addresses for bursts) is added to the list of valid read addresses, which is initially empty.  If write responses are enabled, the addresses are added only when the write response is received; otherwise, they are added when the write is sent.
 * - If the readDelay is greater than zero, a delay elapses before addresses that have been written to are added to the list of valid read addresses.  If a write is issued to an address that has been previously written to, the address is removed from the list of valid read addresses and the readDelay timer for that address is reset.
 * - Writes that are issued to an address that has an outstanding read request in flight are discarded and not issued.  Reads that are issued to an address that has an outstanding write request in flight are not issued (unless write requests are not supported, in which case only a non-zero readDelay prevents this race).
 * - Reads are randomly selected from the list of valid read addresses, so that reads only issue to address that have already been written to.  This means that no reads issue until the first write response has returned.
 * - 50% of reads requests are initiated as bursts, with the burst length randomly selected from the valid range.  If a read burst would include one or more addresses as part of the burst that have not yet been written to, the read request is discarded and not issued.
 *
 */
template <typename axiCfg, typename cfg>
class Master : public sc_module {
  BOOST_STATIC_ASSERT_MSG(axiCfg::useWriteResponses || cfg::numReads == 0 || cfg::readDelay != 0,
                "Must use a substantial read delay if reading without write responses");
 public:
  static const int kDebugLevel = 0;
  typedef axi::axi4<axiCfg> axi4_;

  typename axi4_::read::template master<> if_rd;
  typename axi4_::write::template master<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  std::map<typename axi4_::Addr, typename axi4_::Data> localMem;
  std::map<typename axi4_::Addr, NVUINT8 > localMem_wstrb;
  std::vector<typename axi4_::Addr> validReadAddresses;
  std::vector<typename axi4_::Addr> validReadAddresses_q;
  std::vector< long long > validReadAddresses_ctr;

  static const int bytesPerBeat = axi4_::DATA_WIDTH >> 3;
  static const bool wResp = axiCfg::useWriteResponses;

  sc_out<bool> done;

  SC_CTOR(Master)
      : if_rd("if_rd"), if_wr("if_wr"), reset_bar("reset_bar"), clk("clk") {

    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  void run() {
    static const int bytesPerWord = axi4_::DATA_WIDTH >> 3;
    static const int axiAddrBitsPerWord = nvhls::log2_ceil<bytesPerWord>::val;
    // Workaround for useBurst=0, which sets ALEN field to 0 width
    static const int ALEN_W = axiCfg::useBurst != 0 ? axi4_::ALEN_WIDTH : 1; 
    static const int WSTRB_W = axiCfg::useWriteStrobes != 0 ? axi4_::WSTRB_WIDTH : 1; 
    std::queue <typename axi4_::Addr> raddr_queue;
    std::queue < NVUINTW(ALEN_W) > rlen_queue;
    std::queue <typename axi4_::Addr> waddr_queue;
    std::queue < NVUINTW(ALEN_W) > wlen_queue;
    typename axi4_::Data check_data;
    NVUINT8 check_data_wstrb;
    NVUINT8 rcv_data_wstrb;
    typename axi4_::AddrPayload addr_pld;
    typename axi4_::ReadPayload data_pld;

    // Follow the same priority as nvhls_rand: environment, then preprocessor define, then config
    unsigned int seed = cfg::seed;
#ifdef RAND_SEED
    seed = (RAND_SEED);
#endif
    const char* env_rand_seed = std::getenv("RAND_SEED");
    if (env_rand_seed != NULL) seed = atoi(env_rand_seed);
    boost::random::mt19937 gen(seed);
    boost::random::uniform_int_distribution<uint64_t> random_addr(cfg::addrBoundLower, cfg::addrBoundUpper);
    boost::random::uniform_int_distribution<> random_wstrb(1, pow(2,WSTRB_W)-1);
    boost::random::uniform_int_distribution<> random_burstlen(0, axiCfg::maxBurstSize-1);
    boost::random::uniform_int_distribution<> uniform_rand;

    typename axi4_::Addr wr_addr = cfg::addrBoundLower;
    typename axi4_::Data wr_data = 0xf00dcafe12345678;
    NVUINTW(WSTRB_W) wstrb = ~0;
    NVUINTW(ALEN_W) wr_len = 0;
    typename axi4_::Addr rd_addr_next;
    typename axi4_::Addr rd_addr;
    NVUINTW(ALEN_W) rd_len;
    typename axi4_::AddrPayload wr_addr_pld;
    typename axi4_::WritePayload wr_data_pld;
    typename axi4_::WRespPayload wr_resp_pld;

    done = 0;
    unsigned int numWrites = 0;
    unsigned int numReads = 0;
    unsigned int numWritesOfBurst = 0;
    unsigned int numReadsOfBurst = 0;
    unsigned int numWriteResponses = 0;
    unsigned int numReadResponses = 0;
    bool writeInProgress = false;
    bool rd_conflict = false;
    bool wr_conflict = false;

    if_rd.reset();
    if_wr.reset();

    wait(20);
    while (1) {
      wait();

      // READ
      if (validReadAddresses.size() > 0 && numReads < cfg::numReads) {
        rd_addr_next = validReadAddresses[uniform_rand(gen) % validReadAddresses.size()];
        addr_pld.addr = rd_addr_next;
        if (axiCfg::useBurst) {
          if (uniform_rand(gen) % 2 == 0) { // 50% of reads are bursts (less once valid addresses are disallowed)
            rd_len = random_burstlen(gen);
          } else {
            rd_len = 0;
          }
          addr_pld.len = rd_len;
        }
        wr_conflict = false;
        for (unsigned int i=0; i<(rd_len+1); i++) {
          if (!localMem.count(rd_addr_next+bytesPerBeat*i)) {
            wr_conflict = true; // Not actually a conflict, but the read should be cancelled nonetheless
          }
        }
        std::ostringstream ms3;
        ms3 << "\nError @" << sc_time_stamp() << " from " << name()
            << ": Testharness attempted to read an address that it never wrote to"
            << ", read_addr=" << hex << rd_addr_next
            << endl;
        BOOST_ASSERT_MSG( localMem.count(rd_addr_next), ms3.str().c_str() );
        for (unsigned int j=0; j<waddr_queue.size(); j++) {
          if ((rd_addr_next+bytesPerBeat*rd_len) >= waddr_queue.front() && rd_addr_next <= waddr_queue.front())
                wr_conflict = true;
          waddr_queue.push(waddr_queue.front());
          waddr_queue.pop();
        }
        if (!wr_conflict) {
          rd_addr_next = addr_pld.addr;
          if (if_rd.ar.PushNB(addr_pld)) {
            CDCOUT(sc_time_stamp() << " " << name() << " Sent read request: ["
                          << addr_pld << "]"
                          << endl, kDebugLevel);
            numReads++;
            for (unsigned int i=0; i<(rd_len+1); i++) {
              raddr_queue.push(rd_addr_next);
              rd_addr_next += bytesPerBeat;
            }
            rlen_queue.push(rd_len);
          }
        }
      }
      if (if_rd.r.PopNB(data_pld)) {
        rd_addr = raddr_queue.front();
        if (axiCfg::useWriteStrobes) {
          bool checked_one = false;
          for (int i=0; i<axi4_::WSTRB_WIDTH; i++) {
            if (localMem_wstrb.count(rd_addr+i)) {
              rcv_data_wstrb = nvhls::get_slc<8>(data_pld.data,8*i);
              check_data_wstrb = localMem_wstrb[rd_addr+i];
              std::ostringstream msg;
              msg << "\nError @" << sc_time_stamp() << " from " << name()
                  << ": Incorrect read data response"
                  << ", base_addr=" << hex << rd_addr
                  << ", byte=" << i
                  << ", data=" << hex << rcv_data_wstrb
                  << ", expected=" << hex << check_data_wstrb
                  << std::endl;
              BOOST_ASSERT_MSG( check_data_wstrb == rcv_data_wstrb, msg.str().c_str() );
              checked_one = true;
            }
          } 
          std::ostringstream msg3;
          msg3 << "\nError @" << sc_time_stamp() << " from " << name()
              << ": Testbench did not check any bytes of a read response" << std::endl;
          BOOST_ASSERT_MSG( checked_one, msg3.str().c_str() );
        } else {
          check_data = localMem[rd_addr];
          std::ostringstream msg;
          msg << "\nError @" << sc_time_stamp() << " from " << name()
              << ": Incorrect read data response"
              << ", addr=" << hex << rd_addr
              << ", data=" << hex << data_pld.data.to_uint64()
              << ", expected=" << hex << check_data.to_uint64()
              << std::endl;
          BOOST_ASSERT_MSG( check_data == data_pld.data , msg.str().c_str() );
        }
        std::ostringstream ms2;
        ms2 << "\nError @" << sc_time_stamp() << " from " << name()
            << ": Read response protocol error"
            << ", rresp=" << data_pld.resp.to_uint64()
            << std::endl;
        BOOST_ASSERT_MSG( (data_pld.resp == axi4_::Enc::XRESP::OKAY) |
                          (data_pld.resp == axi4_::Enc::XRESP::EXOKAY), ms2.str().c_str() );
        CDCOUT(sc_time_stamp() << " " << name() << " Received correct read response: ["
                      << data_pld << "]"
                      << endl, kDebugLevel);
        raddr_queue.pop();
        if (numReadsOfBurst++ == rlen_queue.front()) {
          numReadsOfBurst = 0;
          numReadResponses++;
          rlen_queue.pop();
        }
      }

      // WRITE
      wr_addr_pld.addr = wr_addr;
      wr_data_pld.data = wr_data;
      wr_data_pld.wstrb = wstrb;
      wr_addr_pld.len = wr_len;
      if (!writeInProgress && numWrites < cfg::numWrites) {
        rd_conflict = false;
        for (unsigned int j=0; j<raddr_queue.size(); j++) {
          if ((wr_addr+bytesPerBeat*wr_len) >= raddr_queue.front() && wr_addr <= raddr_queue.front())
                rd_conflict = true;
          raddr_queue.push(raddr_queue.front());
          raddr_queue.pop();
        }
        if (!rd_conflict) {
          if (if_wr.aw.PushNB(wr_addr_pld)) {
            CDCOUT(sc_time_stamp() << " " << name() << " Sent write request: ["
                          << wr_addr_pld << "]"
                          << endl, kDebugLevel);
            for (unsigned int i=0; i<(wr_len+1); i++) {
              waddr_queue.push(wr_addr+bytesPerBeat*i);
              // If the address was already written to once, it needs to be removed from the list of valid addresses
              // because a read after this second write could return incorrect data
              validReadAddresses.erase(std::remove(validReadAddresses.begin(), validReadAddresses.end(), waddr_queue.front()), validReadAddresses.end());
              if (!wResp) {
                ptrdiff_t pos = find(validReadAddresses_q.begin(), validReadAddresses_q.end(), waddr_queue.front()) - validReadAddresses_q.begin();
                int size = validReadAddresses_q.size();
                // If the address is already in the queue, just reset the counter
                if (pos < size) {
                  validReadAddresses_ctr[pos] = cfg::readDelay + 20*i;
                } else {
                  validReadAddresses_q.push_back(waddr_queue.front());
                  validReadAddresses_ctr.push_back(cfg::readDelay + 20*i);
                }
                waddr_queue.pop();
              }
            }
            wlen_queue.push(wr_len);
            writeInProgress = true;
          }
        }
      }
      if (writeInProgress) {
        if (axiCfg::useBurst) {
          if (numWritesOfBurst == (wr_len)) {
            wr_data_pld.last = 1;
          } else {
            wr_data_pld.last = 0;
          }
        }
        if (if_wr.w.PushNB(wr_data_pld)) {
          CDCOUT(sc_time_stamp() << " " << name() << " Sent write data:"
                        << " addr=" << hex << wr_addr
                        << " data=[" << wr_data_pld << "]"
                        << " beat=" << dec << numWritesOfBurst
                        << endl, kDebugLevel);
          if (axiCfg::useWriteStrobes) {
            for (int i=0; i<axi4_::WSTRB_WIDTH; i++) {
              if (wr_data_pld.wstrb[i] == 1) {
                localMem_wstrb[wr_addr+i] = nvhls::get_slc<8>(wr_data_pld.data, 8*i);
              }
            }
          }
          localMem[wr_addr] = wr_data_pld.data; // Need to keep track of base addresses, even for wstrb case
          if (++numWritesOfBurst == (wr_len+1)) { // Whole burst is done
            wr_addr = (random_addr(gen) >> axiAddrBitsPerWord) << axiAddrBitsPerWord; // Keep all requests word-aligned
            if (axiCfg::useBurst) {
              if (uniform_rand(gen) % 5 == 0) { // 20% of writes are bursts
                wr_len = random_burstlen(gen);
                if (wr_addr + bytesPerBeat*wr_len > cfg::addrBoundUpper) wr_len = 0;
              } else {
                wr_len = 0;
              }
            }
            writeInProgress = false;
            numWrites++;
            numWritesOfBurst = 0;
          } else { // Only this beat is done
            wr_addr += bytesPerBeat;
          }
          wr_data = 0xf00dcafe12345678
                      ^ ((static_cast<typename axi4_::Data>(wr_addr)) << 16) // Typically touches bits 47:16
                      ^ uniform_rand(gen); // Touches the 32 LSBs
          wr_data.set_slc(axi4_::DATA_WIDTH-8,NVUINT8(uniform_rand(gen))); // Touches the 8 MSBs, in case of wide data words
          if (axiCfg::useWriteStrobes) {
            if (uniform_rand(gen) % 5 == 0) { // 20% of writes have nonuniform strobe
              wstrb = random_wstrb(gen);
            } else {
              wstrb = ~0;
            }
          }
        }
      }
      if (if_wr.b.PopNB(wr_resp_pld)) {
        std::ostringstream msg;
        msg << "\nError @" << sc_time_stamp() << " from " << name()
            << ":  Write response protocol error"
            << ", bresp=" << wr_resp_pld.resp.to_uint64()
            << ", addr=" << hex << waddr_queue.front()
            << std::endl;
        BOOST_ASSERT_MSG( (wr_resp_pld.resp == axi4_::Enc::XRESP::OKAY) |
                          (wr_resp_pld.resp == axi4_::Enc::XRESP::EXOKAY), msg.str().c_str() );
        CDCOUT(sc_time_stamp() << " " << name() << " Received write response"
                      << endl, kDebugLevel);
        numWriteResponses++;
        for (unsigned int i=0; i<wlen_queue.front()+1; i++) {
          validReadAddresses_q.push_back(waddr_queue.front());
          validReadAddresses_ctr.push_back(cfg::readDelay + 20*i);
          waddr_queue.pop();
        }
        wlen_queue.pop();
      }
      if (numWrites == cfg::numWrites &&
          numReads == cfg::numReads &&
          (numWriteResponses == numWrites || !wResp) &&
          numReadResponses == numReads)
            done = 1;
      //DCOUT(sc_time_stamp() << " from " << name() << " numWrites: " << dec << numWrites << "  numWriteResponses: " << numWriteResponses << "  numReads: " << numReads << "  numReadResponses: " << numReadResponses << endl);
      std::ostringstream msg;
      msg << "\nError @" << sc_time_stamp() << " from " << name()
          << ": Number of write responses exceeded total number of writes"
          << ", numWriteResponses=" << dec << numWriteResponses
          << std::endl;
      BOOST_ASSERT_MSG( numWriteResponses <= cfg::numWrites, msg.str().c_str() );
      std::ostringstream ms2;
      ms2 << "\nError @" << sc_time_stamp() << " from " << name()
          << ": Number of read responses exceeded total number of reads"
          << ", numReadResponses=" << dec << numReadResponses
          << std::endl;
      BOOST_ASSERT_MSG( numReadResponses <= cfg::numReads, msg.str().c_str() );

      // If there are no write responses we still want to test reads.
      // Finesse this by enforcing a fixed delay after a write until reads can be issued to that address.
      for (unsigned int i=0; i<validReadAddresses_q.size(); i++) {
        if (validReadAddresses_ctr[i] >= 0) {
          if (validReadAddresses_ctr[i] == 0) {
            validReadAddresses.push_back(validReadAddresses_q[i]);
          }
          validReadAddresses_ctr[i]--;
        }
      }
      //DCOUT("validReadAddresses: ");
      //for (typename std::vector< sc_uint<axi4_::ADDR_WIDTH> >::const_iterator q = validReadAddresses.begin(); q != validReadAddresses.end(); ++q)
      //      std::cout << *q << ' ';
      //DCOUT(endl);
      
    }
  }
};

#endif
