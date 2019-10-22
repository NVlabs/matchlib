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

#include "HybridRouterTop.h"
#include <systemc.h>
#include <mc_scverify.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>
#include <nvhls_connections.h>

#define NVHLS_VERIFY_BLOCKS (HybridRouterTop)
#include <nvhls_verify.h>

#include <map>
#include <vector>
#include <deque>
#include <utility>
#include <sstream>

using namespace ::std;
typedef HybridRouterTop::Flit_t Flit_t;
typedef HybridRouterTop::Credit_t Credit_t;
typedef HybridRouterTop::Credit_ret_t Credit_ret_t;
const unsigned int NUM_LPORTS = spec::noc::numLPorts;
const unsigned int NUM_RPORTS = spec::noc::numRPorts;
const unsigned int NUM_VCHANNELS = spec::noc::numVChannels;
const unsigned int BUFFERSIZE = spec::noc::bufferSize;
const int NOC_DEST_WIDTH = HybridRouterTop::NOC_DEST_WIDTH;
const int NOC2_DEST_WIDTH = HybridRouterTop::NOC2_DEST_WIDTH;
const int NOC_UCAST_DEST_WIDTH = HybridRouterTop::NOC_UCAST_DEST_WIDTH;
const int NOC2_UCAST_DEST_WIDTH = HybridRouterTop::NOC2_UCAST_DEST_WIDTH;
const int NOC_MCAST_DEST_WIDTH = HybridRouterTop::NOC_MCAST_DEST_WIDTH;
const int NOC2_MCAST_DEST_WIDTH = HybridRouterTop::NOC2_MCAST_DEST_WIDTH;
const int DEST_WIDTH = NOC_MCAST_DEST_WIDTH + NOC2_MCAST_DEST_WIDTH;
typedef deque<Flit_t> flits_t;

enum RouteType {
  UniCast,
  UniCast_NoCongestion,
  UniCast_SingleDest,
  MultiCast,
  MultiCast_SingleSource,
  MultiCast_DualSource
};

// RouteType route_type = MultiCast;
RouteType route_type = UniCast;
// RouteType route_type = UniCast_SingleDest;

class Reference {
 public:
  Reference() { init_tracking(); }

  ~Reference() {
    cout << "ref send_cnt = " << send_cnt << endl;
    cout << "ref recv_cnt = " << recv_cnt << endl;
  }

  unsigned send_cnt, recv_cnt;
  void flit_sent(const int& id, const deque<int>& dest, const int& out_vc,
                 const Flit_t& flit) {
    for (deque<int>::const_iterator it = dest.begin(); it != dest.end(); ++it) {
      cout << hex << sc_time_stamp() << " sent flit: " << flit;
      cout << "added to ref_tracking[" << *it << "][" << out_vc << "][" << id
           << "]" << dec << endl;
      ref_tracking[*it][out_vc][id].push_back(flit);
      ++send_cnt;
    }
  }

  void flit_received(const int& id, const Flit_t& flit) {
    flit_received_testing(id, flit, dest_state[id]);
    ++recv_cnt;
  }

  void print_dest_ref(const int& id) {
    cout << "--------------------" << endl;
    for (int inport = 0; inport < HybridRouterTop::NumPorts; ++inport) {
      for (unsigned vc = 0; vc < NUM_VCHANNELS; ++vc) {
        cout << "ref_tracking[" << id << "][" << vc << " ][" << inport
             << "].top= ";

        if (ref_tracking[id][vc][inport].empty()) {
          cout << "EMPTY" << endl;
        } else {
          for (deque<Flit_t>::iterator it =
                   ref_tracking[id][vc][inport].begin();
               it != ref_tracking[id][vc][inport].end(); ++it) {
            Flit_t flit = *it;  // ref_tracking[id][vc][inport].front();
            cout << flit << endl;
          }
        }
      }
    }
    cout << "--------------------" << endl;
  }

 private:
  typedef vector<int> vc_chain_t;
  typedef vector<flits_t> vc_dst_storage_t;  // each entry is storage from a
                                             // given source - the whole
                                             // structure is for a single vc
  typedef vector<vc_dst_storage_t> dst_storage_t;  // storage per vc
  typedef vector<dst_storage_t>
      tracking_t;  // each entry is a destination being tracked

  tracking_t ref_tracking;

