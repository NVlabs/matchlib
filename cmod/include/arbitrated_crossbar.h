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
#ifndef ARBITRATED_CROSSBAR_H
#define ARBITRATED_CROSSBAR_H

#include <nvhls_types.h>
#include <fifo.h>
#include <Arbiter.h>
#include <one_hot_to_bin.h>
#include <hls_globals.h>
#include <nvhls_marshaller.h>
#include <nvhls_message.h>

// Need to add in virtual output queueing at the input. Can replace the input
// FIFOs with a Queue class which internally has multiple parallel FIFOs and
// keeps track of where to put each new input
/**
 * \brief Crossbar with conflict arbitration and input queuing 
 * \ingroup ArbitratedCrossbar
 *
 * \tparam DataType         DataType if input and output 
 * \tparam NumInputs        Number of Inputs 
 * \tparam NumOutputs       Number of Outputs 
 * \tparam LenInputBuffer   Length of Input Buffer 
 * \tparam LenOutputBuffer  Length of Output Buffer 
 *
 * \par A Simple Example
 * \code
 *      #include <arbitrated_crossbar.h>
 *
 *      ...
 *      ArbitratedCrossbar<DataType, NumInputs, NumOutputs, InputQueueLen, OutputQueueLen> xbar;
 *      xbar.reset(); // optional in this case, suitable if xbar is a class member rather than local variable
 *      DataType data_in[NumInputs];    // Array of inputs containing data
 *      OutputIdx dest_in[NumInputs];   // Array of inputs containing destination information for each input
 *      bool valid_in[NumInputs];       // Array of inputs indicating if the input is valid
 *      DataType data_out[NumOutputs];  // Array of outputs containing data
 *      bool valid_out[NumOutputs];     // Array of outputs indicating if the output is valid
 *      bool ready[NumInputs];          // Array of outputs indicating if an input was ready. This also indicates of the input was successfully received by arbitrated Xbar
 *      InputIdx source[NumOutputs];    // Source id of input forwarded to each output
 *      ...
 *      xbar.run(bank_req, bank_sel, bank_req_valid, bank_req_winner,
                     bank_req_winner_valid, input_ready, source);
 *      ...
 *
 * \endcode
 * \par
 *
 */

template <typename DataType, unsigned int NumInputs, unsigned int NumOutputs,
          unsigned int LenInputBuffer, unsigned int LenOutputBuffer>
class ArbitratedCrossbar {

 public:
  // Define int types for input and output indices
  static const int log2_inputs = nvhls::index_width<NumInputs>::val;
  static const int log2_outputs = nvhls::index_width<NumOutputs>::val;

  typedef NVUINTW(log2_inputs) InputIdx;
  typedef NVUINTW(log2_outputs) OutputIdx;
  #ifdef SKIP_LV2TYPE
  typedef NVUINTW(Wrapped<DataType>::width + log2_outputs) DataDestType;
  #endif

 private:
  // Convenience class which stores a data and destination
  // This is what is stored in the input FIFOs
  class DataDest : public nvhls_message {
   public:
    DataType data;
    OutputIdx dest;
    static const int width = Wrapped<DataType>::width + log2_outputs;
    #ifdef SKIP_LV2TYPE
    DataDestType data_dest;

    void update_data_dest() {
      data_dest = static_cast<DataDestType> (data) + (static_cast<DataDestType> (dest) << Wrapped<DataType>::width);
    }

