// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin 2 DCC
///
/// \file   ulf/mx1bin/mx1bin2dcc.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <dcc/dcc.hpp>
#include <expected>
#include <optional>
#include <span>
#include <system_error>
#include "bitfields.hpp"
#include "commands.hpp"
#include "decoder.hpp"
#include "frame.hpp"
#include "message.hpp"

namespace ulf::mx1bin {

/// MX1Bin 2 DCC
///
/// \param bytes  MX1Bin frame
/// \return       DCC Packet
/// \warning
/// Currently only handles short primary MX1 messages, with the exception of
/// `Command 19 L1Ack`
constexpr std::expected<std::optional<Message>, std::errc>
mx1bin_2message(std::span<uint8_t const> bytes) {
  auto res{detail::verify(bytes)};
  if (!res) return std::unexpected(res.error());
  if (!*res) return std::nullopt;

  auto frame{**res};

  StreamDecoder d{frame};
  d.strip(); // Remove SOF

  // Check if at least the head exists
  if (!d.has_at_least(3u)) return std::unexpected(std::errc::invalid_argument);

  d.uint8(); // Skip uSID
  auto const info{bitfields::Info{d.uint8()}};
  auto const code{static_cast<Command>(d.uint8())};

  auto const type{info.messageType};
  if (type == MessageType::Primary) {
    switch (code) {
      case Command::Reset: return decode<Reset>(frame);
      case Command::TrackControl: return decode<TrackControl>(frame);
      case Command::LocoControl: return decode<LocoControl>(frame);
      case Command::InvertFunctionBits:
        return decode<InvertFunctionBits>(frame);
      case Command::Acceleration: return decode<Acceleration>(frame);
      case Command::ShuttleTrain: return decode<ShuttleTrain>(frame);
      case Command::AccessoryControl: return decode<AccessoryControl>(frame);
      case Command::LocoMemoryQuery: return decode<LocoMemoryQuery>(frame);
      case Command::AccessoryMemoryQuery:
        return decode<AccessoryMemoryQuery>(frame);
      case Command::AddressControl: return decode<AddressControl>(frame);
      case Command::CommandStationIOQuery:
        return decode<CommandStationIOQuery>(frame);
      case Command::CommandStationCvManip:
        return decode<CommandStationCvManip>(frame);
      case Command::CommandStationEquipmentQuery:
        return decode<CommandStationEquipmentQuery>(frame);
      case Command::SerialInfo: return decode<SerialInfo>(frame);
      case Command::DecoderCvManip: return decode<DecoderCvManip>(frame);
      default: return std::unexpected(std::errc::invalid_argument);
    }

  } else if (type == MessageType::L1Ack) {
    switch (code) {
      case Command::DecoderCvManip: return decode<Ack>(frame);
      default: return std::unexpected(std::errc::invalid_argument);
    }
  }

  return std::unexpected(std::errc::broken_pipe);
}

} // namespace ulf::mx1bin
