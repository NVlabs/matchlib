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
#ifndef FIFO_H
#define FIFO_H

#include <nvhls_types.h>
#include <mem_array.h>
#include <nvhls_assert.h>
/**
 * \brief Configurable FIFO class 
 * \ingroup FIFO
 *
 * \tparam DataType         DataType of entry in FIFO 
 * \tparam FifoLen          Length of FIFO 
 * \tparam NumBanks         Number of FIFO banks 
 *
 *
 * \par A Simple Example
 * \code
 *      #include <fifo.h>
 *
 *      ...
 *      FIFO<DataType, LenInputBuffer, NumInputs> fifo_inst;
 *      
 *      fifo_inst.reset(); // optional in this case, suitable if fifo is a class member rather than local variable
 *
 *      ...
 *      // Push to FIFO
 *      if (!fifo_inst.isFull(bank_id)) {
 *        fifo_inst.push(input_data, bank_id);
 *      }
 *      ...
 *      // Pop from FIFO
 *      if (!fifo_inst.isEmpty(bank_id)) {
 *        output_data = fifo_inst.pop(bank_id);
 *      }
 *      ...
 *      // Peek from FIFO
 *      if (!fifo_inst.isEmpty(bank_id)) {
 *        output_data = fifo_inst.peek(bank_id);
 *      }
 *      ...
 *      // IncrHead without reading data from FIFO
 *      fifo_inst.incrHead(bank_id);
 *
 * \endcode
 * \par
 *
 */

template <typename DataType, unsigned int FifoLen, unsigned int NumBanks = 1>
class FIFO {

 public:
  static const int BankSelWidth =
      (NumBanks == 1) ? 1 : nvhls::nbits<NumBanks - 1>::val;
  static const int AddrWidth =
      (FifoLen == 1) ? 1 : nvhls::nbits<FifoLen - 1>::val;
  typedef NVUINTW(BankSelWidth) BankIdx;
  typedef NVUINTW(AddrWidth) FifoIdx;
  typedef NVUINTW(AddrWidth+1) FifoIdxPlusOne;

  FifoIdx head[NumBanks];  // where to read from
  FifoIdx tail[NumBanks];  // where to write to
  // FifoLen is number of entries in each bank
  mem_array_sep<DataType, FifoLen * NumBanks, NumBanks> fifo_body;
  bool last_action_was_push[NumBanks];
  static const int width =  mem_array_sep<DataType, FifoLen * NumBanks, NumBanks>::width + 2 * NumBanks * AddrWidth + NumBanks; 

  // Function to do modulo increment of pointer
  FifoIdx ModIncr(FifoIdx curr_idx) {
    if (curr_idx == FifoLen - 1) {
      return 0;
    }
    return curr_idx + 1;
  }

  // Constructor
  FIFO() {
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumBanks; i++) {
      head[i] = 0;
      tail[i] = 0;
      last_action_was_push[i] = false;
    }
  }

  // Function to push data to FIFO
  void push(DataType wr_data, BankIdx bidx = 0) {
    NVHLS_ASSERT_MSG(!isFull(bidx), "Pushing_data_to_full_FIFO");
    FifoIdx tail_local = tail[bidx];
    fifo_body.write(tail_local, bidx, wr_data);
    tail[bidx] = ModIncr(tail_local);
    last_action_was_push[bidx] = true;
  }

  // Function to pop data from FIFO
  DataType pop(BankIdx bidx = 0) {
    NVHLS_ASSERT_MSG(!isEmpty(bidx), "Popping_data_from_empty_FIFO");
    FifoIdx head_local = head[bidx];
    DataType rd_data = fifo_body.read(head_local, bidx);
    head[bidx] = ModIncr(head_local);
    last_action_was_push[bidx] = false;
    return rd_data;
  }

  // Function to increment the head pointer (emulate a pop)
  void incrHead(BankIdx bidx = 0) {
    NVHLS_ASSERT_MSG(!isEmpty(bidx), "Incrementing_Head_of_empty_FIFO");
    FifoIdx head_local = head[bidx];
    head[bidx] = ModIncr(head_local);
    last_action_was_push[bidx] = false;
  }

  // Function to peek from FIFO
  DataType peek(BankIdx bidx = 0) {
    NVHLS_ASSERT_MSG(!isEmpty(bidx), "Peeking_data_from_empty_FIFO");
    FifoIdx head_local = head[bidx];
    return fifo_body.read(head_local, bidx);
  }

  // Checks if FIFO is empty
  bool isEmpty(BankIdx bidx = 0) {
    FifoIdx head_local = head[bidx];
    FifoIdx tail_local = tail[bidx];
    return (tail_local == head_local) && (!last_action_was_push[bidx]);
  }

  // Checks if FIFO is full
  bool isFull(BankIdx bidx = 0) {
    FifoIdx head_local = head[bidx];
    FifoIdx tail_local = tail[bidx];
    return (tail_local == head_local) && (last_action_was_push[bidx]);
  }

  // Returns number of entries filled in FIFO
  FifoIdxPlusOne NumFilled(BankIdx bidx = 0) {
    FifoIdx head_local = head[bidx];
    FifoIdx tail_local = tail[bidx];
    if (isEmpty(bidx)) {
      return 0;
    }
    if (isFull(bidx)) {
      return FifoLen;
    }
    if (head_local < tail_local) {
      return (tail_local - head_local);
    } else {
      return (FifoLen - head_local + tail_local);
    } 
  }

  // Returns number of empty entries in FIFO
  FifoIdxPlusOne NumAvailable(BankIdx bidx = 0) {
      return (FifoLen - NumFilled(bidx));
  }

  // Reset head and tail pointers
  void reset() {
#pragma hls_unroll yes
    for (unsigned i = 0; i < NumBanks; i++) {
      head[i] = 0;
      tail[i] = 0;
      last_action_was_push[i] = false;
    }
  }

  FifoIdx get_head(BankIdx bidx = 0) { return head[bidx]; }
  FifoIdx get_tail(BankIdx bidx = 0) { return tail[bidx]; }

  template<unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    for (unsigned i = 0; i < NumBanks; i++) {
      m & head[i];
      m & tail[i];
      m & last_action_was_push[i];
    }
    m & fifo_body;
  }

};  // end FIFO class

