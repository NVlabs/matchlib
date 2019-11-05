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
//========================================================================
// marshaller.h
//========================================================================
#ifndef __CONNECTIONS__MARSHALLER_H_
#define __CONNECTIONS__MARSHALLER_H_

#include <systemc.h>

#ifdef HLS_CATAPULT
#include <ccs_types.h>
#include <ccs_p2p.h>
#endif

#include "connections_utils.h"
#include "message.h"


//------------------------------------------------------------------------
// Marshaller casting functions

#ifdef HLS_CATAPULT

template<typename A, int vec_width>
void connections_cast_type_to_vector(const A &data, int length, sc_lv<vec_width> &vec) {
  type_to_vector(data, length, vec);
}

template<typename A, int vec_width>
void connections_cast_vector_to_type(const sc_lv<vec_width> &vec, bool is_signed, A *data) {
  vector_to_type(vec, is_signed, data);
}

#else

/**
 * \brief connections_cast_type_to_vector: Converts a datatype to sc_lv<>.
 * \ingroup Marshaller
 *
 * Marshalling depends on casting primitive and sc/ac data types to
 * sc_lv bitvectors. For HLS_CATAPULT mode, type_to_vector() and
 * vector_to_type() functions are used from Catapult's libraries.
 * However, for other HLS tools the user must manually implement
 * these casts for the stub functions provided if the Marshaller
 * is called.
 *
 * Defining AUTO_PORT=TLM_PORT or DIRECT_PORT for SystemC simulations
 * skips Marshalling, as does CONNECTIONS_FAST_SIM mode, and thus
 * casting does not need to be provided for those cases. However,
 * casting definition is required for all HLS runs.
 *
 */
template<typename A, int vec_width>
void connections_cast_type_to_vector(const A &data, int length, sc_lv<vec_width> &vec) {
  // Primitive type conversion to and from sc_lv types must be provided!
  CONNECTIONS_ASSERT_MSG(0, "Must provide a definition for connections_cast_type_to_vector and connections_cast_vector_to_type when not running in Catapult mode!");
#ifdef __SYNTHESIS__ // CONNECTIONS_ASSERT_MSG is transparent in SYNTHESIS, so protect against SYNTHESIS
#error "Must provide a definition for connections_cast_type_to_vector and connections_cast_vector_to_type when not running in Catapult mode!"
#endif
}

/**
 * \brief connections_cast_vector_to_type: Converts an sc_lv<> to a datatype.
 * \ingroup Marshaller
 *
 * Marshalling depends on casting primitive and sc/ac data types to
 * sc_lv bitvectors. For HLS_CATAPULT mode, type_to_vector() and
 * vector_to_type() functions are used from Catapult's libraries.
 * However, for other HLS tools the user must manually implement
 * these casts for the stub functions provided if the Marshaller
 * is called.
 *
 * Defining AUTO_PORT=TLM_PORT or DIRECT_PORT for SystemC simulations
 * skips Marshalling, as does CONNECTIONS_FAST_SIM mode, and thus
 * casting does not need to be provided for those cases. However,
 * casting definition is required for all HLS runs.
 *
 */
template<typename A, int vec_width>
void connections_cast_vector_to_type(const sc_lv<vec_width> &vec, bool is_signed, A *data) {
  // Primitive type conversion to and from sc_lv types must be provided!
  CONNECTIONS_ASSERT_MSG(0, "Must provide a definition for connections_cast_type_to_vector and connections_cast_vector_to_type when not running in Catapult mode!");
#ifdef __SYNTHESIS__ // CONNECTIONS_ASSERT_MSG is transparent in SYNTHESIS, so protect against SYNTHESIS
#error "Must provide a definition for connections_cast_type_to_vector and connections_cast_vector_to_type when not running in Catapult mode!"
#endif
}

#endif


//------------------------------------------------------------------------
// Marshaller

