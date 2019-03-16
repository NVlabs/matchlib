/*
 * Copyright (c) 2018-2019, NVIDIA CORPORATION.  All rights reserved.
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
#ifndef __AXI_CONFIG_H__
#define __AXI_CONFIG_H__

namespace axi {

/**
 * \brief Examples of valid AXI configs.
 * \ingroup AXI
 *
 * \par  An AXI config consists of a struct with an enum defining the following constants:
 * - dataWidth: The bitwidth of an AXI data word.
 * - addrWidth: The bitwidth of the AXI address.
 * - idWidth: The bitwidth of the AXI ID field.
 * - useVariableBeatSize: Set to 1 if the interface supports beats that are less
 * than one data word, 0 otherwise.
 * - useMisalignedAddresses: Set to 1 if addresses not aligned to the data word
 * boundary are supported, 0 otherwise.
 * - useLast: Set to 1 if the last bit in write data and read requests is used,
 * 0 otherwise.
 * - useWriteStrobes: Set to 1 if write strobes are supported, 0 otherwise.
 * - useBurst: Set to 1 if bursts are supported, 0 otherwise
 * - useFixedBurst: Set to 1 if fixed-type bursts are supported, 0 otherwise.
 * - useWrapBurst: Set to 1 if wrap-type bursts are supported, 0 otherwise.
 * - maxBurstSize: The maximum burst length.
 * - useQoS: Set to 1 if the QoS field is supported, 0 otherwise.
 * - useLock: Set to 1 if the Lock field is supported, 0 otherwise.
 * - useProt: Set to 1 if the Prot field is supported, 0 otherwise.
 * - useCache: Set to 1 if the Cache field is supported, 0 otherwise.
 * - useRegion: Set to 1 if the Region field is supported, 0 otherwise.
 * - useWriteResponses: Set to 1 if write responses are supported, 0 otherwise.
 * - aUserWidth: The bitwidth of the AxUser fields.
 * - wUserWidth: The bitwidth of the WUser field.
 * - bUserWidth: The bitwidth of the BUser field.
 * - rUserWidth: The bitwidth of the RUser field.
 */
namespace cfg {

/**
 * \brief A standard AXI configuration.
 */
struct standard {
  enum {
    dataWidth = 64,
    useVariableBeatSize = 0,
    useMisalignedAddresses = 0,
    useLast = 1,
    useWriteStrobes = 1,
    useBurst = 1, useFixedBurst = 0, useWrapBurst = 0, maxBurstSize = 256,
    useQoS = 0, useLock = 0, useProt = 0, useCache = 0, useRegion = 0,
    aUserWidth = 0, wUserWidth = 0, bUserWidth = 0, rUserWidth = 0,
    addrWidth = 32,
    idWidth = 4,
    useWriteResponses = 1,
  };
};
/**
 * \brief An AXI configuration with no write responses.
 */
struct no_wresp {
  enum {
    dataWidth = 64,
    useVariableBeatSize = 0,
    useMisalignedAddresses = 0,
    useLast = 1,
    useWriteStrobes = 1,
    useBurst = 1, useFixedBurst = 0, useWrapBurst = 0, maxBurstSize = 256,
    useQoS = 0, useLock = 0, useProt = 0, useCache = 0, useRegion = 0,
    aUserWidth = 0, wUserWidth = 0, bUserWidth = 0, rUserWidth = 0,
    addrWidth = 32,
    idWidth = 4,
    useWriteResponses = 0,
  };
};
/**
 * \brief An AXI configuration with no write strobes.
 */
struct no_wstrb {
  enum {
    dataWidth = 64,
    useVariableBeatSize = 0,
    useMisalignedAddresses = 0,
    useLast = 1,
    useWriteStrobes = 0,
    useBurst = 1, useFixedBurst = 0, useWrapBurst = 0, maxBurstSize = 256,
    useQoS = 0, useLock = 0, useProt = 0, useCache = 0, useRegion = 0,
    aUserWidth = 0, wUserWidth = 0, bUserWidth = 0, rUserWidth = 0,
    addrWidth = 32,
    idWidth = 4,
    useWriteResponses = 1,
  };
};
/**
 * \brief An AXI configuration corresponding to the AXI4-Lite standard.
 */
struct lite {
  enum {
    dataWidth = 32,
    useVariableBeatSize = 0,
    useMisalignedAddresses = 0,
    useLast = 0,
    useWriteStrobes = 1,
    useBurst = 0, useFixedBurst = 0, useWrapBurst = 0, maxBurstSize = 1,
    useQoS = 0, useLock = 0, useProt = 0, useCache = 0, useRegion = 0,
    aUserWidth = 0, wUserWidth = 0, bUserWidth = 0, rUserWidth = 0,
    addrWidth = 32,
    idWidth = 0,
    useWriteResponses = 1,
  };
};
/**
 * \brief A configuration similar to AXI4-Lite, but without write strobes.
 */
struct lite_nowstrb {
  enum {
    dataWidth = 32,
    useVariableBeatSize = 0,
    useMisalignedAddresses = 0,
    useLast = 0,
    useWriteStrobes = 0,
    useBurst = 0, useFixedBurst = 0, useWrapBurst = 0, maxBurstSize = 1,
    useQoS = 0, useLock = 0, useProt = 0, useCache = 0, useRegion = 0,
    aUserWidth = 0, wUserWidth = 0, bUserWidth = 0, rUserWidth = 0,
    addrWidth = 32,
    idWidth = 0,
    useWriteResponses = 1,
  };
};
}; // namespace cfg
}; // namespace axi

#endif
