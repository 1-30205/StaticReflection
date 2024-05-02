#ifndef _STATIC_REFLECTION_H_
#define _STATIC_REFLECTION_H_

#include <cstdint>
#include <type_traits>
#include <utility>

#define REFL_IDENTITY(...) __VA_ARGS__
#define REFL_STR(x) #x
#define REFL_STRING(x) REFL_STR(x)
#define REFL_CONCATE(x, y) x ## y
#define REFL_EXPAND_CAT(x, y) REFL_CONCATE(x, y)

#define REFL_ARGS_FILTER( \
    _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, _11, _12, _13, _14, _15,\
    _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31,\
    n, ...) n

#define REFL_GET_ARGS_COUNT(...) REFL_IDENTITY(REFL_ARGS_FILTER( \
    unused, ##__VA_ARGS__, \
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, \
    15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0))

#define REFL_REPEAT_A0_0(func, arg0, i)
#define REFL_REPEAT_A0_1(func, arg0, i, arg)        func(arg0, i, arg)
#define REFL_REPEAT_A0_2(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_1(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_3(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_2(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_4(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_3(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_5(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_4(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_6(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_5(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_7(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_6(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_8(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_7(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_9(func, arg0, i, arg, ...)   func(arg0, i, arg) REFL_REPEAT_A0_8(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_10(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_9(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_11(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_10(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_12(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_11(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_13(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_12(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_14(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_13(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_15(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_14(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_16(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_15(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_17(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_16(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_18(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_17(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_19(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_18(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_20(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_19(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_21(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_20(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_22(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_21(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_23(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_22(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_24(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_23(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_25(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_24(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_26(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_25(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_27(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_26(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_28(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_27(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_29(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_28(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_30(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_29(func, arg0, i + 1, __VA_ARGS__)
#define REFL_REPEAT_A0_31(func, arg0, i, arg, ...)  func(arg0, i, arg) REFL_REPEAT_A0_30(func, arg0, i + 1, __VA_ARGS__)

#define REFL_REPEAT_A0_ON(func, arg0, ...) \
    REFL_EXPAND_CAT(REFL_REPEAT_A0_, REFL_GET_ARGS_COUNT(__VA_ARGS__)) (func, arg0, 0, ##__VA_ARGS__)

// 生成结构体域元信息
#define REFL_STRUCT_FIELD(Struct, i, member)                        \
    template<>                                                      \
    struct field<i> {                                               \
        using type = decltype(Struct::member);                      \
        static constexpr const char* name() {                       \
            return REFL_STRING(member);                             \
        }                                                           \
        template<typename T>                                        \
        static constexpr decltype(auto) ref(T&& obj) {              \
            return (std::forward<T>(obj).member);                   \
        }                                                           \
    };

// 反射结构体，生成结构体的元信息，用法如下(在结构体的public域内使用)
// struct Point
// {
//     int x;
//     int y;
//     REFL_STRUCT(Point, x, y);
// };
#define REFL_STRUCT(Struct, ...)                                    \
    struct rf_meta_info {                                           \
        using type = Struct;                                        \
        static constexpr std::size_t field_count =                  \
            REFL_GET_ARGS_COUNT(__VA_ARGS__);                       \
        static constexpr const char* name() {                       \
            return REFL_STRING(Struct);                             \
        }                                                           \
        template <std::size_t> struct field;                        \
        REFL_REPEAT_A0_ON(REFL_STRUCT_FIELD, Struct, ##__VA_ARGS__) \
    }

namespace Refl {

template<typename T, typename = void>
struct is_reflected: std::false_type {};

template<typename T>
struct is_reflected<T, std::void_t<typename T::rf_meta_info>> : std::true_type {};

template<typename T>
inline constexpr bool is_reflected_v = is_reflected<T>::value;

template<typename T, typename = std::enable_if_t<is_reflected<T>::value>>
inline constexpr const char* type_name = T::rf_meta_info::name();

template<typename T, typename = std::enable_if_t<is_reflected<T>::value>>
inline constexpr std::size_t field_count = T::rf_meta_info::field_count;

template<typename T, std::size_t i, typename = std::enable_if_t<is_reflected<T>::value>>
using field = typename T::rf_meta_info::template field<i>;

template<typename T, std::size_t i>
using field_t = typename field<T, i>::type;

template<typename T, std::size_t i>
inline constexpr const char* field_name = field<T, i>::name();

template<typename T>
constexpr std::size_t get_field_count()
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_STRUCT in struct definition first");

    return field_count<DT>;
}

template<typename T, std::size_t i>
constexpr const char* get_field_name()
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_STRUCT in struct definition first");
    static_assert(i < field_count<DT>, "index of out range");

    return field<DT, i>::name();
}

template<std::size_t i, typename T>
constexpr decltype(auto) get_field_ref(T&& obj)
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_STRUCT in struct definition first");
    static_assert(i < field_count<DT>, "index of out range");

    return field<DT, i>::ref(std::forward<T>(obj));
}

template <typename T, typename F, std::size_t... Is>
constexpr void for_each_field_impl(T&& obj, F&& f, std::index_sequence<Is...>)
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_STRUCT in struct definition first");

    (std::forward<F>(f)(Is, field_name<DT, Is>, get_field_ref<Is>(std::forward<T>(obj))), ...);
}

template <typename T, typename F>
constexpr void for_each_field(T&& obj, F&& f)
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_STRUCT in struct definition first");

    for_each_field_impl(std::forward<T>(obj), std::forward<F>(f),
        std::make_index_sequence<field_count<DT>>{});
}

} // namespace Refl

#endif // _STATIC_REFLECTION_H_