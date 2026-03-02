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
#include <utility>

namespace ulf::mx1bin {

/// FrameType
enum class FrameType : uint8_t {
  Short = 0b0u,
  Long = 0b1u,
};

/// MessageType
enum class MessageType : uint8_t {
  Primary = 0b00u,
  L1Ack = 0b10u,
  ReplyL2 = 0b01u,
  L2Ack = 0b11u,
};

/// Sender
enum class Sender : uint8_t {
  CommandStation = 0b0u,
  PC = 0b1u,
};

/// StationType
enum class StationType : uint8_t {
  MX1 = 0x00u,
  MX8 = 0x01u,
  MX9 = 0x02u,
};

constexpr uint8_t make_header_info(FrameType frameType,
                                   MessageType messageType,
                                   Sender sender,
                                   StationType stationType) {
  return {static_cast<uint8_t>(std::to_underlying(frameType) << 7u |
                               std::to_underlying(messageType) << 5u |
                               std::to_underlying(sender) << 4u |
                               std::to_underlying(stationType) << 0u)};
}

} // namespace ulf::mx1bin
