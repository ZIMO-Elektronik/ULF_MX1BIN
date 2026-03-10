#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include <ulf/mx1bin/decoder.hpp>
#include <vector>

TEST(decoder, strip) {
  std::vector<uint8_t> proc{
    0x39, 0x10, 0x30, 0x13, 0x80, 0x03, 0x00, 0x1d, 0xea};

  std::vector<uint8_t> raw{0x01, 0x01};
  std::ranges::copy(proc, std::back_inserter(raw));
  raw.push_back(0x17);

  std::vector<uint8_t> result{};

  ulf::mx1bin::Decoder d{raw};
  d.strip();
  while (auto const val{d.s_uint8()}) { result.push_back(*val); }

  // ASSERT_TRUE(false);
}

TEST(decoder, s) {
  ulf::mx1bin::CommandStationEquipmentQuery q{};
  q.code = ulf::mx1bin::Command::Station_Equipment_Query;
  q.info.frameType = ulf::mx1bin::FrameType::Short;
  q.info.messageType = ulf::mx1bin::MessageType::Primary;
  q.info.sender = ulf::mx1bin::Sender::PC;
  q.info.stationType = ulf::mx1bin::StationType::MX1;

  ztl::inplace_vector<uint8_t, 80u> q_v{};

  ulf::mx1bin::Encoder e{q_v};
  e.addSOF();
  q.encode(e);
  q_v.resize(static_cast<decltype(q_v)::size_type>(e.difference()));
  e.uint8(ulf::mx1bin::crc8(std::span<uint8_t const>{q_v}.subspan(2uz)));
  e.addEOT();

  q_v.resize(static_cast<decltype(q_v)::size_type>(e.difference()));

  ulf::mx1bin::Decoder d{q_v};
  d.strip();
  [[maybe_unused]] auto head{ulf::mx1bin::detail::Head::decode(d)};

  std::span<uint8_t const> sp{q_v};

  [[maybe_unused]] auto message{ulf::mx1bin::mx1bin_2message(sp)};

  // ASSERT_TRUE(false);
}

TEST(decoder, y) {
  std::vector<uint8_t> v{
    0x01,
    0x01,
    0x0a,
    0x10,
    0x30,
    0x03,
    0x80,
    0x03,
    0x00,
    0x0c,
    0x10,
    0x21,
    0x00,
    0x00,
    0x00,
    0x86,
    0x17,
  };

  [[maybe_unused]] auto message{ulf::mx1bin::mx1bin_2message(v)};

  ASSERT_TRUE(true);
}

TEST(bitfield, y) {
  ulf::mx1bin::bitfields::Info info{};
  info.frameType = ulf::mx1bin::FrameType::Long;
  info.messageType = ulf::mx1bin::MessageType::Primary;
  info.sender = ulf::mx1bin::Sender::PC;
  info.stationType = ulf::mx1bin::StationType::MX1;

  auto value{static_cast<uint8_t>(info)};
  ASSERT_EQ(value, 0b10010000u);

  ulf::mx1bin::bitfields::Info result{value};
  ASSERT_EQ(result, info);
}