  void init_tracking() {
    ref_tracking.resize(HybridRouterTop::NumPorts);

    for (int dest = 0; dest < HybridRouterTop::NumPorts; ++dest) {
      ref_tracking[dest].resize(NUM_VCHANNELS);
      for (unsigned vc = 0; vc < NUM_VCHANNELS; ++vc) {
        ref_tracking[dest][vc].resize(HybridRouterTop::NumPorts);
      }
    }

    dest_state.resize(HybridRouterTop::NumPorts);

    send_cnt = recv_cnt = 0;
  }

  struct dest_state_t {
    dest_state_t() : source(NUM_VCHANNELS){};
    vector<deque<int> > source;  // list of potential sources for each VC
    Flit_t extracted_header[NUM_VCHANNELS];  // temporary storage for extracted
                                             // header
  };

  vector<dest_state_t> dest_state;

  void flit_received_testing(const int& id, const Flit_t& flit,
                             dest_state_t& ds) {
    int flit_packet_id = 0;
    flit_packet_id = flit.get_packet_id();
    if (flit.flit_id.isHeader()) {

      ds.source[flit_packet_id].clear();
      // find out who sent this flit - go over all sources and check
      for (int i = 0; i < HybridRouterTop::NumPorts; ++i) {
        if (!ref_tracking[id][flit_packet_id][i].empty()) {
          // there are pending flits to arrive from source i, did this flit
          // arrive from source i?
          // this check assumes unique match
          Flit_t inspected_flit = ref_tracking[id][flit_packet_id][i].front();
          int inspected_flit_packet_id = inspected_flit.get_packet_id();
          assert(inspected_flit_packet_id == flit_packet_id);
          if ((flit.flit_id == inspected_flit.flit_id) &&
              ((flit.data >> (DEST_WIDTH + 1)) ==
               (inspected_flit.data >> (DEST_WIDTH + 1)))) {
            ds.source[flit_packet_id].push_back(i);
            ds.extracted_header[flit_packet_id] = flit;

            ref_tracking[id][flit_packet_id][i].pop_front();
          }
        }
      }
      assert(!ds.source[flit_packet_id].empty());
      if (flit.flit_id.isSingle()) {
        assert(ds.source[flit_packet_id].size() == 1);
        // there will be no body flits following, remove it from sources list
        // right now
        ds.source[flit_packet_id].clear();
      }
    } else {

      if (ds.source[flit_packet_id].size() > 1) {
        bool found = false;
        int found_source;
        // multiple headers did match, find which one is correct
        for (deque<int>::iterator it = ds.source[flit_packet_id].begin();
             it != ds.source[flit_packet_id].end(); ++it) {
          bool matching = false;
          if (!ref_tracking[id][flit_packet_id][*it].empty()) {
            Flit_t expected_flit =
                ref_tracking[id][flit_packet_id][*it].front();
            assert(flit_packet_id == expected_flit.get_packet_id());
            if ((flit.flit_id == expected_flit.flit_id) &&
                (flit.data == expected_flit.data)) {
              assert(found == false);
              found = true;
              matching = true;
              found_source = *it;
            }
          }

          if (!matching) {
            // put back the extracted flit
            ref_tracking[id][flit_packet_id][*it].push_front(
                ds.extracted_header[flit_packet_id]);
          }
        }

        assert(found);
        ds.source[flit_packet_id].clear();
        ds.source[flit_packet_id].push_back(found_source);
        // only a single candidate should remain at the very end
      }
      cout << hex << "Dest ID: : " << id
           << " Source ID: " << ds.source[flit_packet_id].front()
           << " Received: " << flit;
      assert(
          !ref_tracking[id][flit_packet_id][ds.source[flit_packet_id].front()]
               .empty());

      Flit_t expected_flit =
          ref_tracking[id][flit_packet_id][ds.source[flit_packet_id].front()]
              .front();

      cout << " Expected: " << expected_flit << dec << endl;
      assert(flit_packet_id == expected_flit.get_packet_id());
      assert(flit.flit_id == expected_flit.flit_id);
      assert(flit.data == expected_flit.data);

      ref_tracking[id][flit_packet_id][ds.source[flit_packet_id].front()]
          .pop_front();
    }
  }
};

