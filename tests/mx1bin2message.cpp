#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include "helper.hpp"

TEST(mx1bin2message, decode) {
  ulf::mx1bin::TrackControl message{.cAction =
                                      ulf::mx1bin::TrackControl::TrackOn};
  auto stream{encode_message(message)};

  auto const result{ulf::mx1bin::mx1bin_2message(stream)};

  ASSERT_TRUE(result);
  ASSERT_TRUE(*result);
  ASSERT_TRUE(std::holds_alternative<decltype(message)>(**result));
  ASSERT_EQ(std::get<decltype(message)>(**result), message);
}

TEST(mx1bin2message, insufficient_bytes) {
  ulf::mx1bin::TrackControl message{.cAction =
                                      ulf::mx1bin::TrackControl::TrackOn};
  auto stream{encode_message(message)};
  stream.pop_back(); // Remove EOT

  auto const result{ulf::mx1bin::mx1bin_2message(stream)};

  ASSERT_TRUE(result);
  ASSERT_FALSE(*result);
}

TEST(mx1bin2message, wrong_crc) {
  ulf::mx1bin::TrackControl message{.cAction =
                                      ulf::mx1bin::TrackControl::TrackOn};
  auto stream{encode_message(message, 0xFFu)};

  auto const result{ulf::mx1bin::mx1bin_2message(stream)};

  ASSERT_FALSE(result);
}

TEST(mx1bin2message, malformed_sof) {
  ulf::mx1bin::TrackControl message{.cAction =
                                      ulf::mx1bin::TrackControl::TrackOn};
  auto stream{encode_message(message)};
  stream[0] = 0x02u; //
  stream[1] = 0x02u; // Changes to SOF

  auto const result{ulf::mx1bin::mx1bin_2message(stream)};

  ASSERT_FALSE(result);
}
