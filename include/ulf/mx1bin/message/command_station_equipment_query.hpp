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
#include "message_base.hpp"

namespace ulf::mx1bin {

/// \todo Finally write the decoder for this
struct CommandStationEquipmentQuery {
  struct Reply {
    using Head = detail::ReplyLongHead;
    Head head{.info = bitfields::Info{FrameType::Long,
                                      MessageType::L1Ack,
                                      Sender::CommandStation,
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
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e)
        .uint16(cAddress)
        .uint8(cDevice)
        .uint8(cRom_size)
        .uint8(cRam_size)
        .uint16(cPrintver)
        .uint16(cVersion)
        .uint8(cDate_day)
        .uint8(cDate_month)
        .uint8(cDate_century)
        .uint8(cDate_year)
        .uint8(cSwitches)
        .uint8(cDevelopVersion)
        .uint16(cBootRom)
        .uint8(cBootRom_develop)
        .uint8(values)
        .uint8(cSerNum_hi)
        .uint8(cSerNum_mh)
        .uint8(cSerNum_ml)
        .uint8(cSerNum_lo);
    }
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1},
            .code = Command::CommandStationEquipmentQuery};
  uint8_t const zero{}; ///< Zero --- Yes, zero..
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint8(zero);
  }
  template<Decoder D>
  static std::expected<CommandStationEquipmentQuery, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(zero)))
      return std::unexpected(std::errc::invalid_argument);
    return CommandStationEquipmentQuery{.head = *head, .zero = d.uint8()};
  }
};

} // namespace ulf::mx1bin
