// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Track Control Message
///
/// \file   ulf/mx1bin/message/track_control.hpp
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

struct TrackControl {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::TrackControl};
    bitfields::TrackStatus statusBits{}; ///< Track status

    using members = member_list<member<&Reply::statusBits>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::TrackControl};

  enum class Action : uint8_t {
    StopBroadcast = 0u,
    TrackOff = 1u,
    TrackOn = 2u,
    Query = 3u
  } cAction{}; ///< Action

  using members = member_list<member<&TrackControl::cAction>>;

  constexpr bool operator==(TrackControl const&) const = default;
  constexpr TrackControl& operator=(TrackControl const&) = default;
};

} // namespace ulf::mx1bin
