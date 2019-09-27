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


#ifndef __ARBITER_H__
#define __ARBITER_H__

#include <systemc.h>
#include <nvhls_int.h>
#include <nvhls_assert.h>

enum arbiter_type { Static, Roundrobin };


/**
 * \brief A generalized implementation of generic n-way roundrobin arbiter. 
 * \ingroup Arbiter
 *
 * \tparam size_            Number of elements to be arbitrated.
 * \tparam ArbiterType      Selecting arbitration method. Current class implements Roundrobin, and a dedicated [class] (class_arbiter_3_01size___00_01_static_01_4.html) implements Static. (default: Roundrobin).
 *
 * \par Overview
 * - Given a vector indicating which elements are currently valid for selection, and previous selection, a new selection will be made.
 * - Once a selection is made internal state is updated.
 * - Selection is returned in form of a one-hot bitmask.
 * - In case no entry is valid a zero selection mask will be returned.
 *
 * \par A Simple Example
 * \code
 *      #include <Arbiter.h>
 *
 *      ...
 *      Arbiter<4> arbiter;
 *      arbiter.reset(); // optional in this case, suitable if arbiter is a class member rather than local variable
 *
 *      while (1) {
 *          ...
 *          Arbiter<4>::Mask active = ~0; // active mask is set, in this case all set to active
 *          Arbiter<4>::Mask select = arbiter.pick(active);
 *          ...
 *      };
 *
 * \endcode
 * \par
 *
 */
template <unsigned int size_, arbiter_type ArbiterType = Roundrobin>
class Arbiter {
    public:
        typedef NVUINTW(size_) Mask;

    protected:
        enum _ {
            UNROLLED_SIZE = 2 * size_ - 1,
            log_unrolled_size = nvhls::log2_ceil<UNROLLED_SIZE>::val,
            log_size = nvhls::log2_ceil<size_>::val
                //-1 because there is no reason to duplicate LSB
        };

        typedef NVUINTW(UNROLLED_SIZE) UnrolledMask;
        Mask next;

    public:
        Arbiter() { reset(); };

        // reset the state
        inline void reset() { next = ~static_cast<Mask>(0); }

        // picks the next element
        // input : valid mask
        // output : select mask
        // side effect : updates internal state of next select
        Mask pick(const Mask& valid) {
            if (valid == 0) {
              return 0;
            }
            UnrolledMask unrolled_valid = static_cast<UnrolledMask>(0);
            unrolled_valid =
                nvhls::set_slc(unrolled_valid, nvhls::get_slc<size_ - 1>(valid, 1), 0) |
                nvhls::set_slc(unrolled_valid, valid, size_ - 1);

            UnrolledMask unrolled_next = static_cast<UnrolledMask>(0);
            NVUINTW(size_ - 1) temp = ~static_cast<NVUINTW(size_ - 1)>(0);
            unrolled_next = nvhls::set_slc(unrolled_next, next, size_ - 1) |
                nvhls::set_slc(unrolled_next, temp, 0);

            UnrolledMask priority(unrolled_next & unrolled_valid);

            NVUINTW(log_unrolled_size) first_one_idx, num_zeros;
            UnrolledMask unrolled_choice = static_cast<UnrolledMask>(0);

            Mask choice = static_cast<Mask>(0);
            Mask choice_temp = static_cast<Mask>(0);
            if (priority != 0) {
                first_one_idx = nvhls::leading_ones<UNROLLED_SIZE, UnrolledMask,
                              NVUINTW(log_unrolled_size)>(priority);
                unrolled_choice[first_one_idx] = 1;
                choice_temp[0] = unrolled_choice[size_ - 1];
                NVUINTW(size_ - 1)
                    temp2 = (nvhls::get_slc<UNROLLED_SIZE - size_>(unrolled_choice, size_)) |
                    (nvhls::get_slc<size_ - 1>(unrolled_choice, 0));
                choice = nvhls::set_slc(choice_temp, temp2, 1);

                if (first_one_idx != (size_ - 1)) {
                    next[size_ - 1] = 0;
#pragma hls_unroll yes
                    for (unsigned i = size_ - 2; i > 0; --i) {
                        next[i] = next[i + 1] | choice[i + 1];
                    }
                } else {
                    next = ~static_cast<Mask>(0);
                }
            }
            // assert valid!=0  =>  (valid&choice)!=0
            //NVHLS_ASSERT_MSG(valid!=0, "Arbiter_input_is_zero_and_output_is_non-zero");
            return choice;
        }
};

/**
 * \brief Simplified specialization of a single entry arbiter to be implemented as single passthrough of valid bit. Usage identical to generic Arbiter class.
 * \ingroup Arbiter
 */
template<>
class Arbiter<1, Roundrobin> {
    public:
        typedef NVUINT1 Mask;

    public:
        Arbiter() { reset(); };

        // reset the state
        inline void reset() { }

        // picks the next element
        Mask pick(const Mask& valid) {
            if (valid !=0) {
                return static_cast<Mask>(1);
            } else {
                return static_cast<Mask>(0); 
            }
        }
};


/**
 * \brief Static arbitration specialization. Usage similar to generic Arbiter class.
 * \ingroup Arbiter
 *
 * \tparam size_            Number of elements to be arbitrated.
 **/

template <unsigned int size_>
class Arbiter<size_, Static> {
    public:
        typedef NVUINTW(size_) Mask;

    protected:
        enum _ { log_size = nvhls::log2_ceil<size_>::val };

    public:
        // picks the next element
        // input : valid mask
        // output : select mask
        // side effect : updates internal state of next select
        Mask pick(const Mask& valid) {
            Mask select = static_cast<Mask>(0);
            NVUINTW(log_size) first_one_idx;
            if (valid != 0) {
                first_one_idx =
                    nvhls::leading_ones<size_, Mask, NVUINTW(log_size)>(valid);
                select[first_one_idx] = 1;
            }
            return select;
        }
};


#endif  // __ARBITER_H__