/**
 * \brief Marshaller is used to automatically convert types to logic vector and vice versa 
 * \ingroup Marshaller
 *
 * \par Overview
 * Marshaller class: similar to boost's serialization approach, Marshaller class
 * provides an easy way to convert an arbitrary SystemC data structures to/from
 * a sequence of bits. 
 * NOTE: I am assuming all the types that we are dealing with are
 * unsigned which may or may not be true. This matters in places where the
 * Marshaller will use vector_to_type calls.
 *
 * NOTE: The solution for the marshaller below relies on the
 * type_to_vector() and vector_to_type() calls as Catapult libraries deal
 * with the specialization for all C++ primitive types, SystemC types, and
 * the ac_* types. To make this vendor agnostic we would have to implement
 * our own connections_cast() with all the specialization. Since, the Marshaller
 * deals with unpacking and packing all complex types, using the Catapult
 * type conversion functions works well in this case.
 *
 * \par A Simple Example
 * \code
 *      #include <connections/marshaller.h>
 *
 *      ...
 *      class mem_req_t {
 *      public:
 *        bool do_store;
 *        bank_addr_t addr;
 *        DataType    wdata;
 *        static const int width = 1 + addr_width + DataType::width;
 *
 *        template <unsigned int Size>
 *        void Marshall(Marshaller<Size>& m) {
 *          m& do_store;
 *          m& addr;
 *          m& wdata;
 *        }
 *      };
 *
 *
 * \endcode
 * \par
 *
 */
template <unsigned int Size>
class Marshaller {
  sc_lv<Size> glob;
  unsigned int cur_idx;
  bool is_marshalling;

 public:
  /* Constructor.
   *   if is_marshalling == True:
   *     convert type to bits;
   *   else:
   *     convert bits to type. */
  Marshaller() : glob(0), cur_idx(0), is_marshalling(true) {}
  Marshaller(sc_lv<Size> v) : glob(v), cur_idx(0), is_marshalling(false) {}

  /* Add a field to the glob, or extract it. */
  template <typename T, int FieldSize>
  void AddField(T& d) {
    CONNECTIONS_SIM_ONLY_ASSERT_MSG(cur_idx + FieldSize <= Size, "Field size exceeded Size. Is an nvhls_message's width enum missing an element, and are all fields marshalled?");
    if (is_marshalling) {
      sc_lv<FieldSize> bits;
      connections_cast_type_to_vector(d, FieldSize, bits);
      glob.range(cur_idx + FieldSize - 1, cur_idx) = bits;
      cur_idx += FieldSize;
    } else {
      sc_lv<FieldSize> bits = glob.range(cur_idx + FieldSize - 1, cur_idx);
      connections_cast_vector_to_type(bits, false, &d);
      cur_idx += FieldSize;
    }
  }

  /* Return the bit vector. */
  sc_lv<Size> GetResult() {
    CONNECTIONS_SIM_ONLY_ASSERT_MSG(cur_idx==Size, "Size doesn't match current index. Is an nvhls_message's width enum missing an element, and are all fields marshalled?");
    return glob.range(Size - 1, 0); 
  }

  /* Operator &.
   * T needs to have a Marshall() function defined. */
  template <typename T>
  Marshaller<Size>& operator&(T& rhs) {
    rhs.Marshall(*this);
    return *this;
  }
};

/**
 * \brief Generic Wrapped class: wraps different datatypes to communicate with Marshaller 
 * \ingroup Marshaller 
 *
 * \par Overview
 *  This function is used to determine the width of a datatype. For ac_types, it relies on static member inside the class called width. For sc_types and bool, Wrapped class has specilizations that determine its width.
 * \par A Simple Example
 * \code
 *  #include <connections/marshaller.h>
 *
 *  ...
 *  DataType    wdata;
 *  const int width = Wrapped<DataType>::width;
 *
 * \endcode
 * \par
 *
 */
template <typename T>
class Wrapped {
 public:
  T val;
  Wrapped() {}
  Wrapped(const T& v) : val(v) {}
  static const unsigned int width = T::width;
  // Assigning is_signed to false by default. Specializations for basic signed types are defined below
  static const bool is_signed = false;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    val.Marshall<Size>(m);
  }
};

/* Wrapped class specialization for sc_lv.  */
template <int Width>
class Wrapped<sc_lv<Width> > {
 public:
  sc_lv<Width> val;
  Wrapped() {}
  Wrapped(const sc_lv<Width>& v) : val(v) {}
  static const unsigned int width = Width;
  static const bool is_signed = false;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& val;
  }
};

