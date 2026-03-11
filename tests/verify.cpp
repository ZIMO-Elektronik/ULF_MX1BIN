#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include <vector>

TEST(verify, verify_long) {
  std::vector<uint8_t> frame{
    0x01, 0x01, 0x80, 0xFF, 0x0D, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x10, 0x21, 0x00, 0x00, 0x54, 0x0E, 0x10, 0x21, 0x14, 0x19, 0x00, 0x70,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x21, 0x89, 0xed, 0x40, 0xCF, 0x17};

  auto const result{ulf::mx1bin::detail::verify(frame)};
  ASSERT_TRUE(result);
  ASSERT_TRUE(*result);

  for (auto i{0uz}; i < size(**result); i++) {
    ASSERT_EQ(frame[i], (**result)[i]) << "Mismatch at " << i << std::endl;
  }
}

TEST(verify, verify_short) {
  std::vector<uint8_t> frame{
    0x01u, 0x01u, 0x80u, 0x10u, 0x30u, 0x02u, 0x02u, 0x55u, 0x17u};

  auto const result{ulf::mx1bin::detail::verify(frame)};
  ASSERT_TRUE(result);
  ASSERT_TRUE(*result);

  for (auto i{0uz}; i < size(**result); i++) {
    ASSERT_EQ(frame[i], (**result)[i]) << "Mismatch at " << i << std::endl;
  }
}
