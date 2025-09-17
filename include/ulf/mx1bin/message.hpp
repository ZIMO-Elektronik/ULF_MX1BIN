// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Message structs
///
/// \file   ulf/mx1bin/message.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <variant>
#include <ztl/inplace_vector.hpp>
#include "decoder.hpp"
#include "message_base.hpp"
#include "utility.hpp"

namespace ulf::mx1bin {

/// Encodable concept
template<typename T,
         typename E = detail::Encoder<unsigned char*, unsigned char*>>
concept Encodable = requires {
  { &T::template encode<E> } -> std::same_as<E (T::*)(E) const>;
};

/// Decodable concept
///
/// \note This is safe, since return is checked against the actual type
template<typename T>
concept Decodable =
  requires(T t, detail::Decoder<char const*, char const*> const& d) {
    { T::decode(d) } -> std::same_as<std::expected<T, std::errc>>;
  };

template<Decodable T, std::ranges::input_range R>
constexpr std::expected<T, std::errc> decode(R const& r) {
  detail::Decoder d(r);
  return T::decode(d);
}

struct Ack : public detail::ReplyHead {
  template<detail::encoder E>
  auto encode(E e) const {
    return ReplyHead::encode(e);
  }
  template<detail::decoder D>
  static std::expected<Ack, std::errc> decode(D& d) {
    if (auto const base{ReplyHead::decode(d)}) return Ack{*base};
    else return std::unexpected(base.error());
  }
};

struct Nak : public detail::Head {
  template<detail::encoder E>
  auto encode(E e) const {
    return Head::encode(e);
  }
  template<detail::decoder D>
  static std::expected<Nak, std::errc> decode(D& d) {
    if (auto const result{Head::decode(d)}) return Nak{*result};
    else return std::unexpected(result.error());
  }
};

struct Reset : public detail::Head {
  template<detail::encoder E>
  auto encode(E e) const {
    return Head::encode(e);
  }
  template<detail::decoder D>
  static std::expected<Reset, std::errc> decode(D& d) {
    if (auto const result{Head::decode(d)}) return Reset{*result};
    else return std::unexpected(result.error());
  }
};

struct TrackControl : public detail::Head {
  struct Reply : public detail::ReplyHead {
    uint8_t statusBits{};
    template<detail::encoder E>
    auto encode(E e) const {
      ReplyHead::encode(e);
      e.uint8(statusBits);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyHead::decode(d)};
      if (!base || !d.has_at_least(sizeof(cAction)))
        return std::unexpected(std::errc::invalid_argument);
      Reply result{*base};
      result.statusBits = d.uint8();
      return result;
    }
  };

  uint8_t cAction{}; ///> Action
  template<detail::encoder E>
  auto encode(E e) const {
    e = Head::encode(e);
    e.uint8(cAction);
    return e;
  }
  template<detail::decoder D>
  static std::expected<TrackControl, std::errc> decode(D& d) {
    auto const base{Head::decode(d)};
    if (!base || !d.has_at_least(sizeof(cAction)))
      return std::unexpected(std::errc::invalid_argument);
    TrackControl result{*base};
    result.cAction = d.uint8();
    return result;
  }
};

