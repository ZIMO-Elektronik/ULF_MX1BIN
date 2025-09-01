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
constexpr std::expected<std::optional<Message>, std::errc>
mx1bin_2message(std::span<uint8_t const> bytes) {
  auto res{detail::verify(bytes)};
  if (!res) return std::unexpected(res.error());
  if (!*res) return std::nullopt;

  auto frame{**res};
  frame = frame.subspan(2u, size(frame) - 4u);

  detail::Head head{};
  if (!head.decode(frame)) return std::unexpected(std::errc::invalid_argument);

  /// \todo Usually, we need to differentiate between message types
  switch (head.code) {
    case Command::Reset: return Reset::decode(frame);
    case Command::Track_Ctrl: return TrackControl::decode(frame);
    case Command::Loco_Ctrl: return DecoderControl::decode(frame);
    case Command::Invert_Fnkt: return InvertFunctionBits::decode(frame);
    case Command::Accelerate: return Acceleration::decode(frame);
    case Command::Shuttle_Train: return ShuttleTrain::decode(frame);
    case Command::Accessory_Cmd: return Accessory::decode(frame);
    case Command::Loco_Mem_Query: return LocoMemoryQuery::decode(frame);
    case Command::Accessory_Mem_Query:
      return AccessoryMemoryQuery::decode(frame);
    case Command::Address_Ctrl: return AddressControl::decode(frame);
    case Command::Read_IO_State: return CommandStationIOQuery::decode(frame);
    case Command::Station_Cv_Manip: return CommandStationCvManip::decode(frame);
    case Command::Station_Equipment_Query:
      return CommandStationEquipmentQuery::decode(frame);
    case Command::Serial_Info: return SerialInfo::decode(frame);
    case Command::Cv_Manip: return DecoderCvManip::decode(frame);
    default: return std::unexpected(std::errc::invalid_argument);
  }

  return std::unexpected(std::errc::broken_pipe);
}

} // namespace ulf::mx1bin
