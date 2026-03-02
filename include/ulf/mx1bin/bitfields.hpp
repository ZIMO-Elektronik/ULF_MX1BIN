// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin common bitfields
///
/// \file   ulf/mx1bin/bitfields.hpp
/// \author Jonas Gahlert
/// \date   02/10/2025

#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#include <type_traits>
#include <ztl/bits.hpp>
#include "frame.hpp"
#include "info.hpp"
#include "types.hpp"
#include "utility.hpp"

namespace ulf::mx1bin::bitfields {

/// Get concurrent bits from bitfield
/// \tparam P LSB position
/// \tparam N Bit count
/// \tparam T Type of Field
/// \param  t Field
/// \return bits P .. P+N as value
template<std::size_t P, std::size_t N, std::unsigned_integral T>
requires((N + P) <= (sizeof(T) * 8))
constexpr T bits(T const t) {
  return {static_cast<T>(t >> P & ((1u << N) - 1u))};
}

struct Info {
  FrameType frameType : 1;     ///< Frame type
  MessageType messageType : 2; ///< Message type
  Sender sender : 1;           ///< Sender
  StationType stationType : 4; ///< Command station type
  constexpr Info() = default;
  constexpr Info(Info const&) = default;
  constexpr Info(FrameType _frameType,
                 MessageType _messageType,
                 Sender _sender,
                 StationType _stationType)
    : frameType{_frameType}, messageType{_messageType}, sender{_sender},
      stationType{_stationType} {}
  constexpr Info(uint8_t const val) {
    new (this) Info(
      static_cast<FrameType>(ztl::map_value_from<ztl::mask<7u>>(val)),
      static_cast<MessageType>(ztl::map_value_from<ztl::mask<5u, 6u>>(val)),
      static_cast<Sender>(ztl::map_value_from<ztl::mask<4u>>(val)),
      static_cast<StationType>(
        ztl::map_value_from<ztl::mask<3u, 2u, 1u, 0u>>(val)));
  }
  constexpr Info& operator=(uint8_t const val) { return *new (this) Info(val); }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(
      ztl::map_value_to<ztl::mask<7u>>(std::to_underlying(frameType)) |
      ztl::map_value_to<ztl::mask<6u, 5u>>(std::to_underlying(messageType)) |
      ztl::map_value_to<ztl::mask<4u>>(std::to_underlying(sender)) |
      ztl::map_value_to<ztl::mask<3u, 2u, 1u, 0u>>(
        std::to_underlying(stationType)))};
  }
  constexpr bool operator==(Info const&) const = default;
};

struct TrackStatus {
  uint8_t dcc : 1u;           ///< DCC Enabled?
  uint8_t motorola : 1u;      ///< Motorola enabled?
  uint8_t ues : 1u;           ///< UES enabled?
  uint8_t trackVoltage : 1u;  ///< Track voltage enabled?
  uint8_t broadcastStop : 1u; ///< Stop broadcast enabled?
  constexpr TrackStatus() = default;
  constexpr TrackStatus(TrackStatus const&) = default;
  constexpr TrackStatus(uint8_t const _dcc,
                        uint8_t const _motorola,
                        uint8_t const _ues,
                        uint8_t const _trackVoltage,
                        uint8_t const _broadcastStop)
    : dcc{_dcc}, motorola{_motorola}, ues{_ues}, trackVoltage{_trackVoltage},
      broadcastStop{_broadcastStop} {}
  constexpr TrackStatus(uint8_t const val) {
    new (this) TrackStatus(ztl::map_value_from<ztl::mask<7u>>(val),
                           ztl::map_value_from<ztl::mask<6u>>(val),
                           ztl::map_value_from<ztl::mask<2u>>(val),
                           ztl::map_value_from<ztl::mask<1u>>(val),
                           ztl::map_value_from<ztl::mask<0u>>(val));
  }
  constexpr TrackStatus& operator=(uint8_t const val) {
    return *new (this) TrackStatus(val);
  }
  constexpr explicit operator uint8_t() const {
    return {
      static_cast<uint8_t>(ztl::map_value_to<ztl::mask<7u>>(dcc) |
                           ztl::map_value_to<ztl::mask<6u>>(motorola) |
                           ztl::map_value_to<ztl::mask<2u>>(ues) |
                           ztl::map_value_to<ztl::mask<1u>>(trackVoltage) |
                           ztl::map_value_to<ztl::mask<0u>>(broadcastStop))};
  }
  constexpr bool operator==(TrackStatus const&) const = default;
};

