// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin commands
///
/// \file   ulf/mx1bin/commands.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>

namespace ulf::mx1bin {
enum class Command : uint8_t {
  Reset = 0u,                    // Status
  Nak = 1u,                      //
  Track_Ctrl = 2u,               // Track Control
  Loco_Ctrl = 3u,                // Loco Control
  Invert_Fnkt = 4u,              //
  Accelerate = 5u,               //
  Shuttle_Train = 6u,            //
  Accessory_Cmd = 7u,            //
  Loco_Mem_Query = 8u,           // Command Station Memory Query
  Accessory_Mem_Query = 9u,      //
  Address_Ctrl = 10u,            //
  Read_IO_State = 11u,           //
  Station_Cv_Manip = 12u,        //
  Station_Equipment_Query = 13u, // Command Station Equipment Query
  Serial_Info = 17u,             // Serial Info
  Cv_Manip = 19u,                // Cv Manipulation
  Loco_Mem = 255u,               // Decoder Memory Access
  Accessory_Mem = 254u,          //
};

} // namespace ulf::mx1bin
