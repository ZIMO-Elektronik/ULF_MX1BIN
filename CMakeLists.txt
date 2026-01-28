cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY https://github.com/ZIMO-Elektronik/CMakeModules
  GIT_TAG v0.10.0
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  ULF_MX1BIN
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX)

add_library(ULF_MX1BIN INTERFACE)
add_library(ULF::MX1BIN ALIAS ULF_MX1BIN)

target_compile_features(ULF_MX1BIN INTERFACE cxx_std_23)

# https://github.com/espressif/esp-idf/issues/17773
if(PROJECT_IS_TOP_LEVEL AND NOT ESP_PLATFORM)
  target_include_directories(ULF_MX1BIN INTERFACE include)
  target_common_warnings(ULF_MX1BIN INTERFACE)
  target_common_errors(ULF_MX1BIN INTERFACE)
else()
  target_include_directories(ULF_MX1BIN SYSTEM INTERFACE include)
endif()

if(NOT TARGET ctre::ctre)
  cpmaddpackage("gh:hanickadot/compile-time-regular-expressions@3.10.0")
endif()

if(NOT TARGET ZTL::ZTL)
  cpmaddpackage("gh:ZIMO-Elektronik/ZTL@0.24.1")
endif()

if(NOT TARGET DCC::DCC)
  cpmaddpackage("gh:ZIMO-Elektronik/DCC@0.44.5")
endif()

target_link_libraries(ULF_MX1BIN INTERFACE ctre::ctre DCC::DCC)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  add_subdirectory(examples)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC examples/*.[ch]pp include/*.[ch]pp src/*.[ch]pp
       tests/*.[ch]pp)
  add_clang_format_target(ULF_MX1BINFormat OPTIONS -i FILES ${SRC})
endif()

if(BUILD_TESTING
   AND PROJECT_IS_TOP_LEVEL
   AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(tests)
endif()
