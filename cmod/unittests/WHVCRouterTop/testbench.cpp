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

#include "WHVCRouterTop.h"
#include <systemc.h>
#include <mc_scverify.h>
#include <testbench/Pacer.h>
#include <testbench/nvhls_rand.h>
#include <nvhls_connections.h>

#define NVHLS_VERIFY_BLOCKS (WHVCRouterTop)
#include <nvhls_verify.h>

#include <map>
#include <vector>
#include <deque>
#include <utility>
#include <sstream>

using namespace ::std;
typedef WHVCRouterTop::Flit_t Flit_t;
typedef WHVCRouterTop::Credit_t Credit_t;
typedef WHVCRouterTop::Credit_ret_t Credit_ret_t;
static const int kNumVChannels = WHVCRouterTop::kNumVChannels;
static const int kBufferSize = WHVCRouterTop::kBufferSize;
static const int kNumLPorts = WHVCRouterTop::kNumLPorts;
static const int kNumRPorts = WHVCRouterTop::kNumRPorts;
static const int kNumMaxHops = WHVCRouterTop::kNumMaxHops;
static const int kNumPorts = WHVCRouterTop::kNumPorts;
const int LOCAL_DESTPORT_WIDTHPERHOP = kNumLPorts;
const int REMOTE_DESTPORT_WIDTHPERHOP = nvhls::index_width<kNumRPorts>::val;
const int DEST_WIDTHPERHOP = (LOCAL_DESTPORT_WIDTHPERHOP + REMOTE_DESTPORT_WIDTHPERHOP);
const int DEST_WIDTH = DEST_WIDTHPERHOP*kNumMaxHops;
typedef deque<Flit_t> flits_t;

enum RouteType {
UniCast,
UniCast_NoCongestion,
UniCast_SingleDest,
MultiCast,
MultiCast_SingleSource,
MultiCast_DualSource
};

