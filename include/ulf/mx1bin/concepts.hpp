#pragma once

#include <concepts>
#include <cstdint>
#include <optional>

namespace ulf::mx1bin::concepts {

namespace detail {

template<typename T>
concept is_optional =
  requires { typename T::value_type; } &&
  std::same_as<std::remove_cvref_t<T>,
               std::optional<std::remove_cvref_t<typename T::value_type>>>;

template<is_optional T>
using remove_optional_t = std::remove_cvref_t<typename T::value_type>;

template<typename T, typename U>
concept typed_enum =
  std::is_enum_v<T> && std::same_as<std::underlying_type_t<T>, U>;

template<typename T, typename To>
concept explicitly_convertible_to = requires(T t) { static_cast<To>(t); };

} // namespace detail

template<typename T>
concept trivial_data = (std::same_as<std::remove_cvref_t<T>, std::uint8_t>) ||
                       (std::same_as<std::remove_cvref_t<T>, std::uint16_t>);

template<typename T>
concept optional_trivial_data =
  (detail::is_optional<T>) && (trivial_data<detail::remove_optional_t<T>>);

template<typename T, typename S>
concept bitfield =
  (!detail::is_optional<T>) && (!trivial_data<T>) &&
  (std::constructible_from<T, S>) && detail::explicitly_convertible_to<T, S>;

template<typename T, typename S>
concept optional_bitfield =
  (detail::is_optional<T>) && (!optional_trivial_data<T>) &&
  (std::constructible_from<detail::remove_optional_t<T>, S>) &&
  detail::explicitly_convertible_to<detail::remove_optional_t<T>, S>;

template<typename T, typename S>
concept strong_enum =
  (!detail::is_optional<T>) && (!trivial_data<T>) &&
  (detail::typed_enum<T, S>) && (!std::convertible_to<T, S>);

template<typename T, typename S>
concept optional_strong_enum =
  (!detail::is_optional<T>) && (!optional_trivial_data<T>) &&
  (detail::typed_enum<detail::remove_optional_t<T>, S>) &&
  (!std::convertible_to<detail::remove_optional_t<T>, S>);

template<typename T>
concept required =
  (trivial_data<T>) || (bitfield<T, std::uint8_t>) ||
  (bitfield<T, std::uint16_t>) || (strong_enum<T, std::uint8_t>) ||
  (strong_enum<T, std::uint16_t>);

} // namespace ulf::mx1bin::concepts
