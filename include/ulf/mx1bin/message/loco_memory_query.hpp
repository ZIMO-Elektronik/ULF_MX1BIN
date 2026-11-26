// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Loco Memory Query Message
///
/// \file   ulf/mx1bin/message/loco_memory_query.hpp
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

struct LocoMemoryQuery {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::LocoMemoryQuery};
    Error error{Error::NO_ERROR};
    bitfields::DecoderAddress cAdr{}; ///< Address
    bitfields::ControlSpeed cSpeed{}; ///< Speed
    bitfields::ControlData cData1{};  ///< Control data
    uint8_t cData2{};                 ///< F1..8
    uint8_t cData3{};                 ///< F9..12
    uint8_t cAzBz{};                  ///< Accel / Break time
    uint8_t cStatus{};                ///< Active / inactive
    uint8_t cData4{};                 ///< F13..20
    uint8_t cData5{};                 ///< F21..28

    using members = member_list<member<&Reply::error>,
                                member<&Reply::cAdr>,
                                member<&Reply::cSpeed>,
                                member<&Reply::cData1>,
                                member<&Reply::cData2>,
                                member<&Reply::cData3>,
                                member<&Reply::cAzBz>,
                                member<&Reply::cStatus>,
                                member<&Reply::cData4>,
                                member<&Reply::cData5>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::LocoMemoryQuery};
  bitfields::DecoderAddress cAdr{}; ///< Address

  using members = member_list<member<&LocoMemoryQuery::cAdr>>;

  constexpr bool operator==(LocoMemoryQuery const&) const = default;
  constexpr LocoMemoryQuery& operator=(LocoMemoryQuery const&) = default;
};
} // namespace ulf::mx1bin
