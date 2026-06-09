#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, CommandStationIOQuery) {
  check<ulf::mx1bin::Command::CommandStationIOQuery,
        ulf::mx1bin::CommandStationIOQuery>();
}

TEST(Default, CommandStationIOQuery_Reply) {
  check<ulf::mx1bin::Command::CommandStationIOQuery,
        ulf::mx1bin::CommandStationIOQuery::Reply>();
}