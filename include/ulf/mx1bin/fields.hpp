#pragma once

#include <tuple>

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
};

} // namespace ulf::mx1bin
