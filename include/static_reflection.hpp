#ifndef _STATIC_REFLECTION_H_
#define _STATIC_REFLECTION_H_

#include <cstddef>
#include <type_traits>
#include <utility>

#define REFL_EXPAND(...) __VA_ARGS__
#define REFL_STR(x) #x
#define REFL_STRING(x) REFL_STR(x)
#define REFL_CONCAT(x, y) x ## y
#define REFL_PASTE(x, y) REFL_CONCAT(x, y)
#define REFL_EAT(...)
// REFL_PAIR((int) x) => REFL_EXPAND(int) x => int x
#define REFL_PAIR(x) REFL_EXPAND x

#define REFL_GET_3RD_ARG(a, b, c, ...) c
#define REFL_VA_OPT_SUPPORTED_I(...) REFL_GET_3RD_ARG(__VA_OPT__(, ), 1, 0)
#define REFL_VA_OPT_SUPPORTED REFL_VA_OPT_SUPPORTED_I(?)


#define REFL_ARGS_FILTER( \
    _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, _11, _12, _13, _14, _15,\
    _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31,\
    n, ...) n

// msvc使用旧版预处理器(不完全符合标准)时，处理可变参有点特殊
#if (defined(_MSC_VER) && !defined(_MSVC_TRADITIONAL)) || \
    (defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL)

#define REFL_INNER_EXPAND_ARGS(...) \
    REFL_EXPAND(REFL_ARGS_FILTER(__VA_ARGS__, \
        31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, \
        15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0))

#define REFL_REMOVE_COMMA_IF_EMPTY(...) unused, __VA_ARGS__

#define REFL_GET_ARGS_COUNT(...) \
    REFL_INNER_EXPAND_ARGS(REFL_REMOVE_COMMA_IF_EMPTY(__VA_ARGS__))

#define REFL_REPEAT_FIRST_0(op, i, first, arg)
#define REFL_REPEAT_FIRST_1(op, i, first, arg, ...) \
    op(i, first, arg)
#define REFL_REPEAT_FIRST_2(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_1(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_3(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_2(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_4(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_3(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_5(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_4(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_6(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_5(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_7(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_6(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_8(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_7(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_9(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_8(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_10(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_9(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_11(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_10(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_12(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_11(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_13(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_12(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_14(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_13(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_15(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_14(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_16(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_15(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_17(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_16(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_18(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_17(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_19(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_18(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_20(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_19(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_21(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_20(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_22(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_21(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_23(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_22(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_24(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_23(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_25(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_24(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_26(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_25(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_27(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_26(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_28(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_27(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_29(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_28(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_30(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_29(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_31(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_30(op, i+1, first, __VA_ARGS__))
#define REFL_REPEAT_FIRST_32(op, i, first, arg, ...) \
    op(i, first, arg) REFL_EXPAND(REFL_REPEAT_FIRST_31(op, i+1, first, __VA_ARGS__))

#define REFL_REPEAT_FIRST_ON(op, first, ...) \
    REFL_EXPAND( \
        REFL_PASTE(REFL_REPEAT_FIRST_, REFL_GET_ARGS_COUNT(__VA_ARGS__)) \
        (op, 0, first, __VA_ARGS__) \
    )

// REFL_STRIP((int) x) => REFL_EAT(int) x => x
#define REFL_STRIP(x) REFL_EXPAND(REFL_EAT x)

#else // 其他编译器(一般就是gcc、clang和使用了标准预处理器的msvc了)

