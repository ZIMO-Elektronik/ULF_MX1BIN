// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Frame info
///
/// \file   ulf/mx1bin/info.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <span>

namespace {

// Masks
constexpr uint8_t frame_type_msk{0b10000000};
constexpr uint8_t message_type_msk{0b01100000};
constexpr uint8_t sender_msk{0b00010000};
constexpr uint8_t station_type_msk{0b00001111};

} // namespace

namespace ulf::mx1bin {

/// FrameType
enum class FrameType : uint8_t {
  Short = 0b0u << 7u,
  Long = 0b1u << 7u,
};

/// MessageType
enum class MessageType : uint8_t {
  Primary = 0b00u << 5u,
  L1Ack = 0b10u << 5u,
  ReplyL2 = 0b01u << 5u,
  L2Ack = 0b11u << 5u,
};

/// Sender
enum class Sender : uint8_t {
  CommandStation = 0b0u << 4u,
  PC = 0b1u << 4u,
};

/// StationType
enum class StationType : uint32_t {
  MX1 = 0x00u << 0u,
  MX8 = 0x01u << 0u,
  MX9 = 0x02u << 0u,
};

/// Make Header info
///
/// @param frameType    FrameType of header
/// @param messageType  MessageType of header
/// @param sender       Sender of header
/// @param stationType  StationType of header
/// @return Header info byte
constexpr uint8_t make_header_info(FrameType frameType,
                                   MessageType messageType,
                                   Sender sender,
                                   StationType stationType) {
  return std::to_underlying(frameType) | std::to_underlying(messageType) |
         std::to_underlying(sender) | std::to_underlying(stationType);
}

constexpr FrameType get_frame_type(uint8_t const info) {
  return static_cast<FrameType>(info & frame_type_msk);
}

constexpr FrameType get_frame_type(std::span<uint8_t const> frame) {
  return get_frame_type(frame[2]);
}

constexpr MessageType get_message_type(uint8_t const info) {
  return static_cast<MessageType>(info & message_type_msk);
}

constexpr MessageType get_message_type(std::span<uint8_t const> frame) {
  return get_message_type(frame[2]);
}

constexpr Sender get_sender(uint8_t const info) {
  return static_cast<Sender>(info & sender_msk);
}

constexpr Sender get_sender(std::span<uint8_t const> frame) {
  return get_sender(frame[2]);
}

constexpr StationType get_station_type(uint8_t const info) {
  return static_cast<StationType>(info & station_type_msk);
}

constexpr StationType get_station_type(std::span<uint8_t const> frame) {
  return get_station_type(frame[2]);
}

} // namespace ulf::mx1bin
