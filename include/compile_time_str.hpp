#ifndef REFL_COMPILE_TIME_STR_H
#define REFL_COMPILE_TIME_STR_H

#include <cstddef>
#include <type_traits>
#include <utility>

#define MakeCharSeq(n, cstr)    \
    Refl::StrUtil::get_char_from_cstr<0x##n##0>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##1>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##2>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##3>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##4>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##5>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##6>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##7>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##8>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##9>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##a>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##b>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##c>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##d>(cstr), \
    Refl::StrUtil::get_char_from_cstr<0x##n##e>(cstr), Refl::StrUtil::get_char_from_cstr<0x##n##f>(cstr)

// MakeCharSequence("hello") => 'h', 'e', 'l', 'l', '0', '\0', '\0', '\0', ...    64 total
#define MakeCharSequence(cstr) MakeCharSeq(0, cstr), MakeCharSeq(1, cstr), MakeCharSeq(2, cstr), MakeCharSeq(3, cstr)

#define MKCTS64(cstr) Refl::StrUtil::compile_time_str<MakeCharSequence(cstr)>

#define MkCts(cstr) decltype(Refl::StrUtil::strip_end_zero(std::declval<MKCTS64(cstr)>()))

#define MKCTS(cstr) Refl::StrUtil::cts_split_once_t1<MKCTS64(cstr), '\0'>

// 只能直接创建对象，没办法通过decltype包装一下直接获取类型，因为decltype里面不能有lambda表达式，有点头疼
#define MkCtsObj(cstr) make_cts_obj([]() -> decltype(auto) { return cstr; })

namespace Refl
{

    namespace StrUtil
    {

    template <char C>
    using char_const = std::integral_constant<char, C>;

    // 编译期字符串，compile_time_str<'h','e','l','l','o'>表示hello
    template <char... Cs>
    struct compile_time_str {};

    using empty_cts = compile_time_str<>; // 空字符串给个别名

    // 这应该是个consteval函数(必须在编译期求值)，可惜c++17不支持，用constexpr代替(cstr是字符串字面量时可编译期求值)
    template <std::size_t M, std::size_t N>
    constexpr char get_char_from_cstr(const char(&cstr)[N])
    {
        return cstr[ M < N ? M : N - 1 ];
    }

    // 判断是否为编译期字符串 ----------------------------------------------------------------------------------------------
    template <typename CTS>
    struct is_cts : std::false_type {};

    template <char... Cs>
    struct is_cts<compile_time_str<Cs...>> : std::true_type {};

    template <typename CTS>
    inline constexpr bool is_cts_v = is_cts<CTS>::value;

    // 长度 -------------------------------------------------------------------------------------------------------------
    template <typename CTS>
    struct cts_size
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
    };

    template <char... Cs>
    struct cts_size<compile_time_str<Cs...>>
        : std::integral_constant<std::size_t, sizeof...(Cs)> {};

    template <typename CTS>
    inline constexpr std::size_t cts_size_v = cts_size<CTS>::value;