/* Operator & for Marshaller and sc_lv. */
template <unsigned int Size, int K>
Marshaller<Size>& operator&(Marshaller<Size>& m, sc_lv<K>& rhs) {
  m.template AddField<sc_lv<K>, K>(rhs);
  return m;
}

/* Wrapped class specialization for bool.  */
template <>
class Wrapped<bool> {
 public:
  bool val;
  Wrapped() {}
  Wrapped(const bool& v) : val(v) {}
  static const unsigned int width = 1;
  static const bool is_signed = false;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& val;
  }
};

/* Operator & for Marshaller and bool. */
template <unsigned int Size>
Marshaller<Size>& operator&(Marshaller<Size>& m, bool& rhs) {
  m.template AddField<bool, 1>(rhs);
  return m;
}

/* Use the preprocessor to define many similar wrapper specializations. */
#define MarshallEnum(Type, EnumSize)                              \
  template <>                                                     \
  class Wrapped<Type> {                                           \
   public:                                                        \
    Type val;                                                     \
    Wrapped() {}                                                  \
    Wrapped(const Type& v) : val(v) {}                            \
    static const unsigned int width = EnumSize;                   \
    static const bool is_signed = false;                          \
    template <unsigned int Size>                                  \
    void Marshall(Marshaller<Size>& m) {                          \
      m& val;                                                     \
    }                                                             \
  };                                                              \
                                                                  \
  template <unsigned int Size>                                    \
  Marshaller<Size>& operator&(Marshaller<Size> & m, Type & rhs) { \
    m.template AddField<Type, EnumSize>(rhs);                     \
    return m;                                                     \
  }

#define SpecialUnsignedWrapper(Type)                                         \
  template <int Width>                                               \
  class Wrapped<Type<Width> > {                                      \
   public:                                                           \
    Type<Width> val;                                                 \
    Wrapped() {}                                                     \
    Wrapped(const Type<Width>& v) : val(v) {}                        \
    static const unsigned int width = Width;                         \
    static const bool is_signed = false;                             \
    template <unsigned int Size>                                     \
    void Marshall(Marshaller<Size>& m) {                             \
      m& val;                                                        \
    }                                                                \
  };                                                                 \
                                                                     \
  template <unsigned int Size, int K>                                \
  Marshaller<Size>& operator&(Marshaller<Size> & m, Type<K> & rhs) { \
    m.template AddField<Type<K>, K>(rhs);                            \
    return m;                                                        \
  }

#define SpecialSignedWrapper(Type)                                         \
  template <int Width>                                               \
  class Wrapped<Type<Width> > {                                      \
   public:                                                           \
    Type<Width> val;                                                 \
    Wrapped() {}                                                     \
    Wrapped(const Type<Width>& v) : val(v) {}                        \
    static const unsigned int width = Width;                         \
    static const bool is_signed = true;                              \
    template <unsigned int Size>                                     \
    void Marshall(Marshaller<Size>& m) {                             \
      m& val;                                                        \
    }                                                                \
  };                                                                 \
                                                                     \
  template <unsigned int Size, int K>                                \
  Marshaller<Size>& operator&(Marshaller<Size> & m, Type<K> & rhs) { \
    m.template AddField<Type<K>, K>(rhs);                            \
    return m;                                                        \
  }

#define SpecialWrapper2(Type)                                              \
  template <int Width, bool Sign>                                          \
  class Wrapped<Type<Width, Sign> > {                                      \
   public:                                                                 \
    Type<Width> val;                                                       \
    Wrapped() {}                                                           \
    Wrapped(const Type<Width>& v) : val(v) {}                              \
    static const unsigned int width = Width;                               \
    static const bool is_signed = Sign;                                    \
    template <unsigned int Size>                                           \
    void Marshall(Marshaller<Size>& m) {                                   \
      m& val;                                                              \
    }                                                                      \
  };                                                                       \
                                                                           \
  template <unsigned int Size, int K, bool Sign>                           \
  Marshaller<Size>& operator&(Marshaller<Size> & m, Type<K, Sign> & rhs) { \
    m.template AddField<Type<K, Sign>, K>(rhs);                            \
    return m;                                                              \
  }

