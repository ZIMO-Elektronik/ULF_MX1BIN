# MX1Bin

[![build](https://github.com/ZIMO-Elektronik/ULF_MX1BIN/actions/workflows/build.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ULF_MX1BIN/actions/workflows/build.yml) [![tests](https://github.com/ZIMO-Elektronik/ULF_MX1BIN/actions/workflows/tests.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ULF_MX1BIN/actions/workflows/tests.yml) [![license](https://img.shields.io/github/license/ZIMO-Elektronik/ULF_MX1BIN)](https://github.com/ZIMO-Elektronik/ULF_MX1BIN/raw/master/LICENSE)

ULF_MX1BIN is one of several [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) protocols. It is used to control a running command station from e.g. PC. This can be done during normal operation and in addition to a present interface on the command station. 

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#protocol">Protocol</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#build">Build</a></li>
      </ul>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

## Protocol 

The protocol is based on type-sized messages, that can be roughly categorized into long/short and primary/reply messages. As a general rule, each primary message must generate at least a synchronous reply (Level 1). Additionally, depending on the primary message, an additional, asynchronous message (Level 2) can be transmitted.

The stream representation is encoded before and after transfer. Hence, the actual message size may differ from the stream size. 

Diagrams

:construction:

## Getting Started
### Prerequisites
- C++23 compatible compiler
- [CMake](https://cmake.org/) ( >= 3.25 )

### Installation
This library is meant to be consumed with CMake.

```cmake
#Either by including it with CPM
cpmaddpackage("gh:ZIMO-Elektronik/ULF_MX1BIN@0.1.0")

# or the FetchContent module
FetchContent_Declare(
  ULF_MX1BIN
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/ULF_MX1BIN"
  GIT_TAG 0.1.0)
FetchContent_MakeAvailable(ULF_MX1BIN)

target_link_libraries(YourTarget PRIVATE ULF::MX1Bin)
```

### Build
:construction:

## Usage
To convert a MX1Bin string to a message, `mx1bin_2message` can be used. In order to be able to distinguish between an error case and the case where the data is still incomplete, the return value of the functions is `std::expected<std::optional<Message>, std::errc>`. If the pattern is not recognized at all, i.e. in the event of an error, then a `std::errc` is returned. If something is found but the data is not yet complete, a `std::nullopt` is returned. Otherwise the found data is returned as a `Message` variant. The following snippet shows how `mx1bin_2message` can be used.
> [!NOTE]  
> As of now, this library contains mostly MX1 type short messages. Also, `mx1bin_2message` only works correctly for primary messages (with the exception of Command 19, L1Ack). 

```cpp
// Get MX1Bin Message from MX1Bin string
auto maybe_message{ulf::mx1bin::mx1bin_2message(string)};

// Could be MX1Bin message
if (maybe_message) {
  // Already complete?
  if (*maybe_message) {
    // Complete MX1Bin message - discern type
    if (const auto rst{std::get_if<ulf::mx1bin::Reset>(**maybe_message)})
      // Handle Reset
    else if (const auto ctrl{std::get_if<ulf::mx1bin::TrackControl>(**maybe_message)})
      // handle TrackControl
    // ... //
  }
  // No, still missing characters
  else {}
}
// Error, not MX1Bin message
else {}
```

A matching  `Reply` type is contained in each primary `Message`. Once filled, it can be converted to a response string. 
```cpp
// Create Response from Feedback
auto response{ulf::mx1bin::response2mx1bin(reply)};
```




