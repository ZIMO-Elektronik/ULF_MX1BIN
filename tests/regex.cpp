#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ctre.hpp>
#include <vector>

#include <ulf/mx1bin.hpp>

#include <ulf/mx1bin/crc16.hpp>
#include <ulf/mx1bin/crc8.hpp>
#include <vector>

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

TEST(test, auth) {
  std::vector<uint8_t> result{
    0x01, 0x01, 0x80, 0xFF, 0x0D, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x10, 0x21, 0x00, 0x00, 0x54, 0x0E, 0x10, 0x21, 0x14, 0x19, 0x00, 0x70,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x21, 0x89, 0xed, 0x40, 0xCF, 0x17};

  ulf::mx1bin::CommandStationEquipmentQueryReply reply{};

  reply.uSID = 0x80;
  reply.type = 0xFF;
  reply.code = 0x0D;
  reply.lengthOfHeader = 0x05;
  reply.reply_uSID = 0x00;
  reply.cAddress = 0x0000;
  reply.cDevice = 0x04;
  reply.cRom_size = 0x00;
  reply.cRam_size = 0x00;
  reply.cPrintver = 0x0100;
  reply.cVersion = 0x0054;
  reply.cDate_day = 0x0E;
  reply.cDate_month = 0x01;
  reply.cDate_century = 0x14;
  reply.cDate_year = 0x19;
  reply.cSwitches = 0x00;
  reply.cDevelopVersion = 0x70;
  reply.cBootRom = 0x0000;
  reply.cBootRom_develop = 0x00;
  reply.values = 0x00;
  reply.cSerNum_hi = 0x00;
  reply.cSerNum_mh = 0x01;
  reply.cSerNum_ml = 0x89;
  reply.cSerNum_lo = 0xed;

  auto tmp{ulf::mx1bin::response2mx1bin(reply)};

  ASSERT_TRUE(std::ranges::equal(result, tmp));
}