struct ControlSpeed {
  uint8_t stop : 1u;  ///< Emergency stop
  uint8_t speed : 7u; ///< Speed
  constexpr ControlSpeed() = default;
  constexpr ControlSpeed(ControlSpeed const&) = default;
  constexpr ControlSpeed(uint8_t const _stop, uint8_t const _speed)
    : stop{_stop}, speed{_speed} {}
  constexpr ControlSpeed(uint8_t const val) {
    new (this) ControlSpeed(
      ztl::map_value_from<ztl::mask<7u>>(val),
      ztl::map_value_from<ztl::mask<6u, 5u, 4u, 3u, 2u, 1u, 0u>>(val));
  }
  constexpr ControlSpeed& operator=(uint8_t const val) {
    return *new (this) ControlSpeed(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(
      ztl::map_value_to<ztl::mask<7u>>(stop) |
      ztl::map_value_to<ztl::mask<6u, 5u, 4u, 3u, 2u, 1u, 0u>>(speed))};
  }
  constexpr bool operator==(ControlSpeed const&) const = default;
};

struct ControlData {
  uint8_t manual : 1u;     ///< Manual control
  uint8_t direction : 1u;  ///< Direction
  uint8_t headlights : 1u; ///< F0 (Headlights)
  uint8_t speed_step : 2u; ///< Speed step system
  uint8_t az_enable : 1u;  ///< Accel time enable
  uint8_t bz_enable : 1u;  ///< Break time enable
  constexpr ControlData() = default;
  constexpr ControlData(ControlData const&) = default;
  constexpr ControlData(uint8_t const _manual,
                        uint8_t const _direction,
                        uint8_t const _headlights,
                        uint8_t const _speed_step,
                        uint8_t const _az_enable,
                        uint8_t const _bz_enable)
    : manual{_manual}, direction{_direction}, headlights{_headlights},
      speed_step{_speed_step}, az_enable{_az_enable}, bz_enable{_bz_enable} {}
  constexpr ControlData(uint8_t const val) {
    new (this) ControlData(ztl::map_value_from<ztl::mask<7u>>(val),
                           ztl::map_value_from<ztl::mask<5u>>(val),
                           ztl::map_value_from<ztl::mask<4u>>(val),
                           ztl::map_value_from<ztl::mask<3u, 2u>>(val),
                           ztl::map_value_from<ztl::mask<1u>>(val),
                           ztl::map_value_from<ztl::mask<0u>>(val));
  }
  constexpr ControlData& operator=(uint8_t const val) {
    return *new (this) ControlData(val);
  }
  constexpr explicit operator uint8_t() const {
    return {
      static_cast<uint8_t>(ztl::map_value_to<ztl::mask<7u>>(manual) |
                           ztl::map_value_to<ztl::mask<5u>>(direction) |
                           ztl::map_value_to<ztl::mask<4u>>(headlights) |
                           ztl::map_value_to<ztl::mask<3u, 2u>>(speed_step) |
                           ztl::map_value_to<ztl::mask<1u>>(az_enable) |
                           ztl::map_value_to<ztl::mask<0u>>(bz_enable))};
  }
  constexpr bool operator==(ControlData const&) const = default;
};

struct ControlPayload {
  uint8_t format : 2;      ///< Address format specification
  SpeedStep speedStep : 2; ///< Speed step system
  uint8_t trackState : 1;  ///< Track state
  constexpr ControlPayload() = default;
  constexpr ControlPayload(ControlPayload const&) = default;
  constexpr ControlPayload(uint8_t const _format,
                           SpeedStep const _speedStep,
                           uint8_t const _trackState)
    : format{_format}, speedStep{_speedStep}, trackState{_trackState} {}
  constexpr ControlPayload(uint8_t const val) {
    new (this) ControlPayload(
      ztl::map_value_from<ztl::mask<7u, 6u>>(val),
      static_cast<SpeedStep>(ztl::map_value_from<ztl::mask<3u, 2u>>(val)),
      ztl::map_value_from<ztl::mask<0u>>(val));
  }
  constexpr ControlPayload& operator=(uint8_t const val) {
    return *new (this) ControlPayload(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(
      ztl::map_value_to<ztl::mask<7u, 6u>>(format) |
      (ztl::map_value_to<ztl::mask<3u, 2u>>(std::to_underlying(speedStep)) |
       ztl::map_value_to<ztl::mask<0u>>(trackState)))};
  }
  constexpr bool operator==(ControlPayload const&) const = default;
};