// 判断空可变参处理策略
#if REFL_VA_OPT_SUPPORTED // c++20标准支持__VA_OPT__
#define REFL_GET_ARGS_COUNT(...) \
    REFL_ARGS_FILTER(unused, __VA_OPT__(, ) __VA_ARGS__, \
        31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16 \
        15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

#elif defined(__GNUC__) // 不支持__VA_OPT__，但使用的是gcc或者clang，支持##__VA_ARGS__的用法
#define REFL_GET_ARGS_COUNT(...) \
    REFL_ARGS_FILTER(unused, ##__VA_ARGS__, \
        31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16 \
        15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

#else // 其他情况，REFL_GET_ARGS_COUNT()可能返回1
#define REFL_GET_ARGS_COUNT(...) \
    REFL_ARGS_FILTER(unused, __VA_ARGS__, \
        31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16 \
        15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

#endif // 判断空可变参处理策略结束

#define REFL_REPEAT_FIRST_0(op, i, first, arg)
#define REFL_REPEAT_FIRST_1(op, i, first, arg, ...) \
    op(i, first, arg)
#define REFL_REPEAT_FIRST_2(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_1(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_3(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_2(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_4(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_3(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_5(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_4(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_6(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_5(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_7(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_6(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_8(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_7(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_9(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_8(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_10(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_9(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_11(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_10(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_12(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_11(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_13(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_12(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_14(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_13(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_15(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_14(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_16(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_15(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_17(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_16(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_18(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_17(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_19(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_18(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_20(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_19(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_21(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_20(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_22(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_21(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_23(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_22(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_24(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_23(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_25(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_24(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_26(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_25(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_27(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_26(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_28(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_27(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_29(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_28(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_30(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_29(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_31(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_30(op, i+1, first, __VA_ARGS__)
#define REFL_REPEAT_FIRST_32(op, i, first, arg, ...) \
    op(i, first, arg) REFL_REPEAT_FIRST_31(op, i+1, first, __VA_ARGS__)

#define REFL_REPEAT_FIRST_ON(op, first, ...) \
    REFL_PASTE(REFL_REPEAT_FIRST_, REFL_GET_ARGS_COUNT(__VA_ARGS__)) \
    (op, 0, first, __VA_ARGS__)

// REFL_STRIP((int) x) => REFL_EAT(int) x => x
#define REFL_STRIP(x) REFL_EAT x

#endif // 编译器判断结束

// 生成结构体域元信息
#define REFL_FIELD(I, Struct, member)                               \
    template<typename Unused>                                       \
    struct field<I, Unused> {                                       \
        using type = decltype(Struct::member);                      \
        static constexpr const char* name = REFL_STRING(member);    \
        template<typename T>                                        \
        static constexpr decltype(auto) ref(T&& obj) {              \
            return (std::forward<T>(obj).member);                   \
        }                                                           \
    };

#if REFL_GET_ARGS_COUNT() != 0
#warning "REFL_GET_ARGS_COUNT() != 0, reflect empty struct may result compile error"
#endif

// 反射结构体，生成结构体的元信息，用法如下(宏需要在结构体的public域内使用，被反射的成员变量不需要为public):
// struct Point
// {
//     int x;
//     int y;
//     REFL_CLASS(Point, x, y);
// };
#define REFL_CLASS(Struct, ...)                                    \
    struct rf_meta_info {                                           \
        using type = Struct;                                        \
        static constexpr const char* name = REFL_STRING(Struct);    \
        static constexpr std::size_t field_count =                  \
            REFL_GET_ARGS_COUNT(__VA_ARGS__);                       \
        template<std::size_t, typename> struct field;               \
        REFL_REPEAT_FIRST_ON(REFL_FIELD, Struct, __VA_ARGS__)       \
    };                                                              \
    template<std::size_t I, typename Unused>                        \
    friend struct rf_meta_info::field

namespace Refl {

template<typename T, typename = void>
struct is_reflected: std::false_type {};

template<typename T>
struct is_reflected<T, std::void_t<typename T::rf_meta_info>> : std::true_type {};

template<typename T>
inline constexpr bool is_reflected_v = is_reflected<T>::value;

template<typename T, typename = std::enable_if_t<is_reflected<T>::value>>
inline constexpr const char* struct_name = T::rf_meta_info::name;

template<typename T, typename = std::enable_if_t<is_reflected<T>::value>>
inline constexpr std::size_t field_count = T::rf_meta_info::field_count;

template<typename T, std::size_t I, typename = std::enable_if_t<is_reflected<T>::value>>
using field = typename T::rf_meta_info::template field<I, void>;

template<typename T, std::size_t I>
using field_type = typename field<T, I>::type;

template<typename T, std::size_t I>
inline constexpr const char* field_name = field<T, I>::name;

template<std::size_t I, typename T>
constexpr decltype(auto) field_ref(T&& obj)
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_CLASS in struct definition first");
    static_assert(I < field_count<DT>, "index of out range");

    return field<DT, I>::ref(std::forward<T>(obj));
}

template<typename T, typename F, std::size_t... Is>
constexpr void for_each_field_impl(T&& obj, F&& f, std::index_sequence<Is...>)
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_CLASS in struct definition first");

    (std::forward<F>(f)(Is, field_name<DT, Is>, field_ref<Is>(std::forward<T>(obj))), ...);
}

template<typename T, typename F>
constexpr void for_each_field(T&& obj, F&& f)
{
    using DT = std::decay_t<T>;
    static_assert(is_reflected<DT>::value,
        "T is not reflected, please use REFL_CLASS in struct definition first");

    for_each_field_impl(std::forward<T>(obj), std::forward<F>(f),
        std::make_index_sequence<field_count<DT>>{});
}

} // namespace Refl

#endif // _STATIC_REFLECTION_H_