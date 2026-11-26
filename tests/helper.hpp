#pragma once

#include <gtest/gtest.h>
#include <concepts>
#include <ulf/mx1bin.hpp>

/// Encode -> Decode Result
template<typename T,
         std::ranges::output_range<uint8_t> S = std::vector<uint8_t>>
struct Result {
  T message; /// Message struct (decoded)
  S stream;  /// Message stream (encoded)
};

/// Concept to check if a type is a primary message
template<typename T>
concept IsPrimary =
  (requires { typename T::Reply; } // Check for a type Reply
   &&
   !std::same_as<T, typename T::Reply>) // Make sure its not the class itself...
  || std::same_as<T, ulf::mx1bin::Nak>  // Again with the special cases...
  ;

/// Concept to check if a type is a ReplyL2
///
/// \note Since this is rare, a list of types should suffice
template<typename T>
concept IsReplyL2 = std::same_as<T, ulf::mx1bin::DecoderCvManip::ReplyL2> ||
                    std::same_as<T, ulf::mx1bin::DecoderCvManip::Error>;

/// Concept to check if a type is a L1Ack
template<typename T>
concept IsL1Ack = !IsPrimary<T> && !IsReplyL2<T>;

/// Encode and decode helper
/// \tparam T type to encode / decode
/// \param in Input
/// \return Result
template<ulf::mx1bin::CompatibleProtocolMessage T>
constexpr Result<T> encode_and_decode(T const& in) {
  std::vector<uint8_t> encoded{};

  ulf::mx1bin::encode(in, encoded);
  auto const decoded{ulf::mx1bin::decode<T>(encoded)};

  assert(decoded); // GTest ASSERT won't compile here somehow..
  return Result{.message = *decoded, .stream = encoded};
}

template<ulf::mx1bin::CompatibleProtocolMessage T>
constexpr void match_head(T const& correct, T const& compare) {
  ASSERT_EQ(correct.head.uSID, compare.head.uSID);
  ASSERT_EQ(correct.head.info, compare.head.info);
  ASSERT_EQ(correct.head.code, compare.head.code);

  // Check if we have a reply
  if constexpr (!IsPrimary<T>) {
    ASSERT_EQ(correct.head.reply_uSID, compare.head.reply_uSID);

    // Check if we have a long reply
    if constexpr (ulf::mx1bin::Long<T>) {
      ASSERT_EQ(correct.head.lengthOfHeader, compare.head.lengthOfHeader);
    }
  }
}

template<ulf::mx1bin::EncodableProtocolMessage T>
constexpr std::vector<uint8_t> encode_message(T const& t) {
  std::vector<uint8_t> result{};
  ulf::mx1bin::StreamEncoder e(result);
  e.addSOF();
  ulf::mx1bin::encode(t.head, e);
  ulf::mx1bin::encode(t, e);
  if constexpr (ulf::mx1bin::Long<T>) {
    e.encode(ulf::mx1bin::crc16(std::span<uint8_t const>{result}.subspan(2uz)));
  } else {
    e.encode(ulf::mx1bin::crc8(std::span<uint8_t const>{result}.subspan(2uz)));
  }
  e.addEOT();
  return result;
}

template<ulf::mx1bin::EncodableProtocolMessage T>
constexpr std::vector<uint8_t> encode_message(T const& t, size_t crc) {
  std::vector<uint8_t> result{};
  ulf::mx1bin::StreamEncoder e(result);
  e.addSOF();
  ulf::mx1bin::encode(t.head, e);
  ulf::mx1bin::encode(t, e);
  if constexpr (ulf::mx1bin::Long<T>) {
    e.encode(static_cast<uint16_t>(crc));
  } else {
    e.encode(static_cast<uint8_t>(crc));
  }
  e.addEOT();
  return result;
}
