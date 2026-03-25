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
#include "commands.hpp"
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

  auto const head{decode<detail::Head>(frame)};
  if (!head) return std::unexpected(std::errc::invalid_argument);

  auto const type{head->info.messageType};
  if (type == MessageType::Primary) {
    switch (head->code) {
      case Command::Reset: return decode<Reset>(frame);
      case Command::Track_Ctrl: return decode<TrackControl>(frame);
      case Command::Loco_Ctrl: return decode<DecoderControl>(frame);
      case Command::Invert_Fnkt: return decode<InvertFunctionBits>(frame);
      case Command::Accelerate: return decode<Acceleration>(frame);
      case Command::Shuttle_Train: return decode<ShuttleTrain>(frame);
      case Command::Accessory_Cmd: return decode<Accessory>(frame);
      case Command::Loco_Mem_Query: return decode<LocoMemoryQuery>(frame);
      case Command::Accessory_Mem_Query:
        return decode<AccessoryMemoryQuery>(frame);
      case Command::Address_Ctrl: return decode<AddressControl>(frame);
      case Command::Read_IO_State: return decode<CommandStationIOQuery>(frame);
      case Command::Station_Cv_Manip:
        return decode<CommandStationCvManip>(frame);
      case Command::Station_Equipment_Query:
        return decode<CommandStationEquipmentQuery>(frame);
      case Command::Serial_Info: return decode<SerialInfo>(frame);
      case Command::Cv_Manip: return decode<DecoderCvManip>(frame);
      default: return std::unexpected(std::errc::invalid_argument);
    }

  } else if (type == MessageType::L1Ack) {
    switch (head->code) {
      case Command::Cv_Manip: return decode<Ack>(frame);
      default: return std::unexpected(std::errc::invalid_argument);
    }
  }

  return std::unexpected(std::errc::broken_pipe);
}

} // namespace ulf::mx1bin
