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
         typename OutputIt = std::back_insert_iterator<std::vector<uint8_t>>>
concept Encodable = requires {
  {
    &T::template encode<OutputIt>
  } -> std::same_as<OutputIt (T::*)(OutputIt&) const>;
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

struct Reset : public detail::Head {
  template<detail::decoder D>
  static std::expected<Reset, std::errc> decode(D& d) {
    if (auto const result{Head::decode(d)}) return Reset{*result};
    else return std::unexpected(result.error());
  }
};

struct Nak : public detail::Head {
  template<detail::decoder D>
  static std::expected<Nak, std::errc> decode(D& d) {
    if (auto const result{Head::decode(d)}) return Nak{*result};
    else return std::unexpected(result.error());
  }
  template<std::ranges::input_range R>
  static auto decode(R const& r) {
    detail::Decoder d(r);
    return decode(d);
  }
};

struct TrackControl : public detail::Head {
  struct Reply : public detail::ReplyHead {
    uint8_t statusBits{};
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyHead::encode(out);
      detail::encode_8(statusBits, out);
      return out;
    }
  };

  uint8_t cAction{}; ///> Action
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      return ReplyDecoderControlBase::encode(out);
    }
  };

  uint8_t cSpeed{};                ///> Speed
  std::optional<uint8_t> cData1{}; ///>
  std::optional<uint8_t> cData2{}; ///>
  std::optional<uint8_t> cData3{}; ///>
  std::optional<uint8_t> cData4{}; ///>
  std::optional<uint8_t> cData5{}; ///>
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      return ReplyDecoderControlBase::encode(out);
    }
  };

  uint8_t cData1{}; ///>
  uint8_t cData2{}; ///>
  uint8_t cData3{}; ///>
  uint8_t cData4{}; ///>
  uint8_t cData5{}; ///>
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      return ReplyDecoderControlBase::encode(out);
    }
  };

  uint8_t cAzBz{}; ///> Accelleration / Deccelleration
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
  };

  template<detail::decoder D>
  static std::expected<ShuttleTrain, std::errc> decode(D& d) {
    if (auto const base{ShuttleTrain_Accessory_Base::decode(d)})
      return ShuttleTrain{*base};
    else return std::unexpected(base.error());
  }
};

struct Accessory : public detail::ShuttleTrain_Accessory_Base {
  struct Reply : public detail::ReplyDecoderControlBase {
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      return ReplyDecoderControlBase::encode(out);
    }
  };
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyErrorBase::encode(out);
      detail::encode_16(cAdr, out);
      detail::encode_8(cSpeed, out);
      detail::encode_8(cData1, out);
      detail::encode_8(cData2, out);
      detail::encode_8(cData3, out);
      detail::encode_8(cAzBz, out);
      detail::encode_8(cStatus, out);
      detail::encode_8(cData4, out);
      detail::encode_8(cData5, out);
      return out;
    }
  };

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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyErrorBase::encode(out);
      detail::encode_16(cAdr, out);
      detail::encode_8(cPair, out);
      detail::encode_8(cOutputs, out);
      return out;
    }
  };

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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyHead::encode(out);
      detail::encode_8(payload, out);
      detail::encode_8(cOutputs, out);
      return out;
    }
  };

  uint8_t cControl{};                ///>
  std::optional<uint8_t> cOutputs{}; ///>
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyHead::encode(out);
      detail::encode_8(values, out);
      detail::encode_16(cCurrent1, out);
      detail::encode_8(cVoltage1, out);
      detail::encode_16(cCurrent2, out);
      detail::encode_8(cVoltage2, out);
      detail::encode_8(cAux, out);
      return out;
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyErrorBase::encode(out);
      detail::encode_8(value, out);
      return out;
    }
  };

  uint16_t variable{};            ///> Cv Address
  std::optional<uint8_t> value{}; ///> Cv Value
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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyLongHead::encode(out);
      detail::encode_16(cAddress, out);
      detail::encode_8(cDevice, out);
      detail::encode_8(cRom_size, out);
      detail::encode_8(cRam_size, out);
      detail::encode_16(cPrintver, out);
      detail::encode_16(cVersion, out);
      detail::encode_8(cDate_day, out);
      detail::encode_8(cDate_month, out);
      detail::encode_8(cDate_century, out);
      detail::encode_8(cDate_year, out);
      detail::encode_8(cSwitches, out);
      detail::encode_8(cDevelopVersion, out);
      detail::encode_16(cBootRom, out);
      detail::encode_8(cBootRom_develop, out);
      detail::encode_8(values, out);
      if (cSerNum_hi) detail::encode_8(*cSerNum_hi, out);
      if (cSerNum_mh) detail::encode_8(*cSerNum_mh, out);
      if (cSerNum_ml) detail::encode_8(*cSerNum_ml, out);
      if (cSerNum_lo) detail::encode_8(*cSerNum_lo, out);
      return out;
    }
  };

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
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyHead::encode(out);
      detail::encode_16(cAdr, out);
      detail::encode_16(variable, out);
      detail::encode_8(cValue, out);
      detail::encode_8(cError, out);
      return out;
    }
  };

  struct Busy : public detail::ReplyHead {
    uint8_t const busy{0x04u};
    uint16_t cAdr{};
    uint16_t variable{};
    std::optional<uint8_t> activeUSID{};
    std::optional<uint16_t> activeAddr{};
    std::optional<uint16_t> activeCv{};
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyHead::encode(out);
      detail::encode_8(busy, out);
      detail::encode_16(cAdr, out);
      detail::encode_16(variable, out);
      if (!activeUSID) return out;
      detail::encode_8(*activeUSID, out);
      if (!activeAddr) return out;
      detail::encode_16(*activeAddr, out);
      if (!activeCv) return out;
      detail::encode_16(*activeCv, out);
      return out;
    }
  };

  struct Error : public detail::ReplyHead {
    uint16_t cAdr{};
    uint8_t cError{};
    template<std::output_iterator<uint8_t> OutputIt>
    auto encode(OutputIt& out) const {
      ReplyHead::encode(out);
      detail::encode_16(cAdr, out);
      detail::encode_8(cError, out);
      return out;
    }
  };

  uint16_t variable{};            ///> Cv Address
  std::optional<uint8_t> value{}; ///> Cv Value
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

struct Ack : public detail::ReplyHead {
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) const {
    return ReplyHead::encode(out);
  }
  template<detail::decoder D>
  static std::expected<Ack, std::errc> decode(D& d) {
    if (auto const base{ReplyHead::decode(d)}) return Ack{*base};
    else return std::unexpected(base.error());
  }
};

/// Message
using Message = std::variant<Reset,
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

static_assert(Encodable<TrackControl::Reply>);
static_assert(!Encodable<TrackControl>);

} // namespace ulf::mx1bin
