#pragma once
#include<type_traits>
template<typename T> concept enumeration = std::is_enum_v<T>;
template<enumeration T> constexpr inline T  operator~  (T a) noexcept       { return static_cast<T>(~static_cast<std::underlying_type_t<T> >(a)); }
template<enumeration T> constexpr inline T  operator|  (T a,  T b) noexcept { return static_cast<T>( static_cast<std::underlying_type_t<T> >(a) | static_cast<std::underlying_type_t<T> >(b)); }
template<enumeration T> constexpr inline T  operator&  (T a,  T b) noexcept { return static_cast<T>( static_cast<std::underlying_type_t<T> >(a) & static_cast<std::underlying_type_t<T> >(b)); }
template<enumeration T> constexpr inline T  operator^  (T a,  T b) noexcept { return static_cast<T>( static_cast<std::underlying_type_t<T> >(a) ^ static_cast<std::underlying_type_t<T> >(b)); }
template<enumeration T> constexpr inline T& operator|= (T& a, T b) noexcept { return a = a | b; }
template<enumeration T> constexpr inline T& operator&= (T& a, T b) noexcept { return a = a & b; }
template<enumeration T> constexpr inline T& operator^= (T& a, T b) noexcept { return a = a ^ b; }
template<enumeration T> constexpr inline bool operator! (T a) noexcept { return !static_cast<std::underlying_type_t<T>>(a); }