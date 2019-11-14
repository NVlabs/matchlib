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
#ifndef MATCH_MODULE_H
#define MATCH_MODULE_H

#ifndef __SYNTHESIS__
#include <map>
#endif

/**
 * \brief ENABLE_SYNC_RESET define: Select synchronous or asynchronous reset.
 * \ingroup nvhls_module
 *
 * Matchlib uses asynchronous, active-low reset by default. Defining ENABLE_SYNC_RESET
 * will use synchronous, active-low reset instead.
 * The macros NVHLS_NEG_RESET_SIGNAL_IS() and NVHLS_POS_RESET_SIGNAL_IS() can be used
 * in place of SystemC's reset_signal_is() and async_reset_signal_is() so that
 * ENABLE_SYNC_RESET can select type.
 *
 * Active-low and -high are separate macros, in case module code assumes a priority.
 */
#if defined(ENABLE_SYNC_RESET)
#define NVHLS_NEG_RESET_SIGNAL_IS(port) reset_signal_is(port,false)
#define NVHLS_POS_RESET_SIGNAL_IS(port) reset_signal_is(port,true)
#else // defined(ENABLE_SYNC_RESET)
#define NVHLS_NEG_RESET_SIGNAL_IS(port) async_reset_signal_is(port,false)
#define NVHLS_POS_RESET_SIGNAL_IS(port) async_reset_signal_is(port,true)
#endif // defined(ENABLE_SYNC_RESET)

#include <nvhls_trace.h>
#include <nvhls_marshaller.h>
#include <nvhls_message.h>

/**
 * \brief nvhls_concat define: Concatenate two strings, separate with an underscore.
 * \ingroup nvhls_module
 *
 * Useful for concatenating two strings to derive a module name.
 *
 * Catapult synthesis in C++11 mode currently doesn't support + operators on
 * strings, so only s2 is used.
 */
#if defined(__SYNTHESIS__) && defined(HLS_CATAPULT) && __cplusplus >= 201103L
#define nvhls_concat(s1,s2) (std::string(s2)).c_str()
#else
#define nvhls_concat(s1,s2) (std::string(s1) + "_" + std::string(s2)).c_str()
#endif

namespace match {
/**
 * \brief Matchlib Module class: a wrapper of sc_module with tracing and stats support. 
 * \ingroup nvhls_module
 *
 * \par A Simple Example
 * \code
 * #include <nvhls_module.h>
 *
 * class CrossbarModuleConnectionsInterface : public match::Module {
 *  public:
 *   ... 
 *   void Process();
 * 
 *   SC_HAS_PROCESS(CrossbarModuleConnectionsInterface);
 *   CrossbarModuleConnectionsInterface(sc_module_name name_)
 *       : match::Module(name_) {
 *     SC_THREAD(Process);
 *     sensitive << clk.pos();
 *     async_reset_signal_is(rst, false);
 * 
 *     this->SetTraceLevel(2); // Set trace level
 *   }
 * };
 * 
 * void CrossbarModuleConnectionsInterface::Process() {
 *   ...
 * 
 *   while(1) {
 *     wait();
 *     T(1) << " Entered DUT" << EndT; // Trace print statements
 * 
 *     #pragma hls_unroll yes
 *     for(int inp_lane=0; inp_lane<NumInputLanes; inp_lane++) {
 *       data_in_reg[inp_lane] = data_in[inp_lane].Pop();
 *       T(2) << "data_in["   << inp_lane << "] = " << data_in_reg[inp_lane] << EndT;
 *     }
 *     ...
 *   }
 * }
 *
 * \endcode
 *
 */

class Module : public sc_module, public nvhls_message {
 public:
  // Interface in/out
  sc_in_clk clk;
  sc_in<bool> rst;
#ifndef __SYNTHESIS__
  sc_attr_base* module_indicator;
#endif

