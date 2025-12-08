#ifndef REFL_SEQUENCE_TYPE_TRAITS_HPP
#define REFL_SEQUENCE_TYPE_TRAITS_HPP

#include <cstddef>
#include <type_traits>
#include <utility>
#include "type_list.hpp"

namespace Refl::Util
{

// 判断是否为std::integer_sequence
template <typename T>
struct is_seq : std::false_type {};

template <typename T, T... Ints>
struct is_seq<std::integer_sequence<T, Ints...>> : std::true_type {};

template <typename T>
inline constexpr bool is_seq_v = is_seq<T>::value;


// 获取std::integer_sequence<T, Ints...>的整数类型T
template <typename T>
struct seq_int
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
};

template <typename T, T... Ints>
struct seq_int<std::integer_sequence<T, Ints...>>
{
    using type = T;
};

template <typename T>
using seq_int_t = typename seq_int<T>::type;


// 元素数量
template <typename T>
struct seq_size
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
};

template <typename T, T... Ints>
struct seq_size<std::integer_sequence<T, Ints...>>
    : std::integer_sequence<std::size_t, sizeof...(Ints)> {};

template <typename T>
inline constexpr std::size_t seq_size_v = seq_size<T>::value;

// 判空
template <typename T>
inline constexpr bool is_empty_seq_v = (seq_size_v<T> == 0);


// 头插
template <typename T, auto I>
struct seq_push_front
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, decltype(I)>, "T and I has to match");
};

template <typename T, T... Ints, T I>
struct seq_push_front<std::integer_sequence<T, Ints...>, I>
{
    using type = std::integer_sequence<T, I, Ints...>;
};

template <typename T, auto I>
using seq_push_front_t = typename seq_push_front<T, I>::type;


// 尾插
template <typename T, auto I>
struct seq_push_back
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, decltype(I)>, "T and I has to match");
};

template <typename T, T... Ints, T I>
struct seq_push_back<std::integer_sequence<T, Ints...>, I>
{
    using type = std::integer_sequence<T, Ints..., I>;
};

template <typename T, auto I>
using seq_push_back_t = typename seq_push_back<T, I>::type;


// 获取第N个元素
template<typename T, std::size_t N>
struct seq_get_nth
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(N < seq_size_v<T>, "index out of range");
};

template <typename T, T Head, T... Ints>
struct seq_get_nth<std::integer_sequence<T, Head, Ints...>, 0>
{
    using type = std::integral_constant<T, Head>;
};

template <typename T, T Head, T... Ints, std::size_t N>
struct seq_get_nth<std::integer_sequence<T, Head, Ints...>, N>
    : seq_get_nth<std::integer_sequence<T, Ints...>, N - 1> {};

template<typename T, std::size_t N>
using seq_get_nth_t = typename seq_get_nth<T, N>::type;

template<typename T, std::size_t N>
inline constexpr auto seq_get_nth_v = seq_get_nth_t<T, N>::value;

// 首元素
template <typename T>
using seq_front_t = seq_get_nth_t<T, 0>;

template <typename T>
inline constexpr auto seq_front_v = seq_front_t<T>::value;

// 尾元素
template <typename T>
using seq_back_t = seq_get_nth_t<T, seq_size_v<T> - 1>;

template <typename T>
inline constexpr auto seq_back_v = seq_back_t<T>::value;


// 过滤(根据std::index_sequence 过滤旧sequence形成新的sequence)
template <typename T, typename IdxSeq>
struct seq_filter
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(is_seq_v<IdxSeq> && std::is_same_v<seq_int_t<IdxSeq>, std::size_t>,
        "IdxSeq must be std::index_sequence");
};

template <typename T, std::size_t... Is>
struct seq_filter<T, std::index_sequence<Is...>>
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    using type = std::integer_sequence<seq_int_t<T>, seq_get_nth_v<T, Is>...>;
};

template <typename T, typename IdxSeq>
using seq_filter_t = typename seq_filter<T, IdxSeq>::type;


// 删头
template <typename T>
struct seq_pop_front
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(seq_size_v<T> > 0, "sequence empty, nothing to pop");
};

template <typename T, T Head, T... Ints>
struct seq_pop_front<std::integer_sequence<T, Head, Ints...>>
{
    using type = std::integer_sequence<T, Ints...>;
};

template <typename T>
using seq_pop_front_t = typename seq_pop_front<T>::type;


// 删尾
template <typename T>
struct seq_pop_back
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(seq_size_v<T> > 0, "sequence empty, nothing to pop");
    using type = seq_filter_t<T, std::make_index_sequence<seq_size_v<T> - 1>>;
};

template <typename T>
using seq_pop_back_t = typename seq_pop_back<T>::type;