    void extract_data_dest() {
      data = static_cast<DataType>  (data_dest);
      dest = static_cast<OutputIdx> (data_dest >> Wrapped<DataType>::width);
    }
    #endif
    template <unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
    #ifdef SKIP_LV2TYPE
      m& data_dest;
    #else
      m& data;
      m& dest;
    #endif
    }
  };

  // Input + output FIFOs, arbiters
  #ifndef SKIP_LV2TYPE
  FIFO<DataDest, LenInputBuffer, NumInputs> input_queues;
  #else
  FIFO<DataDestType, LenInputBuffer,  NumInputs> input_queues;
  #endif
  FIFO<DataType, LenOutputBuffer, NumOutputs> output_queues;

  Arbiter<NumInputs> arbiters[NumOutputs];

 public:
  ArbitratedCrossbar() { reset(); }

  // reset function: reset all the queues
  void reset() {
#pragma hls_unroll yes
    for (unsigned in = 0; in < NumInputs; in++) {
      input_queues.reset();
    }
#pragma hls_unroll yes
    for (unsigned out = 0; out < NumOutputs; out++) {
      output_queues.reset();
      arbiters[out].reset();
    }
  }

  // The next few functions report status of a given input or output lane
  bool isInputEmpty(InputIdx index) {
    NVHLS_ASSERT_MSG(index <= NumInputs, "Input_index_greater_than_number_of_inputs");
    return input_queues.isEmpty(index);
  }

  bool isOutputEmpty(OutputIdx index) {
    NVHLS_ASSERT_MSG(index <= NumOutputs, "Output_index_greater_than_number_of_outputs");
    return output_queues.isEmpty(index);
  }

  bool isInputFull(InputIdx index) {
    NVHLS_ASSERT_MSG(index <= NumInputs, "Input_index_greater_than_number_of_inputs");
    return input_queues.isFull(index);
  }

  bool isOutputFull(OutputIdx index) {
    NVHLS_ASSERT_MSG(index <= NumOutputs, "Output_index_greater_than_number_of_outputs");
    return output_queues.isFull(index);
  }

  // Add data to a specified input lane, with a specified destination lane
  void push(DataType data, InputIdx src, OutputIdx dest) {
    DataDest tmp;
    tmp.data = data;
    tmp.dest = dest;
    #ifndef SKIP_LV2TYPE
    input_queues.push(tmp, src);
    #else
    tmp.update_data_dest();
    DataDestType tmp_data_dest;
    tmp_data_dest = tmp.data_dest;
    input_queues.push(tmp_data_dest, src);
    #endif
  }

  DataType peek(OutputIdx index) { return output_queues.peek(index); }

  // Pop the data from a specified output lane
  DataType pop(OutputIdx index) { return output_queues.pop(index); }

  // Run the crossbar (not the queues)
  void xbar(DataDest input_data[NumInputs], bool input_valid[NumInputs],
            bool input_consumed[NumInputs], DataType data_out[NumOutputs],
            bool valid_out[NumOutputs], bool output_ready[NumOutputs], InputIdx source[NumOutputs]) {

    // For each input lane, read the data at the head of the queue, and store it
    // in a temporary array
    // Also check the destination requested and form a request matrix (along
    // with its transpose)
    // Doing this facilitates writing it in one dimension and reading it in
    // another
    NVUINTW(NumInputs) requests[NumOutputs];  // requests for all outputs

    NVUINTW(NumOutputs) requests_transpose[NumInputs];

#pragma hls_unroll yes
    for (unsigned in = 0; in < NumInputs; in++) {
      // The request from one input lane is just a one-hot representation of its
      // destination field
      // but should be 0 if this input lane is empty
      NVUINTW(NumOutputs) empty = input_valid[in];
      empty <<= input_data[in].dest;
      requests_transpose[in] = empty;
    }

// Form transpose request matrix
#pragma hls_unroll yes
    for (unsigned out = 0; out < NumOutputs; out++) {
#pragma hls_unroll yes
      for (unsigned in = 0; in < NumInputs; in++) {
        requests[out][in] = requests_transpose[in][out];
      }
    }

// Keep track of which input queues need to be popped
// This signal is the OR of all grant bits sent by output lanes to this
// input lane
// Need to do this so that all queues are popped once at the end in a input
// lane loop
// This removes a bottleneck in HLS where it was inferring a dependency
// among pops
#pragma hls_unroll yes
    for (unsigned in = 0; in < NumInputs; in++) {
      input_consumed[in] = false;
    }

// Loop over output lanes: run arbiter, then resolve contention
#pragma hls_unroll yes
    for (unsigned out = 0; out < NumOutputs; out++) {
      valid_out[out] = false;

      NVUINTW(NumInputs) one_hot_grant = 0;
      InputIdx source_local;

      // Stall the arbiters and the crossbar if the output is full
      // This is also needed to get any pipelining (otherwise the tool will
      // infer that you want to write in a single cycle)
      // For some reason separating these two if statements gives better results
      if (output_ready[out]) {

        // Run through the Arbiter pick() function, convert to binary
        one_hot_grant = arbiters[out].pick(requests[out]);
        one_hot_to_bin<NumInputs, log2_inputs>(one_hot_grant, source_local);
      }

// Grant logic on input queues (OR gate)
#pragma hls_unroll
      for (unsigned in = 0; in < NumInputs; in++) {
        // pop_inputs[in] = pop_inputs[in] | (one_hot_grant[in] == 1);
        input_consumed[in] = input_consumed[in] | (one_hot_grant[in] == 1);
      }

      // XBAR (using the data that was staged in the temporary array input_data)
      if ((!(one_hot_grant == 0)) && (output_ready[out])) {
        data_out[out] = input_data[source_local].data;
        valid_out[out] = true;
        source[out] = source_local;
      }
    }
  }  // end xbar() function

 public:
  bool isAllInputEmpty() {
    bool fifo_empty_internal[NumInputs + 1];
    fifo_empty_internal[0] = true;
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumInputs; i++) {
      fifo_empty_internal[i + 1] = (isInputEmpty(i)) & fifo_empty_internal[i];
    }
    return fifo_empty_internal[NumInputs];
  }

  bool isAllOutputEmpty() {
    bool fifo_empty_internal[NumOutputs + 1];
    fifo_empty_internal[0] = true;
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumOutputs; i++) {
      fifo_empty_internal[i + 1] = (isOutputEmpty(i)) & fifo_empty_internal[i];
    }
    return fifo_empty_internal[NumOutputs];
  }

  bool isAllInputReady() {
    bool fifo_ready_internal[NumInputs + 1];
    fifo_ready_internal[0] = true;
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumInputs; i++) {
      fifo_ready_internal[i + 1] = (!isInputFull(i)) & fifo_ready_internal[i];
    }
    return fifo_ready_internal[NumInputs];
  }

  // Pop the data from all selected output lanes, data is already got from peek
  void pop_all_lanes(bool valid_out[NumOutputs]) {
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumOutputs; i++) {
      if (valid_out[i]) {
        output_queues.pop(i);
      }
    }
    return;
  }
