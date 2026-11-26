// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Command Station Equipment Query Message
///
/// \file   ulf/mx1bin/message/command_station_equipment_query.hpp
/// \author Jonas Gahlert
/// \date   26/03/2026

#pragma once

#include <cstdint>
#include "../bitfields.hpp"
#include "../commands.hpp"
#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../error.hpp"
#include "../fields.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

/// \todo Finally write the decoder for this
struct CommandStationEquipmentQuery {
  struct Reply {
    using Head = detail::ReplyLongHead;
    Head head{.info = bitfields::Info{FrameType::Long,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::CommandStationEquipmentQuery};
    uint16_t cAddress{};                 ///< CAN address
    uint8_t cDevice{};                   ///< Device ID
    uint8_t cRom_size{};                 ///< ROM size
    uint8_t cRam_size{};                 ///< RAM size
    uint16_t cPrintver{};                ///<
    uint16_t cVersion{};                 ///< Version Major/Minor
    uint8_t cDate_day{};                 ///< Software date Day
    uint8_t cDate_month{};               ///< Software date Month
    uint8_t cDate_century{};             ///< Software date Century
    uint8_t cDate_year{};                ///< Software date Year
    uint8_t cSwitches{};                 ///<
    uint8_t cDevelopVersion{};           ///< Version Patch
    uint16_t cBootRom{};                 ///<
    uint8_t cBootRom_develop{};          ///<
    uint8_t values{};                    ///<
    std::optional<uint8_t> cSerNum_hi{}; ///< Serial Number [0]
    std::optional<uint8_t> cSerNum_mh{}; ///< Serial Number [1]
    std::optional<uint8_t> cSerNum_ml{}; ///< Serial Number [2]
    std::optional<uint8_t> cSerNum_lo{}; ///< Serial Number [3]

    using members = member_list<member<&Reply::cAddress>,
                                member<&Reply::cDevice>,
                                member<&Reply::cRom_size>,
                                member<&Reply::cRam_size>,
                                member<&Reply::cPrintver>,
                                member<&Reply::cVersion>,
                                member<&Reply::cDate_day>,
                                member<&Reply::cDate_month>,
                                member<&Reply::cDate_century>,
                                member<&Reply::cDate_year>,
                                member<&Reply::cSwitches>,
                                member<&Reply::cDevelopVersion>,
                                member<&Reply::cBootRom>,
                                member<&Reply::cBootRom_develop>,
                                member<&Reply::values>,
                                member<&Reply::cSerNum_hi>,
                                member<&Reply::cSerNum_mh>,
                                member<&Reply::cSerNum_ml>,
                                member<&Reply::cSerNum_lo>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::CommandStationEquipmentQuery};
  uint8_t const zero{}; ///< Zero --- Yes, zero..

  using members = member_list<member<&CommandStationEquipmentQuery::zero>>;

  constexpr bool
  operator==(CommandStationEquipmentQuery const&) const = default;
  constexpr CommandStationEquipmentQuery&
  operator=(CommandStationEquipmentQuery const&) = default;
};

} // namespace ulf::mx1bin
