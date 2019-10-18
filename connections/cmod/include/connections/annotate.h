/*
 * Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
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
//========================================================================
// annotate.h
//========================================================================


#ifndef __CONNECTIONS__ANNOTATE_H__
#define __CONNECTIONS__ANNOTATE_H__

#include <iostream>
#include <string>

#include "connections.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>

namespace Connections {

#ifdef CONNECTIONS_ACCURATE_SIM
  
  void __annotate_vector(std::vector<Connections::Connections_BA_abs*> &v, const std::string &root_name, rapidjson::Document &d) {
    // Check if array exists, if not add it.
    if(! d.HasMember("channels")) {
      rapidjson::Value v_channels;
      // v_channels.SetArray();
      v_channels.SetObject();
      d.AddMember("channels", v_channels, d.GetAllocator());
    }
    rapidjson::Value &v_channels = d["channels"];
    
    // Annotate
    for ( std::vector<Connections::Connections_BA_abs*>::iterator it=v.begin(); it!=v.end(); ++it ) {
      // Determine name after removing root_name
      std::string it_name = (*it)->name();
      std::size_t pos = it_name.find(root_name);
      if(pos == std::string::npos) { continue; } // Skip if doesn't match root_name
      it_name.erase(pos, root_name.length());

      // Check if this channel exists in DOM, if not add it.
      if(! v_channels.HasMember(it_name.c_str())) {
	rapidjson::Value v_name;
	v_name.SetString(it_name.c_str(), d.GetAllocator());
	rapidjson::Value v_channel;
	v_channel.SetObject();
	v_channels.AddMember(v_name, v_channel, d.GetAllocator());
      }
      rapidjson::Value &v_channel = v_channels[it_name.c_str()];

      // Check if latency exists, if not add it
      if(! v_channel.HasMember("latency")) {
	rapidjson::Value v_latency;
	v_latency.SetInt(0);
	v_channel.AddMember("latency", v_latency, d.GetAllocator());
      }
      rapidjson::Value &v_latency = v_channel["latency"];
      
      // Check if capacity exists, if not add it
      if(! v_channel.HasMember("capacity")) {
	rapidjson::Value v_capacity;
	v_capacity.SetInt(0);
	v_channel.AddMember("capacity", v_capacity, d.GetAllocator());
      }
      rapidjson::Value &v_capacity = v_channel["capacity"];

      // Get the src_name and dest_name after subtracting off root
      std::string src_name = (*it)->src_name();
      {
	if(src_name.substr(src_name.length() - 4,4) == "_val") { src_name.erase(src_name.length() - 4,4); }
	std::size_t pos = src_name.find(root_name);
	if(pos != std::string::npos) { src_name.erase(pos, root_name.length()); }
      }
      
      std::string dest_name = (*it)->dest_name();
      {
	if(dest_name.substr(dest_name.length() - 4,4) == "_val") { dest_name.erase(dest_name.length() - 4,4); }
	std::size_t pos = dest_name.find(root_name);
	if(pos != std::string::npos) { dest_name.erase(pos, root_name.length()); }
      }
      
      // Add the net driver/receiver, always an output never an input.
      if(v_channel.HasMember("src_name")) {
	if(strcmp(v_channel["src_name"].GetString(),src_name.c_str()) != 0) {
	  cout << "Error: During annotation src_name in input doesn't match real src_name (" << v_channel["src_name"].GetString() << " != " << src_name.c_str() << ")" << endl;
	  assert(0);
	}
      } else {
	rapidjson::Value v_src_name;
	v_src_name.SetString(src_name.c_str(), d.GetAllocator());
	v_channel.AddMember("src_name", v_src_name, d.GetAllocator());
      }
      
      if(v_channel.HasMember("dest_name")) {
	if(strcmp(v_channel["dest_name"].GetString(),dest_name.c_str()) != 0) {
	  cout << "Error: During annotation dest_name in input doesn't match real dest_name (" << v_channel["dest_name"].GetString() << " != " << dest_name.c_str() << ")" << endl;
	  assert(0);
	}
      } else {
	rapidjson::Value v_dest_name;
	v_dest_name.SetString(dest_name.c_str(), d.GetAllocator());
	v_channel.AddMember("dest_name", v_dest_name, d.GetAllocator());
      }
      
      // Annotate based on the value.
      assert(v_latency.IsInt());
      assert(v_capacity.IsInt());
      (*it)->annotate(v_latency.GetInt(), v_capacity.GetInt());
    }
  }

  void annotate_design(const sc_object &root, std::string base_name = "", std::string input_dir_path = "", std::string output_dir_path = "") {
    bool explicit_input_dir=false;
    // If empty basename, set it to name() of object...
    if(base_name.length() == 0) { base_name = root.name(); }
    
    // Add delim if non-empty base_name
    if(base_name.length() > 0) { base_name += "."; }

    // Sanity check input and output paths if they exist
    if(input_dir_path.length() > 0) {
      input_dir_path += "/";
      explicit_input_dir = true;
    }
    if(output_dir_path.length() > 0) {
      output_dir_path += "/";
    }
    
    std::string input_path  = input_dir_path + base_name + "input.json";
    std::string output_path = output_dir_path + base_name + "output.json";

    // Create DOM object.
    rapidjson::Document d;

    // Try reading document from input.json
    std::ifstream ifs(input_path.c_str());
    if(! ifs.fail()) {
      CONNECTIONS_COUT("Info: Reading back-annotation from " << input_path << ". Please diff " << input_path << " and " << output_path << " to ensure it matches expectations." << endl);
      rapidjson::IStreamWrapper isw(ifs);
      d.ParseStream(isw);
    } else {
      if(explicit_input_dir) {
	CONNECTIONS_ASSERT_MSG(0, ("Warning: Could not read input json " + input_path).c_str());
      } else {
	CONNECTIONS_COUT("Warning: Could not read input json " << input_path.c_str() << endl);
      }
      d.SetObject();
    }
    
    std::string root_name = std::string(root.name()) + ".";
    assert(root_name.length() > 1); // Should be something other than just "."

    __annotate_vector(Connections::get_conManager().tracked_annotate, root_name, d);
    
    // Output DOM to file
    std::ofstream ofs(output_path.c_str());
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    d.Accept(writer);
  }

#else

/**
 * \brief Back annotate Combinational connections for a design
 * \ingroup Connections
 *
 * \tparam root          Root sc_object to annotate.
 * \tparam base_name     Base file name (optional, defaults to name of root within hierarchy)
 * \tparam base_name     Input directory (optional, defaults to current working directory)
 * \tparam base_name     Output directory (optional, defaults to current working directory)
 *
 * \par Description
 *      When a design is annotated, it writes out a base_name.output.json with all annotatable
 *      Combinational LI channels, defaulting to a latency and capacity of 0 for each (simple
 *      wire). base_name.output.json can be used as a template for base_name.input.json, which
 *      it will read in and annotate based on given latency and capacity. Latency is given in
 *      units of global clock cycles, while capacity is number of units that can be stored in
 *      the channel. For a retimed path, latency and capacity should be equal, while for
 *      buffered paths capacity will exceed latency.
 *      
 *      Back annotation will only work with CONNECTIONS_ACCURATE_SIM define enabled, and
 *      in MARSHALL_PORT or DIRECT_PORT modes. It will not work in TLM_PORT (CONNECTIONS_FAST_SIM)
 *      or SYN_PORT modes. Additionally, it is dependent on RapidJSON as a git submodule to read
 *      and write JSON file format.
 *
 *      In the base_name.output.json, a list of combinational names and connecting ports is given.
 *      UNBOUND indicates a Combinational channel that hasn't been Bind()'d on one or more ports,
 *      while TLM_INTERFACE indicates a channel that is along a TLM_PORT interface in cosimulation
 *      cases.
 *
 * \par A Simple Example
 * \code
 *      #include <connections/connections.h>
 *      #include <connections/annotate.h>
 *
 *      ...
 *      // Add to sc_main, to annotate testbench DUT.
 *      // Always generates a my_testbench.dut.output.json of the annotated design
 *      // and tries to read in my_testbench.dut.input.json if it exists.
 *      annotate_design(my_testbench.dut);
 *      ...
 * \endcode
 * \par
 *
 */
  void annotate_design(const sc_object &root, std::string base_name = "", std::string input_dir_path = "", std::string output_dir_path = "") {
    cerr << "WARNING: Cannot annotate_design() unless running in sim accurate mode!" << endl;
    cerr << "WARNING: Design will not be annotated." << endl;
  }

#endif
};

#endif // __CONNECTIONS__ANNOTATE_H__
