// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Accessory Memory Query Message
///
/// \file   ulf/mx1bin/message/accessory_memory_query.hpp
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

struct AccessoryMemoryQuery {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::AccessoryMemoryQuery};
    Error error{Error::NO_ERROR};     ///< Error
    bitfields::DecoderAddress cAdr{}; ///< Address
    uint8_t cPair{};                  ///<
    uint8_t cOutputs{};               ///< Acc decoder outputs

    using members = member_list<member<&Reply::error>,
                                member<&Reply::cAdr>,
                                member<&Reply::cPair>,
                                member<&Reply::cOutputs>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::AccessoryMemoryQuery};
  bitfields::DecoderAddress cAdr{}; ///< Address

  using members = member_list<member<&AccessoryMemoryQuery::cAdr>>;

  constexpr bool operator==(AccessoryMemoryQuery const&) const = default;
  constexpr AccessoryMemoryQuery&
  operator=(AccessoryMemoryQuery const&) = default;
};

} // namespace ulf::mx1bin
