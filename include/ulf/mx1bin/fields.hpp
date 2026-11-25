#pragma once

#include <tuple>
#include "concepts.hpp"
#include "utility.hpp"

namespace ulf::mx1bin {

template<typename Class, typename Member>
Member member_type_of(Member Class::*);

template<typename T>
using member_type_t =
  decltype(member_type_of(std::declval<std::remove_cvref_t<T>>()));

template<auto Member>
struct member {
  static constexpr auto inner = Member;
};

template<typename... Ts>
struct member_list {
  inline static constexpr auto members = std::tuple{Ts::inner...};

  inline static constexpr std::size_t stream_size = std::apply(
    []<typename... Members>(Members... members) constexpr {
      return (std::size_t{0} + ... +
              detail::stream_size<member_type_t<decltype(members)>>());
    },
    members);

  inline static constexpr std::size_t required_stream_size = std::apply(
    []<typename... Members>(Members... members) constexpr {
      return (std::size_t{0} + ... +
              detail::required_stream_size<member_type_t<decltype(members)>>());
    },
    members);
};

} // namespace ulf::mx1bin
