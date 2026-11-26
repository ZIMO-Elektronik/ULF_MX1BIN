// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Command Station CV Manip Message
///
/// \file   ulf/mx1bin/message/command_station_cv_manip.hpp
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

struct CommandStationCvManip {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::CommandStationCvManip};
    Error error{Error::NO_ERROR}; ///< Error
    uint8_t value{};              ///< CV Value

    using members = member_list<member<&Reply::error>, member<&Reply::value>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::CommandStationCvManip};
  uint16_t variable{};            ///< Cv Address
  std::optional<uint8_t> value{}; ///< Cv Value

  using members = member_list<member<&CommandStationCvManip::variable>,
                              member<&CommandStationCvManip::value>>;

  constexpr bool operator==(CommandStationCvManip const&) const = default;
  constexpr CommandStationCvManip&
  operator=(CommandStationCvManip const&) = default;
};

} // namespace ulf::mx1bin