struct DecoderControl : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyDecoderControlBase {
    template<detail::encoder E>
    auto encode(E e) const {
      return ReplyDecoderControlBase::encode(e);
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      if (auto const result{ReplyDecoderControlBase::decode(d)})
        return Reply{*result};
      else return std::unexpected(result.error());
    }
  };

  uint8_t cSpeed{};                ///> Speed
  std::optional<uint8_t> cData1{}; ///>
  std::optional<uint8_t> cData2{}; ///>
  std::optional<uint8_t> cData3{}; ///>
  std::optional<uint8_t> cData4{}; ///>
  std::optional<uint8_t> cData5{}; ///>
  template<detail::encoder E>
  auto encode(E e) {
    e = DecoderControlBase::encode(e);
    e.uint8(cSpeed);
    e.uint8(cData1);
    e.uint8(cData2);
    e.uint8(cData3);
    e.uint8(cData4);
    e.uint8(cData5);
    return e;
  }
  template<detail::decoder D>
  static std::expected<DecoderControl, std::errc> decode(D& d) {
    auto const base{DecoderControlBase::decode(d)};
    if (!base || !d.has_at_least(sizeof(cSpeed)))
      return std::unexpected(std::errc::invalid_argument);
    DecoderControl result{*base};
    result.cSpeed = d.uint8();
    // Add optional data
    result.cData1 = d.s_uint8();
    result.cData2 = d.s_uint8();
    result.cData3 = d.s_uint8();
    result.cData4 = d.s_uint8();
    result.cData5 = d.s_uint8();
    return result;
  }
};

struct InvertFunctionBits : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyDecoderControlBase {
    template<detail::encoder E>
    auto encode(E e) const {
      return ReplyDecoderControlBase::encode(e);
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      if (auto const base{ReplyDecoderControlBase::decode(d)})
        return Reply{*base};
      else return std::unexpected(base.error());
    }
  };

  uint8_t cData1{}; ///>
  uint8_t cData2{}; ///>
  uint8_t cData3{}; ///>
  uint8_t cData4{}; ///>
  uint8_t cData5{}; ///>
  template<detail::encoder E>
  auto encode(E e) const {
    e = DecoderControlBase::encode(e);
    e.uint8(cData1);
    e.uint8(cData2);
    e.uint8(cData3);
    e.uint8(cData4);
    e.uint8(cData5);
    return e;
  }
  template<detail::decoder D>
  static std::expected<InvertFunctionBits, std::errc> decode(D& d) {
    auto const base{DecoderControlBase::decode(d)};
    if (!base ||
        !d.has_at_least(sizeof(cData1) + sizeof(cData2) + sizeof(cData3) +
                        sizeof(cData4) + sizeof(cData5)))
      return std::unexpected(std::errc::invalid_argument);
    InvertFunctionBits result{*base};
    result.cData1 = d.uint8();
    result.cData2 = d.uint8();
    result.cData3 = d.uint8();
    result.cData4 = d.uint8();
    result.cData5 = d.uint8();
    return result;
  }
};

struct Acceleration : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyDecoderControlBase {
    template<detail::encoder E>
    auto encode(E e) const {
      return ReplyDecoderControlBase::encode(e);
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      if (auto const base{ReplyDecoderControlBase::decode(d)})
        return Reply{*base};
      else return std::unexpected{base.error()};
    }
  };

  uint8_t cAzBz{}; ///> Accelleration / Deccelleration
  template<detail::encoder E>
  auto encode(E e) const {
    e = DecoderControlBase::encode(e);
    e.uint8(cAzBz);
    return e;
  }
  template<detail::decoder D>
  static std::expected<Acceleration, std::errc> decode(D& d) {
    auto const base{DecoderControlBase::decode(d)};
    if (!base || !d.has_at_least(sizeof(cAzBz)))
      return std::unexpected(std::errc::invalid_argument);
    Acceleration result{*base};
    result.cAzBz = d.uint8();
    return result;
  }
};

struct ShuttleTrain : public detail::ShuttleTrain_Accessory_Base {
  struct Reply : public detail::ReplyDecoderControlBase {
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      return ReplyDecoderControlBase::encode(out);
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      if (auto const base{ReplyDecoderControlBase::decode(d)})
        return Reply{*base};
      else return std::unexpected(base.error());
    }
  };

  template<detail::encoder E>
  auto encode(E e) const {
    return ShuttleTrain_Accessory_Base::encode(e);
  }
  template<detail::decoder D>
  static std::expected<ShuttleTrain, std::errc> decode(D& d) {
    if (auto const base{ShuttleTrain_Accessory_Base::decode(d)})
      return ShuttleTrain{*base};
    else return std::unexpected(base.error());
  }
};

