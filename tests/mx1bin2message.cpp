#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include "helper.hpp"

TEST(mx1bin2message, decode) {
  ulf::mx1bin::TrackControl message{.cAction =
                                      ulf::mx1bin::TrackControl::TrackOn};
  auto const stream{encode_message(message)};

  auto const result{ulf::mx1bin::mx1bin_2message(stream)};

  ASSERT_TRUE(result);
  ASSERT_TRUE(*result);
  ASSERT_TRUE(std::holds_alternative<decltype(message)>(**result));
  ASSERT_EQ(std::get<decltype(message)>(**result), message);
}

TEST(mx1bin2message, insufficient_bytes) {}

TEST(mx1bin2message, wrong_crc) {}

TEST(mx1bin2message, malformed_sof) {}
