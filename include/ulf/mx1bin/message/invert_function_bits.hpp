// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Invert Function Bits Message
///
/// \file   ulf/mx1bin/message/invert_function_bits.hpp
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

struct InvertFunctionBits {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::InvertFunctionBits};
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
            .code = Command::InvertFunctionBits};
  bitfields::DecoderAddress cAdr{}; ///< Address
  bitfields::ControlData cData1{};  ///< Control data
  uint8_t cData2{};                 ///< F1..8
  uint8_t cData3{};                 ///< F9..12
  uint8_t cData4{};                 ///< F13..20
  uint8_t cData5{};                 ///< F21..28

  using members = member_list<member<&InvertFunctionBits::cAdr>,
                              member<&InvertFunctionBits::cData1>,
                              member<&InvertFunctionBits::cData2>,
                              member<&InvertFunctionBits::cData3>,
                              member<&InvertFunctionBits::cData4>,
                              member<&InvertFunctionBits::cData5>>;

  constexpr bool operator==(InvertFunctionBits const&) const = default;
  constexpr InvertFunctionBits& operator=(InvertFunctionBits const&) = default;
};

} // namespace ulf::mx1bin
