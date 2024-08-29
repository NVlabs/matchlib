/*
 * Copyright (c) 2024, NVIDIA CORPORATION.  All rights reserved.
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

#include "ScratchpadClassTop.h"
#include <mc_scverify.h>

class Top : public sc_module
{
 public:
  CCS_DESIGN(ScratchpadClassTop) CCS_INIT_S1(dut1);

  sc_clock clk;
  SC_SIG(bool, rst_bar);
  local_mem::word_type*  ref_mem;
  bool* ref_mem_valid;
  unsigned match_count{0};
  unsigned mismatch_count{0};

  Connections::Combinational<local_mem::word_type>  CCS_INIT_S1(out1);
  Connections::Combinational<dut_in_t>          CCS_INIT_S1(in1);

  SC_CTOR(Top)
    :   clk("clk", 1, SC_NS, 0.5,0,SC_NS,true) {
    sc_object_tracer<sc_clock> trace_clk(clk);

    ref_mem = new local_mem::word_type[local_mem::capacity_in_words];
    ref_mem_valid = new bool[local_mem::capacity_in_words];

    for (unsigned i=0; i < local_mem::capacity_in_words; i++)
      ref_mem_valid[i] = 0;

    dut1.clk(clk);
    dut1.rst_bar(rst_bar);
    dut1.out1(out1);
    dut1.in1(in1);

    SC_CTHREAD(reset, clk);

    SC_THREAD(stim);
    sensitive << clk.posedge_event();
    async_reset_signal_is(rst_bar, false);

    SC_THREAD(resp);
    sensitive << clk.posedge_event();
    async_reset_signal_is(rst_bar, false);
  }

  struct my_fifo_t : public tlm::tlm_fifo<uint64> {
    my_fifo_t() : tlm_fifo(32) {}
  };

  my_fifo_t read_fifo1;
  static const int test_count = 1000;


  void stim() {
    CCS_LOG("Stimulus started");
    in1.ResetWrite();
    wait();

    dut_in_t dut_in1;

    CCS_LOG("addr_width bank_sel_width capacity_in_words: " << std::dec << local_mem::addr_width << " " << local_mem::bank_sel_width << " " << local_mem::capacity_in_words);

    // Write random coefficient values to ascending memory locations:
    unsigned addr = 0;
    for (unsigned r = 0; r < local_mem::words_per_bank; r++) {
      dut_in1.is_load = 0;
      dut_in1.addr = addr;
      for (unsigned i=0; i < local_mem::num_inputs; i++) {
        dut_in1.data[i] = rand();
        ++addr;
        ref_mem_valid[addr] = 1;
        ref_mem[addr] = dut_in1.data[i];
      }
      in1.Push(dut_in1);
    }

    // Do multiply-accumulate operations using coefficients in scratchpad and input data 
    // weights sent here.
    // starting address for the coefficient reads is randomly selected here
    for (unsigned r = 0; r < test_count; r++) {
      unsigned addr;
      while (1) {
        addr = rand() &  (( 1 << local_mem::addr_width ) - 1);
        if (addr < local_mem::capacity_in_words - local_mem::num_inputs)
          break;
      }
      dut_in1.is_load = 1;
      dut_in1.addr = addr;

      // compute MAC for ref model checking here:
      unsigned sum = 0;
      for (unsigned i=0; i < local_mem::num_inputs; i++) {
        dut_in1.data[i] = rand();
        ++addr;
        sum += dut_in1.data[i] * ref_mem[addr];
      }

      in1.Push(dut_in1);
      read_fifo1.put(sum);
    }

    wait(100);
    CCS_LOG("MATCH    COUNT: " << std::dec << match_count);
    CCS_LOG("MISMATCH COUNT: " << std::dec << mismatch_count);
    sc_stop();
  }

  void resp() {
    out1.ResetRead();
    wait();

    while (1) {
      for (unsigned i=0; i < test_count; i++) {
          uint64 ref = read_fifo1.get();
          local_mem::word_type rsp = out1.Pop();
          if (ref != rsp) {
            ++mismatch_count;
          }
          else {
            ++match_count;
          }
      }
    }
  }

  void reset() {
    rst_bar.write(0);
    wait(5);
    rst_bar.write(1);
    wait();
  }
};

int sc_main(int argc, char **argv)
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  sc_report_handler::set_actions(SC_ERROR, SC_DISPLAY);
  //sc_trace_file *trace_file_ptr = sc_trace_static::setup_trace_file("trace");

  Top top("top");
  //trace_hierarchy(&top, trace_file_ptr);
  sc_start();
  if (sc_report_handler::get_count(SC_ERROR) > 0) {
    std::cout << "Simulation FAILED" << std::endl;
    return -1;
  }
  std::cout << "Simulation PASSED" << std::endl;
  return 0;
}

