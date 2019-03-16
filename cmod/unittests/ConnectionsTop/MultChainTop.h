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
//========================================================================
// MultChainTop.h
//========================================================================
// Composes two multiplier chain modules and exercises different types of
// connections.

#ifndef MULT_CHAIN_TOP_H
#define MULT_CHAIN_TOP_H

#include <systemc.h>
#include "MultChain.h"

//------------------------------------------------------------------------
// ConnectionType
//------------------------------------------------------------------------

enum ConnectionType {
  COMBINATIONAL,
  BYPASS,
  BUFFER
};

//------------------------------------------------------------------------
// MultChains
//------------------------------------------------------------------------

// Forward declaration
template<
  typename       T,
  unsigned int   N,
  ConnectionType C
> class MultChains;

// Specialize for COMBINATIONAL connection
template<
  typename       T,
  unsigned int   N
>
class MultChains<T,N,COMBINATIONAL> : public sc_module {
  SC_HAS_PROCESS(MultChains);

 public:
  // Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::In<T>  in_;
  Connections::Out<T> out;

  // Sub Modules
  MultChain<T,N> unit1;
  MultChain<T,N> unit2;

  Connections::Combinational<T> connect;

  MultChains(sc_module_name name) :
    sc_module(name),
    clk("clk"),
    rst("rst"),
    in_("in_"),
    out("out"),
    unit1(sc_gen_unique_name("mult_chain")),
    unit2(sc_gen_unique_name("mult_chain"))
  {
    unit1.clk(clk);
    unit1.rst(rst);
    unit2.clk(clk);
    unit2.rst(rst);

    unit1.in_(in_);
    unit1.out(connect);
    unit2.in_(connect);
    unit2.out(out);
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    unit1.line_trace();
    unit2.line_trace();
  }
#endif
};

// Specialize for BYPASS connection
template<
  typename       T,
  unsigned int   N
>
class MultChains<T,N,BYPASS> : public sc_module {
  SC_HAS_PROCESS(MultChains);

 public:
  // Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::In<T>  in_;
  Connections::Out<T> out;

  // Sub Modules
  MultChain<T,N> unit1;
  MultChain<T,N> unit2;

  Connections::Bypass<T>        connect;
  Connections::Combinational<T> enq_wire;
  Connections::Combinational<T> deq_wire;

  MultChains(sc_module_name name) :
    sc_module(name),
    clk("clk"),
    rst("rst"),
    in_("in_"),
    out("out"),
    unit1("unit1"),
    unit2("unit2")
  {
    unit1.clk(clk);
    unit1.rst(rst);
    unit2.clk(clk);
    unit2.rst(rst);
    connect.clk(clk);
    connect.rst(rst);

    unit1.in_(in_);
    unit1.out(enq_wire);
    connect.enq(enq_wire);
    connect.deq(deq_wire);
    unit2.in_(deq_wire);
    unit2.out(out);
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    unit1.line_trace();
    unit2.line_trace();
  }
#endif
};

// Specialize for BUFFER connection
template<
  typename       T,
  unsigned int   N
>
class MultChains<T,N,BUFFER> : public sc_module {
  SC_HAS_PROCESS(MultChains);

 public:
  // Interface
  sc_in_clk           clk;
  sc_in<bool>         rst;
  Connections::In<T>  in_;
  Connections::Out<T> out;

  // Sub Modules
  MultChain<T,N> unit1;
  MultChain<T,N> unit2;

  Connections::Buffer<T,2>      connect;
  Connections::Combinational<T> enq_wire;
  Connections::Combinational<T> deq_wire;

  MultChains(sc_module_name name) :
    sc_module(name),
    clk(sc_gen_unique_name("clk")),
    rst(sc_gen_unique_name("rst")),
    in_("in_"),
    out("out"),
    unit1("unit1"),
    unit2("unit2")
  {
    unit1.clk(clk);
    unit1.rst(rst);
    unit2.clk(clk);
    unit2.rst(rst);
    connect.clk(clk);
    connect.rst(rst);

    unit1.in_(in_);
    unit1.out(enq_wire);
    connect.enq(enq_wire);
    connect.deq(deq_wire);
    unit2.in_(deq_wire);
    unit2.out(out);
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    unit1.line_trace();
    unit2.line_trace();
  }
#endif
};

//------------------------------------------------------------------------
// MultChainTop
//------------------------------------------------------------------------

class MultChainTop : public sc_module {
  SC_HAS_PROCESS(MultChainTop);

 public:
  typedef sc_uint<32> Bits;

  // Interface
  sc_in_clk              clk;
  sc_in<bool>            rst;
  Connections::In<Bits>  in_;
  Connections::Out<Bits> out;

  // Sub Module
  MultChains<Bits,2,BUFFER> unit;

  MultChainTop(sc_module_name name) :
    sc_module(name),
    clk("clk"),
    rst("rst"),
    in_("in_"),
    out("out"),
    unit("unit")
  {
    unit.clk(clk);
    unit.rst(rst);
    unit.in_(in_);
    unit.out(out);
  }

#ifndef __SYNTHESIS__
 public:
  void line_trace() {
    unit.line_trace();
  }
#endif
};

#endif  // MULT_CHAIN_TOP_H
