// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Message Base structs
///
/// \file   ulf/mx1bin/message/message_base.hpp
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
#include "../fields.hpp"
#include "../utility.hpp"

namespace ulf::mx1bin::detail {

constexpr Sender defaultSender{
#if defined MASTER
  Sender::PC
#else
  Sender::CommandStation
#endif
};

struct Head {
  uint8_t uSID{};         ///< Unique Service ID
  bitfields::Info info{}; ///< Header info byte
  Command code{};         ///< Command code

  constexpr bool operator==(Head const&) const = default;
  constexpr Head& operator=(Head const&) = default;

  using members =
    member_list<member<&Head::uSID>, member<&Head::info>, member<&Head::code>>;
};

struct ReplyHead {
  uint8_t uSID{};         ///< Unique Service ID
  bitfields::Info info{}; ///< Header info byte
  Command code{};         ///< Command code
  uint8_t reply_uSID{};   ///< Reply uSID reference

  using members = member_list<member<&ReplyHead::uSID>,
                              member<&ReplyHead::info>,
                              member<&ReplyHead::code>,
                              member<&ReplyHead::reply_uSID>>;

  constexpr bool operator==(ReplyHead const&) const = default;
  constexpr ReplyHead& operator=(ReplyHead const&) = default;
};

struct ReplyLongHead {
  uint8_t uSID{};         ///< Unique Service ID
  bitfields::Info info{}; ///< Header info byte
  Command code{};         ///< Command code
  uint8_t const lengthOfHeader{
    5u}; ///< Length of header --- There surely is a reason why this is always 5
  uint8_t reply_uSID{}; ///< Reply uSID reference

  using members = member_list<member<&ReplyLongHead::uSID>,
                              member<&ReplyLongHead::info>,
                              member<&ReplyLongHead::code>,
                              member<&ReplyLongHead::lengthOfHeader>,
                              member<&ReplyLongHead::reply_uSID>>;

  constexpr bool operator==(ReplyLongHead const&) const = default;
  constexpr ReplyLongHead& operator=(ReplyLongHead const&) = default;
};

} // namespace ulf::mx1bin::detail
