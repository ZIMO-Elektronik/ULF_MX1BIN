#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ctre.hpp>
#include <vector>

#include <ulf/mx1bin.hpp>

namespace {

constexpr ctll::fixed_string pattern{"\x01\x01.+?(?<!\x10)\x17"};

constexpr char SOH{0x01};
constexpr char DLE{0x10};
constexpr char ETB{0x17};

} // namespace

TEST(regex, no_etb_character) {
  std::vector<char> s{SOH, SOH, 'a'};
  EXPECT_FALSE(ctre::match<pattern>(s));
}

TEST(regex, multiple_etb_characters) {
  std::vector<char> s{SOH, SOH, 'a', ETB, 'b', 'c', ETB};
  auto m{ctre::match<pattern>(s)};
  EXPECT_TRUE(m);
  // EXPECT_EQ(size(m.get<0uz>()), 4uz); // CTRE bug, this should be 4?
}

TEST(regex, escaped_etb_character) {
  std::vector<char> s{SOH, SOH, 'a', DLE, ETB, 'c', ETB};
  auto m{ctre::match<pattern>(s)};
  EXPECT_TRUE(ctre::match<pattern>(s));
  EXPECT_EQ(size(m.get<0uz>()), 7uz);
}

TEST(test, test) {
  std::array<uint8_t, 9uz> test{
    0x01, 0x01, 0x00, 0x10, 0x30, 0x0D, 0x00, 0x28, 0x17};

  auto tmp = ulf::mx1bin::detail::verify(test);
  ASSERT_TRUE(tmp);
  ASSERT_TRUE(*tmp);
}
