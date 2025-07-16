#pragma once

#include <cstdint>

namespace ulf::mx1bin {
enum class Commands : uint8_t {
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
  Tool_Info = 17u,               // Serial Info
  Cv_Manip = 19u,                // Cv Manipulation
  Loco_Mem = 255u,               // Decoder Memory Access
  Accessory_Mem = 254u,          //
};

} // namespace ulf::mx1bin
