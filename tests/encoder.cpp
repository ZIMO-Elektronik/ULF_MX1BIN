#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include <ulf/mx1bin/encoder.hpp>
#include <vector>

TEST(encoder, sof) {

  std::vector<uint8_t> expected{ulf::mx1bin::detail::soh,
                                ulf::mx1bin::detail::soh};

  std::vector<uint8_t> result{};
  ulf::mx1bin::Encoder encoder{std::back_inserter(result)};

  encoder.addSOF();
  ASSERT_EQ(result, expected);
}

TEST(encoder, eoh) {
  std::vector<uint8_t> expected{ulf::mx1bin::detail::eot};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.addEOT();
  ASSERT_EQ(result, expected);
}

TEST(encoder, encode_uint8) {
  uint8_t const value{0x80u};
  std::vector<uint8_t> expected{value};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.uint8(value);
  ASSERT_EQ(result, expected);
}

TEST(encoder, encode_uint8_optional) {
  std::optional<uint8_t> const opt_value{0x80u}, nopt_value{};
  std::vector<uint8_t> expected{*opt_value};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.uint8(opt_value);
  encoder.uint8(nopt_value);
  ASSERT_EQ(result, expected);
}

TEST(encoder, encode_uint8_escaped) {
  uint8_t const value{0x01u};
  std::vector<uint8_t> expected{ulf::mx1bin::detail::dle,
                                0x01u ^ ulf::mx1bin::detail::cypher};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.uint8(value);
  ASSERT_EQ(result, expected);
}

TEST(encoder, encode_uint16) {
  uint16_t const value{0x80'81u};
  std::vector<uint8_t> expected{0x80u, 0x81u};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.uint16(value);
  ASSERT_EQ(result, expected);
}

TEST(encoder, encode_uint16_optional) {
  std::optional<uint16_t> opt_value{0x80'81u}, nopt_value{};
  std::vector<uint8_t> expected{0x80u, 0x81u};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.uint16(opt_value);
  encoder.uint16(nopt_value);
  ASSERT_EQ(result, expected);
}

TEST(encoder, encode_uint16_escaped) {
  uint16_t const value{0x01'17u};
  std::vector<uint8_t> expected{ulf::mx1bin::detail::dle,
                                0x01u ^ ulf::mx1bin::detail::cypher,
                                ulf::mx1bin::detail::dle,
                                0x17u ^ ulf::mx1bin::detail::cypher};

  std::vector<uint8_t> result{};
  result.reserve(16u);
  ulf::mx1bin::Encoder encoder{result};

  encoder.uint16(value);
  ASSERT_EQ(result, expected);
}
