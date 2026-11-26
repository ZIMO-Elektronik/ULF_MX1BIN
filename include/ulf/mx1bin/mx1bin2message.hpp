// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin 2 Message
///
/// \file   ulf/mx1bin/mx1bin2message.hpp
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
#include "new_decoder.hpp"

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

  neww::NewDecoder d{frame};
  d.strip(); // Remove SOF

  // Check if at least the head exists
  if (!d.has_at_least(3u)) return std::unexpected(std::errc::invalid_argument);
  //
  d.decode<uint8_t>(); // Skip uSID
  auto const info{d.decode<bitfields::Info>()};
  auto const code{d.decode<Command>()};

  auto const type{info.messageType};
  if (type == MessageType::Primary) {
    switch (code) {
      case Command::Reset: return neww::decode<Reset>(frame);
      case Command::TrackControl: return neww::decode<TrackControl>(frame);
      case Command::LocoControl: return neww::decode<LocoControl>(frame);
      case Command::InvertFunctionBits:
        return neww::decode<InvertFunctionBits>(frame);
      case Command::Acceleration: return neww::decode<Acceleration>(frame);
      case Command::ShuttleTrain: return neww::decode<ShuttleTrain>(frame);
      case Command::AccessoryControl:
        return neww::decode<AccessoryControl>(frame);
      case Command::LocoMemoryQuery:
        return neww::decode<LocoMemoryQuery>(frame);
      case Command::AccessoryMemoryQuery:
        return neww::decode<AccessoryMemoryQuery>(frame);
      case Command::AddressControl: return neww::decode<AddressControl>(frame);
      case Command::CommandStationIOQuery:
        return neww::decode<CommandStationIOQuery>(frame);
      case Command::CommandStationCvManip:
        return neww::decode<CommandStationCvManip>(frame);
      case Command::CommandStationEquipmentQuery:
        return neww::decode<CommandStationEquipmentQuery>(frame);
      case Command::SerialInfo: return neww::decode<SerialInfo>(frame);
      case Command::DecoderCvManip: return neww::decode<DecoderCvManip>(frame);
      default: return std::unexpected(std::errc::invalid_argument);
    }

  } else if (type == MessageType::L1Ack) {
    switch (code) {
      case Command::DecoderCvManip: return neww::decode<Ack>(frame);
      default: return std::unexpected(std::errc::invalid_argument);
    }
  }

  return std::unexpected(std::errc::broken_pipe);
}

} // namespace ulf::mx1bin