// 小于
template <typename T, typename U>
struct seq_less
{
    static_assert(is_seq_v<T> && is_seq_v<U>, "T && U must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, seq_int_t<U>>, "T && U must have same int type");
};

template <typename T>
struct seq_less<std::integer_sequence<T>, std::integer_sequence<T>> : std::false_type {};

template <typename T, T Head, T... Ints>
struct seq_less<std::integer_sequence<T>, std::integer_sequence<T, Head, Ints...>> : std::true_type {};

template <typename T, T Head, T... Ints>
struct seq_less<std::integer_sequence<T, Head, Ints...>, std::integer_sequence<T>> : std::false_type {};

template <typename T, T Head1, T Head2, T... Ints>
struct seq_less<std::integer_sequence<T, Head1, Ints...>, std::integer_sequence<T, Head2, Ints...>>
    : std::bool_constant<Head1 < Head2> {};

template <typename T, T Head, T... Ints1, T... Ints2>
struct seq_less<std::integer_sequence<T, Head, Ints1...>, std::integer_sequence<T, Head, Ints2...>>
    : seq_less<std::integer_sequence<T, Ints1...>, std::integer_sequence<T, Ints2...>> {};

template <typename T, typename U>
inline constexpr bool seq_less_v = seq_less<T, U>::value;


// 相等
template <typename T, typename U>
struct seq_equal : std::false_type
{
    static_assert(is_seq_v<T> && is_seq_v<U>, "T && U must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, seq_int_t<U>>, "T && U must have same int type");
};

template <typename T, T... Ints>
struct seq_equal<std::integer_sequence<T, Ints...>, std::integer_sequence<T, Ints...>>
    : std::true_type {};

template <typename T, typename U>
inline constexpr bool seq_equal_v = seq_equal<T, U>::value;


// 合并
template <typename T, typename U>
struct seq_merge
{
    static_assert(is_seq_v<T> && is_seq_v<U>, "T && U must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, seq_int_t<U>>, "T && U must have same int type");
};

template <typename T, T... Ints1, T... Ints2>
struct seq_less<std::integer_sequence<T, Ints1...>, std::integer_sequence<T, Ints2...>>
{
    using type = std::integral_constant<T, Ints1..., Ints2...>;
};

template <typename T, typename U>
using seq_merge_t = typename seq_merge<T, U>::type;


// 分割(以给定整数，分割一次)
template <typename T, auto I>
struct seq_split_once
{
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, decltype(I)>, "T and I has to match");
};

template <typename T, T I>
struct seq_split_once<std::integer_sequence<T>, I> // 空序列
{
    using type1 = std::integer_sequence<T>;
    using type2 = std::integer_sequence<T>;
};

template <typename T, T Head, T... Ints>
struct seq_split_once<std::integer_sequence<T, Head, Ints...>, Head> // 首元素就是分割符
{
    using type1 = std::integer_sequence<T>;
    using type2 = std::integer_sequence<T, Ints...>;
};

template <typename T, T Head, T... Ints, T I>
struct seq_split_once<std::integer_sequence<T, Head, Ints...>, I> // 首元素不是分割符
{
    using type1 = seq_merge_t<
        std::integer_sequence<T, Head>,
        typename seq_split_once<std::integer_sequence<T, Ints...>, I>::type1
    >;
    using type2 = typename seq_split_once<std::integer_sequence<T, Ints...>, I>::type2;
};

template <typename T, auto I>
using seq_split_once_t1 = typename seq_split_once<T, I>::type1;

template <typename T, auto I>
using seq_split_once_t2 = typename seq_split_once<T, I>::type2;

// 分割(以给定整数，完全分割序列，返回一个type_list)
template <typename T, auto Delim>
struct seq_split
{
private:
    static_assert(is_seq_v<T>, "T must be std::integer_sequence");
    static_assert(std::is_same_v<seq_int_t<T>, decltype(Delim)>, "T and delim has to match");

    using First = seq_split_once_t1<T, Delim>;
    using Rest = seq_split_once_t2<T, Delim>;
    using RawType = std::conditional_t<is_empty_seq_v<Rest>, // 分割一次，判断第二部分是否为空序列
        typelist<First>,    // 第二部分是空，分割结果就是First
        tl_push_front_t<typename seq_split<Rest, Delim>::type, First>>; // 第二部分不空，结果就是First + 对剩下部分Rest进行分割的结果

public:
    using type = tl_remove_t<RawType, std::integer_sequence<seq_int_t<T>>>; // 去除原始分割结果里面的空序列
};

template <typename Int, Int Delim>
struct seq_split<std::integer_sequence<Int>, Delim>
{
    using type = typelist<>;
};

template <typename T, auto Delim>
using seq_split_t = typename seq_split<T, Delim>::type;

} // namespace Refl::Util

#endif // REFL_SEQUENCE_TYPE_TRAITS_HPP