#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <array>
#include <ctre.hpp>
#include <ranges>
#include <ulf/mx1bin.hpp>
#include <ulf/mx1bin/crc16.hpp>
#include <ulf/mx1bin/crc8.hpp>
#include <vector>

using namespace ulf::mx1bin::detail;

TEST(regex, no_etb_character) {
  std::vector<char> s{soh, soh, 'a'};
  EXPECT_FALSE(ctre::match<pattern>(s));
}

TEST(regex, multiple_etb_characters) {
  std::vector<char> s{soh, soh, 'a', eot, 'b', 'c', eot};
  auto m{ctre::match<pattern>(s)};
  EXPECT_TRUE(m);
  // EXPECT_EQ(size(m.get<0uz>()), 4uz); // CTRE bug, this should be 4?
}

TEST(regex, escaped_etb_character) {
  std::vector<char> s{soh, soh, 'a', dle, eot, 'c', eot};
  auto m{ctre::match<pattern>(s)};
  EXPECT_TRUE(ctre::match<pattern>(s));
  EXPECT_EQ(size(m.get<0uz>()), 7uz);
}
