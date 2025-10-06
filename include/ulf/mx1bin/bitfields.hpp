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
  constexpr Info() = default;
  constexpr Info(Info const&) = default;
  constexpr Info(FrameType _frameType,
                 MessageType _messageType,
                 Sender _sender,
                 StationType _stationType)
    : frameType{_frameType}, messageType{_messageType}, sender{_sender},
      stationType{_stationType} {}
  constexpr Info(uint8_t const val) {
    new (this) Info(static_cast<FrameType>(bits<7u, 1u>(val)),
                    static_cast<MessageType>(bits<5u, 2u>(val)),
                    static_cast<Sender>(bits<4u, 1u>(val)),
                    static_cast<StationType>(bits<0u, 4u>(val)));
  }
  constexpr Info& operator=(uint8_t const val) { return *new (this) Info(val); }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(std::to_underlying(frameType) << 7u |
                                 std::to_underlying(messageType) << 5u |
                                 std::to_underlying(sender) << 4u |
                                 std::to_underlying(stationType) << 0u)};
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
    new (this) TrackStatus(bits<7u, 1u>(val),
                           bits<6u, 1u>(val),
                           bits<2u, 1u>(val),
                           bits<1u, 1u>(val),
                           bits<0u, 1u>(val));
  }
  constexpr TrackStatus& operator=(uint8_t const val) {
    return *new (this) TrackStatus(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(dcc << 7u | motorola << 6u | ues << 2u |
                                 trackVoltage << 1u | broadcastStop << 0u)};
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
    new (this) ControlSpeed(bits<7u, 1u>(val), bits<0u, 7u>(val));
  }
  constexpr ControlSpeed& operator=(uint8_t const val) {
    return *new (this) ControlSpeed(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(stop << 7u | speed << 0u)};
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
    new (this) ControlData(bits<7u, 1u>(val),
                           bits<5u, 1u>(val),
                           bits<4u, 1u>(val),
                           bits<2u, 2u>(val),
                           bits<1u, 1u>(val),
                           bits<0u, 1u>(val));
  }
  constexpr ControlData& operator=(uint8_t const val) {
    return *new (this) ControlData(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(manual << 7u | direction << 5u |
                                 headlights << 4u | speed_step << 2u |
                                 az_enable << 1u | bz_enable << 0u)};
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
    new (this) ControlPayload(bits<6u, 2u>(val),
                              static_cast<SpeedStep>(bits<2u, 2u>(val)),
                              bits<0u, 1u>(val));
  }
  constexpr ControlPayload& operator=(uint8_t const val) {
    return *new (this) ControlPayload(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(
      format << 6u | std::to_underlying(speedStep) << 2u | trackState)};
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
    new (this) AddressControl_Control(bits<7u, 1u>(val),
                                      bits<5u, 1u>(val),
                                      bits<1u, 1u>(val),
                                      bits<0u, 1u>(val));
  }
  constexpr AddressControl_Control& operator=(uint8_t const val) {
    return *new (this) AddressControl_Control(val);
  }
  constexpr explicit operator uint8_t() const {
    return {
      static_cast<uint8_t>(set << 7u | type << 5u | lock << 1u | log << 0u)};
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
    new (this) AddressControl_Payload(bits<6u, 2u>(val),
                                      bits<5u, 1u>(val),
                                      bits<2u, 2u>(val),
                                      bits<1u, 1u>(val),
                                      bits<0u, 1u>(val));
  }
  constexpr AddressControl_Payload& operator=(uint8_t const val) {
    return *new (this) AddressControl_Payload(val);
  }
  constexpr explicit operator uint8_t() const {
    return {static_cast<uint8_t>(format << 6u | type << 5u | speedStep << 2u |
                                 lock << 1u | log << 0u)};
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
    new (this) DecoderAddress(bits<14u, 2u>(val), bits<0u, 14u>(val));
  }
  constexpr DecoderAddress& operator=(uint16_t const val) {
    return *new (this) DecoderAddress(val);
  }
  constexpr explicit operator uint16_t() const {
    return {static_cast<uint16_t>(format << 14u | address << 0u)};
  }
  constexpr bool operator==(DecoderAddress const&) const = default;
};

} // namespace ulf::mx1bin::bitfields

#pragma GCC diagnostic pop
