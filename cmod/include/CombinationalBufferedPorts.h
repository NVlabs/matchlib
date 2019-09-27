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
// CombinationalBufferedPorts.h
//========================================================================


#ifndef COMBINATIONAL_BUFFERED_PORTS_H_
#define COMBINATIONAL_BUFFERED_PORTS_H_

#include <nvhls_connections.h>

namespace Connections {

  template <typename Message, int BufferSizeRead = 1, int BufferSizeWrite = 1>
  class CombinationalBufferedPorts : public Combinational<Message> {
    typedef NVUINTW(nvhls::index_width<BufferSizeWrite+1>::val) AddressPlusOne;
    FIFO<Message, BufferSizeRead> fifo_read;
    FIFO<Message, BufferSizeWrite> fifo_write;

  public:
    CombinationalBufferedPorts()
      : Combinational<Message>(),
      fifo_read(),
      fifo_write()
    {}
    
    explicit CombinationalBufferedPorts(const char* name)
      : Combinational<Message>(name),
      fifo_read(),
      fifo_write()
    {}
    
    void ResetRead() {
      Combinational<Message>::ResetRead();
      fifo_read.reset();
    }

    void ResetWrite() {
      Combinational<Message>::ResetWrite();
      fifo_write.reset();
    }
    
    // Empty
    bool EmptyRead() { return fifo_read.isEmpty(); }

    Message Pop() { return fifo_read.pop(); }

    void IncrHeadRead() { fifo_read.incrHead(); }

    Message PeekRead() { return fifo_read.peek(); }

    void TransferNBRead() {
      if (!fifo_read.isFull()) {
	Message msg;
	if (Combinational<Message>::PopNB(msg)) {
	  fifo_read.push(msg);
	}
      }
    }
    
    // Full
    bool FullWrite() { return fifo_write.isFull(); }
    // Empty
    bool EmptyWrite() { return fifo_write.isEmpty(); }

    AddressPlusOne NumAvailableWrite() { return fifo_write.NumAvailable(); }

    void Push(const Message& msg) { fifo_write.push(msg); }

    void TransferNBWrite() {
      if (!fifo_write.isEmpty()) {
	Message msg = fifo_write.peek();
	if (Combinational<Message>::PushNB(msg)) {
	  fifo_write.pop();
	}
      }
    }

    // Overload these so we don't accidently call them
    virtual bool PopNB(Message& data) { NVHLS_ASSERT_MSG(0,"Calling_PopNB_on_Buffered_port_is_not_valid"); return false; }
    virtual bool PushNB(const Message& m) { NVHLS_ASSERT_MSG(0,"Calling_PushNB_on_Buffered_port_is_not_valid"); return false; }
  };


  template <typename Message, int BufferSizeRead>
  class CombinationalBufferedPorts <Message,BufferSizeRead,0> : public Combinational<Message> {
    FIFO<Message, BufferSizeRead> fifo_read;

  public:
    CombinationalBufferedPorts()
      : Combinational<Message>(),
      fifo_read()
    {}
    
    explicit CombinationalBufferedPorts(const char* name)
      : Combinational<Message>(name),
      fifo_read()
    {}

    void ResetRead() {
      Combinational<Message>::ResetRead();
      fifo_read.reset();
    }

    // Empty
    bool EmptyRead() { return fifo_read.isEmpty(); }

    Message Pop() { return fifo_read.pop(); }

    void IncrHeadRead() { fifo_read.incrHead(); }

    Message PeekRead() { return fifo_read.peek(); }

    void TransferNBRead() {
      if (!fifo_read.isFull()) {
	Message msg;
	if (Combinational<Message>::PopNB(msg)) {
	  fifo_read.push(msg);
	}
      }
    }
    
    // Overload these so we don't accidently call them
    virtual bool PopNB(Message& data) { NVHLS_ASSERT_MSG(0,"Calling_PopNB_on_Buffered_port_is_not_valid"); return false; }
  };
  
  template <typename Message, int BufferSizeWrite>
  class CombinationalBufferedPorts<Message,0,BufferSizeWrite> : public Combinational<Message> {
    typedef NVUINTW(nvhls::index_width<BufferSizeWrite+1>::val) AddressPlusOne;
    FIFO<Message, BufferSizeWrite> fifo_write;

  public:
    CombinationalBufferedPorts()
      : Combinational<Message>(),
      fifo_write()
    {}
    
    explicit CombinationalBufferedPorts(const char* name)
      : Combinational<Message>(name),
      fifo_write()
    {}

    void ResetWrite() {
      Combinational<Message>::ResetWrite();
      fifo_write.reset();
    }
    
    // Full
    bool FullWrite() { return fifo_write.isFull(); }
    // Empty
    bool EmptyWrite() { return fifo_write.isEmpty(); }

    AddressPlusOne NumAvailableWrite() { return fifo_write.NumAvailable(); }

    void Push(const Message& msg) { fifo_write.push(msg); }

    void TransferNBWrite() {
      if (!fifo_write.isEmpty()) {
	Message msg = fifo_write.peek();
	if (Combinational<Message>::PushNB(msg)) {
	  fifo_write.pop();
	}
      }
    }

    // Overload these so we don't accidently call them
    virtual bool PushNB(const Message& m) { NVHLS_ASSERT_MSG(0,"Calling_PushNB_on_Buffered_port_is_not_valid"); return false; }
  };
  
};
#endif // COMBINATIONAL_BUFFERED_PORTS_H_
