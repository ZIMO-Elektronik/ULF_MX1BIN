cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY https://github.com/ZIMO-Elektronik/CMakeModules
  GIT_TAG v0.9.5
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  MX1Bin
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX)

add_library(MX1Bin INTERFACE ${SRC})
add_library(ULF::MX1Bin ALIAS MX1Bin)

target_compile_features(MX1Bin INTERFACE cxx_std_23)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  target_include_directories(MX1Bin INTERFACE include)
else()
  target_include_directories(MX1Bin SYSTEM INTERFACE include)
endif()

target_common_warnings(MX1Bin INTERFACE)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  add_clang_format_target(MX1BinFormat OPTIONS -i FILES ${SRC})
endif()

if(BUILD_TESTING
   AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME
   AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(examples)
  add_subdirectory(tests)
endif()
