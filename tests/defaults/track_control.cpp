#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, TrackControl) {
  check<ulf::mx1bin::Command::TrackControl, ulf::mx1bin::TrackControl>();
}

TEST(Default, TrackControl_Reply) {
  check<ulf::mx1bin::Command::TrackControl, ulf::mx1bin::TrackControl::Reply>();
}
