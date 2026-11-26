// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Serial Info Message
///
/// \file   ulf/mx1bin/message/serial_info.hpp
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
#include "ack.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

struct SerialInfo {
  using Reply = TAck<Command::SerialInfo>;
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::SerialInfo};
  uint8_t toolID{}; ///< Tool ID
  uint8_t action{}; ///<

  using members =
    member_list<member<&SerialInfo::toolID>, member<&SerialInfo::action>>;

  constexpr bool operator==(SerialInfo const&) const = default;
  constexpr SerialInfo& operator=(SerialInfo const&) = default;
};

} // namespace ulf::mx1bin
