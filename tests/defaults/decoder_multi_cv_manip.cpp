#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, DecoderMultiCvManip) {
  check<ulf::mx1bin::Command::DecoderMultiCvManip,
        ulf::mx1bin::DecoderMultiCvManip>();
}

TEST(Default, DecoderMultiCvManip_Reply) {
  check<ulf::mx1bin::Command::DecoderMultiCvManip,
        ulf::mx1bin::DecoderMultiCvManip::Reply>();
}

TEST(Default, DecoderMultiCvManip_ReplyL2) {
  check<ulf::mx1bin::Command::DecoderMultiCvManip,
        ulf::mx1bin::DecoderMultiCvManip::ReplyL2>();
}

TEST(Default, DecoderMultiCvManip_Busy) {
  check<ulf::mx1bin::Command::DecoderMultiCvManip,
        ulf::mx1bin::DecoderMultiCvManip::Busy>();
}

TEST(Default, DecoderMultiCvManip_Error) {
  check<ulf::mx1bin::Command::DecoderMultiCvManip,
        ulf::mx1bin::DecoderMultiCvManip::Error>();
}
