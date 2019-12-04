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

#ifndef __AXIMASTERGATE_H__
#define __AXIMASTERGATE_H__

#include <systemc.h>
#include <ac_reset_signal_is.h>
#include <axi/axi4.h>
#include <mem_array.h>
#include <fifo.h>
#include <ReorderBuf.h>
#include <axi/AxiMasterGate/ReorderBufWBeats.h>
#include <nvhls_connections.h>
#include <nvhls_assert.h>
#include <axi/AxiMasterGate/AxiMasterGateIf.h>

/**
 * \brief An AXI master that converts from a simple request/response interface to AXI with reordering support.
 * \ingroup AXI
 *
 * \tparam Cfg                A valid AXI config.
 * \tparam ROBDepth           The depth of the reorder buffers.
 * \tparam MaxInFlightTrans   The number of independent AXI requests that can be in flight simultaneously.
 *
 * \par Overview
 * This block takes as inputs RdRequest and WrRequest Connections. The block converts the requests into
 * AXI format, sends them to AXI master ports, and processes the responses into RdResponse and WrResponse ports.
 * ReorderBuf and ReorderBufWBeats are used to allow reordering via use of the AXI ID field.
 *
 * \par Usage Guidelines
 *
 * This module sets the stall mode to flush by default to mitigate possible RTL
 * bugs that can occur in the default stall mode. If you are confident that
 * this class of bugs will not occur in your use case, you can change the stall
 * mode via TCL directive:
 *
 * \code
 * directive set /path/to/AxiMasterGate/run_rd/while -PIPELINE_STALL_MODE stall
 * \endcode
 *
 * This may reduce area/power.
 * \par
 *
 */
template <typename Cfg, int ROBDepth = 8, int MaxInFlightTrans = 4>
class AxiMasterGate : public sc_module {
 private:
  typedef axi::axi4<Cfg> axi4_;

  typedef NVUINTW(axi4_::DATA_WIDTH) Data;

  ReorderBuf<WrResp<Cfg>, ROBDepth, MaxInFlightTrans> wr_rob;
  ReorderBufWBeats<RdResp<Cfg>, ROBDepth, MaxInFlightTrans> rd_rob;

  FIFO<RdRequest<Cfg>, 4> rdReqFifo;
  FIFO<WrRequest<Cfg>, 4> wrReqFifo;

 public:
  typename axi4_::read::template master<> if_rd;
  typename axi4_::write::template master<> if_wr;

  sc_in<bool> reset_bar;
  sc_in<bool> clk;
  Connections::In<WrRequest<Cfg> > wrRequestIn;
  Connections::Out<WrResp<Cfg> > wrRespOut;
  Connections::In<RdRequest<Cfg> > rdRequestIn;
  Connections::Out<RdResp<Cfg> > rdRespOut;

  SC_CTOR(AxiMasterGate)
      : if_rd("if_rd"), if_wr("if_wr"), reset_bar("reset_bar"), clk("clk") {
    SC_THREAD(run_rd);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);

