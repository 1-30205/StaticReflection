#ifndef REFL_TYPE_LIST_H
#define REFL_TYPE_LIST_H

#include <cstddef>
#include <type_traits>
#include <utility>

namespace Refl
{
    namespace Util
    {

    template <typename... Ts>
    struct typelist {};

    // 判断是否为type_lit ------------------------------------------------------------------------------------------------
    template <typename TL>
    struct is_tl : std::false_type {};

    template <typename... Ts>
    struct is_tl<typelist<Ts...>> : std::true_type {};

    template <typename TL>
    inline constexpr bool is_tl_v = is_tl<TL>::value;

    // 长度 -------------------------------------------------------------------------------------------------------------
    template <typename TL>
    struct tl_size
    {
        static_assert(is_tl_v<TL>, "T must be typelist");
    };

    template <typename... Ts>
    struct tl_size<typelist<Ts...>>
        : std::integral_constant<std::size_t, sizeof...(Ts)> {};

    template <typename TL>
    inline constexpr std::size_t tl_size_v = tl_size<TL>::value;

    // 判空 -------------------------------------------------------------------------------------------------------------
    template <typename TL>
    struct is_tl_empty
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
    };

    template <typename... Ts>
    struct is_tl_empty<typelist<Ts...>>
        : std::bool_constant<sizeof...(Ts) != 0> {};

    template <typename TL>
    inline constexpr std::size_t is_tl_empty_v = is_tl_empty<TL>::value;

    // 头插 -------------------------------------------------------------------------------------------------------------
    template <typename TL, typename T>
    struct tl_push_front
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
    };

    template <typename... Ts, typename T>
    struct tl_push_front<typelist<Ts...>, T>
    {
        using type = typelist<T, Ts...>;
    };

    template <typename TL, typename T>
    using tl_push_front_t = typename tl_push_front<TL, T>::type;

    // 尾插 -------------------------------------------------------------------------------------------------------------
    template <typename TL, typename T>
    struct tl_push_back
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
    };

    template <typename... Ts, typename T>
    struct tl_push_back<typelist<Ts...>, T>
    {
        using type = typelist<Ts..., T>;
    };

    template <typename TL, typename T>
    using tl_push_back_t = typename tl_push_back<TL, T>::type;


    // 按下标获取元素 -----------------------------------------------------------------------------------------------------
    template <typename TL, std::size_t I>
    struct tl_get
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
        static_assert(I < tl_size_v<TL>, "index out of range");
    };

    template <typename T, typename... Ts>
    struct tl_get<typelist<T, Ts...>, 0>
    {
        using type = T;
    };

    template <typename T, typename... Ts, std::size_t I>
    struct tl_get<typelist<T, Ts...>, I>: tl_get<typelist<Ts...>, I - 1> {};

    template <typename TL, std::size_t I>
    using tl_get_t = typename tl_get<TL, I>::type;

    // 过滤(根据std::index_sequence 过滤旧typelist形成新的typelist)
    template <typename TL, typename IndexSeq>
    struct tl_filter
    {
        static_assert(is_tl_v<TL>, "TL must be typelist, IndexSeq must be std::index_sequence");
    };

    template <typename TL, std::size_t... Is>
    struct tl_filter<TL, std::index_sequence<Is...>>
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
        using type = typelist<tl_get_t<TL, Is>...>;
    };

    template <typename TL, typename IndexSeq>
    using tl_filter_t = typename tl_filter<TL, IndexSeq>::type;


    // 丢弃头元素 --------------------------------------------------------------------------------------------------------
    template <typename TL>
    struct tl_pop_front
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
        static_assert(tl_size_v<TL> > 0, "TL is empty, nothing to pop");
    };

    template <typename T, typename... Ts>
    struct tl_pop_front<typelist<T, Ts...>>
    {
        using type = typelist<Ts...>;
    };

    template <typename TL>
    using tl_pop_front_t = typename tl_pop_front<TL>::type;


    // 丢弃尾元素 --------------------------------------------------------------------------------------------------------
    template <typename TL>
    struct tl_pop_back
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
        static_assert(tl_size_v<TL> > 0, "TL is empty, nothing to pop");
        using type = tl_filter_t<TL, std::make_index_sequence<tl_size_v<TL> - 1>>;
    };

    template <typename TL>
    using tl_pop_back_t = typename tl_pop_back<TL>::type;

    // 首元素 -----------------------------------------------------------------------------------------------------------
    template <typename TL>
    using tl_front_t = tl_get_t<TL, 0>;


    // 尾元素 -----------------------------------------------------------------------------------------------------------
    template <typename TL>
    using tl_back_t = tl_get_t<TL, tl_size_v<TL> - 1>;

    // 移除
    template <typename TL, typename T>
    struct tl_remove
    {
        static_assert(is_tl_v<TL>, "TL must be typelist");
    };

    template <typename T>
    struct tl_remove<typelist<>, T>
    {
        using type = typelist<>;
    };

    template <typename Head, typename... Ts>
    struct tl_remove<typelist<Head, Ts...>, Head>
    {
        using type = typename tl_remove<typelist<Ts...>, Head>::type;
    };

    template <typename Head, typename... Ts, typename T>
    struct tl_remove<typelist<Head, Ts...>, T>
    {
        using type = tl_push_front_t<typename tl_remove<typelist<Ts...>, T>::type, Head>;
    };

    template <typename TL, typename T>
    using tl_remove_t = typename tl_remove<TL, T>::type;

    } // namespace Util
} // namespace Refl

#endif // REFL_TYPE_LIST_H