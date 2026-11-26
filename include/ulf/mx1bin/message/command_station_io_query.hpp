// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Command Station IO Query Message
///
/// \file   ulf/mx1bin/message/command_station_io_query.hpp
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

struct CommandStationIOQuery {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::CommandStationIOQuery};
    uint8_t values{};
    uint16_t cCurrent1{};
    uint8_t cVoltage1{};
    uint16_t cCurrent2{};
    uint8_t cVoltage2{};
    uint8_t cAux{};

    using members = member_list<member<&Reply::values>,
                                member<&Reply::cCurrent1>,
                                member<&Reply::cVoltage1>,
                                member<&Reply::cCurrent2>,
                                member<&Reply::cVoltage2>,
                                member<&Reply::cAux>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::CommandStationIOQuery};
  uint8_t const zero{};

  using members = member_list<member<&CommandStationIOQuery::zero>>;

  constexpr bool operator==(CommandStationIOQuery const&) const = default;
  constexpr CommandStationIOQuery&
  operator=(CommandStationIOQuery const&) = default;
};

} // namespace ulf::mx1bin
