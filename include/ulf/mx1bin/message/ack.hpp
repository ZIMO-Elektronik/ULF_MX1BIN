// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Ack Message
///
/// \file   ulf/mx1bin/message/ack.hpp
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

/// Templated Ack
///
/// @tparam C Command
template<Command C>
struct TAck {
  using Head = detail::ReplyHead;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::L1Ack,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = C};
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e);
  }
  template<Decoder D>
  static std::expected<TAck<C>, std::errc> decode(D& d) {
    if (auto const head{Head::decode(d)}) return TAck<C>{.head = *head};
    else return std::unexpected(head.error());
  }
  constexpr bool operator==(TAck const&) const = default;
  constexpr TAck& operator=(TAck const&) = default;
};

/// Ack definition when decoding, command must be filled manually
using Ack = TAck<Command::Reset>;

} // namespace ulf::mx1bin
