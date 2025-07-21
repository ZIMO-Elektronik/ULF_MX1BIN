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
#include "message_base.hpp"
#include "utility.hpp"

namespace ulf::mx1bin {

struct Reset : public detail::Head {
  Reset& decode(std::span<uint8_t const> bytes) {
    Head::decode(bytes);
    return *this;
  }
};

struct Nak : public detail::Head {
  Nak& decode(std::span<uint8_t const> bytes) {
    Head::decode(bytes);
    return *this;
  }
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    return Head::encode(out);
  }
};

struct TrackControl : public detail::Head {
  uint8_t cAction{};
  TrackControl& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    Head::decode(iter);
    cAction = detail::decode(iter);
    return *this;
  }
};

struct DecoderControl : public detail::DecoderControlBase {
  uint8_t cSpeed{};
  std::optional<uint8_t> cData1{};
  std::optional<uint8_t> cData2{};
  std::optional<uint8_t> cData3{};
  std::optional<uint8_t> cData4{};
  std::optional<uint8_t> cData5{};
  DecoderControl& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    auto end{cend(bytes)};
    DecoderControlBase::decode(iter);
    cSpeed = detail::decode(iter);
    cData1 =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    cData2 =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    cData3 =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    cData4 =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    cData5 =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    return *this;
  }
};

struct InvertFunctionBits : public detail::DecoderControlBase {
  uint8_t cData1{};
  uint8_t cData2{};
  uint8_t cData3{};
  uint8_t cData4{};
  uint8_t cData5{};
  InvertFunctionBits& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    DecoderControlBase::decode(iter);
    cData1 = detail::decode(iter);
    cData2 = detail::decode(iter);
    cData3 = detail::decode(iter);
    cData4 = detail::decode(iter);
    cData5 = detail::decode(iter);
    return *this;
  }
};

struct Acceleration : public detail::DecoderControlBase {
  uint8_t cAzBz{};
  Acceleration& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    DecoderControlBase::decode(iter);
    cAzBz = detail::decode(iter);
    return *this;
  }
};

struct ShuttleTrain : public detail::ShuttleTrain_Accessory_Base {
  ShuttleTrain& decode(std::span<uint8_t const> bytes) {
    ShuttleTrain_Accessory_Base::decode(bytes);
    return *this;
  }
};

struct Accessory : public detail::ShuttleTrain_Accessory_Base {
  Accessory& decode(std::span<uint8_t const> bytes) {
    ShuttleTrain_Accessory_Base::decode(bytes);
    return *this;
  }
};

struct LocoMemoryQuery : public detail::DecoderControlBase {
  LocoMemoryQuery& decode(std::span<uint8_t const> bytes) {
    DecoderControlBase::decode(bytes);
    return *this;
  }
};

struct AccessoryMemoryQuery : public detail::DecoderControlBase {
  AccessoryMemoryQuery& decode(std::span<uint8_t const> bytes) {
    DecoderControlBase::decode(bytes);
    return *this;
  }
};

struct AddressControl : public detail::DecoderControlBase {
  uint8_t cControl{};
  std::optional<uint8_t> cOutputs{};
  AddressControl& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    auto end{cend(bytes)};
    DecoderControlBase::decode(iter);
    cControl = detail::decode(iter);
    cOutputs =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    return *this;
  }
};

struct CommandStationIOQuery : public detail::CommandStationQueryBase {
  CommandStationIOQuery& decode(std::span<uint8_t const> bytes) {
    CommandStationQueryBase::decode(bytes);
    return *this;
  }
};

struct CommandStationCvManip : public detail::Head {
  uint8_t variable_hi{};
  uint8_t variable_lo{};
  std::optional<uint8_t> value{};
  CommandStationCvManip& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    auto end{cend(bytes)};
    Head::decode(iter);
    variable_hi = detail::decode(iter);
    variable_lo = detail::decode(iter);
    value =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    return *this;
  }
};

struct CommandStationEquipmentQuery : public detail::CommandStationQueryBase {
  CommandStationEquipmentQuery& decode(std::span<uint8_t const> bytes) {
    CommandStationQueryBase::decode(bytes);
    return *this;
  }
};

struct SerialInfo : public detail::Head {
  uint8_t toolID{};
  uint8_t action{};
  SerialInfo& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    Head::decode(iter);
    toolID = detail::decode(iter);
    action = detail::decode(iter);
    return *this;
  }
};

struct DecoderCvManip : public detail::DecoderControlBase {
  uint8_t variable_hi{};
  uint8_t variable_lo{};
  std::optional<uint8_t> value{};
  DecoderCvManip& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    auto end{cend(bytes)};
    DecoderControlBase::decode(iter);
    variable_hi = detail::decode(iter);
    variable_lo = detail::decode(iter);
    value =
      (iter != end) ? std::make_optional(detail::decode(iter)) : std::nullopt;
    return *this;
  }
};

struct Ack : public detail::ReplyHead {};

struct TrackControlReply : public detail::ReplyHead {};

struct DecoderControlReply : public detail::ReplyDecoderControlBase {};

struct InvertFunctionBitsReply : public detail::ReplyDecoderControlBase {};

struct AccelerationReply : public detail::ReplyDecoderControlBase {};

struct ShuttleTrainReply : public detail::ReplyDecoderControlBase {};

struct AccessoryReply : public detail::ReplyDecoderControlBase {};