struct Accessory : public detail::ShuttleTrain_Accessory_Base {
  struct Reply : public detail::ReplyDecoderControlBase {
    template<detail::encoder E>
    auto encode(E e) const {
      return ReplyDecoderControlBase::encode(e);
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      if (auto const base{ReplyDecoderControlBase::decode(d)})
        return Reply{*base};
      else return std::unexpected(base.error());
    }
  };

  template<detail::encoder E>
  auto encode(E e) const {
    return ShuttleTrain_Accessory_Base::encode(e);
  }
  template<detail::decoder D>
  static std::expected<Accessory, std::errc> decode(D& d) {
    if (auto const base{ShuttleTrain_Accessory_Base::decode(d)})
      return Accessory{*base};
    else return std::unexpected(base.error());
  }
};

struct LocoMemoryQuery : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyErrorBase {
    uint16_t cAdr{};
    uint8_t cSpeed{};
    uint8_t cData1{};
    uint8_t cData2{};
    uint8_t cData3{};
    uint8_t cAzBz{};
    uint8_t cStatus{};
    uint8_t cData4{};
    uint8_t cData5{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyErrorBase::encode(e);
      e.uint16(cAdr);
      e.uint8(cSpeed);
      e.uint8(cData1);
      e.uint8(cData2);
      e.uint8(cData3);
      e.uint8(cAzBz);
      e.uint8(cStatus);
      e.uint8(cData4);
      e.uint8(cData5);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyErrorBase::decode(d)};
      if (!base ||
          !d.has_at_least(sizeof(cAdr) + sizeof(cSpeed) + sizeof(cData1) +
                          sizeof(cData2) + sizeof(cData3) + sizeof(cAzBz) +
                          sizeof(cStatus) + sizeof(cData4) + sizeof(cData5)))
        return std::unexpected(base.error());
      Reply result{*base};
      result.cAdr = d.uint16();
      result.cSpeed = d.uint8();
      result.cData1 = d.uint8();
      result.cData2 = d.uint8();
      result.cData3 = d.uint8();
      result.cAzBz = d.uint8();
      result.cStatus = d.uint8();
      result.cData4 = d.uint8();
      result.cData5 = d.uint8();
      return result;
    }
  };

  template<detail::encoder E>
  auto encode(E e) const {
    return DecoderControlBase::encode(e);
  }
  template<detail::decoder D>
  static std::expected<LocoMemoryQuery, std::errc> decode(D& d) {
    if (auto const base{DecoderControlBase::decode(d)})
      return LocoMemoryQuery{*base};
    else return std::unexpected(base.error());
  }
};

struct AccessoryMemoryQuery : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyErrorBase {
    uint16_t cAdr{};
    uint8_t cPair{};
    uint8_t cOutputs{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyErrorBase::encode(e);
      e.uint16(cAdr);
      e.uint8(cPair);
      e.uint8(cOutputs);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyErrorBase::decode(d)};
      if (!base ||
          !d.has_at_least(sizeof(cAdr) + sizeof(cPair) + sizeof(cOutputs)))
        return std::unexpected{std::errc::invalid_argument};
      Reply result{*base};
      result.cAdr = d.uint16();
      result.cPair = d.uint8();
      result.cOutputs = d.uint8();
      return result;
    }
  };

  template<detail::encoder E>
  auto encode(E e) const {
    return ReplyErrorBase(e);
  }
  template<detail::decoder D>
  static std::expected<AccessoryMemoryQuery, std::errc> decode(D& d) {
    if (auto const base{DecoderControlBase::decode(d)})
      return AccessoryMemoryQuery{*base};
    else return std::unexpected(base.error());
  }
};

