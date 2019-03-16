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

#ifndef __AXI4_ENCODING_H__
#define __AXI4_ENCODING_H__

namespace axi {

/**
 * \brief Hardcoded values associated with the AXI4 standard.
 * \ingroup AXI
 *
 * \par
 * These enumerated values are defined by the AXI standard and should not be modified.
 *
 */
class AXI4_Encoding {
 public:
  /**
  * \brief Hardcoded values for the ARCACHE field.
  */
  class ARCACHE {
   public:
    enum {
      _WIDTH = 4,  // bits

      DEVICE_NON_BUF = 0,
      DEVICE_BUF = 1,
      NON_CACHE_NON_BUF = 2,
      NON_CACHE_BUF = 3,
      WR_THRU_NO_ALLOC = 10,
      WR_THRU_RD_ALLOC = 5,
      WR_THRU_WR_ALLOC = 10,
      WR_THRU_RW_ALLOC = 14,
      WR_BACK_NO_ALLOC = 11,
      WR_BACK_RD_ALLOC = 7,
      WR_BACK_WR_ALLOC = 11,
      WR_BACK_RW_ALLOC = 15,
    };
  };

  /**
  * \brief Hardcoded values for the AWCACHE field.
  */
  class AWCACHE {
   public:
    enum {
      _WIDTH = 4,  // bits

      DEVICE_NON_BUF = 0,
      DEVICE_BUF = 1,
      NON_CACHE_NON_BUF = 2,
      NON_CACHE_BUF = 3,
      WR_THRU_NO_ALLOC = 6,
      WR_THRU_RD_ALLOC = 6,
      WR_THRU_WR_ALLOC = 14,
      WR_THRU_RW_ALLOC = 14,
      WR_BACK_NO_ALLOC = 7,
      WR_BACK_RD_ALLOC = 7,
      WR_BACK_WR_ALLOC = 15,
      WR_BACK_RW_ALLOC = 15,
    };
  };

  /**
  * \brief Hardcoded values shared by the ARBURST and AWBURST fields.
  */
  class AXBURST {
   public:
    enum {
      _WIDTH = 2,  // bits

      FIXED = 0,
      INCR = 1,
      WRAP = 2,
    };
  };

  /**
  * \brief Hardcoded values shared by the RRESP and BRESP fields.
  */
  class XRESP {
   public:
    enum {
      _WIDTH = 2,  // bits

      OKAY = 0,
      EXOKAY = 1,
      SLVERR = 2,
      DECERR = 3,
    };
  };
};
};

#endif
