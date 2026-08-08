#pragma once

#include <gtest/gtest.h>
#include <concepts>
#include <ulf/mx1bin.hpp>

/// Decode helper
template<ulf::mx1bin::Decodable D, std::ranges::input_range S>
constexpr auto decode(S const& s) {
  ulf::mx1bin::StreamDecoder d{s};
  return D::decode(d);
}

/// Encode helper
template<ulf::mx1bin::Encodable E,
         std::ranges::output_range<uint8_t> S = std::vector<uint8_t>>
constexpr auto encode(E const& e) {
  S result;
  ulf::mx1bin::StreamEncoder encoder{result};
  e.encode(encoder);
  return result;
}

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
concept IsReplyL2 =
  std::same_as<T, ulf::mx1bin::DecoderCvManip::ReplyL2> ||
  std::same_as<T, ulf::mx1bin::DecoderMultiCvManip::ReplyL2> ||
  std::same_as<T, ulf::mx1bin::DecoderCvManip::Error> ||
  std::same_as<T, ulf::mx1bin::DecoderMultiCvManip::Error>;

/// Concept to check if a type is a L1Ack
template<typename T>
concept IsL1Ack = !IsPrimary<T> && !IsReplyL2<T>;

/// Checks if the type is `Encodable` AND `Decodable`
template<typename T>
concept Codable = ulf::mx1bin::Encodable<T> && ulf::mx1bin::Decodable<T>;

template<Codable T>
constexpr void encode_decode_encode(T const& in) {
  auto const encoded{encode(in)};
  auto const decoded{decode<T>(encoded)};

  ASSERT_TRUE(decoded.has_value());
  ASSERT_EQ(*decoded, in);

  auto const re_encoded{encode(*decoded)};

  ASSERT_EQ(encoded, re_encoded);
}

template<Codable T>
constexpr void match_head(T const& lhs, T const& rhs) {
  ASSERT_EQ(lhs.head.uSID, rhs.head.uSID);
  ASSERT_EQ(lhs.head.info, rhs.head.info);
  ASSERT_EQ(lhs.head.code, rhs.head.code);

  // Check if we have a reply
  if constexpr (!IsPrimary<T>) {
    ASSERT_EQ(lhs.head.reply_uSID, rhs.head.reply_uSID);

    // Check if we have a long reply
    if constexpr (ulf::mx1bin::Long<T>) {
      ASSERT_EQ(lhs.head.lengthOfHeader, rhs.head.lengthOfHeader);
    }
  }
}

template<Codable T>
constexpr std::vector<uint8_t> encode_message(
  T const& t,
  std::optional<std::conditional_t<ulf::mx1bin::Long<T>, uint16_t, uint8_t>>
    o_crc = {}) {
  std::vector<uint8_t> result{};
  ulf::mx1bin::StreamEncoder e(result);
  e.addSOF();
  t.encode(e);
  if constexpr (ulf::mx1bin::Long<T>) {
    e.uint16(ulf::mx1bin::crc16(
      o_crc ? *o_crc : std::span<uint8_t const>{result}.subspan(2uz)));
  } else {
    e.uint8(
      o_crc ? *o_crc
            : ulf::mx1bin::crc8(std::span<uint8_t const>{result}.subspan(2uz)));
  }
  e.addEOT();
  return result;
}