#define SpecialWrapper3(Type)                                              \
  template <int Width, int IWidth, bool Sign>                                          \
  class Wrapped<Type<Width, IWidth, Sign> > {                                      \
   public:                                                                 \
    Type<Width, IWidth, Sign> val;                                                       \
    Wrapped() {}                                                           \
    Wrapped(const Type<Width, IWidth, Sign>& v) : val(v) {}                              \
    static const unsigned int width = Width;                               \
    static const bool is_signed = Sign;                                    \
    template <unsigned int Size>                                           \
    void Marshall(Marshaller<Size>& m) {                                   \
      m& val;                                                              \
    }                                                                      \
  };                                                                       \
                                                                           \
  template <unsigned int Size, int K, int J, bool Sign>                           \
  Marshaller<Size>& operator&(Marshaller<Size> & m, Type<K, J, Sign> & rhs) { \
    m.template AddField<Type<K, J, Sign>, K>(rhs);                            \
    return m;                                                              \
  }


#define SpecialWrapperIfc(Type)                                            \
  template <typename Message>                                              \
  class Wrapped<Type<Message> > {                                          \
   public:                                                                 \
    Type<Message> val;                                                     \
    Wrapped() : val() {}                                                   \
    Wrapped(const Type<Message>& v) : val(v) {}                            \
    static const unsigned int width = Wrapped<Message>::width;             \
    static const bool is_signed = Wrapped<Message>::is_signed;             \
    template <unsigned int Size>                                           \
    void Marshall(Marshaller<Size>& m) {                                   \
      m& val;                                                              \
    }                                                                      \
  };                                                                       \
                                                                           \
  template <unsigned int Size, typename Message>                           \
  Marshaller<Size>& operator&(Marshaller<Size> & m, Type<Message> & rhs) { \
    m.template AddField<Message, Wrapped<Message>::width>(rhs);            \
    return m;                                                              \
  }

SpecialUnsignedWrapper(sc_bv);
SpecialUnsignedWrapper(sc_uint);
SpecialSignedWrapper(sc_int);
SpecialUnsignedWrapper(sc_biguint);
SpecialSignedWrapper(sc_bigint);
SpecialWrapperIfc(sc_in);
SpecialWrapperIfc(sc_out);
SpecialWrapperIfc(sc_signal);

#ifdef HLS_CATAPULT
#include <ac_int.h>
SpecialWrapper2(ac_int);
SpecialWrapper3(ac_fixed);

//------------------------------------------------------------------------
// Wrapped< p2p<>::in<T> >
//------------------------------------------------------------------------
// Specialization for p2p<>::in

template <typename Message>
class Wrapped<p2p<>::in<Message> > {
 public:
  p2p<>::in<Message> val;
  Wrapped() : val("in") {}
  Wrapped(const p2p<>::in<Message>& v) : val(v) {}
  static const unsigned int width = Wrapped<Message>::width;
  static const bool is_signed = Wrapped<Message>::is_signed;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& val;
  }
};

template <unsigned int Size, typename Message>
Marshaller<Size>& operator&(Marshaller<Size>& m, p2p<>::in<Message>& rhs) {
  m.template AddField<Message, Wrapped<Message>::width>(rhs);
  return m;
}

//------------------------------------------------------------------------
// Wrapped< p2p<>::out<T> >
//------------------------------------------------------------------------
// Specialization for p2p<>::out

template <typename Message>
class Wrapped<p2p<>::out<Message> > {
 public:
  p2p<>::out<Message> val;
  Wrapped() : val("out") {}
  Wrapped(const p2p<>::out<Message>& v) : val(v) {}
  static const unsigned int width = Wrapped<Message>::width;
  static const bool is_signed = Wrapped<Message>::is_signed;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& val;
  }
};

template <unsigned int Size, typename Message>
Marshaller<Size>& operator&(Marshaller<Size>& m, p2p<>::out<Message>& rhs) {
  m.template AddField<Message, Wrapped<Message>::width>(rhs);
  return m;
}

//------------------------------------------------------------------------
// Wrapped< p2p<>::chan<T> >
//------------------------------------------------------------------------
// Specialization for p2p<>::chan

template <typename Message>
class Wrapped<p2p<>::chan<Message> > {
 public:
  p2p<>::chan<Message> val;
  Wrapped() : val("chan") {}
  Wrapped(const p2p<>::chan<Message>& v) : val(v) {}
  static const unsigned int width = Wrapped<Message>::width;
  static const bool is_signed = Wrapped<Message>::is_signed;
  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& val;
  }
};