SC_MODULE(Source) {
  Connections::Out<Flit_t> out;
  sc_in<bool> clk;
  sc_in<bool> rst;
  const unsigned int id;
  Pacer pacer;
  Reference& ref;

  int select_vc() {
    // randomally select a vc that has >0 credits
    // ok also not to select any some probablity

    deque<int> valid_vc;

    for (unsigned i = 0; i < NUM_VCHANNELS; i++) {
      valid_vc.push_back(i);
    }

    if (valid_vc.empty()
#ifndef DISABLE_PACER
        || ((rand() % 10) < 3)  // occasionally don't pick any vc
#endif
        )
      return -1;
    return valid_vc[rand() % valid_vc.size()];
  }

  void run() {
    // reset
    vector<flits_t> packet(NUM_VCHANNELS);
    vector<deque<int> > dest_per_vc(NUM_VCHANNELS);
    int out_vc;
    out.Reset();

    // use this to limit which sources actually emit packets
    // if (id!=0 && id!=1 ) return;

    while (1) {
      wait();
      // make sure all vcs have active packets to push
      for (unsigned i = 0; i < NUM_VCHANNELS; i++) {
        if (packet[i].empty()) {
          dest_per_vc[i].clear();
          packet[i] = generate_packet(i, dest_per_vc[i]);
          // assert(!dest_per_vc[i].empty());
        }
      }

      // at this point all VCs should have flits to be sent
      // but not all VCs will have credits
      // randomly select a vc
      out_vc = select_vc();
      if (!dest_per_vc[out_vc].empty()) {
        if (out_vc != -1) {
          Flit_t flit = packet[out_vc].front();
          if (out.PushNB(flit)) {
            packet[out_vc].pop_front();
            ref.flit_sent(id, dest_per_vc[out_vc], out_vc, flit);
          } else {
            cout << "ID: " << id << " Push Unsuccessful\n";
          }
        }
        while (pacer.tic())
          wait();
      }
    }
  }

  flits_t generate_packet(int vc, deque<int>& dest) {
    static NVUINTW(Flit_t::data_width) unique = 0;
    flits_t packet;

    Flit_t flit;

    int num_flits = (rand() % 9) + 1;

    if (rand() % 20 < 2) {
      num_flits = 16;
    } else if (rand() % 20 < 2) {
      num_flits = 1;
    }
#if (PACKETIDWIDTH > 0)
    flit.packet_id = vc;
#endif
    for (int i = 0; i < num_flits; i++) {

      // Packet id should be unique for every source. It will be used to detect
      // flit at sink
      flit.data = (++unique);
      if (num_flits == 1) {
        flit.flit_id.set(
            FlitId2bit::SNGL);  // FIXME: make it grabable from Flit_t
      } else if (i == 0) {
        flit.flit_id.set(FlitId2bit::HEAD);
      } else if (i == num_flits - 1) {
        flit.flit_id.set(FlitId2bit::TAIL);
      } else {
        flit.flit_id.set(FlitId2bit::BODY);
      }

      if (flit.flit_id.isHeader()) {
        NVUINTW(DEST_WIDTH) route = create_route(num_flits);
        flit.data = (unique << DEST_WIDTH + 1) + route;
        flit.data[DEST_WIDTH] = ((route_type == MultiCast) ||
                                 (route_type == MultiCast_SingleSource) ||
                                 (route_type == MultiCast_DualSource));
        int dest_local[NUM_LPORTS + NUM_RPORTS] = {0, 0, 0, 0, 0};
        const int NoCRouteLUT_local[NOC_DEST_WIDTH] = {
            3, 3, 4, 1, 1, 3, 3, 0};
        if ((route_type == MultiCast) ||
            (route_type == MultiCast_SingleSource) ||
            (route_type == MultiCast_DualSource)) {
          // mcast format
          for (int i = NOC2_MCAST_DEST_WIDTH;
               i < NOC2_MCAST_DEST_WIDTH + NOC_MCAST_DEST_WIDTH; ++i) {
            if (flit.data[i] == 1) {
              dest_local[NoCRouteLUT_local[i - NOC2_MCAST_DEST_WIDTH]] = 1;
            }
          }
        } else {
          // ucast format
          NVUINTW(NOC_UCAST_DEST_WIDTH)
          dest_id = nvhls::get_slc<NOC_UCAST_DEST_WIDTH>(flit.data, 0);
          dest_local[NoCRouteLUT_local[dest_id]] = 1;
        }
        cout << "Route: " << route.to_uint64() << " Dest: ";
        for (unsigned i = 0; i < NUM_LPORTS + NUM_RPORTS; ++i) {
          cout << dest_local[i] << " ";
        }
        cout << endl;
        for (unsigned i = 0; i < NUM_LPORTS + NUM_RPORTS; ++i) {
          if (dest_local[i] == 1) {
            dest.push_back(i);
          }
        }

        if ((route_type == MultiCast) ||
            (route_type == MultiCast_SingleSource) ||
            (route_type == MultiCast_DualSource)) {
          assert(dest.size() == 1 || id == 0 || num_flits > 1 ||
                 DEST_WIDTH != Flit_t::data_width);  // only id==0 can write to
                                                     // local ports for single
                                                     // flit packets (and thus
                                                     // create a snigle flit
                                                     // multicast)
        } else {
          assert(dest.size() ==
                 1);  // FIXME - for now on paper should support more
        }

        cout << "###### " << dest.size() << endl;
      }

      cout << "@" << sc_time_stamp() << " Source ID: " << id
           << " :: Write = " << flit << endl;
      if (dest.size() != 0)
        packet.push_back(flit);
    }
    return packet;
  }

  NVUINTW(DEST_WIDTH) create_route(int num_flits) {
    NVUINTW(DEST_WIDTH) route = 0;
    // in case of a single flit packet we must have the "rest" of the bits,
    // left to current hop, to carry some information.
    // otherwise we won't be able to identify the origin of this flit at
    // destination. In case of flits to local ports we
    // are required to zero out that part to prevent recognizing remote port
    // 0 as destination. Therefore the testbench
    // can not cover single flit packets sent to local ports. Given that the
    // internal implementation of router is implemented
    // without distinguishing between local and remote ports after routing
    // decompression, I think it is a tolerable limitation.
    // To ease it a bit, we can allow single flit packets sent to local
    // ports by only one source (id==0), then identification
    // won't be an issue.
    int dest_id;
    route = 0;
    switch (route_type) {
      case UniCast:
        dest_id = rand() % NOC_DEST_WIDTH;
        route = dest_id;
        break;
      case UniCast_NoCongestion:
        route = id;
        break;
      case UniCast_SingleDest:
        route = 1;
        break;
      case MultiCast:
        route = nvhls::get_rand<NOC_MCAST_DEST_WIDTH>();
        route <<= NOC2_MCAST_DEST_WIDTH;
        route[0] = 1;  // Indicates local noc2
        break;
      case MultiCast_SingleSource:
        if (id == 1) {
          route = nvhls::get_rand<NOC_MCAST_DEST_WIDTH>();
          route <<= NOC2_MCAST_DEST_WIDTH;
          route[0] = 1;  // Indicates local noc2
        } else {
          route = 0;
        }
        break;
      case MultiCast_DualSource:
        if ((id == 1) || (id == 0)) {
          route = nvhls::get_rand<NOC_MCAST_DEST_WIDTH>();
          route <<= NOC2_MCAST_DEST_WIDTH;
          route[0] = 1;  // Indicates local noc2
        } else {
          route = 0;
        }
        break;
      default:
        break;
    }
    return route;
  }

  SC_HAS_PROCESS(Source);

  Source(sc_module_name name_, const unsigned int& id_, const Pacer& pacer_,
         Reference& ref_)
      : sc_module(name_),
        out("out"),
        clk("clk"),
        rst("rst"),
        id(id_),
        pacer(pacer_),
        ref(ref_) {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }
};