struct AddressControl : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyHead {
    uint8_t payload{};
    uint8_t cOutputs{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyHead::encode(e);
      e.uint8(payload);
      e.uint8(cOutputs);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyHead::decode(d)};
      if (!base || d.has_at_least(sizeof(payload) + sizeof(cOutputs)))
        return std::unexpected{std::errc::invalid_argument};
      Reply result{*base};
      result.payload = d.uint8();
      result.cOutputs = d.uint8();
      return result;
    }
  };

  uint8_t cControl{};                ///>
  std::optional<uint8_t> cOutputs{}; ///>
  template<detail::encoder E>
  auto encode(E e) const {
    e = DecoderControlBase::encode(e);
    e.uint8(cControl);
    e.uint8(cOutputs);
    return e;
  }
  template<detail::decoder D>
  static std::expected<AddressControl, std::errc> decode(D& d) {
    auto const base{DecoderControlBase::decode(d)};
    if (!base || !d.has_at_least(sizeof(cControl)))
      return std::unexpected(std::errc::invalid_argument);
    AddressControl result{*base};
    result.cControl = d.uint8();
    // Add optional data
    result.cOutputs = d.s_uint8();
    return result;
  }
};

struct CommandStationIOQuery : public detail::CommandStationQueryBase {
  struct Reply : public detail::ReplyHead {
    uint8_t values{};
    uint16_t cCurrent1{};
    uint8_t cVoltage1{};
    uint16_t cCurrent2{};
    uint8_t cVoltage2{};
    uint8_t cAux{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyHead::encode(e);
      e.uint8(values);
      e.uint16(cCurrent1);
      e.uint8(cVoltage1);
      e.uint16(cCurrent2);
      e.uint8(cVoltage2);
      e.uint8(cAux);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyHead::decode(d)};
      if (!base || !d.has_at_least(sizeof(values) + sizeof(cCurrent1) +
                                   sizeof(cVoltage1) + sizeof(cCurrent2) +
                                   sizeof(cVoltage2) + sizeof(cAux)))
        return std::unexpected{std::errc::invalid_argument};
      Reply result{base};
      result.values = d.uint8();
      result.cCurrent1 = d.uint16();
      result.cVoltage1 = d.uint8();
      result.cCurrent2 = d.uint16();
      result.cVoltage2 = d.uint8();
      result.cAux = d.uint8();
      return result;
    }
  };
  template<detail::decoder D>
  static std::expected<CommandStationIOQuery, std::errc> decode(D& d) {
    if (auto const base{CommandStationQueryBase::decode(d)})
      return CommandStationIOQuery{*base};
    else return std::unexpected(base.error());
  }
};

struct CommandStationCvManip : public detail::Head {
  struct Reply : public detail::ReplyErrorBase {
    uint8_t value{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyErrorBase::encode(e);
      e.uint8(value);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyErrorBase::decode(d)};
      if (!base || !d.has_at_least(sizeof(value)))
        return std::unexpected{std::errc::invalid_argument};
      Reply result{*base};
      result.value = d.uint8();
      return result;
    }
  };

  uint16_t variable{};            ///> Cv Address
  std::optional<uint8_t> value{}; ///> Cv Value
  template<detail::encoder E>
  auto encode(E e) const {
    e = Head::encode(e);
    e.uint16(variable);
    e.uint8(value);
    return e;
  }
  template<detail::decoder D>
  static std::expected<CommandStationCvManip, std::errc> decode(D& d) {
    auto const base{Head::decode(d)};
    if (!base || !d.has_at_least(sizeof(variable)))
      return std::unexpected(std::errc::invalid_argument);
    CommandStationCvManip result{*base};
    result.variable = d.uint16();
    // Add optional data
    result.value = d.s_uint8();
    return result;
  }
};