  Module() : sc_module(sc_gen_unique_name("module")), clk("clk"), rst("rst") {
#ifndef __SYNTHESIS__
    module_indicator = new sc_attr_base("match_module");
    this->add_attribute(*module_indicator);
#endif
  }
  Module(sc_module_name nm) : sc_module(nm), clk("clk"), rst("rst") {
#ifndef __SYNTHESIS__
    module_indicator = new sc_attr_base("match_module");
    this->add_attribute(*module_indicator);
#endif
  }

  ~Module() {
#ifndef __SYNTHESIS__
    delete module_indicator;
#endif
  }

 protected:

#ifndef __SYNTHESIS__
  /* A map to store simulation stats. Not instantiated for synthesis. */
  std::map<std::string, uint64> stats_;
#endif
  Tracer tracer_;
  Flusher EndT;  // Note: this variable is excused from following naming
                 // conventions.
#ifndef __SYNTHESIS__


  bool IsModule(sc_object* obj) {
    if (!obj)
      return false;
    else {
       if(!obj->kind() || !obj->name()) 
         return false;
       else {
#ifndef __SYNTHESIS__
         return (obj->get_attribute("match_module") != 0);
#else
         std::string objName = obj->name();
         return ((strncmp(obj->kind(), "sc_module", 9) == 0) &&
                  (objName.find("dummyPortManager") == std::string::npos ) &&
                  (objName.find("rocket") == std::string::npos ) &&
                  (objName.find("router") == std::string::npos )
                  );
#endif
       }
    }
  }
  /* Return a vector of children Modules. A wrapper of get_child_objects(). */
  std::vector<Module*> GetChildren() {
    std::vector<sc_object*> children = get_child_objects();
    std::vector<Module*> result;

    if(children.size() != 0) {
      for (unsigned i = 0; i < children.size(); i++) {
        if (IsModule(children[i])) {
          result.push_back((Module*)(children[i]));
        }
      }
    }
    
    return result;
  }

#endif

  void IncrStat(const std::string& name, unsigned int num = 1) {
#ifndef __SYNTHESIS__
    stats_[name] = stats_[name] + num;
#endif
  }

  void IncrStatIndexed(const std::string& name, unsigned int idx,
                       unsigned int num = 1) {
#ifndef __SYNTHESIS__
    std::stringstream final_name;
    final_name << name << "_" << idx;
    stats_[final_name.str()] = stats_[final_name.str()] + num;
#endif
  }

  Tracer& T(int l = 0) {
#ifdef NOPRINT
    l = 10;
#endif
#ifndef __SYNTHESIS__
    tracer_.SetCurrentLevel(l);
    tracer_ << sc_time_stamp() << ": ";
    if (get_parent() != NULL) {
      if (get_parent()->get_parent() != NULL) {
        if (get_parent()->get_parent()->get_parent() != NULL) {
          if (get_parent()->get_parent()->get_parent()->get_parent() != NULL) {
            if (get_parent()->get_parent()->get_parent()->get_parent()->get_parent() != NULL) {
              if (strncmp(get_parent()->get_parent()->get_parent()->get_parent()->get_parent()->basename(), "", 1)) {
                tracer_ << get_parent()->get_parent()->get_parent()->get_parent()->get_parent()->basename() << ".";
              }
            }

            if (strncmp(get_parent()->get_parent()->get_parent()->get_parent()->basename(), "", 1)) {
              tracer_ << get_parent()->get_parent()->get_parent()->get_parent()->basename() << ".";
            }
          }

          if (strncmp(get_parent()->get_parent()->get_parent()->basename(), "", 1)) {
            tracer_ << get_parent()->get_parent()->get_parent()->basename() << ".";
          }
        }

        if (strncmp(get_parent()->get_parent()->basename(), "", 1)) {
          tracer_ << get_parent()->get_parent()->basename() << ".";
        }
      }

      if (strncmp(get_parent()->basename(), "", 1)) {
        tracer_ << get_parent()->basename() << ".";
      }
    }
    if (strncmp(basename(), "", 1)) {
      tracer_ << basename() << ": ";
    }
#endif
    return tracer_;
  }
  
