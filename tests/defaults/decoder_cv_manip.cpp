#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, DecoderCvManip) {
  check<ulf::mx1bin::Command::DecoderCvManip, ulf::mx1bin::DecoderCvManip>();
}

TEST(Default, DecoderCvManip_Reply) {
  check<ulf::mx1bin::Command::DecoderCvManip,
        ulf::mx1bin::DecoderCvManip::Reply>();
}

TEST(Default, DecoderCvManip_ReplyL2) {
  check<ulf::mx1bin::Command::DecoderCvManip,
        ulf::mx1bin::DecoderCvManip::ReplyL2>();
}

TEST(Default, DecoderCvManip_Busy) {
  check<ulf::mx1bin::Command::DecoderCvManip,
        ulf::mx1bin::DecoderCvManip::Busy>();
}

TEST(Default, DecoderCvManip_Error) {
  check<ulf::mx1bin::Command::DecoderCvManip,
        ulf::mx1bin::DecoderCvManip::Error>();
}