    SC_THREAD(run_wr);
    sensitive << clk.pos();
    async_reset_signal_is(reset_bar, false);
  }

 protected:
  typedef sc_uint<axi4_::ID_WIDTH> Id;

  void run_wr() {

    NVHLS_ASSERT_MSG( MaxInFlightTrans <= (1 << axi4_::ID_WIDTH) , "Number_of_inflight_transactions_cannot_exceed_number_of_unique_IDs");

    if_wr.reset();
    wrRequestIn.Reset();
    wrRespOut.Reset();
    wr_rob.reset();
    wrReqFifo.reset();

    bool addr_sent = false;
    WrRequest<Cfg> wrRequest;
    bool wrRequestValid = false;
    Id wrRequestId;
    bool wrRequestIdValid = false;
    bool isBurstInFlight = false;

    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();
#ifdef DEBUGMODE
      cout << "@" << sc_time_stamp() << "\t wr_rob is empty? "
           << wr_rob.isEmpty() << endl;
#endif
      // send response
      if (wr_rob.topResponseReady()) {
        WrResp<Cfg> wrResp;
        wrResp = wr_rob.popResponse();

#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp()
             << "\t\t Pop first response from rob and push it into host"
             << "\tresp = " << wrResp.resp << endl;
#endif

        wrRespOut.Push(wrResp);  // I expect the other side to be always ready,
                                 // therefore using blocking push here
#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp() << "\t\t Done pushing resp to host"
             << endl;
#endif
      }

      WrRequest<Cfg> wrRequest_local = wrRequest;
      bool wrRequestValid_local = wrRequestValid;
      Id wrRequestId_local = wrRequestId;
      bool wrRequestIdValid_local = wrRequestIdValid;
      bool wrReqFifo_isFull = wrReqFifo.isFull();

      // receive requests
      if (!wrRequestValid_local && !wrReqFifo.isEmpty()) {
        wrRequest_local = wrReqFifo.pop();
        wrRequestValid_local = true;
        wrRequestIdValid_local = false;

#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp() << "\t\t wr:pop wrReqFifo" << endl;
#endif
      }

      // receive responses
      {
        typename axi4_::WRespPayload resp_pld;
#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp() << "\t\t Receiving resp from slave"
             << endl;
#endif
        if (if_wr.b.PopNB(resp_pld)) {
          WrResp<Cfg> wrResp;
          wrResp.resp = resp_pld.resp;

#ifdef DEBUGMODE
          cout << "@" << sc_time_stamp() << "\t\t wr:Post response to rob"
               << "\tid = " << resp_pld.id << "\tresp = " << resp_pld.resp
               << endl;
#endif
          wr_rob.addResponse(static_cast<sc_uint<axi4_::ID_WIDTH> >(resp_pld.id), wrResp);
        }
      }

      if (wrRequestValid_local) {
        bool isBurstInFlight_local = isBurstInFlight;
        wrRequestIdValid_local =
            wrRequestIdValid_local || isBurstInFlight_local;

        // allocate a new id
        if (!wrRequestIdValid_local && wr_rob.canAcceptRequest()) {
          wrRequestId_local = wr_rob.addRequest();
          wrRequestIdValid_local = true;
#ifdef DEBUGMODE
          cout << "@" << sc_time_stamp()
               << "\t\t wr:new id allocated = " << wrRequestId_local << endl;
#endif
        }

        if (wrRequestIdValid_local) {
          NVHLS_ASSERT_MSG(wrRequestValid_local, "Should_be_impossible_to_reach_this_state");

          bool addr_sent_local = addr_sent;
          if (!addr_sent_local) {
            // send addr
            typename axi4_::AddrPayload addr_pld;
            wrRequest_local.copyToAddrPayload(addr_pld);
            addr_pld.id = static_cast<typename axi4_::Id>(wrRequestId_local);

            addr_sent_local = if_wr.aw.PushNB(addr_pld);
#ifdef DEBUGMODE
            cout << "@" << sc_time_stamp() << "\t\t wr:sending addr pass? "
                 << addr_sent_local << "\taddr = " << wrRequest_local.addr
                 << "\tid = " << wrRequestId_local
                 << "\tlen = " << wrRequest_local.len
                 << "\tsize = " << wrRequest_local.size
                 << "\tburst = " << wrRequest_local.burst << endl;
#endif
          } else {
            // send data
            typename axi4_::WritePayload write_pld;
            write_pld.data = wrRequest_local.data;
            write_pld.last = wrRequest_local.last;
            write_pld.wstrb = ~0;

            bool pushed;
            pushed = if_wr.w.PushNB(write_pld);
            wrRequestValid_local = wrRequestValid_local && !pushed;
            wrRequestIdValid_local = wrRequestIdValid_local && !pushed;
#ifdef DEBUGMODE
            cout << "@" << sc_time_stamp() << "\t\t sending data pass? "
                 << !addr_sent_local << "\tdata = " << wrRequest_local.data
                 << "\tlast = " << wrRequest_local.last << endl;
#endif
            isBurstInFlight_local = (wrRequest_local.last != 1);

            // if this is a burst, and not the last beat next time we need to
            // keep sending data, not addr yet
            bool sendWNext = isBurstInFlight_local || !pushed;
            // addr_sent_local==false will mean the next cycle that addr has to
            // be sent
            addr_sent_local = sendWNext;
          }
          addr_sent = addr_sent_local;
        }
        isBurstInFlight = isBurstInFlight_local;
      }

      wrRequest = wrRequest_local;
      wrRequestValid = wrRequestValid_local;
      wrRequestId = wrRequestId_local;
      wrRequestIdValid = wrRequestIdValid_local;

      if (!wrReqFifo_isFull) {
        WrRequest<Cfg> wrRequest1;
        // receive requests
        if (wrRequestIn.PopNB(wrRequest1)) {
          wrReqFifo.push(wrRequest1);

#ifdef DEBUGMODE
          cout << "@" << sc_time_stamp() << "\t\t wr:m got request!" << endl;
#endif
        }
      }
    }
  }

  void run_rd() {
    rdRequestIn.Reset();
    rdRespOut.Reset();
    rd_rob.reset();
    if_rd.reset();
    rdReqFifo.reset();

    RdRequest<Cfg> rdRequest;
    bool rdRequestValid = false;
    Id rdRequestId;
    bool rdRequestIdValid = false;
    bool rdBurstInFlight = false;
    bool rdReceivingBurstBeats = false;

    #pragma hls_pipeline_init_interval 1
    #pragma pipeline_stall_mode flush
    while (1) {
      wait();

#ifdef DEBUGMODE
      cout << "@" << sc_time_stamp() << "\t rd_rob is empty? "
           << rd_rob.isEmpty() << endl;
#endif
      // send response
      if (rd_rob.topResponseReady()) {
        RdResp<Cfg> rdResp;
        rdResp = rd_rob.popResponse();

#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp()
             << "\t\t rd:Pop first response from rob and push it into host"
             //<< "\tresp = " << rdResp
             << endl;
#endif

        rdRespOut.Push(rdResp);  // I expect the other side to be always ready,
                                 // therefore using blocking push here
#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp() << "\t\t rd:Done pushing resp to host"
             << endl;
#endif
      }

      RdRequest<Cfg> rdRequest_local = rdRequest;
      bool rdRequestValid_local = rdRequestValid;
      Id rdRequestId_local = rdRequestId;
      bool rdRequestIdValid_local = rdRequestIdValid;
      bool rdReqFifo_isFull = rdReqFifo.isFull();
      bool rdBurstInFlight_local = rdBurstInFlight;

      if (!rdRequestValid_local && !rdReqFifo.isEmpty()) {
        rdRequest_local = rdReqFifo.pop();
        rdRequestValid_local = true;
        rdRequestIdValid_local = false;
#ifdef DEBUGMODE
        cout << "@" << sc_time_stamp() << "\t\t rd:pop rdReqFifo" << endl;
#endif
      }

      if (rdRequestValid_local) {

        // rdRequest_local.len!=0 is a multi beat (burst) read
        // we will wait for all previous read transactions to complete first
        // otherwise we just need a slot in rob

        bool isBurst = (rdRequest_local.len != 0);
        bool robReady =
            (isBurst ? rd_rob.isEmpty() : rd_rob.canAcceptRequest());

        if (!rdRequestIdValid_local && robReady && !rdBurstInFlight_local) {
          rdRequestId_local = rd_rob.addRequest();
          rdRequestIdValid_local = true;
          rdBurstInFlight_local = isBurst;
#ifdef DEBUGMODE
          cout << "@" << sc_time_stamp()
               << "\t\t rd:new id allocated = " << rdRequestId << endl;
#endif
        }

        if (rdRequestIdValid_local) {
          typename axi4_::AddrPayload addr_pld;
          rdRequest_local.copyToAddrPayload(addr_pld);
          addr_pld.id = static_cast<typename axi4_::Id>(rdRequestId_local);

          bool pushed = if_rd.ar.PushNB(addr_pld);
          rdRequestValid_local = !pushed;
          rdRequestIdValid_local = !pushed;
        }
      }

      rdRequest = rdRequest_local;
      rdRequestValid = rdRequestValid_local;
      rdRequestId = rdRequestId_local;
      rdRequestIdValid = rdRequestIdValid_local;

      if (!rdReqFifo_isFull) {
        RdRequest<Cfg> rdRequest1;
        // receive requests
        if (rdRequestIn.PopNB(rdRequest1)) {
          rdReqFifo.push(rdRequest1);

#ifdef DEBUGMODE
          cout << "@" << sc_time_stamp() << "\t\t rd:m got request ? "
               << rdRequestValid << endl;
#endif
        }
      }

      {
        bool rdReceivingBurstBeats_local = rdReceivingBurstBeats;
        typename axi4_::ReadPayload data_pld;
        // for single beat transactions rob is guaranteed to have an entry for
        // response
        // but for multi beat, starting from the second beat we don't know and
        // have to check it
        if (!rdReceivingBurstBeats_local || rd_rob.canReceiveBeats()) {
          if (if_rd.r.PopNB(data_pld)) {
            RdResp<Cfg> rdResp;
            rdResp.data = data_pld.data;
            rdResp.resp = data_pld.resp;
            rdResp.last = data_pld.last;

            if (!rdReceivingBurstBeats_local) {
              rd_rob.addResponse(static_cast<sc_uint<axi4_::ID_WIDTH> >(data_pld.id), rdResp);
            } else {
              rd_rob.addBeat(rdResp);
              if (static_cast<sc_uint<1> >(data_pld.last) == 1) {
                // if this is the last beat, return to normal operation mode
                rdBurstInFlight_local = false;
              }
            }

            // subsequent beats (after the first one will have to know to use a
            // different api to be pushed into Rob)
            rdReceivingBurstBeats_local = rdBurstInFlight_local;
          }
        }
        rdReceivingBurstBeats = rdReceivingBurstBeats_local;
      }
      rdBurstInFlight = rdBurstInFlight_local;
    }
  }
};

#endif
