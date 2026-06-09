#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, CommandStationCvManip) {
  check<ulf::mx1bin::Command::CommandStationCvManip,
        ulf::mx1bin::CommandStationCvManip>();
}

TEST(Default, CommandStationCvManip_Reply) {
  check<ulf::mx1bin::Command::CommandStationCvManip,
        ulf::mx1bin::CommandStationCvManip::Reply>();
}