struct LocoMemoryQueryReply : public detail::ReplyErrorBase {
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t cSpeed{};
  uint8_t cData1{};
  uint8_t cData2{};
  uint8_t cData3{};
  uint8_t cAzBz{};
  uint8_t cStatus{};
  uint8_t cData4{};
  uint8_t cData5{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyErrorBase::encode(out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(cSpeed, out);
    detail::encode(cData1, out);
    detail::encode(cData2, out);
    detail::encode(cData3, out);
    detail::encode(cAzBz, out);
    detail::encode(cStatus, out);
    detail::encode(cData4, out);
    detail::encode(cData5, out);
    return out;
  }
};

struct AccessoryMemoryQueryReply : public detail::ReplyErrorBase {
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t cPair{};
  uint8_t cOutputs{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyErrorBase::encode(out);
    detail::encode(error, out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(cPair, out);
    detail::encode(cOutputs, out);
    return out;
  }
};

struct AddressControlReply : public detail::ReplyHead {
  uint8_t payload{};
  uint8_t cOutputs{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyHead::encode(out);
    detail::encode(payload, out);
    detail::encode(cOutputs, out);
    return out;
  }
};

struct CommandStationIOQueryReply : public detail::ReplyHead {
  uint8_t values{};
  uint8_t cCurrent1_hi{};
  uint8_t cCurrent1_lo{};
  uint8_t cVoltage1{};
  uint8_t cCurrent2_hi{};
  uint8_t cCurrent2_lo{};
  uint8_t cVoltage2{};
  uint8_t cAux{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyHead::encode(out);
    detail::encode(values, out);
    detail::encode(cCurrent1_hi, out);
    detail::encode(cCurrent1_lo, out);
    detail::encode(cVoltage1, out);
    detail::encode(cCurrent2_hi, out);
    detail::encode(cCurrent2_lo, out);
    detail::encode(cVoltage2, out);
    detail::encode(cAux, out);
    return out;
  }
};

struct CommandStationCvManipReply : public detail::ReplyErrorBase {
  uint8_t value{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyErrorBase::encode(out);
    detail::encode(value, out);
    return out;
  }
};

struct CommandStationEquipmentQueryReply {
  uint8_t cAddress_hi{};
  uint8_t cAddress_lo{};
  uint8_t cDevice{};
  uint8_t cRom_size{};
  uint8_t cRam_size{};
  uint8_t cPrintver_hi{};
  uint8_t cPrintver_lo{};
  uint8_t cVersion_hi{};
  uint8_t cVersion_lo{};
  uint8_t cDate_day{};
  uint8_t cDate_month{};
  uint8_t cDate_century{};
  uint8_t cDate_year{};
  uint8_t cSwitches{};
  uint8_t cDevelopVersion{};
  uint8_t cBootRom_hi{};
  uint8_t cBootRom_lo{};
  uint8_t cBootRom_develop{};
  uint8_t values{};
  uint8_t cSerNum_hi{};
  uint8_t cSerNum_mh{};
  uint8_t cSerNum_ml{};
  uint8_t cSerNum_lo{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    detail::encode(cAddress_hi, out);
    detail::encode(cAddress_lo, out);
    detail::encode(cDevice, out);
    detail::encode(cRom_size, out);
    detail::encode(cRam_size, out);
    detail::encode(cPrintver_hi, out);
    detail::encode(cPrintver_lo, out);
    detail::encode(cVersion_hi, out);
    detail::encode(cVersion_lo, out);
    detail::encode(cDate_day, out);
    detail::encode(cDate_month, out);
    detail::encode(cDate_century, out);
    detail::encode(cDate_year, out);
    detail::encode(cSwitches, out);
    detail::encode(cDevelopVersion, out);
    detail::encode(cBootRom_hi, out);
    detail::encode(cBootRom_lo, out);
    detail::encode(cBootRom_develop, out);
    detail::encode(values, out);
    detail::encode(cSerNum_hi, out);
    detail::encode(cSerNum_mh, out);
    detail::encode(cSerNum_ml, out);
    detail::encode(cSerNum_lo, out);
    return out;
  }
};

struct DecoderCvManipReply : public detail::ReplyHead {
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t variable_hi{};
  uint8_t variable_lo{};
  uint8_t cValue{};
  uint8_t cError{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyHead::encode(out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(variable_hi, out);
    detail::encode(variable_lo, out);
    detail::encode(cValue, out);
    detail::encode(cError, out);
    return out;
  }
};

struct DecoderCvManipErrorReply : public detail::ReplyHead {
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t cError{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyHead::encode(out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(cError, out);
    return out;
  }
};

struct DecoderCvManipBusyReply : public detail::ReplyHead {
  uint8_t const busy{0x04u};
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t variable_hi{};
  uint8_t variable_lo{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    ReplyHead::encode(out);
    detail::encode(busy, out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(variable_hi, out);
    detail::encode(variable_lo, out);
    return out;
  }
};

/// Message
using Message = std::variant<Reset,
                             TrackControl,
                             DecoderControl,
                             InvertFunctionBits,
                             Acceleration,
                             ShuttleTrain,
                             Accessory,
                             LocoMemoryQuery,
                             AccessoryMemoryQuery,
                             AddressControl,
                             CommandStationIOQuery,
                             CommandStationCvManip,
                             CommandStationEquipmentQuery,
                             SerialInfo,
                             DecoderCvManip>;

/// Response
using Response = std::variant<Ack,
                              Nak,
                              TrackControlReply,
                              DecoderControlReply,
                              InvertFunctionBitsReply,
                              ShuttleTrainReply,
                              AccessoryReply,
                              LocoMemoryQueryReply,
                              AccessoryMemoryQueryReply,
                              AddressControlReply,
                              CommandStationIOQueryReply,
                              CommandStationCvManipReply,
                              CommandStationEquipmentQueryReply,
                              DecoderCvManipReply,
                              DecoderCvManipErrorReply,
                              DecoderCvManipBusyReply>;

} // namespace ulf::mx1bin
