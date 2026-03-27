#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, CommandStationEquipmentQuery) {
  check<ulf::mx1bin::Command::CommandStationEquipmentQuery,
        ulf::mx1bin::CommandStationEquipmentQuery>();
}

/// \todo Make This struct comply to the codable concept
// TEST(Default, CommandStationEquipmentQuery_Reply) {
//   check<ulf::mx1bin::Command::CommandStationEquipmentQuery,
//         ulf::mx1bin::CommandStationEquipmentQuery::Reply>();
// }
