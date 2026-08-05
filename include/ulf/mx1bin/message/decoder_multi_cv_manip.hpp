// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Decoder Multi CV Manip Message
///
/// \file   ulf/mx1bin/message/decoder_multi_cv_manip.hpp
/// \author Jonas Gahlert
/// \date   15/07/2026

#pragma once

#include <cstdint>
#include <ztl/ztl.hpp>
#include "../bitfields.hpp"
#include "../commands.hpp"
#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../error.hpp"
#include "ack.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

/// Decoder Multi CV Manipulation messages
///
/// \details
/// This command allows reading or writing up to 4 concurrent CVs with one
/// message. Since this command was made to use DCC XPom, it is usually faster
/// than the regular \ref DecoderCvManip
///
/// Only one query can be active at a time (including \ref DecoderCvManip)
struct DecoderMultiCvManip {
  using Reply = TAck<Command::DecoderMultiCvManip>;

  /// ReplyL2 (Asynchronous)
  ///
  /// \details
  /// This reply is sent by the command station upon successful query
  /// completion. If the query was unsuccessful, an `Error` reply is sent
  /// instead.
  struct ReplyL2 {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::ReplyL2,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::DecoderMultiCvManip};
    bitfields::DecoderAddress cAdr{};  ///< Address of Decoder
    uint16_t index{};                  ///< CV page index
    uint8_t variable{};                ///< CV address
    uint8_t sequenceID{};              ///< XPom sequence ID
    std::array<uint8_t, 4uz> values{}; ///< Values
    uint8_t cError{};                  ///< Error code (if any)
    template<Encoder E>
    /// Encode
    E encode(E e) const {
      return head.encode(e)
        .uint16(cAdr)
        .uint16(index)
        .uint8(variable)
        .uint8(sequenceID)
        .uint8(values[0])
        .uint8(values[1])
        .uint8(values[2])
        .uint8(values[3])
        .uint8(cError);
    }
    /// Decode
    template<Decoder D>
    static std::expected<ReplyL2, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head ||
          !d.has_at_least(sizeof(cAdr) + sizeof(index) + sizeof(variable) +
                          sizeof(sequenceID) + 4uz /*values*/ + sizeof(cError)))
        return std::unexpected{std::errc::invalid_argument};
      return ReplyL2{
        .head = *head,
        .cAdr = d.uint16(),
        .index = d.uint16(),
        .variable = d.uint8(),
        .sequenceID = d.uint8(),
        .values = std::array{d.uint8(), d.uint8(), d.uint8(), d.uint8()},
        .cError = d.uint8(),
      };
    }
    constexpr bool operator==(ReplyL2 const&) const = default;
    constexpr ReplyL2& operator=(ReplyL2 const&) = default;
  };

  /// This reply is sent by the command station in case the query can't be
  /// handled. This usually means, that there currently is another query being
  /// handled.
  struct Busy {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::DecoderMultiCvManip};
    uint8_t const busy{0x04u};        ///< Busy
    bitfields::DecoderAddress cAdr{}; ///< Decoder address
    uint16_t index{};                 ///< CV page index
    uint8_t variable{};               ///< CV index
    uint8_t sequenceID{};             ///< XPom sequence ID
    enum Error : uint8_t {
      QueueFull = 0u,           ///< Queue is full
      AddressMismatch = 1u,     ///< Address does not match queue
      SequenceIDNotUnique = 2u, ///< SequenceID already occupied in queue
      Undefined = 255u,         ///< Undefined
    } error;                    ///< Error (Rejection reason)
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e)
        .uint8(busy)
        .uint16(cAdr)
        .uint16(index)
        .uint8(variable)
        .uint8(sequenceID)
        .uint8(error);
    }
    template<Decoder D>
    static std::expected<Busy, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(busy) + sizeof(cAdr) + sizeof(index) +
                                   sizeof(variable) + sizeof(sequenceID) +
                                   sizeof(error)))
        return std::unexpected{std::errc::invalid_argument};
      return Busy{
        .head = *head,
        .busy = d.uint8(),
        .cAdr = d.uint16(),
        .index = d.uint16(),
        .variable = d.uint8(),
        .sequenceID = d.uint8(),
        .error = static_cast<Error>(d.uint8()),
      };
    }
    constexpr bool operator==(Busy const&) const = default;
    constexpr Busy& operator=(Busy const&) = default;
  };

  /// This reply is sent by the command station in case an error (e.g. timeout)
  /// occurred during query execution.
  struct Error {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::ReplyL2,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::DecoderMultiCvManip};
    bitfields::DecoderAddress cAdr{}; ///< Active Decoder address
    uint8_t cError{};                 ///< Error
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e).uint16(cAdr).uint8(cError);
    }
    template<Decoder D>
    static std::expected<Error, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(cError)))
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
            .code = Command::DecoderMultiCvManip};
  bitfields::DecoderAddress cAdr{};           ///< Address of Decoder
  uint16_t index{};                           ///< CV page index
  uint8_t variable{};                         ///< CV address
  uint8_t sequenceID{};                       ///< XPom sequence ID
  ztl::inplace_vector<uint8_t, 4uz> values{}; ///< Values
  /// Encode
  template<Encoder E>
  E encode(E e) const {
    e = head.encode(e).uint16(cAdr).uint16(index).uint8(variable).uint8(
      sequenceID);
    std::ranges::for_each(values, [&e](uint8_t v) { e = e.uint8(v); });
    return e;
  }
  /// Decode
  template<Decoder D>
  static std::expected<DecoderMultiCvManip, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(index) +
                                 sizeof(variable) + sizeof(sequenceID)))
      return std::unexpected{std::errc::invalid_argument};
    DecoderMultiCvManip result{
      .head = *head,
      .cAdr = d.uint16(),
      .index = d.uint16(),
      .variable = d.uint8(),
      .sequenceID = d.uint8(),
    };
    while (!result.values.full()) {
      if (auto const value{d.s_uint8()}; !value) break;
      else result.values.push_back(*value);
    }
    return result;
  }
  constexpr bool operator==(DecoderMultiCvManip const&) const = default;
  constexpr DecoderMultiCvManip&
  operator=(DecoderMultiCvManip const&) = default;
};

} // namespace ulf::mx1bin
