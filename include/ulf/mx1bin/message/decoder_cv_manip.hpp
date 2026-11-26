// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Decoder CV Manip Message
///
/// \file   ulf/mx1bin/message/decoder_cv_manip.hpp
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

struct DecoderCvManip {
  using Reply = TAck<Command::DecoderCvManip>;
  struct ReplyL2 {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::ReplyL2,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::DecoderCvManip};
    bitfields::DecoderAddress cAdr{};
    uint16_t variable{};
    uint8_t cValue{};
    uint8_t cError{};

    using members = member_list<member<&ReplyL2::cAdr>,
                                member<&ReplyL2::variable>,
                                member<&ReplyL2::cValue>,
                                member<&ReplyL2::cError>>;

    constexpr bool operator==(ReplyL2 const&) const = default;
    constexpr ReplyL2& operator=(ReplyL2 const&) = default;
  };

  struct Busy {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::DecoderCvManip};
    uint8_t const busy{0x04u};
    bitfields::DecoderAddress cAdr{};
    uint16_t variable{};
    std::optional<uint8_t> activeUSID{};
    std::optional<bitfields::DecoderAddress> activeAddr{};
    std::optional<uint16_t> activeCv{};

    using members = member_list<member<&Busy::busy>,
                                member<&Busy::cAdr>,
                                member<&Busy::variable>,
                                member<&Busy::activeUSID>,
                                member<&Busy::activeAddr>,
                                member<&Busy::activeCv>>;

    constexpr bool operator==(Busy const&) const = default;
    constexpr Busy& operator=(Busy const&) = default;
  };

  struct Error {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::ReplyL2,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::DecoderCvManip};
    bitfields::DecoderAddress cAdr{};
    uint8_t cError{};

    using members = member_list<member<&Error::cAdr>, member<&Error::cError>>;

    constexpr bool operator==(Error const&) const = default;
    constexpr Error& operator=(Error const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::DecoderCvManip};
  bitfields::DecoderAddress cAdr{}; ///< Address
  uint16_t variable{};              ///< Cv Address
  std::optional<uint8_t> value{};   ///< Cv Value

  using members = member_list<member<&DecoderCvManip::cAdr>,
                              member<&DecoderCvManip::variable>,
                              member<&DecoderCvManip::value>>;

  constexpr bool operator==(DecoderCvManip const&) const = default;
  constexpr DecoderCvManip& operator=(DecoderCvManip const&) = default;
};

} // namespace ulf::mx1bin
