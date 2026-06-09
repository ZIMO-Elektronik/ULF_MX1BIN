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
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint8(toolID).uint8(action);
  }
  template<Decoder D>
  static std::expected<SerialInfo, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(toolID) + sizeof(action)))
      return std::unexpected(std::errc::invalid_argument);
    return SerialInfo{.head = *head, .toolID = d.uint8(), .action = d.uint8()};
  }
  constexpr bool operator==(SerialInfo const&) const = default;
  constexpr SerialInfo& operator=(SerialInfo const&) = default;
};

} // namespace ulf::mx1bin
