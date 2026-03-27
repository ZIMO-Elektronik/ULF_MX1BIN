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
    template<Encoder E>
    E encode(E e) const {
      e = head.encode(e);
      e.uint8(statusBits);
      return e;
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const base{Head::decode(d)};
      if (!base || !d.has_at_least(sizeof(cAction)))
        return std::unexpected(std::errc::invalid_argument);
      return Reply{.head = *base, .statusBits = d.uint8()};
    }
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::TrackControl};

  enum Action : uint8_t {
    StopBroadcast = 0u,
    TrackOff = 1u,
    TrackOn = 2u,
    Query = 3u
  } cAction{}; ///< Action
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint8(cAction);
  }
  template<Decoder D>
  static std::expected<TrackControl, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAction)))
      return std::unexpected(std::errc::invalid_argument);
    return TrackControl{.head = *head,
                        .cAction = static_cast<Action>(d.uint8())};
  }
};

} // namespace ulf::mx1bin
