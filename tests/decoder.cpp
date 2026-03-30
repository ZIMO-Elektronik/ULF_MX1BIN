#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include <ulf/mx1bin/decoder.hpp>
#include <vector>

using testing::_;

TEST(decoder, strip) {
  std::vector<uint8_t> message{
    0x39u, 0x10u, 0x13u, 0x80u, 0x03u, 0x00u, 0x1Du, 0xEAu};

  std::vector<uint8_t> encoded_message{
    0x39u, 0x10u, 0x30u, 0x13u, 0x80u, 0x03u, 0x00u, 0x1Du, 0xEAu};

  std::vector<uint8_t> frame{0x01u, 0x01u};
  std::ranges::copy(encoded_message, std::back_inserter(frame));
  frame.push_back(0x17u);

  std::vector<uint8_t> result{};

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, strip_no_escaped) {
  std::vector<uint8_t> const message{0x80u, 0x00u, 0x00u, 0x62u};

  std::vector<uint8_t> frame{0x01u, 0x01u};
  std::ranges::copy(message, std::back_inserter(frame));
  frame.push_back(0x17u);

  std::vector<uint8_t> result{};

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, strip_empty_range_assert) {
  std::vector<uint8_t> frame{};

  ulf::mx1bin::StreamDecoder d{frame};

  EXPECT_DEATH(d.strip(), _);
}

TEST(decoder, strip_crc8) {
  std::vector<uint8_t> message{0xAAu, 0x55u, 0xAAu}, frame, result;
  std::ranges::copy(message, std::back_inserter(frame));
  frame.push_back(0xFFu); // Dummy CRC8

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip_crc<uint8_t>();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, strip_encoded_crc8) {
  std::vector<uint8_t> message{0xAAu, 0x55u, 0xAAu}, frame, result;
  std::ranges::copy(message, std::back_inserter(frame));
  frame.push_back(ulf::mx1bin::detail::dle);    //
  frame.push_back(ulf::mx1bin::detail::soh ^    //
                  ulf::mx1bin::detail::cypher); // Dummy CRC8

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip_crc<uint8_t>();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, strip_crc16) {
  std::vector<uint8_t> message{0xAAu, 0x55u, 0xAAu}, frame, result;
  std::ranges::copy(message, std::back_inserter(frame));
  frame.push_back(0x11u); //
  frame.push_back(0xFFu); // Dummy CRC16

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip_crc<uint16_t>();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, strip_half_encoded_crc16) {
  std::vector<uint8_t> message{0xAAu, 0x55u, 0xAAu}, frame, result;
  std::ranges::copy(message, std::back_inserter(frame));
  frame.push_back(0xFFu);                       //
  frame.push_back(ulf::mx1bin::detail::dle);    //
  frame.push_back(ulf::mx1bin::detail::soh ^    //
                  ulf::mx1bin::detail::cypher); // Dummy CRC16

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip_crc<uint16_t>();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, strip_encoded_crc16) {
  std::vector<uint8_t> message{0xAAu, 0x55u, 0xAAu}, frame, result;
  std::ranges::copy(message, std::back_inserter(frame));
  frame.push_back(ulf::mx1bin::detail::dle);    //
  frame.push_back(ulf::mx1bin::detail::soh ^    //
                  ulf::mx1bin::detail::cypher); //
  frame.push_back(ulf::mx1bin::detail::dle);    //
  frame.push_back(ulf::mx1bin::detail::soh ^    //
                  ulf::mx1bin::detail::cypher); // Dummy CRC16

  ulf::mx1bin::StreamDecoder d{frame};
  d.strip_crc<uint16_t>();

  while (auto const val{d.s_uint8()}) { result.push_back(*val); }
  ASSERT_EQ(message, result);
}

TEST(decoder, decode_uint8) {
  uint8_t const value{0x80u};

  std::vector<uint8_t> frame{value};
  ulf::mx1bin::StreamDecoder d{frame};

  ASSERT_EQ(d.uint8(), value);
}

TEST(decoder, decode_escaped_uint8) {
  uint8_t const value{0x10};

  std::vector<uint8_t> frame{ulf::mx1bin::detail::dle,
                             value ^ ulf::mx1bin::detail::cypher};
  ulf::mx1bin::StreamDecoder d{frame};

  ASSERT_EQ(d.uint8(), value);
}

TEST(decoder, decode_uint16) {
  uint16_t const value{0x8080u};

  std::vector<uint8_t> frame{static_cast<uint8_t>(value >> 8u),
                             static_cast<uint8_t>(value >> 0u)};
  ulf::mx1bin::StreamDecoder d{frame};

  ASSERT_EQ(d.uint16(), value);
}

TEST(decoder, decode_escaped_uint16) {
  uint16_t const value{0x1010u};

  std::vector<uint8_t> frame{
    ulf::mx1bin::detail::dle,
    static_cast<uint8_t>(value >> 8u ^ ulf::mx1bin::detail::cypher),
    ulf::mx1bin::detail::dle,
    static_cast<uint8_t>(value >> 0u ^ ulf::mx1bin::detail::cypher)};
  ulf::mx1bin::StreamDecoder d{frame};

  ASSERT_EQ(d.uint16(), value);
}

TEST(decoder, decode_optional_uint8) {
  uint8_t const value{0x80u};

  std::vector<uint8_t> frame{value};
  ulf::mx1bin::StreamDecoder d{frame};

  auto const result{d.s_uint8()};

  ASSERT_TRUE(result);
  ASSERT_EQ(*result, value);
}

TEST(decoder, decode_optional_uint8_empty) {
  std::vector<uint8_t> frame{};
  ulf::mx1bin::StreamDecoder d{frame};

  ASSERT_FALSE(d.s_uint8());
}

TEST(decoder, decode_optional_uint16) {
  uint16_t const value{0x8080u};

  std::vector<uint8_t> frame{static_cast<uint8_t>(value >> 8u),
                             static_cast<uint8_t>(value >> 0u)};
  ulf::mx1bin::StreamDecoder d{frame};

  auto const result{d.s_uint16()};

  ASSERT_TRUE(result);
  ASSERT_EQ(*result, value);
}

TEST(decoder, decode_optional_uint16_empty) {
  std::vector<uint8_t> frame{};
  ulf::mx1bin::StreamDecoder d{frame};

  ASSERT_FALSE(d.s_uint16());
}
