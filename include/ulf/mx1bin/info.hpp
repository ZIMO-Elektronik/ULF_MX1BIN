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

enum class FrameType : uint8_t {
  Short = 0x00u,
  Long = 0x01u,
};

enum class MessageType : uint8_t {
  Primary = 0x00u,
  L1Ack = 0x02u,
  ReplyL2 = 0x01u,
  L2Ack = 0x03u,
};

enum class Sender : uint8_t { CommandStation = 0x00u, PC = 0x01u };

enum class StationType : uint32_t { MX1 = 0x00u, MX8 = 0x01u, MX9 = 0x02u };

constexpr FrameType get_frame_type(std::span<uint8_t const> frame) {
  return static_cast<FrameType>((frame[2] & frame_type_msk) >> 7uz);
}

constexpr MessageType get_message_type(std::span<uint8_t const> frame) {
  return static_cast<MessageType>((frame[2] & message_type_msk) >> 5uz);
}

constexpr Sender get_sender(std::span<uint8_t const> frame) {
  return static_cast<Sender>((frame[2] & sender_msk) >> 4uz);
}

constexpr StationType get_station_type(std::span<uint8_t const> frame) {
  return static_cast<StationType>((frame[2] & station_type_msk) >> 0uz);
}

} // namespace ulf::mx1bin
