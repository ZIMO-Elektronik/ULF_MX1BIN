#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ulf/mx1bin/decoder.hpp>
#include <vector>

TEST(decoder, strip) {
  std::vector<uint8_t> proc{
    0x39, 0x10, 0x30, 0x13, 0x80, 0x03, 0x00, 0x1d, 0xea};

  std::vector<uint8_t> raw{0x01, 0x01};
  std::ranges::copy(proc, std::back_inserter(raw));
  raw.push_back(0x17);

  std::vector<uint8_t> result{};

  ulf::mx1bin::detail::Decoder d{raw};
  d.strip();
  while (auto const val{d.s_uint8()}) { result.push_back(*val); }

  ASSERT_TRUE(false);
}
