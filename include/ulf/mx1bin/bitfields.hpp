#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#include <type_traits>
#include "types.hpp"

namespace ulf::mx1bin::bitfields {

/// Get concurrent bits from bitfield
/// @tparam P LSB position
/// @tparam N Bit count
/// @tparam T Type of Field
/// @param t Field
/// @return bits P .. P+N as value
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
  Info() = default;
  Info(uint8_t const val)
    : frameType{static_cast<FrameType>(bits<7u, 1u>(val))},
      messageType{static_cast<MessageType>(bits<5u, 2u>(val))},
      sender{static_cast<Sender>(bits<4u, 1u>(val))},
      stationType{static_cast<StationType>(bits<0u, 4u>(val))} {}
  Info& operator=(uint8_t const val) {
    frameType = static_cast<FrameType>(bits<7u, 1u>(val));
    messageType = static_cast<MessageType>(bits<5u, 2u>(val));
    sender = static_cast<Sender>(bits<4u, 1u>(val));
    stationType = static_cast<StationType>(bits<0u, 4u>(val));
    return *this;
  }
  explicit operator uint8_t() const {
    return {static_cast<uint8_t>(std::to_underlying(frameType) << 7u |
                                 std::to_underlying(messageType) << 5u |
                                 std::to_underlying(sender) << 4u |
                                 std::to_underlying(stationType) << 0u)};
  }
  bool operator==(Info const&) const = default;
};

struct TrackStatus {
  uint8_t dcc : 1u;
  uint8_t motorola : 1u;
  uint8_t ues : 1u;
  uint8_t trackVoltage : 1u;
  uint8_t broadcastStop : 1u;
  TrackStatus() = default;
  TrackStatus(uint8_t const val)
    : dcc{bits<7u, 1u>(val)}, motorola{bits<6u, 1u>(val)},
      ues{bits<2u, 1u>(val)}, trackVoltage{bits<1u, 1u>(val)},
      broadcastStop{bits<0u, 1u>(val)} {}
  TrackStatus& operator=(uint8_t const val) {
    dcc = bits<7u, 1u>(val);
    motorola = bits<6u, 1u>(val);
    ues = bits<2u, 1u>(val);
    trackVoltage = bits<1u, 1u>(val);
    broadcastStop = bits<0u, 1u>(val);
    return *this;
  }
  explicit operator uint8_t() const {
    return {static_cast<uint8_t>(dcc << 7u | motorola << 6u | ues << 2u |
                                 trackVoltage << 1u | broadcastStop << 0u)};
  }
  bool operator==(TrackStatus const&) const = default;
};

struct ControlData {
  uint8_t manual : 1u;     ///< Manual control
  uint8_t direction : 1u;  ///< Direction
  uint8_t headlights : 1u; ///< F0 (Headlights)
  uint8_t speed_step : 2u; ///< Speed step system
  uint8_t az_enable : 1u;  ///< Accel time enable
  uint8_t bz_enable : 1u;  ///< Break time enable
  ControlData() = default;
  ControlData(uint8_t const val)
    : manual{bits<7u, 1u>(val)}, direction{bits<5u, 1u>(val)},
      headlights{bits<4u, 1u>(val)}, speed_step{bits<2u, 2u>(val)},
      az_enable{bits<1u, 1u>(val)}, bz_enable{bits<0u, 1u>(val)} {}
  ControlData& operator=(uint8_t const val) {
    manual = bits<7u, 1u>(val);
    direction = bits<5u, 1u>(val);
    headlights = bits<4u, 1u>(val);
    speed_step = bits<2u, 2u>(val);
    az_enable = bits<1u, 1u>(val);
    bz_enable = bits<0u, 1u>(val);
    return *this;
  }
  explicit operator uint8_t() const {
    return {static_cast<uint8_t>(manual << 7u | direction << 5u |
                                 headlights << 4u | speed_step << 2u |
                                 az_enable << 1u | bz_enable << 0u)};
  }
  bool operator==(ControlData const&) const = default;
};