SC_MODULE(Dest) {
  Connections::In<Flit_t> in;
  sc_in<bool> clk;
  sc_in<bool> rst;
  const unsigned int id;
  Pacer pacer;
  Reference& ref;

  void run() {
    // reset
    in.Reset();
    Flit_t flit;

    wait();
    while (1) {
      // print_dest_ref(id);
      if (in.PopNB(flit)) {
        cout << "@" << sc_time_stamp() << hex << ": " << name()
             << " received flit: " << flit << endl;
        ref.flit_received(id, flit);
      }
      wait();
      while (pacer.tic())
        wait();  // FIXME get the order of real/pacer waits consistent with
                 // source to make it preatty
    }
  };

  SC_HAS_PROCESS(Dest);
  Dest(sc_module_name name_, const unsigned int& id_, const Pacer& pacer_,
       Reference& ref_)
      : sc_module(name_),
        in("in"),
        clk("clk"),
        rst("rst"),
        id(id_),
        pacer(pacer_),
        ref(ref_) {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(rst, false);
  }
};

// utility function to generate variety for probabilities for different
// instances
double rate_gen(int i, double slope, double offset) {
  double linear = slope * i + offset;
  return (linear - static_cast<int>(linear));
}

SC_MODULE(testbench) {
  NVHLS_DESIGN(HybridRouterTop) router;

  typedef Connections::Combinational<Flit_t> DataChan;

  sc_clock clk;
  sc_signal<bool> rst;
  Reference ref;

  // JTAG for NoC
  sc_signal<HybridRouterTop::NoCRouteLUT_t> NoCRouteLUT_jtag[NOC_DEST_WIDTH];
  sc_signal<HybridRouterTop::NoC2RouteLUT_t> NoC2RouteLUT_jtag[NOC2_DEST_WIDTH];
  sc_signal<HybridRouterTop::noc2_id_t> noc2_id_jtag;

  SC_CTOR(testbench)
      : router("router"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, false),
        rst("rst") {

    Connections::set_sim_clk(&clk);

    router.clk(clk);
    router.rst(rst);

    // Connect JTAG
    const int NoCRouteLUT_data[NOC_DEST_WIDTH] = {
        8, 8, 16, 2, 2, 8, 8, 1};
    const int NoC2RouteLUT_data[NOC2_DEST_WIDTH] = {0x08, 0x08, 0x08, 0x08};
    for (int i = 0; i < NOC_DEST_WIDTH; ++i) {
      router.NoCRouteLUT_jtag[i](NoCRouteLUT_jtag[i]);
      NoCRouteLUT_jtag[i].write(NoCRouteLUT_data[i]);
    }
    for (int i = 0; i < NOC2_DEST_WIDTH; ++i) {
      router.NoC2RouteLUT_jtag[i](NoC2RouteLUT_jtag[i]);
      NoC2RouteLUT_jtag[i].write(NoC2RouteLUT_data[i]);
    }
    router.noc2_id_jtag(noc2_id_jtag);
    noc2_id_jtag.write(0);

    for (int i = 0; i < HybridRouterTop::NumPorts; ++i) {
      {  // attach a dest
        // Create and attach DEST where expected by config table
        // FIXME: I don't care about never destructing these
        ostringstream ss;
        ss << "dest_on_port_" << i;
        Dest* dest =
            new Dest(ss.str().c_str(), i,
                     Pacer(rate_gen(i, 0.2, 0.2), rate_gen(i, 0.3, 0.8)), ref);
        DataChan* data_chan = new DataChan();

        dest->clk(clk);
        dest->rst(rst);
        dest->in(*data_chan);
        router.out_port[i](*data_chan);
      }
      {  // attach a source
        // Create and attach SOURCE where expected by config table
        // FIXME: I don't care about never destructing these
        ostringstream ss;
        ss << "source_on_port_" << i;
        Source* source = new Source(
            ss.str().c_str(), i,
            Pacer(rate_gen(i, 0.2, 0.2), rate_gen(i, 0.3, 0.8)), ref);
        DataChan* data_chan = new DataChan();
        source->clk(clk);
        source->rst(rst);
        source->out(*data_chan);
        router.in_port[i](*data_chan);
      }
    }

    SC_THREAD(run);
  }

  void run() {
    // reset
    rst = 0;
    cout << "@" << sc_time_stamp() << " Asserting Reset " << endl;
    wait(2, SC_NS);
    cout << "@" << sc_time_stamp() << " Deasserting Reset " << endl;
    rst = 1;
    wait(5000, SC_NS);
    cout << "@" << sc_time_stamp() << " Stop " << endl;
    sc_stop();
  }
};

int sc_main(int argc, char* argv[]) {
  nvhls::set_random_seed();
  testbench my_testbench("my_testbench");
  sc_report_handler::set_actions(SC_ERROR, SC_DISPLAY);
  sc_start();
  bool rc = (sc_report_handler::get_count(SC_ERROR) > 0);

  if (my_testbench.ref.send_cnt < 7000) {
    rc = true;
  }
  if (my_testbench.ref.recv_cnt < 7000) {
    rc = true;
  }

  if (rc)
    cout << "Simulation FAILED\n";
  else
    cout << "Simulation PASSED\n";
  return rc;
};
