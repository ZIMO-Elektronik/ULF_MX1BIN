#pragma once

#include <gtest/gtest.h>
#include <concepts>
#include <ulf/mx1bin.hpp>

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
concept IsReplyL2 = std::same_as<T, ulf::mx1bin::DecoderCvManip::ReplyL2> ||
                    std::same_as<T, ulf::mx1bin::DecoderCvManip::Error>;

/// Concept to check if a type is a L1Ack
template<typename T>
concept IsL1Ack = !IsPrimary<T> && !IsReplyL2<T>;

/// Checks if the type is `Encodable` AND `Decodable`
template<typename T>
concept Codable = ulf::mx1bin::Encodable<T> && ulf::mx1bin::Decodable<T>;

/// Encode and decode helper
/// \tparam T type to encode / decode
/// \param in Input
/// \return Result
template<Codable T>
constexpr Result<T> encode_and_decode(T const& in) {
  auto const encoded{encode(in)};
  auto const decoded{ulf::mx1bin::decode<T>(encoded)};

  assert(decoded); // GTest ASSERT won't compile here somehow..
  return Result{.message = *decoded, .stream = encoded};
}

template<Codable T>
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
