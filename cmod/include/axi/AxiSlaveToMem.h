/*
 * Copyright (c) 2016-2020, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef __AXISLAVETOMEM_H__
#define __AXISLAVETOMEM_H__

#include <systemc.h>
#include <ac_reset_signal_is.h>
#include <hls_globals.h>
#include <axi/axi4.h>
#include <mem_array.h>
#include <fifo.h>

/**
 * \brief An AXI slave SRAM.
 * \ingroup AXI
 *
 * \tparam axiCfg     A valid AXI config.
 * \tparam capacity   The capacity in bytes of the local SRAM.
 * \tparam fifoDepth  Depth of the input and output FIFO queues.
 *
 * \par Overview
 * AxiSlaveToMem is an AXI slave with an internal dual-ported memory used for storage.
 * The module only handles AXI addresses within the range of its internal memory, with a base address of 0.
 * It does not support write strobes.
 * It has internal queues to handle multiple simultaneous requests in flight, and can handle read and write requests independently, but it does not reorder requests.
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/AxiSlaveToMem/run/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <typename axiCfg, int capacity, int fifoDepth = 8>
class AxiSlaveToMem : public sc_module {
 private:
  static const int capacity_in_bytes = capacity;
  static const int banks = 1;

  typedef NVUINTW(axiCfg::dataWidth) Data;
  typedef mem_array_sep<Data, capacity_in_bytes, banks> Memarray;

  Memarray memarray;

 public:
  static const int kDebugLevel = 1;

  typedef typename axi::axi4<axiCfg> axi4_;
  static const int bytesPerWord = axiCfg::dataWidth >> 3;

  typename axi4_::read::template slave<> if_rd;
  typename axi4_::write::template slave<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  FIFO<typename axi4_::ReadPayload, fifoDepth> rd_resp;
  FIFO<typename axi4_::AddrPayload, fifoDepth> wr_addr;
  FIFO<typename axi4_::AddrPayload, fifoDepth> rd_addr;
  FIFO<typename axi4_::WRespPayload, fifoDepth> wr_resp;

  SC_CTOR(AxiSlaveToMem)
      : if_rd("if_rd"), if_wr("if_wr"), reset_bar("reset_bar"), clk("clk") {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  void run() {
    if_rd.reset();
    if_wr.reset();

    rd_resp.reset();
    rd_addr.reset();
    wr_resp.reset();
    wr_addr.reset();

    sc_uint<axi4_::ALEN_WIDTH> rd_beat_cnt = 0;
    sc_uint<axi4_::ALEN_WIDTH> wr_beat_cnt = 0;

    #pragma hls_pipeline_init_interval 2
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      bool rd_resp_full = rd_resp.isFull();

      if (!rd_resp.isEmpty()) {
        typename axi4_::ReadPayload data_pld;
        data_pld = rd_resp.peek();
        if (if_rd.nb_rwrite(data_pld)) {
          rd_resp.incrHead();
        }
      }

      if (!rd_resp_full && !rd_addr.isEmpty()) {
        typename axi4_::AddrPayload rd_addr_pld;
        rd_addr_pld = rd_addr.peek();

        CDCOUT(sc_time_stamp() << " " << name() << " Received read request: "
                      << rd_addr_pld
                      << endl, kDebugLevel);

        typename axi4_::ReadPayload data_pld;
        data_pld.data = memarray.read(rd_addr_pld.addr + bytesPerWord*rd_beat_cnt);
        data_pld.resp = axi4_::Enc::XRESP::OKAY;
        data_pld.id = rd_addr_pld.id;

        auto rd_beat_cnt_local = rd_beat_cnt;

        if (rd_beat_cnt_local == rd_addr_pld.len) {
          rd_beat_cnt_local = 0;
          rd_addr.incrHead();
          data_pld.last = 1;
        } else {
          data_pld.last = 0;
          ++rd_beat_cnt_local;
        }

        rd_beat_cnt = rd_beat_cnt_local;
        rd_resp.push(data_pld);
      }

      if (!rd_addr.isFull()) {
        typename axi4_::AddrPayload rd_addr_pld;
        if (if_rd.nb_aread(rd_addr_pld)) {
          rd_addr.push(rd_addr_pld);
        }
      }

      bool wr_resp_full = wr_resp.isFull();
      bool wr_addr_full = wr_addr.isFull();

      if (!wr_resp.isEmpty()) {
        typename axi4_::WRespPayload resp_pld;
        resp_pld = wr_resp.peek();
        if (if_wr.nb_bwrite(resp_pld)) {
          wr_resp.incrHead();
        }
      }

      if (!wr_resp_full && !wr_addr.isEmpty()) {
        typename axi4_::AddrPayload wr_addr_pld;
        wr_addr_pld = wr_addr.peek();

        typename axi4_::WritePayload write_pld;

        if (if_wr.w.PopNB(write_pld)) {
          auto wr_beat_cnt_local = wr_beat_cnt;
          memarray.write(static_cast<typename Memarray::LocalIndex>(static_cast<sc_uint<axi4_::ADDR_WIDTH> >
                        (wr_addr_pld.addr) + bytesPerWord*wr_beat_cnt_local), 0, write_pld.data);
          CDCOUT(sc_time_stamp() << " " << name() << " Received write request:"
                        << " addr=[" << wr_addr_pld << "]"
                        << " data=[" << write_pld << "]"
                        << " beat=" << dec << wr_beat_cnt_local
                        << endl, kDebugLevel);

          if (wr_beat_cnt_local == wr_addr_pld.len) {
            wr_beat_cnt_local = 0;
            NVHLS_ASSERT_MSG(write_pld.last == true, "WRLEN_indicates_that_this_should_be_the_last_beat,_but_WRLAST_is_not_set");
            wr_addr.incrHead();

            // push resp
            typename axi4_::WRespPayload resp_pld;
            resp_pld.resp = axi4_::Enc::XRESP::OKAY;
            resp_pld.id = wr_addr_pld.id;
            wr_resp.push(resp_pld);
          } else {
            NVHLS_ASSERT_MSG(write_pld.last == false, "WRLEN_indicates_that_this_should_not_be_the_last_beat,_but_WRLAST_is_set");
            ++wr_beat_cnt_local;
          }

          wr_beat_cnt = wr_beat_cnt_local;
        }
      }

      if (!wr_addr_full) {
        typename axi4_::AddrPayload wr_addr_pld;
        if (if_wr.aw.PopNB(wr_addr_pld)) {
          wr_addr.push(wr_addr_pld);
        }
      }
    }
  }
};

#endif
