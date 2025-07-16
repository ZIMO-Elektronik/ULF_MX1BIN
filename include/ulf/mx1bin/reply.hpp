#pragma once

#include <cstdint>
#include <span>
#include <ztl/inplace_vector.hpp>
#include "utility.hpp"

namespace ulf::mx1bin {

struct Head {
  uint8_t uSID{};
  uint8_t type{};
  uint8_t code{};
  uint8_t reply_uSID{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    detail::encode(uSID, out);
    detail::encode(type, out);
    detail::encode(code, out);
    detail::encode(reply_uSID, out);
    return out;
  }
  Head& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    uSID = detail::decode(iter);
    type = detail::decode(iter);
    code = detail::decode(iter);
    reply_uSID = detail::decode(iter);
    return *this;
  }
};

struct ShortHead {
  uint8_t uSID{};
  uint8_t type{};
  uint8_t code{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    detail::encode(uSID, out);
    detail::encode(type, out);
    detail::encode(code, out);
    return out;
  }
};

struct DecoderControl {
  Head head{};
  uint8_t error{};
  uint8_t payload{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(error, out);
    detail::encode(payload, out);
    return out;
  }
};

struct LocoMemoryQuery {
  Head head{};
  uint8_t error{};
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t cSpeed{};
  uint8_t cData1{};
  uint8_t cData2{};
  uint8_t cData3{};
  uint8_t cAzBz{};
  uint8_t cStatus{};
  uint8_t cData4{};
  uint8_t cData5{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(error, out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(cSpeed, out);
    detail::encode(cData1, out);
    detail::encode(cData2, out);
    detail::encode(cData3, out);
    detail::encode(cAzBz, out);
    detail::encode(cStatus, out);
    detail::encode(cData4, out);
    detail::encode(cData5, out);
    return out;
  }
};

struct AccessoryMemoryQuery {
  Head head{};
  uint8_t error{};
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t cPair{};
  uint8_t cOutputs{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(error, out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(cPair, out);
    detail::encode(cOutputs, out);
    return out;
  }
};

struct AddressControl {
  Head head{};
  uint8_t error{};
  uint8_t payload{};
  uint8_t cOutputs{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(error, out);
    detail::encode(payload, out);
    detail::encode(cOutputs, out);
    return out;
  }
};

struct CommandStationIOQuery {
  Head head{};
  uint8_t values{};
  uint8_t cCurrent1_hi{};
  uint8_t cCurrent1_lo{};
  uint8_t cVoltage1{};
  uint8_t cCurrent2_hi{};
  uint8_t cCurrent2_lo{};
  uint8_t cVoltage2{};
  uint8_t cAux{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(values, out);
    detail::encode(cCurrent1_hi, out);
    detail::encode(cCurrent1_lo, out);
    detail::encode(cVoltage1, out);
    detail::encode(cCurrent2_hi, out);
    detail::encode(cCurrent2_lo, out);
    detail::encode(cVoltage2, out);
    detail::encode(cAux, out);
    return out;
  }
};

struct CommandStationCvManip {
  Head head{};
  uint8_t error{};
  uint8_t value{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(error, out);
    detail::encode(value, out);
    return out;
  }
};

struct CommandStationEquipmentQuery {
  uint8_t cAddress_hi{};
  uint8_t cAddress_lo{};
  uint8_t cDevice{};
  uint8_t cRom_size{};
  uint8_t cRam_size{};
  uint8_t cPrintver_hi{};
  uint8_t cPrintver_lo{};
  uint8_t cVersion_hi{};
  uint8_t cVersion_lo{};
  uint8_t cDate_day{};
  uint8_t cDate_month{};
  uint8_t cDate_century{};
  uint8_t cDate_year{};
  uint8_t cSwitches{};
  uint8_t cDevelopVersion{};
  uint8_t cBootRom_hi{};
  uint8_t cBootRom_lo{};
  uint8_t cBootRom_develop{};
  uint8_t values{};
  uint8_t cSerNum_hi{};
  uint8_t cSerNum_mh{};
  uint8_t cSerNum_ml{};
  uint8_t cSerNum_lo{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    detail::encode(cAddress_hi, out);
    detail::encode(cAddress_lo, out);
    detail::encode(cDevice, out);
    detail::encode(cRom_size, out);
    detail::encode(cRam_size, out);
    detail::encode(cPrintver_hi, out);
    detail::encode(cPrintver_lo, out);
    detail::encode(cVersion_hi, out);
    detail::encode(cVersion_lo, out);
    detail::encode(cDate_day, out);
    detail::encode(cDate_month, out);
    detail::encode(cDate_century, out);
    detail::encode(cDate_year, out);
    detail::encode(cSwitches, out);
    detail::encode(cDevelopVersion, out);
    detail::encode(cBootRom_hi, out);
    detail::encode(cBootRom_lo, out);
    detail::encode(cBootRom_develop, out);
    detail::encode(values, out);
    detail::encode(cSerNum_hi, out);
    detail::encode(cSerNum_mh, out);
    detail::encode(cSerNum_ml, out);
    detail::encode(cSerNum_lo, out);
    return out;
  }
};

struct CvManip {
  Head head{};
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t variable_hi{};
  uint8_t variable_lo{};
  uint8_t cValue{};
  uint8_t cError{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(variable_hi, out);
    detail::encode(variable_lo, out);
    detail::encode(cValue, out);
    detail::encode(cError, out);
    return out;
  }
};

struct CvManipError {
  Head head{};
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t cError{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(cError, out);
    return out;
  }
};

struct CvManipBusy {
  Head head{};
  uint8_t const busy{0x04u};
  uint8_t cAdr_hi{};
  uint8_t cAdr_lo{};
  uint8_t variable_hi{};
  uint8_t variable_lo{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt out) {
    head.encode(out);
    detail::encode(busy, out);
    detail::encode(cAdr_hi, out);
    detail::encode(cAdr_lo, out);
    detail::encode(variable_hi, out);
    detail::encode(variable_lo, out);
    return out;
  }
};

} // namespace ulf::mx1bin