template <unsigned int Size, typename Message>
Marshaller<Size>& operator&(Marshaller<Size>& m, p2p<>::chan<Message>& rhs) {
  m.template AddField<Message, Wrapped<Message>::width>(rhs);
  return m;
}
#endif

/**
 * \brief StaticMax Class: returns the larger value between two unsigned integers 
 * \ingroup StaticMax 
 *
 * \par A Simple Example
 * \code
 *  #include <connections/marshaller.h>
 *
 *  ...
 *  static const unsigned int larger_width =
 *     StaticMax<width1, width1>::value
 *
 * \endcode
 * \par
 *
 */
template <unsigned int A, unsigned int B>
class StaticMax {
 public:
  static const unsigned int value = A > B ? A : B;
};

/**
 * \brief  BitUnion2 class: A union class to hold two Marshallers. 
 * \ingroup BitUnion2
 *
 * \par A Simple Example
 * \code
 *  #include <connections/marshaller.h>
 *
 *  ...
 *  typedef BitUnion2<TypeA, TypeB> UnionType;
 *  UnionType type_union;
 *  TypeA type_a;
 *  TypeB type_b;
 *  ....
 *  type_union.Set(type_a); // Assign variable of TypeA to TypeUnion
 *  if (type_union.IsA()) {
 *    TypeA tmp = type_union.GetA();
 *  }
 *
 *  type_union.Set(type_b); // Assign variable of TypeB to TypeUnion
 *  if (type_union.IsB()) {
 *    TypeB tmp = type_union.GetB();
 *  }
 *  
 * \endcode
 * \par
 *
 */
template <typename A, typename B>
class BitUnion2 {
 public:
  static const unsigned int larger_width =
      StaticMax<Wrapped<A>::width, Wrapped<B>::width>::value;
  static const unsigned int width = larger_width + 1;
  static const bool is_signed = false; 

  /* Constructors. */
  BitUnion2() : payload(0), tag(0) {}
  BitUnion2(const A& initdata) : payload(0), tag(0) { Set(initdata); }
  BitUnion2(const B& initdata) : payload(0), tag(1) { Set(initdata); }

  bool IsA() const { return tag == 0; }
  bool IsB() const { return tag == 1; }

  A GetA() const {
    CONNECTIONS_SIM_ONLY_ASSERT_MSG(tag == 0, "Tag doesn't match request! Use GetB() instead.");
    Marshaller<width> m(payload);
    Wrapped<A> result;
    result.Marshall<width>(m);
    return result.val;
  }
  B GetB() const {
    CONNECTIONS_SIM_ONLY_ASSERT_MSG(tag == 1, "Tag doesn't match request! Use GetA() instead.");
    Marshaller<width> m(payload);
    Wrapped<B> result;
    result.Marshall<width>(m);
    return result.val;
  }

  void Set(const A& data) {
    Wrapped<A> wdata(data);
    Marshaller<Wrapped<A>::width> m;
    wdata.Marshall<Wrapped<A>::width>(m);
    payload = 0; //note, we could directly assign m.GetResult here. Assuming payload is wider, systemC would pad with zeros.
                 //but if payload is narrower, systemc would silently truncate. We'd want to avoid that. Range below would 
                 //fail if payload is narrower
    payload.range(Wrapped<A>::width - 1, 0) = m.GetResult();
    tag = 0;
  }
  void Set(const B& data) {
    Wrapped<B> wdata(data);
    Marshaller<Wrapped<B>::width> m;
    wdata.Marshall<Wrapped<B>::width>(m);
    payload = 0; //same as in Set(A) above
    payload.range(Wrapped<B>::width - 1, 0) = m.GetResult();
    tag = 1;
  }

  template <unsigned int Size>
  void Marshall(Marshaller<Size>& m) {
    m& payload;
    m& tag;
  }

 protected:
  /* Stores the value of the current Marshaller.*/
  sc_lv<larger_width> payload;
  /* Signal whether A or B is set (0 for A and 1 for B). */
  sc_lv<1> tag;
};

#endif  // __CONNECTIONS__MARSHALLER_H_
