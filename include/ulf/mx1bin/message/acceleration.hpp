// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Acceleration Message
///
/// \file   ulf/mx1bin/message/acceleration.hpp
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

struct Acceleration {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::Acceleration};
    Error error{Error::NO_ERROR};
    bitfields::ControlPayload payload{};

    using members = member_list<member<&Reply::error>, member<&Reply::payload>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };

  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::Acceleration};
  bitfields::DecoderAddress cAdr{}; ///< Address
  uint8_t cAzBz{};                  ///< Accel / Break time

  using members =
    member_list<member<&Acceleration::cAdr>, member<&Acceleration::cAzBz>>;

  constexpr bool operator==(Acceleration const&) const = default;
  constexpr Acceleration& operator=(Acceleration const&) = default;
};

} // namespace ulf::mx1bin