    // 判空 -------------------------------------------------------------------------------------------------------------
    template <typename CTS>
    struct is_cts_empty
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
    };

    template <char... Cs>
    struct is_cts_empty<compile_time_str<Cs...>>
        : std::bool_constant<sizeof...(Cs) != 0> {};

    template <typename CTS>
    inline constexpr std::size_t is_cts_empty_v = is_cts_empty<CTS>::value;

    // 丢弃第一个字符 ----------------------------------------------------------------------------------------------------
    template <typename CTS>
    struct cts_pop_front
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
        static_assert(cts_size_v<CTS> > 0, "CTS is empty, nothing to pop");
    };

    template <char C, char... Cs>
    struct cts_pop_front<compile_time_str<C, Cs...>>
    {
        using type = compile_time_str<Cs...>;
    };

    template <typename CTS>
    using cts_pop_front_t = typename cts_pop_front<CTS>::type;

    // 追加字符 ---------------------------------------------------------------------------------------------------------
    template <typename CTS, char C>
    struct cts_push_back
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
    };

    template <char... Cs, char C>
    struct cts_push_back<compile_time_str<Cs...>, C>
    {
        using type = compile_time_str<Cs..., C>;
    };

    template <typename CTS, char C>
    using cts_push_back_t = cts_push_back<CTS, C>;

    // 删除末尾字符 to do ------------------------------------------------------------------------------------------------
    template <typename CTS>
    struct cts_pop_back
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
    };

    // 合并 -------------------------------------------------------------------------------------------------------------
    template <typename CTS1, typename CTS2>
    struct cts_merge
    {
        static_assert(is_cts_v<CTS1> && is_cts_v<CTS2>, "CTS1, CTS2 must be compile_time_str");
    };

    template <char... Cs1, char... Cs2>
    struct cts_merge<compile_time_str<Cs1...>, compile_time_str<Cs2...>>
    {
        using type = compile_time_str<Cs1..., Cs2...>;
    };

    template <typename CTS1, typename CTS2>
    using cts_merge_t = typename cts_merge<CTS1, CTS2>::type;

    // 根据下标获取字符 --------------------------------------------------------------------------------------------------
    template <typename CTS, std::size_t I>
    struct cts_get_nth_char
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
        static_assert(I < cts_size_v<CTS>, "index out of range");
    };

    template <char C, char... Cs>
    struct cts_get_nth_char<compile_time_str<C, Cs...>, 0>
    {
        using type = char_const<C>;
    };

    template <char C, char... Cs, std::size_t I>
    struct cts_get_nth_char<compile_time_str<C, Cs...>, I>
        : cts_get_nth_char<compile_time_str<Cs...>, I - 1> {};

    template <typename CTS, std::size_t I>
    using cts_get_nth_char_t = typename cts_get_nth_char<CTS, I>::type;

    template <typename CTS, std::size_t I>
    inline constexpr char cts_get_nth_char_v = cts_get_nth_char_t<CTS, I>::value;

    // 小于 -------------------------------------------------------------------------------------------------------------
    template <typename CTS1, typename CTS2>
    struct cts_less
    {
        static_assert(is_cts_v<CTS1> && is_cts_v<CTS2>, "CTS1 and CTS2 must be compile_time_str");
    };

    template <> // 都是空字符串
    struct cts_less<empty_cts, empty_cts>
        : std::false_type {};

    template <char C, char... Cs> // CTS1为空， CTS2不空
    struct cts_less<empty_cts, compile_time_str<C, Cs...>>
        : std::true_type {};

    template <char C, char... Cs> // CTS1不空， CTS2为空
    struct cts_less<compile_time_str<C, Cs...>, empty_cts>
        : std::false_type {};

    template <char C1, char... Cs1, char C2, char... Cs2> // 都是非空字符串
    struct cts_less<compile_time_str<C1, Cs1...>, compile_time_str<C2, Cs2...>>
        : std::conditional_t<C1 != C2,
            std::bool_constant<C1 < C2>, // 首字符不同
            typename cts_less<compile_time_str<Cs1...>, compile_time_str<Cs2...>>::type // 首字符相同去掉首字符继续比较
        > {};

    template <typename CTS1, typename CTS2>
    using cts_less_t = typename cts_less<CTS1, CTS2>::type;

    template <typename CTS1, typename CTS2>
    inline constexpr bool cts_less_v = cts_less<CTS1, CTS2>::value;

    // 相等 -------------------------------------------------------------------------------------------------------------
    template <typename CTS1, typename CTS2,
        std::enable_if<is_cts_v<CTS1> && is_cts_v<CTS2>, int> = 0>
    struct cts_equal : std::is_same<CTS1, CTS2> {};

    template <typename CTS1, typename CTS2>
    inline constexpr bool cts_equal_v = cts_equal<CTS1, CTS2>::value;


    // 分割 -------------------------------------------------------------------------------------------------------------
    template <typename CTS, char delim>
    struct cts_split_once
    {
        static_assert(is_cts_v<CTS>, "CTS must be compile_time_str");
    };

    template <char delim>
    struct cts_split_once<empty_cts, delim> // 空字符串分割结果结果必然是两个空字符串
    {
        using type1 = empty_cts;
        using type2 = empty_cts;
    };

    template <char delim, char... Cs>
    struct cts_split_once<compile_time_str<delim, Cs...>, delim> // 首字符就是分割符
    {
        using type1 = empty_cts;
        using type2 = compile_time_str<Cs...>;
    };

    template <char C, char... Cs, char delim>
    struct cts_split_once<compile_time_str<C, Cs...>, delim> // 首字符不是分隔符
    {
        // type1 等于 首字符 + 去掉首字符后剩下部分的分割结果的type1
        using type1 = cts_merge_t<compile_time_str<C>,
            typename cts_split_once<compile_time_str<Cs...>, delim>::type1>;
        // type2 等于 去掉首字符后剩下部分的分割结果的type2
        using type2 = typename cts_split_once<compile_time_str<Cs...>, delim>::type2;
    };

    template <typename CTS, char delim>
    using cts_split_once_t1 = typename cts_split_once<CTS, delim>::type1;
    template <typename CTS, char delim>
    using cts_split_once_t2 = typename cts_split_once<CTS, delim>::type2;

    // 通过重载递归去除compile_time_str末尾所有的'\0' -----------------------------------------------------------------------
    template <char... Cs>
    auto recombine(compile_time_str<Cs...>)
        -> compile_time_str<Cs...>;

    template <char... Cs>
    auto recombine(compile_time_str<'\0'>, compile_time_str<'\0'>, compile_time_str<Cs>...)
        -> empty_cts;

    template <char... Cs1, char... Cs2>
    auto recombine(compile_time_str<Cs1...>, compile_time_str<'\0'>, compile_time_str<Cs2>...)
        -> compile_time_str<Cs1...>;

    template <char Other, char... Cs1, char... Cs2>
    auto recombine(compile_time_str<Cs1...>, compile_time_str<Other>, compile_time_str<Cs2>...)
        -> decltype(recombine(std::declval<compile_time_str<Cs1..., Other>>(),
            std::declval<compile_time_str<Cs2>>()...));

    template <char... Cs>
    auto strip_end_zero(compile_time_str<Cs...>)
        -> decltype(recombine(std::declval<compile_time_str<Cs>>()...));

    // 字符串字面量转compile_time_str对象 ---------------------------------------------------------------------------------
    template <typename T, std::size_t... Is>
    constexpr auto make_cts_obj_impl(T cstr_wrapper, std::index_sequence<Is...>)
    {
        return compile_time_str<cstr_wrapper()[Is]...>{};
    }

    template <typename T>
    constexpr auto make_cts_obj(T cstr_wrapper)
    {
        return make_cts_obj_impl(cstr_wrapper, std::make_index_sequence<sizeof(cstr_wrapper()) - 1>{});
    }

    } // StrUtil

} // namespace Refl

#endif // REFL_COMPILE_TIME_STR_H