struct ControlPayload {
  uint8_t format : 2;      ///< Address format specification
  SpeedStep speedStep : 2; ///< Speed step system
  uint8_t trackState : 1;  ///< Track state
  ControlPayload() = default;
  ControlPayload(uint8_t const val)
    : format{bits<6u, 2u>(val)}, speedStep{bits<2u, 2u>(val)},
      trackState{bits<0u, 1u>(val)} {}
  ControlPayload& operator=(uint8_t const val) {
    format = bits<6u, 2u>(val);
    speedStep = static_cast<SpeedStep>(bits<2u, 2u>(val));
    trackState = bits<0u, 1u>(val);
    return *this;
  }
  explicit operator uint8_t() const {
    return {static_cast<uint8_t>(
      format << 6u | std::to_underlying(speedStep) << 2u | trackState)};
  }
  bool operator==(ControlPayload const&) const = default;
};

struct AddressControl_Control {
  uint8_t set : 1u;  ///< [0] Query [1] Set
  uint8_t type : 1u; ///< Address type
  uint8_t lock : 1u; ///< Lock address
  uint8_t log : 1u;  ///< Log external changes
  AddressControl_Control() = default;
  AddressControl_Control(uint8_t const val)
    : set{bits<7u, 1u>(val)}, type{bits<5u, 1u>(val)}, lock{bits<1u, 1u>(val)},
      log{bits<0u, 1u>(val)} {}
  AddressControl_Control& operator=(uint8_t const val) {
    set = bits<7u, 1u>(val);
    type = bits<5u, 1u>(val);
    lock = bits<1u, 1u>(val);
    log = bits<0u, 1u>(val);
    return *this;
  }
  explicit operator uint8_t() const {
    return {
      static_cast<uint8_t>(set << 7u | type << 5u | lock << 1u | log << 0u)};
  }
  bool operator==(AddressControl_Control const&) const = default;
};

struct AddressControl_Payload {
  uint8_t format : 2u;    ///< Address format specification
  uint8_t type : 1u;      ///< Address type
  uint8_t speedStep : 2u; ///< Speed step system
  uint8_t lock : 1u;      ///< Lock address
  uint8_t log : 1u;       ///< Log external changes
  AddressControl_Payload() = default;
  AddressControl_Payload(uint8_t const val)
    : format{bits<6u, 2u>(val)}, type{bits<5u, 1u>(val)},
      speedStep{bits<2u, 2u>(val)}, lock{bits<1u, 1u>(val)},
      log{bits<0u, 1u>(val)} {}
  AddressControl_Payload& operator=(uint8_t const val) {
    format = bits<6u, 2u>(val);
    type = bits<5u, 1u>(val);
    speedStep = bits<2u, 2u>(val);
    lock = bits<1u, 1u>(val);
    log = bits<0u, 1u>(val);
    return *this;
  }
  explicit operator uint8_t() const {
    return {static_cast<uint8_t>(format << 6u | type << 5u | speedStep << 2u |
                                 lock << 1u | log << 0u)};
  }
  bool operator==(AddressControl_Payload const&) const = default;
};

struct DecoderAddress {
  uint16_t format : 2u;   ///< Address format specification
  uint16_t address : 14u; ///< Address value
  DecoderAddress() = default;
  DecoderAddress(uint16_t const val)
    : format{bits<14u, 2u>(val)}, address{bits<0u, 14u>(val)} {}
  DecoderAddress& operator=(uint16_t const val) {
    format = bits<14u, 2u>(val);
    address = bits<0u, 14u>(val);
    return *this;
  }
  explicit operator uint16_t() const {
    return {static_cast<uint16_t>(format << 14u | address << 0u)};
  }
  bool operator==(DecoderAddress const&) const = default;
};

} // namespace ulf::mx1bin::bitfields

#pragma GCC diagnostic pop
