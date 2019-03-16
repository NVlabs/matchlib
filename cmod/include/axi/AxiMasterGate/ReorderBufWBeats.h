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

#ifndef __REORDERBUFWBEATS_H__
#define __REORDERBUFWBEATS_H__

#include <ReorderBuf.h>

/**
 * \brief An extension of ReorderBuf that allows one entry to contain multiple beats of data.
 * \ingroup ReorderBuffer
 *
 * \tparam Data             DataType
 * \tparam Depth            Depth of queue
 * \tparam InFlight         Number of inflight entries
 */
template <typename Data, unsigned int Depth, unsigned int InFlight>
class ReorderBufWBeats : public ReorderBuf<Data, Depth, InFlight> {

 public:
  ReorderBufWBeats() : ReorderBuf<Data, Depth, InFlight>() {}

  bool canReceiveBeats()
  {
    // Beats do not have a guaranteed space in storage
    // since storage corresponds to vbits fifo,
    // we can just check that vfifo is not full
    return (!ReorderBuf<Data, Depth, InFlight>::vbits.isFull());
  }

  void addBeat(const Data& data) {
    typename ReorderBuf<Data, Depth, InFlight>::EntryNum entryNum =
        ReorderBuf<Data, Depth, InFlight>::vbits.get_tail();
    ReorderBuf<Data, Depth, InFlight>::storage.write(entryNum, 0, data);
    // so that response can be read out later
    ReorderBuf<Data, Depth, InFlight>::vbits.push(true); 
  }
};

#endif