#ifndef ENABLE_MULTICAST
RouteType route_type = UniCast;
#else
RouteType route_type = MultiCast;
#endif

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
      cout <<"added to ref_tracking[" << *it << "][" << out_vc << "][" << id
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
    for (int inport = 0; inport < WHVCRouterTop::kNumPorts; ++inport) {
      for (int vc = 0; vc < kNumVChannels; ++vc) {
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
    ref_tracking.resize(WHVCRouterTop::kNumPorts);

    for (int dest = 0; dest < WHVCRouterTop::kNumPorts; ++dest) {
      ref_tracking[dest].resize(kNumVChannels);
      for (int vc = 0; vc < kNumVChannels; ++vc) {
        ref_tracking[dest][vc].resize(WHVCRouterTop::kNumPorts);
      }
    }

    dest_state.resize(WHVCRouterTop::kNumPorts);

    send_cnt = recv_cnt = 0;
  }

  struct dest_state_t {
    dest_state_t() : source(kNumVChannels){};
    vector<deque<int> > source;  // list of potential sources for each VC
    Flit_t extracted_header[kNumVChannels];  // temporary storage for extracted
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
      for (int i = 0; i < WHVCRouterTop::kNumPorts; ++i) {
        if (!ref_tracking[id][flit_packet_id][i].empty()) {
          // there are pending flits to arrive from source i, did this flit
          // arrive from source i?
          // this check assumes unique match
          Flit_t inspected_flit = ref_tracking[id][flit_packet_id][i].front();
          int inspected_flit_packet_id = inspected_flit.get_packet_id();
          assert(inspected_flit_packet_id == flit_packet_id);
          NVUINTW(DEST_WIDTH) route = nvhls::get_slc<DEST_WIDTH>(inspected_flit.data, 0);
          NVUINTW(DEST_WIDTH) next_dests_local = route >> DEST_WIDTHPERHOP;
          inspected_flit.data = nvhls::set_slc(inspected_flit.data,next_dests_local, 0);;
          if ((flit.flit_id == inspected_flit.flit_id) &&
              (flit.data == inspected_flit.data)) {
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
      cout << hex << "Dest ID: : " << id << " Source ID: " << ds.source[flit_packet_id].front() << " Received: " << flit; 
      assert(
          !ref_tracking[id][flit_packet_id][ds.source[flit_packet_id].front()]
               .empty());

      Flit_t expected_flit =
          ref_tracking[id][flit_packet_id][ds.source[flit_packet_id].front()]
              .front();
      if (expected_flit.flit_id.isHeader()) { // header - throw away routing info of last hop
        NVUINTW(DEST_WIDTH) route = nvhls::get_slc<DEST_WIDTH>(expected_flit.data, 0);
        NVUINTW(DEST_WIDTH) next_dests_local = route >> DEST_WIDTHPERHOP;
        expected_flit.data = nvhls::set_slc(expected_flit.data,next_dests_local, 0);;
      }
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
  Connections::In<Credit_ret_t> credit[kNumVChannels];
  sc_in<bool> clk;
  sc_in<bool> rst;
  const unsigned int id;
  Credit_t credit_reg[kNumVChannels];
  Pacer pacer;
  Reference& ref;

  int select_vc() {
    // randomally select a vc that has >0 credits
    // ok also not to select any some probablity

    deque<int> valid_vc;

    for (int i = 0; i < kNumVChannels; i++) {
      if (credit_reg[i] > 0)
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
    for (int i = 0; i < kNumVChannels; i++) {
      credit[i].Reset();
      credit_reg[i] = kBufferSize;
    }

    vector<flits_t> packet(kNumVChannels);
    vector<deque<int> > dest_per_vc(kNumVChannels);

    int out_vc;
    out.Reset();

    // use this to limit which sources actually emit packets
    // if (id!=0 && id!=1 ) return;

    while (1) {
      wait();

      Credit_ret_t temp;
      for (int i = 0; i < kNumVChannels; i++) {
        if (credit[i].PopNB(temp)) {
          credit_reg[i] += temp;
          assert(credit_reg[i] >= temp);
        }
      }

        // make sure all vcs have active packets to push
        for (int i = 0; i < kNumVChannels; i++) {
          if (packet[i].empty()) {
            dest_per_vc[i].clear();
            packet[i] = generate_packet(i, dest_per_vc[i]);
            //assert(!dest_per_vc[i].empty());
          }
        }

        // at this point all VCs should have flits to be sent
        // but not all VCs will have credits
        // randomly select a vc
        out_vc = select_vc();
      if (!dest_per_vc[out_vc].empty()) {
        if (credit_reg[out_vc] >  0) {
          if (out_vc != -1) {
            Flit_t flit = packet[out_vc].front();
            if (out.PushNB(flit)) {
              packet[out_vc].pop_front();
              credit_reg[out_vc] -= 1;

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
    //#if (PACKETIDWIDTH > 0)
    //  flit.packet_id = vc;
    //#endif
    for (int i = 0; i < num_flits; i++) {

      // Packet id should be unique for every source. It will be used to detect
      // flit at sink
      flit.data = (++unique);
      if (num_flits == 1) {
        flit.flit_id.set(
            FlitId2bit::SNGL);
      } else if (i == 0) {
        flit.flit_id.set(FlitId2bit::HEAD);
      } else if (i == num_flits - 1) {
        flit.flit_id.set(FlitId2bit::TAIL);
      } else {
        flit.flit_id.set(FlitId2bit::BODY);
      }

      if (flit.flit_id.isHeader()) {
        flit.data = (unique << DEST_WIDTH) + create_route(num_flits);

        for (int local = 0; local < kNumLPorts; ++local) {
          if (flit.data[local] == 1) {
            dest.push_back(local);
          }
        }

        unsigned int NEXT_HOP_MASK = 0xffffffff;
        //if (FLIT_DATAWIDTH == 32) {
	      //  NEXT_HOP_MASK = 0xffff;
	      //}
        if (flit.data & (NEXT_HOP_MASK << DEST_WIDTHPERHOP)) {
          // using the same termination heuristic to add remote ports - extract
          // remote only if there is any non-zero in next hops
          // actually extract remote ports bits, and colelct them to list of
          // destinations
          dest.push_back(((flit.data.to_uint64() >> kNumLPorts) &
                          ((1 << REMOTE_DESTPORT_WIDTHPERHOP) - 1)) +
                         kNumLPorts);
        }

        if ((route_type == MultiCast) || (route_type == MultiCast_SingleSource) || (route_type == MultiCast_DualSource)) {
          assert(dest.size() == 1 || id==0 || num_flits > 1 || DEST_WIDTH!=Flit_t::data_width);  // only id==0 can write to local ports for single flit packets (and thus create a snigle flit multicast)
        } else {
	  assert(dest.size() == 1); 
        }

        cout << "###### " << dest.size() << endl;
      }

      cout << "@" << sc_time_stamp() << " Source ID: " << id << " :: Write = "
                << flit << endl;
      if (dest.size()!=0)
        packet.push_back(flit);
    }
    return packet;
  }

  NVUINTW(DEST_WIDTH) create_route(int num_flits) {
    NVUINTW(DEST_WIDTH) route; 
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
        if (((route_type == MultiCast) || (route_type == MultiCast_SingleSource)) && (Flit_t::data_width==DEST_WIDTH) && (rand() % 2 ||
            ((num_flits == 1) &&
             (id > 0))))
        {
          route &= (0xfffffffff << kNumLPorts);  // kill locals for now
        } else {
          int dest_id;
          switch(route_type) {
            case UniCast:
                 dest_id = rand()%WHVCRouterTop::kNumPorts; 
                 if (dest_id < kNumLPorts) {
                    route = 0; 
                    route[dest_id] = 1;
                 } else {
                   route = ((id+1) << DEST_WIDTHPERHOP) + ((dest_id-kNumLPorts) << kNumLPorts);
                 }
#ifndef DISABLE_PACER
                 // Sometimes we explicitly mask the destination of next hop to kill routing to remote ports
                 if (rand()%1000 == 0) 
                   route &= (1 << DEST_WIDTHPERHOP) -1;
#endif
                 break;
            case UniCast_NoCongestion: 
                 if (id < kNumLPorts) {
                   route = 0;
                   route[id] = 1;
                 } else {
                   route = ((id+1) << DEST_WIDTHPERHOP) + ((id-kNumLPorts) << kNumLPorts);
                 }
                 break;
            case UniCast_SingleDest: 
                 route = 0; 
                 route[0] = 1;
                 break;
            case MultiCast: 
                 route = nvhls::get_rand<DEST_WIDTH>();
#ifndef DISABLE_PACER
                 // Sometimes we explicitly mask the destination of next hop to kill routing to remote ports
                 if (rand()%1000 == 0) 
                   route &= (1 << DEST_WIDTHPERHOP) -1;
#endif
                 break;
            case MultiCast_SingleSource: 
                 if (id == 1) {
                   route = nvhls::get_rand<DEST_WIDTH>();
                 } else {
                   route = 0; 
                 }
#ifndef DISABLE_PACER
                 // Sometimes we explicitly mask the destination of next hop to kill routing to remote ports
                 if (rand()%1000 == 0) 
                   route &= (1 << DEST_WIDTHPERHOP) -1;
#endif
                 break;
            case MultiCast_DualSource: 
                 if ((id == 1) || (id==0)) {
                   route = nvhls::get_rand<DEST_WIDTH>();
                 } else {
                   route = 0; 
                 }
#ifndef DISABLE_PACER
                 // Sometimes we explicitly mask the destination of next hop to kill routing to remote ports
                 if (rand()%1000 == 0) 
                   route &= (1 << DEST_WIDTHPERHOP) -1;
#endif
                 break;
            default: break; 
          }
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
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

SC_MODULE(Dest) {
  Connections::In<Flit_t> in;
  Connections::Out<Credit_ret_t> credit[kNumVChannels];
  sc_in<bool> clk;
  sc_in<bool> rst;
  const unsigned int id;
  Credit_t credit_reg[kNumVChannels];
  Pacer pacer;
  Reference& ref;

  void run() {
    // reset
    for (int i = 0; i < kNumVChannels; i++) {
      credit[i].Reset();
      credit_reg[i] = 0;
    }

    in.Reset();
    Flit_t flit;

    wait();
    while (1) {
      // print_dest_ref(id);
      if (in.PopNB(flit)) {
        cout << "@" << sc_time_stamp() << hex << ": " << name() << " received flit: "
                  << flit << endl;
        ++credit_reg[flit.get_packet_id()];
        ref.flit_received(id, flit);
      }

      // flush out credits
      for (int i = 0; i < kNumVChannels; i++) {
        if (credit_reg[i] > 0) {
	  Credit_ret_t temp = 1;
          if (credit[i].PushNB(temp)) {
            credit_reg[i]--;
          }
        }
      }

      wait();

      while (pacer.tic())
        wait(); 
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
    NVHLS_NEG_RESET_SIGNAL_IS(rst);
  }
};

// utility function to generate variety for probabilities for different
// instances
double rate_gen(int i, double slope, double offset) {
  double linear = slope * i + offset;
  return (linear - static_cast<int>(linear));
}

SC_MODULE(testbench) {
  NVHLS_DESIGN(WHVCRouterTop) router;

  typedef Connections::Combinational<Credit_ret_t> CreditChan;
  typedef Connections::Combinational<Flit_t> DataChan;

  sc_clock clk;
  sc_signal<bool> rst;
  Reference ref;

  SC_CTOR(testbench)
      : router("router"),
        clk("clk", 1.0, SC_NS, 0.5, 0, SC_NS, true),
        rst("rst") {

    Connections::set_sim_clk(&clk);

    router.clk(clk);
    router.rst(rst);

    for (int i = 0; i < WHVCRouterTop::kNumPorts; ++i) {
      {  // attach a dest
        // Create and attach DEST where expected by config table
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

        for (int vc = 0; vc < kNumVChannels; ++vc) {
          CreditChan* credit_chan = new CreditChan();
          dest->credit[vc](*credit_chan);
          router.in_credit[i * kNumVChannels + vc](*credit_chan);
        }
      }
      {  // attach a source
        // Create and attach SOURCE where expected by config table
        ostringstream ss;
        ss << "source_on_port_" << i;
        Source* source = new Source(
            ss.str().c_str(), i,
            Pacer(rate_gen(i, 0.2, 0.2), rate_gen(i, 0.3, 0.8)), ref);
        DataChan* data_chan = new DataChan();
        vector<CreditChan*> credit_chan(kNumVChannels);

        source->clk(clk);
        source->rst(rst);
        source->out(*data_chan);

        router.in_port[i](*data_chan);

        for (int vc = 0; vc < kNumVChannels; ++vc) {
          CreditChan* credit_chan = new CreditChan();
          source->credit[vc](*credit_chan);
          router.out_credit[i * kNumVChannels + vc](*credit_chan);
        }
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
  if (rc)
    cout << "Simulation FAILED\n";
  else
    cout << "Simulation PASSED\n";
  return rc;
};
