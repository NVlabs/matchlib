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

#ifndef __LITESLAVETOMEM_H__
#define __LITESLAVETOMEM_H__

#include <systemc.h>
#include <ac_reset_signal_is.h>
#include <ac_int.h>
#include <hls_globals.h>
#include <axi/axi4.h>
#include <mem_array.h>

/**
 * \brief An AXI slave SRAM for the AXI-Lite protocol.
 * \ingroup AXI
 *
 * \tparam capacity   The capacity in bytes of the local SRAM.
 *
 * \par Overview
 * AxiLiteSlaveToMem is an AXI slave compliant with the AXI-Lite protocol (32-bit data words, no bursts).
 * The module only handles AXI addresses within the range of its internal memory, with a base address of 0.
 * It does not support write strobes.
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/AxiLiteSlaveToMem/run/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <int capacity>
class AxiLiteSlaveToMem : public sc_module {
 private:
  typedef typename axi::axi4<axi::cfg::lite_nowstrb> axi_;

  static const int capacity_in_bytes = capacity;
  static const int banks = 1; // Not templated - only support 1 bank for now

  typedef typename axi_::Data Data;
  typedef mem_array_sep<Data, capacity_in_bytes, banks> Memarray;

  Memarray memarray;

 public:
  axi_::read::template slave<> if_rd;
  axi_::write::template slave<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;

  SC_CTOR(AxiLiteSlaveToMem)
      : if_rd("if_rd"), if_wr("if_wr"), reset_bar("reset_bar"), clk("clk") {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  void run() {
    axi_::AddrPayload rd_addr_pld;
    axi_::AddrPayload wr_addr_pld;
    axi_::ReadPayload data_pld;
    axi_::WritePayload write_pld;
    axi_::WRespPayload resp_pld;
    bool rd_resp_pend;
    bool wr_resp_pend;

    if_rd.reset();
    if_wr.reset();

    rd_resp_pend = false;
    wr_resp_pend = false;

    #pragma hls_pipeline_init_interval 4
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

      bool rd_resp_pend_local = rd_resp_pend;
      bool wr_resp_pend_local = wr_resp_pend;
      bool rd_mem;
      bool wr_mem;

      if (!rd_resp_pend_local) {
        rd_mem = (if_rd.nb_aread(rd_addr_pld));
        if (rd_mem) {
          data_pld.resp = axi_::Enc::XRESP::OKAY;
          rd_resp_pend_local = true;
        }
      }

      if (!wr_resp_pend_local) {
        wr_mem = if_wr.nb_wread(wr_addr_pld, write_pld);

        if (wr_mem) {
          resp_pld.resp = axi_::Enc::XRESP::OKAY;
          wr_resp_pend_local = true;
        }
      }

      if (rd_mem)
        data_pld.data = memarray.read(rd_addr_pld.addr);
      if (wr_mem)
        memarray.write(wr_addr_pld.addr,0,write_pld.data);
      if (rd_resp_pend_local) {
        rd_resp_pend_local = !(if_rd.nb_rwrite(data_pld));
      }

      if (wr_resp_pend_local) {
        wr_resp_pend_local = !(if_wr.nb_bwrite(resp_pld));
      }
      rd_resp_pend = rd_resp_pend_local;
      wr_resp_pend = wr_resp_pend_local;
    }
  }
};

#endif