struct CommandStationEquipmentQuery : public detail::CommandStationQueryBase {
  struct Reply : public detail::ReplyLongHead {
    uint16_t cAddress{};                 ///> CAN address
    uint8_t cDevice{};                   ///> Device ID
    uint8_t cRom_size{};                 ///> ROM size
    uint8_t cRam_size{};                 ///> RAM size
    uint16_t cPrintver{};                ///>
    uint16_t cVersion{};                 ///> Version Major/Minor
    uint8_t cDate_day{};                 ///> Software date Day
    uint8_t cDate_month{};               ///> Software date Month
    uint8_t cDate_century{};             ///> Software date Century
    uint8_t cDate_year{};                ///> Software date Year
    uint8_t cSwitches{};                 ///>
    uint8_t cDevelopVersion{};           ///> Version Patch
    uint16_t cBootRom{};                 ///>
    uint8_t cBootRom_develop{};          ///>
    uint8_t values{};                    ///>
    std::optional<uint8_t> cSerNum_hi{}; ///> Serial Number [0]
    std::optional<uint8_t> cSerNum_mh{}; ///> Serial Number [1]
    std::optional<uint8_t> cSerNum_ml{}; ///> Serial Number [2]
    std::optional<uint8_t> cSerNum_lo{}; ///> Serial Number [3]
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyLongHead::encode(e);
      e.uint16(cAddress);
      e.uint8(cDevice);
      e.uint8(cRom_size);
      e.uint8(cRam_size);
      e.uint16(cPrintver);
      e.uint16(cVersion);
      e.uint8(cDate_day);
      e.uint8(cDate_month);
      e.uint8(cDate_century);
      e.uint8(cDate_year);
      e.uint8(cSwitches);
      e.uint8(cDevelopVersion);
      e.uint16(cBootRom);
      e.uint8(cBootRom_develop);
      e.uint8(values);
      e.uint8(cSerNum_hi);
      e.uint8(cSerNum_mh);
      e.uint8(cSerNum_ml);
      e.uint8(cSerNum_lo);
      return e;
    }
  };

  template<detail::encoder E>
  auto encode(E e) const {
    return CommandStationQueryBase::encode(e);
  }
  template<detail::decoder D>
  static std::expected<CommandStationEquipmentQuery, std::errc> decode(D& d) {
    if (auto const base{CommandStationQueryBase::decode(d)})
      return CommandStationEquipmentQuery{*base};
    else return std::unexpected(base.error());
  }
};

struct SerialInfo : public detail::Head {
  uint8_t toolID{}; ///> Tool ID
  uint8_t action{}; ///>
  template<detail::encoder E>
  auto encode(E e) const {
    e = Head::encode(e);
    e.uint8(toolID);
    e.uint8(action);
    return e;
  }
  template<detail::decoder D>
  static std::expected<SerialInfo, std::errc> decode(D& d) {
    auto const base{Head::decode(d)};
    if (!base || !d.has_at_least(sizeof(toolID) + sizeof(action)))
      return std::unexpected(std::errc::invalid_argument);
    SerialInfo result{*base};
    result.toolID = d.uint8();
    result.action = d.uint8();
    return result;
  }
};

struct DecoderCvManip : public detail::DecoderControlBase {
  struct Reply : public detail::ReplyHead {
    uint16_t cAdr{};
    uint16_t variable{};
    uint8_t cValue{};
    uint8_t cError{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyHead::encode(e);
      e.uint16(cAdr);
      e.uint16(variable);
      e.uint8(cValue);
      e.uint8(cError);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{ReplyHead::decode(d)};
      if (!base || !d.has_at_least(sizeof(cAdr) + sizeof(variable) +
                                   sizeof(cValue) + sizeof(cError)))
        return std::unexpected{std::errc::invalid_argument};
      Reply result{*base};
      result.cAdr = d.uint16();
      result.variable = d.uint16();
      result.cValue = d.uint8();
      result.cError = d.uint8();
      return result;
    }
  };

