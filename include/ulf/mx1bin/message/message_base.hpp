// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Message Base structs
///
/// \file   ulf/mx1bin/message_base.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <expected>
#include <span>
#include <system_error>
#include <ztl/inplace_vector.hpp>
#include "../bitfields.hpp"
#include "../commands.hpp"
#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../error.hpp"
#include "../utility.hpp"

namespace ulf::mx1bin::detail {

template<Command C>
struct Head {
  uint8_t uSID{};         ///< Unique Service ID
  bitfields::Info info{}; ///< Header info byte
  Command const code{C};  ///< Command code
  template<Encoder E>
  auto encode(E e) const {
    e.uint8(uSID);
    e.uint8(static_cast<uint8_t>(info));
    e.uint8(std::to_underlying(code));
    return e;
  }
  template<Decoder D>
  static std::expected<Head, std::errc> decode(D& d) {
    if (!d.has_at_least(sizeof(uSID) + sizeof(info) + sizeof(code)))
      return std::unexpected{std::errc::invalid_argument};
    auto const result{Head{.uSID{d.uint8()}, .info{d.uint8()}}};

    // Command code MUST match
    assert(std::to_underlying(result.code) == d.uint8());
    return result;
  }
};

template<Command C>
struct ReplyHead {
  uint8_t uSID{};         ///< Unique Service ID
  bitfields::Info info{}; ///< Header info byte
  Command const code{C};  ///< Command code
  uint8_t reply_uSID{};   ///< Reply uSID reference
  template<Encoder E>
  auto encode(E e) const {
    e.uint8(uSID);
    e.uint8(static_cast<uint8_t>(info));
    e.uint8(std::to_underlying(code));
    e.uint8(reply_uSID);
    return e;
  }
  template<Decoder D>
  static std::expected<ReplyHead, std::errc> decode(D& d) {
    if (!d.has_at_least(sizeof(uSID) + sizeof(info) + sizeof(Command) +
                        sizeof(reply_uSID)))
      return std::unexpected{std::errc::invalid_argument};
    auto result{ReplyHead{.uSID{d.uint8()}, .info{d.uint8()}}};

    // Command code MUST match
    assert(std::to_underlying(result.code) == d.uint8());

    result.reply_uSID = d.uint8();
    return result;
  }
};

template<Command C>
struct ReplyLongHead {
  uint8_t uSID{};         ///< Unique Service ID
  bitfields::Info info{}; ///< Header info byte
  Command const code{C};  ///< Command code
  uint8_t const lengthOfHeader{
    5u}; ///< Length of header --- There surely is a reason why this is always 5
  uint8_t reply_uSID{}; ///< Reply uSID reference
  template<Encoder E>
  auto encode(E e) const {
    e.uint8(uSID);
    e.uint8(static_cast<uint8_t>(info));
    e.uint8(std::to_underlying(code));
    e.uint8(lengthOfHeader);
    e.uint8(reply_uSID);
    return e;
  }
  template<Decoder D>
  static std::expected<ReplyLongHead, std::errc> decode(D& d) {
    if (!d.has_at_least(sizeof(uSID) + sizeof(info) + sizeof(code) +
                        sizeof(lengthOfHeader) + sizeof(reply_uSID)))
      return std::unexpected{std::errc::invalid_argument};
    auto result{ReplyLongHead{.uSID{d.uint8()}, .info{d.uint8()}}};

    // Command code MUST match
    assert(std::to_underlying(result.code) == d.uint8());

    result.lengthOfHeader = d.uint8();
    result.reply_uSID = d.uint8();
    return result;
  }
};

} // namespace ulf::mx1bin::detail
