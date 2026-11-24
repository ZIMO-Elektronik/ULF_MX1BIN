#include <iostream>
#include <ulf/mx1bin.hpp>

int main() {
  ulf::mx1bin::LocoControl m{};

  m.head.uSID = 80;

  m.cAdr.address = 3;

  std::vector<uint8_t> v{};
  std::vector<uint8_t> t_v{};

  m.encode(ulf::mx1bin::StreamEncoder(v));

  auto d_m{ulf::mx1bin::neww::decode<ulf::mx1bin::LocoControl>(v)};

  ulf::mx1bin::neww::encode(d_m, t_v);

  auto cout_vector{[](std::vector<uint8_t> vector) {
    for (auto const b : vector) {
      std::cout << static_cast<unsigned int>(b) << " ";
    }
    std::cout << std::endl;
  }};

  std::cout << "Vector: " << std::hex << std::endl;
  cout_vector(v);

  std::cout << "Stuff happened Vector: " << std::endl;
  cout_vector(t_v);
}