  struct Busy : public detail::ReplyHead {
    uint8_t const busy{0x04u};
    uint16_t cAdr{};
    uint16_t variable{};
    std::optional<uint8_t> activeUSID{};
    std::optional<uint16_t> activeAddr{};
    std::optional<uint16_t> activeCv{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyHead::encode(e);
      e.uint8(busy);
      e.uint16(cAdr);
      e.uint16(variable);
      e.uint8(activeUSID);
      e.uint16(activeAddr);
      e.uint16(activeCv);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Busy, std::errc> decode(D& d) {
      auto const base{ReplyHead::decode(d)};
      if (!base ||
          !d.has_at_least(sizeof(busy) + sizeof(cAdr) + sizeof(variable)))
        return std::unexpected{std::errc::invalid_argument};
      Busy result{*base};
      d.uint8(); /// Busy is always 0x04
      result.cAdr = d.uint8();
      result.variable = d.uint8();
      result.activeUSID = d.s_uint8();
      result.activeAddr = d.s_uint8();
      result.activeCv = d.s_uint8();
      return result;
    }
  };

  struct Error : public detail::ReplyHead {
    uint16_t cAdr{};
    uint8_t cError{};
    template<detail::encoder E>
    auto encode(E e) const {
      e = ReplyHead::encode(e);
      e.uint16(cAdr);
      e.uint8(cError);
      return e;
    }
    template<detail::decoder D>
    static std::expected<Error, std::errc> decode(D& d) {
      auto const base{ReplyHead::decode(d)};
      if (!base || d.has_at_least(sizeof(cAdr) + sizeof(cError)))
        return std::unexpected{std::errc::invalid_argument};
      Error result{*base};
      result.cAdr = d.uint16();
      result.cError = d.uint8();
      return result;
    }
  };

  uint16_t variable{};            ///> Cv Address
  std::optional<uint8_t> value{}; ///> Cv Value
  template<detail::encoder E>
  auto encode(E e) const {
    e = DecoderControlBase::encode(e);
    e.uint16(variable);
    e.uint8(value);
    return e;
  }
  template<detail::decoder D>
  static std::expected<DecoderCvManip, std::errc> decode(D& d) {
    auto const base{DecoderControlBase::decode(d)};
    if (!base || !d.has_at_least(sizeof(variable)))
      return std::unexpected(std::errc::invalid_argument);
    DecoderCvManip result{*base};
    result.variable = d.uint16();
    // Add optional data
    result.value = d.s_uint8();
    return result;
  }
};

/// Message
using Message = std::variant<Ack,
                             Nak,
                             Reset,
                             TrackControl,
                             TrackControl::Reply,
                             DecoderControl,
                             DecoderControl::Reply,
                             InvertFunctionBits,
                             InvertFunctionBits::Reply,
                             Acceleration,
                             Acceleration::Reply,
                             ShuttleTrain,
                             ShuttleTrain::Reply,
                             Accessory,
                             Accessory::Reply,
                             LocoMemoryQuery,
                             LocoMemoryQuery::Reply,
                             AccessoryMemoryQuery,
                             AccessoryMemoryQuery::Reply,
                             AddressControl,
                             AddressControl::Reply,
                             CommandStationIOQuery,
                             CommandStationIOQuery::Reply,
                             CommandStationCvManip,
                             CommandStationCvManip::Reply,
                             CommandStationEquipmentQuery,
                             CommandStationEquipmentQuery::Reply,
                             SerialInfo,
                             DecoderCvManip,
                             DecoderCvManip::Reply,
                             DecoderCvManip::Busy,
                             DecoderCvManip::Error>;

template<typename T>
concept Long = requires(T t) {
  requires std::same_as<std::remove_cv_t<std::remove_reference_t<T>>,
                        CommandStationEquipmentQuery::Reply>;
};

template<typename T>
concept Short = requires(T t) { requires !Long<T>; };

using Packet = ztl::inplace_vector<uint8_t, sizeof(Message) * 2u>;

} // namespace ulf::mx1bin