/**
 * \brief Top-Level function for Arbitrated Crossbar 
 * \ingroup ArbitratedCrossbar
 *
 */
  // Top-Level function for Arbitrated Crossbar that returns source
  // Interface description
  // data_in - Array of inputs containing data
  // dest_in - Array of inputs containing destination information for each input
  // valid_in - Array of inputs indicating if the input is valid
  // data_out - Array of outputs containing data
  // valid_out - Array of outputs indicating if the output is valid
  // ready - Array of outputs indicating if an input was ready. This also
  // indicates of the input was successfully received by arbitrated Xbar
  void run(DataType data_in[NumInputs], OutputIdx dest_in[NumInputs],
           bool valid_in[NumInputs], DataType data_out[NumOutputs],
           bool valid_out[NumOutputs], bool ready[NumInputs], InputIdx source[NumOutputs]
    ) {
    // Need to read data into temporary variables to avoid scheduling problem in
    // Catapult
    OutputIdx destin_tmp[NumInputs];
    bool valid_in_tmp[NumInputs];

#pragma hls_unroll yes
    for (unsigned in = 0; in < NumInputs; in++) {
      destin_tmp[in] = dest_in[in];
      valid_in_tmp[in] = valid_in[in];
    }

    DataDest input_data[NumInputs];
    bool input_valid[NumInputs];
    bool input_consumed[NumInputs];
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumInputs; i++) {
      input_data[i] = BitsToType<DataDest>(0);
    }
    DataType output_data[NumOutputs];
    bool output_valid[NumOutputs];
    bool output_ready[NumOutputs];
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumOutputs; i++) {
      output_data[i] = BitsToType<DataType>(0);
    }

    if (LenInputBuffer > 0) {
// If lane is ready and input data is valid, write to it
#pragma hls_unroll yes
      for (unsigned in = 0; in < NumInputs; in++) {
        ready[in] = !isInputFull(in) || !valid_in_tmp[in];
        if (!isInputFull(in) & valid_in_tmp[in]) {
          // push(datain_tmp[in], in, destin_tmp[in]);
          push(data_in[in], in, destin_tmp[in]);
        }
        input_valid[in] = !isInputEmpty(in);
        if (input_valid[in]) {
	  #ifndef SKIP_LV2TYPE
          input_data[in] = input_queues.peek(in);
	  #else
	  input_data[in].data_dest = input_queues.peek(in);
	  input_data[in].extract_data_dest();
	  #endif
        }
      }

    } else {
#pragma hls_unroll yes
      for (unsigned in = 0; in < NumInputs; in++) {
        input_data[in].data = data_in[in];
        input_data[in].dest = dest_in[in];
        input_valid[in] = valid_in_tmp[in];
      }
    }
	for (unsigned in = 0; in < NumInputs; in++) {
      //DCOUT("DUT - Input: " << in << "\t valid: " << input_valid[in] << "\t dest: " << input_data[in].dest << "\t data: " << input_data[in].data << endl);
	}
    if (LenOutputBuffer > 0) {
#pragma hls_unroll yes
      for (unsigned out = 0; out < NumOutputs; out++) {
        output_ready[out] = !isOutputFull(out);
      }
    } else {
#pragma hls_unroll yes
      for (unsigned out = 0; out < NumOutputs; out++) {
        output_ready[out] = true;
      }
    }

    // Process the XBAR and arbiters
    xbar(input_data, input_valid, input_consumed, output_data, output_valid,
         output_ready, source);
	for (unsigned out = 0; out < NumOutputs; out++) {
      //DCOUT("DUT - Output: " << out << "\t valid: " << output_valid[out] << "\t data: " << output_data[out] << "\tReady: " << output_ready[out] << endl);
	}

    if (LenInputBuffer > 0) {
// Increment the head pointer of the input queues if their requests were granted
// Do this in a single loop over input lanes so it's clear to the tool that each
// input channel
// is only popped once
#pragma hls_unroll yes
      for (unsigned in = 0; in < NumInputs; in++) {
        if (input_consumed[in]) {
          input_queues.incrHead(in);
        }
      }
    } else {
#pragma hls_unroll yes
      for (unsigned in = 0; in < NumInputs; in++) {
        ready[in] = input_consumed[in];
      }
    }

    if (LenOutputBuffer > 0) {
// Read from each output channel if it is not empty
#pragma hls_unroll yes
      for (unsigned out = 0; out < NumOutputs; out++) {
        if (output_valid[out]) {
          output_queues.push(output_data[out], out);
        }
        valid_out[out] = !isOutputEmpty(out);
        if (!isOutputEmpty(out)) {
          data_out[out] = peek(out);
        }
        /*peek only
        if (!isOutputEmpty(out)) {
            data_out[out] = pop(out);
        }
        */
      }
	  for (unsigned out = 0; out < NumOutputs; out++) {
        //DCOUT("DUT - ArbXbar Output: " << out << "\t valid: " << valid_out[out] << "\t data: " << data_out[out] << endl);
	  }
    } else {
#pragma hls_unroll yes
      for (unsigned out = 0; out < NumOutputs; out++) {
        data_out[out] = output_data[out];
        valid_out[out] = output_valid[out];
      }
    }
  }  // end run() function
/**
 * \brief Top-Level function for Arbitrated Crossbar that does not return source 
 * \ingroup ArbitratedCrossbar
 *
 */
  void run(DataType data_in[NumInputs], OutputIdx dest_in[NumInputs],
           bool valid_in[NumInputs], DataType data_out[NumOutputs],
           bool valid_out[NumOutputs], bool ready[NumInputs]) {
    InputIdx source[NumOutputs];
    run(data_in, dest_in,
           valid_in, data_out,
           valid_out, ready, source);
  } // end run() function


};  // end ArbitratedCrossbar class

#endif  // end ARBITRATED_CROSSBAR_H
