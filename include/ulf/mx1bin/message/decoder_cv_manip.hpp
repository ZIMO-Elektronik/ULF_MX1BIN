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
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e).uint16(cAdr).uint16(variable).uint8(cValue).uint8(
        cError);
    }
    template<Decoder D>
    static std::expected<ReplyL2, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(variable) +
                                   sizeof(cValue) + sizeof(cError)))
        return std::unexpected{std::errc::invalid_argument};
      return ReplyL2{.head = *head,
                     .cAdr = d.uint16(),
                     .variable = d.uint16(),
                     .cValue = d.uint8(),
                     .cError = d.uint8()};
    }
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
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e)
        .uint8(busy)
        .uint16(cAdr)
        .uint16(variable)
        .uint8(activeUSID)
        .uint16(activeAddr)
        .uint16(activeCv);
    }
    template<Decoder D>
    static std::expected<Busy, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head ||
          !d.has_at_least(sizeof(busy) + sizeof(cAdr) + sizeof(variable)))
        return std::unexpected{std::errc::invalid_argument};
      return Busy{.head = *head,
                  .busy = d.uint8(),
                  .cAdr = d.uint8(),
                  .variable = d.uint8(),
                  .activeUSID = d.s_uint8(),
                  .activeAddr = d.s_uint8(),
                  .activeCv = d.s_uint8()};
    }
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
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e).uint16(cAdr).uint8(cError);
    }
    template<Decoder D>
    static std::expected<Error, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || d.has_at_least(sizeof(cAdr) + sizeof(cError)))
        return std::unexpected{std::errc::invalid_argument};
      return Error{.head = *head, .cAdr = d.uint16(), .cError = d.uint8()};
    }
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
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint16(cAdr).uint16(variable).uint8(value);
  }
  template<Decoder D>
  static std::expected<DecoderCvManip, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(variable)))
      return std::unexpected(std::errc::invalid_argument);
    return DecoderCvManip{.head = *head,
                          .cAdr = d.uint16(),
                          .variable = d.uint16(),
                          // Add optional data
                          .value = d.s_uint8()};
  }
  constexpr bool operator==(DecoderCvManip const&) const = default;
  constexpr DecoderCvManip& operator=(DecoderCvManip const&) = default;
};

} // namespace ulf::mx1bin
