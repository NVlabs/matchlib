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
#ifndef MEM_ARRAY_H
#define MEM_ARRAY_H

#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_array.h>
#include <nvhls_marshaller.h>
#include <TypeToBits.h>

// T: data type
// N: number of lines
template <typename T, int N>
class mem_array {
 public:
  T data[N];
  mem_array() {
    T value;
    for (unsigned i = 0; i < N; i++) {
#ifndef SLEC_CPC
      data[i] = value;
#endif
    }
  }
};

// T: data type
// N: number of lines
// A: associativity
template <typename T, int N, int A>
class mem_array_2d {
 public:
  T data[N / A][A];
};

template <typename T, int N, int A>
class mem_array_2d_transp {
 public:
  T data[A][N / A];
};

/**
 * \brief Abstract Memory Class 
 * \ingroup MemArray
 *
 * \tparam T                Datatype of an entry to be stored in memory 
 * \tparam NumEntries       Number of entries in memory 
 * \tparam NumBanks         Number of banks in memory
 *
 *
 * \par A Simple Example
 * \code
 *      #include <mem_array.h>
 *        ...
 *        mem_array_sep <MemWord_t, NUM_ENTRIES, NBANKS> banks;
 *      
 *        read_data = banks.read(bank_addr, bank_sel);
 *
 *        banks.write(bank_addr, bank_sel, write_data);
 *        ...
 *      
 *
 * \endcode
 * \par
 *
 */

template <typename T, int NumEntries, int NumBanks, int NumByteEnables=1>
class mem_array_sep {
 public:
  typedef Wrapped<T> WData_t;
  static const unsigned int NumEntriesPerBank = NumEntries/NumBanks;
  static const unsigned int WordWidth = WData_t::width;
  static const unsigned int SliceWidth = WordWidth/NumByteEnables;
  typedef NVUINTW(nvhls::index_width<NumEntriesPerBank>::val) LocalIndex;
  typedef NVUINTW(nvhls::index_width<NumEntriesPerBank*NumByteEnables>::val) LocalSliceIndex;
  typedef NVUINTW(nvhls::index_width<NumBanks>::val) BankIndex;
  typedef sc_lv<WordWidth> Data_t;
  typedef sc_lv<SliceWidth> Slice_t;
  typedef NVUINTW(NumByteEnables) WriteMask;
  typedef NVUINTW(nvhls::index_width<NumByteEnables>::val) ByteEnableIndex;

  typedef Slice_t BankType[NumEntriesPerBank*NumByteEnables];
  nvhls::nv_array<BankType, NumBanks> bank;
  static const int width =  NumEntries * WordWidth;

  mem_array_sep() {
    Slice_t value;
    for (unsigned i = 0; i < NumBanks; i++) {
      for (unsigned j = 0; j < NumByteEnables* NumEntriesPerBank; j++) {
#ifndef SLEC_CPC
        bank[i][j] = value;
#endif
      }
    }
  }
 
  void clear() {
    Slice_t value = 0;
    for (unsigned i = 0; i < NumBanks; i++) {
      for (unsigned j = 0; j < NumByteEnables * NumEntriesPerBank; j++) {
        bank[i][j] = value;
      }
    }
  }

  T read(LocalIndex idx, BankIndex bank_sel=0) {
    Data_t read_data = TypeToBits<NVUINTW(WordWidth)>(0);
    #pragma hls_unroll yes
    for (int i = 0; i < NumByteEnables; i++) {
      LocalSliceIndex local_slice_index = idx * NumByteEnables + i;
      NVHLS_ASSERT_MSG(bank_sel<NumBanks, "bank_index_out_of_bounds");
      NVHLS_ASSERT_MSG(idx<NumEntriesPerBank, "local_index_out_of_bounds");
      read_data.range((i+1)*SliceWidth-1, i*SliceWidth) = bank[bank_sel][local_slice_index];
    } 
    CMOD_ASSERT_MSG(read_data.xor_reduce()!=sc_logic('X'), "Read_data_is_X");
    return BitsToType<T>(read_data);
  }

  void write(LocalIndex idx, BankIndex bank_sel, T val, WriteMask write_mask=~static_cast<WriteMask>(0), bool wce=1) {
    Slice_t tmp[NumByteEnables];
    Data_t write_data = TypeToBits<T>(val);
    #pragma hls_unroll yes
    for (int i = 0; i < NumByteEnables; i++) {
      tmp[i] = write_data.range((i+1)*SliceWidth-1, i*SliceWidth);
    }
    if (wce) {
      #pragma hls_unroll yes
      for (int i = 0; i < NumByteEnables; i++) {
        if (write_mask[i] == 1) {
          LocalSliceIndex local_slice_index = idx * NumByteEnables + i;
          NVHLS_ASSERT_MSG(bank_sel<NumBanks, "bank_index_out_of_bounds");
          NVHLS_ASSERT_MSG(idx<NumEntriesPerBank, "local_index_out_of_bounds");
          bank[bank_sel][local_slice_index] = tmp[i];
          CMOD_ASSERT_MSG(tmp[i].xor_reduce()!=sc_logic('X'), "Write_data_is_X");
        }
      }
    }
  }

  template<unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    for (unsigned i = 0; i < NumBanks; i++) {
      for (unsigned j = 0; j < NumByteEnables* NumEntriesPerBank; j++) {
        m & bank[i][j];
      }
    }
  } 
};


#endif
