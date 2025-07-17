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
std::expected<std::optional<Message>, std::errc>
mx1bin_2message(std::span<uint8_t const> bytes) {
  auto res{detail::verify(bytes)};
  if (!res) return std::unexpected(res.error());
  if (!*res) return std::nullopt;

  auto frame{**res};

  switch (static_cast<Commands>(frame[2])) {
    case Commands::Reset: {
      // Reset r{};
      // r.decode(frame);
      // return r;
      return Message{Reset{}.decode(frame)};
    }
    case Commands::Nak: break;
    case Commands::Track_Ctrl: break;
    case Commands::Loco_Ctrl: break;
    case Commands::Invert_Fnkt: break;
    case Commands::Accelerate: break;
    case Commands::Shuttle_Train: break;
    case Commands::Accessory_Cmd: break;
    case Commands::Loco_Mem_Query: break;
    case Commands::Accessory_Mem_Query: break;
    case Commands::Address_Ctrl: break;
    case Commands::Read_IO_State: break;
    case Commands::Station_Cv_Manip: break;
    case Commands::Station_Equipment_Query: break;
    case Commands::Tool_Info: break;
    case Commands::Cv_Manip: break;
    case Commands::Loco_Mem: break;
    case Commands::Accessory_Mem: break;
    default: return std::unexpected(std::errc::invalid_argument);
  }

  return {};
}

} // namespace ulf::mx1bin
