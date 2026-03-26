// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Reset Message
///
/// \file   ulf/mx1bin/message/reset.hpp
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

struct Reset {
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1},
            .code = Command::Reset};
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e);
  }
  template<Decoder D>
  static std::expected<Reset, std::errc> decode(D& d) {
    if (auto const head{Head::decode(d)}) return Reset{.head = *head};
    else return std::unexpected(head.error());
  }
};

} // namespace ulf::mx1bin