struct AddressControl_Control {
  uint8_t set : 1u;  ///< [0] Query [1] Set
  uint8_t type : 1u; ///< Address type
  uint8_t lock : 1u; ///< Lock address
  uint8_t log : 1u;  ///< Log external changes
  constexpr AddressControl_Control() = default;
  constexpr AddressControl_Control(AddressControl_Control const&) = default;
  constexpr AddressControl_Control(uint8_t const _set,
                                   uint8_t const _type,
                                   uint8_t const _lock,
                                   uint8_t const _log)
    : set{_set}, type{_type}, lock{_lock}, log{_log} {}
  constexpr AddressControl_Control(uint8_t const val) {
    new (this) AddressControl_Control(ztl::map_value_from<ztl::mask<7u>>(val),
                                      ztl::map_value_from<ztl::mask<5u>>(val),
                                      ztl::map_value_from<ztl::mask<1u>>(val),
                                      ztl::map_value_from<ztl::mask<0u>>(val));
  }
  constexpr AddressControl_Control& operator=(uint8_t const val) {
    return *new (this) AddressControl_Control(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(ztl::map_value_to<ztl::mask<7u>>(set) |
                                 ztl::map_value_to<ztl::mask<5u>>(type) |
                                 ztl::map_value_to<ztl::mask<1u>>(lock) |
                                 ztl::map_value_to<ztl::mask<0u>>(log))};
  }
  constexpr bool operator==(AddressControl_Control const&) const = default;
};

struct AddressControl_Payload {
  uint8_t format : 2u;    ///< Address format specification
  uint8_t type : 1u;      ///< Address type
  uint8_t speedStep : 2u; ///< Speed step system
  uint8_t lock : 1u;      ///< Lock address
  uint8_t log : 1u;       ///< Log external changes
  constexpr AddressControl_Payload() = default;
  constexpr AddressControl_Payload(AddressControl_Payload const&) = default;
  constexpr AddressControl_Payload(uint8_t const _format,
                                   uint8_t const _type,
                                   uint8_t const _speedStep,
                                   uint8_t const _lock,
                                   uint8_t const _log)
    : format{_format}, type{_type}, speedStep{_speedStep}, lock{_lock},
      log{_log} {}
  constexpr AddressControl_Payload(uint8_t const val) {
    new (this)
      AddressControl_Payload(ztl::map_value_from<ztl::mask<7u, 6u>>(val),
                             ztl::map_value_from<ztl::mask<5u>>(val),
                             ztl::map_value_from<ztl::mask<3u, 2u>>(val),
                             ztl::map_value_from<ztl::mask<1u>>(val),
                             ztl::map_value_from<ztl::mask<0u>>(val));
  }
  constexpr AddressControl_Payload& operator=(uint8_t const val) {
    return *new (this) AddressControl_Payload(val);
  }
  constexpr explicit operator uint8_t() const {
    return {
      static_cast<uint8_t>(ztl::map_value_to<ztl::mask<7u, 6u>>(format) |
                           ztl::map_value_to<ztl::mask<5u>>(type) |
                           ztl::map_value_to<ztl::mask<3u, 2u>>(speedStep) |
                           ztl::map_value_to<ztl::mask<1u>>(lock) |
                           ztl::map_value_to<ztl::mask<0u>>(log))};
  }
  constexpr bool operator==(AddressControl_Payload const&) const = default;
};

struct DecoderAddress {
  uint16_t format : 2u;   ///< Address format specification
  uint16_t address : 14u; ///< Address value
  constexpr DecoderAddress() = default;
  constexpr DecoderAddress(DecoderAddress const&) = default;
  constexpr DecoderAddress(uint16_t const& _format, uint16_t const& _address)
    : format{_format}, address{_address} {}
  constexpr DecoderAddress(uint16_t const val) {
    new (this) DecoderAddress(
      ztl::map_value_from<ztl::mask<15u, 14u>>(val),
      ztl::map_value_from<
        ztl::mask<13u, 12u, 11u, 10u, 9u, 8u, 7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u>>(
        val));
  }
  constexpr DecoderAddress& operator=(uint16_t const val) {
    return *new (this) DecoderAddress(val);
  }
  constexpr explicit operator uint16_t() const {
    return {static_cast<uint16_t>(
      ztl::map_value_to<ztl::mask<15u, 14u>>(format) |
      ztl::map_value_to<
        ztl::mask<13u, 12u, 11u, 10u, 9u, 8u, 7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u>>(
        address))};
  }
  constexpr bool operator==(DecoderAddress const&) const = default;
};

} // namespace ulf::mx1bin::bitfields

#pragma GCC diagnostic pop