  Tracer& ASSERT(bool cond) {
#ifndef __SYNTHESIS__
    if (!cond) {
      // Guarantee a print-out.
      tracer_.SetCurrentLevel(tracer_.GetTraceLevel());
      tracer_.SetFatal();
    } else {
      // Guarantee it won't print out.
      tracer_.SetCurrentLevel(tracer_.GetTraceLevel() + 1);
    }
    tracer_ << "ASSERTION FAILURE: ";
    tracer_ << sc_time_stamp() << ": ";
    if (strncmp(name(), "", 1)) {
      tracer_ << name() << ": ";
    }
#endif
    return tracer_;
  }

 public:
  void SetTraceLevel(int l) {
#ifndef __SYNTHESIS__
    tracer_.SetTraceLevel(l);
    std::vector<Module*> children = GetChildren();
    for (unsigned int i = 0; i < children.size(); i++) {
      children[i]->SetTraceLevel(l);
    }
#endif
  }

 protected:
  void RecordEvent(const std::string& name, uint64 param = 0) {
// Param is ignored fore now.
#ifndef __SYNTHESIS__
    IncrStat(name);
#endif
  }

  void PrintStats(std::ostream& ofile, unsigned int lvl, Module* aggregator) {
#ifndef __SYNTHESIS__
      for (std::map<std::string, uint64>::iterator it = stats_.begin();
           it != stats_.end(); it++) {
        Indent(ofile, lvl);
        ofile << it->first << ": " << it->second << std::endl;
        // Add stat into subtotal if necessary.
        if (aggregator)
          aggregator->IncrStat(it->first, it->second);
      }
#endif
  }
  void Indent(std::ostream& ofile, unsigned int lvl) {
#ifndef __SYNTHESIS__
    for (unsigned int x = 0; x < 2 * lvl; x++) {
      ofile << " ";
    }
#endif
  }

 public:
  bool HasStats() {
#ifndef __SYNTHESIS__
    return (stats_.size() != 0);
#else
    return false;
#endif
  }
  bool ChildrenHaveStats() {
    bool has_stats = false;
#ifndef __SYNTHESIS__
    std::vector<Module*> children = GetChildren();
    if(children.size() != 0) {
        for (unsigned int x = 0; x < children.size(); x++) {
          has_stats |= children[x]->HasStats();
          if(!has_stats) {
            has_stats |= children[x]->ChildrenHaveStats();
          }
      }
    }
#endif
    return has_stats;
  }
  void DumpStats(std::ostream& ofile, unsigned int lvl, Module* aggregator) {
#ifndef __SYNTHESIS__
    if (!HasStats() && !ChildrenHaveStats()) {
      return;
    }

    Indent(ofile, lvl);
    ofile << name() << ":" << std::endl;
    Module* final_aggregator = ChildrenHaveStats() ? NULL : aggregator;
    if(HasStats())
      PrintStats(ofile, lvl + 1, final_aggregator);

    std::vector<Module*> children = GetChildren();

    if (ChildrenHaveStats()) {

      for (unsigned int x = 0; x < children.size(); x++) {
        children[x]->DumpStats(ofile, lvl + 1, this);
      }

      if (lvl == 0) {
        Indent(ofile, 0);
        ofile << "Totals:" << std::endl;
        PrintStats(ofile, 1, aggregator);
      } else {
        Indent(ofile, lvl + 1);
        ofile << "Sub-totals (" << name() << "):" << std::endl;
        PrintStats(ofile, lvl + 2, aggregator);
      }
    }
#endif
  }
  static const unsigned int width = 0;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
  }
};  // class Module

}  // namespace match

#endif  // MATCH_MODULE_H