template <typename DataType, unsigned int NumBanks>
class FIFO<DataType, 0, NumBanks> {  // 0 entries, NumBanks
  // make sure no one is accessing a fifo with 0 entries ever
  // it will still be fine to create it
  static const int BankSelWidth =
      (NumBanks == 1) ? 1 : nvhls::nbits<NumBanks - 1>::val;

 public:
  typedef NVUINTW(BankSelWidth) BankIdx;
  typedef NVUINTW(1) FifoIdx;
  FIFO() {}

  void push(DataType wr_data, BankIdx bidx = 0) {NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero");}

  DataType pop(BankIdx bidx = 0) { NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); return DataType(); }

  void incrHead(BankIdx bidx = 0) {NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); }

  DataType peek(BankIdx bidx = 0) { NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); return DataType(); }

  bool isEmpty(BankIdx bidx = 0) {NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); return true; }

  bool isFull(BankIdx bidx = 0) {NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); return true; }

  FifoIdx NumFilled(BankIdx bidx = 0) {NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); return 0; }

  FifoIdx NumAvailable(BankIdx bidx = 0) {NVHLS_ASSERT_MSG(0, "FIFO_size_is_zero"); return 0; }

  void reset() {}
};

/**
 * \brief Specialization for single entry and single bank for QoR optimization 
 * \ingroup FIFO
 *
 * \tparam DataType         DataType of entry in FIFO 
 *
 */
template <typename DataType>
class FIFO<DataType, 1, 1> {  
    DataType data;
    bool valid;

 public:
    static const int width =  Wrapped<DataType>::width + 1; 
    typedef NVUINTW(1) T;  //redundant

    FIFO() {reset();}

    inline void push(DataType wr_data, T bidx = 0) 
    {        
        NVHLS_ASSERT_MSG(!isFull(), "Pushing_data_to_full_FIFO");
        data = wr_data;
        valid = true;
    }

    inline DataType pop(T bidx = 0)
    {      
        incrHead();  
        return data;
    }

    inline void incrHead(T bidx = 0)
    {
        NVHLS_ASSERT_MSG(!isEmpty(), "Incrementing_head_of_empty_FIFO");
        valid = false;
    }

    inline DataType peek(T bidx = 0) 
    { 
        NVHLS_ASSERT_MSG(!isEmpty(), "Peeking_data_from_empty_FIFO");
        return data;
    }

    inline bool isEmpty(T bidx = 0)
    {   
        return !valid; 
    }

    inline bool isFull(T bidx = 0) 
    { 
        return valid; 
    }

    inline T NumFilled(T bidx = 0) {
        return valid; 
    }

    inline T NumAvailable(T bidx = 0) {
      return !valid;
    }

    inline void reset() 
    {
        valid = false;
    }
    template<unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      m & valid;
      m & data;
    }
};
/**
 * \brief Specialization for single entry and NumBanks for QoR optimization 
 * \ingroup FIFO
 *
 * \tparam DataType         DataType of entry in FIFO 
 * \tparam NumBanks         Number of FIFO banks 
 *
 */
template <typename DataType, unsigned int NumBanks>
class FIFO<DataType, 1, NumBanks> {  
    DataType data[NumBanks];
    bool     valid[NumBanks];
    static const int width =  NumBanks * Wrapped<DataType>::width + NumBanks; 

 public:
    static const int BankSelWidth =
      (NumBanks == 1) ? 1 : nvhls::nbits<NumBanks - 1>::val;
    typedef NVUINTW(BankSelWidth) BankIdx;
    typedef NVUINTW(1) T;

    FIFO() {
      reset();
    }

    inline void push(DataType wr_data, BankIdx bidx = 0) {        
      NVHLS_ASSERT_MSG(!isFull(bidx), "Pushing_data_to_full_FIFO");
      data[bidx]  = wr_data;
      valid[bidx] = true;
    }

    inline DataType pop(BankIdx bidx = 0) {      
      incrHead(bidx);  
      return data[bidx];
    }

    inline void incrHead(BankIdx bidx = 0) {
      NVHLS_ASSERT_MSG(!isEmpty(bidx), "Incrementing_Head_of_empty_FIFO");
      valid[bidx] = false;
    }

    inline DataType peek(BankIdx bidx = 0) { 
      NVHLS_ASSERT_MSG(!isEmpty(bidx), "Peeking_data_from_empty_FIFO");
      return data[bidx];
    }

    inline bool isEmpty(BankIdx bidx = 0) {   
      return !valid[bidx]; 
    }

    inline bool isFull(BankIdx bidx = 0) { 
      return valid[bidx]; 
    }

    inline T NumFilled(T bidx = 0) {
        return valid; 
    }

    inline T NumAvailable(T bidx = 0) {
      return !valid;
    }

    inline void reset() {
      #pragma hls_unroll yes
      for(unsigned i=0; i<NumBanks; i++) {
        valid[i] = false;
      }
    }
    template<unsigned int Size>
    void Marshall(Marshaller<Size>& m) {
      for (unsigned i = 0; i < NumBanks; i++) {
        m & valid[i];
        m & data[i];
      }
    }
};

#endif  // end #define FIFO_H macro
