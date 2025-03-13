#pragma once

#include <concepts>

namespace Coplt
{
    template <class F, class R, class... Args>
    concept Fn = std::invocable<F, Args...> && requires(F&& f, Args&&... args)
    {
        { f(std::forward<Args>(args)...) } -> std::convertible_to<R>;
    };

    template <class F, class T>
    concept Hash = Fn<F, size_t, const T&> || Fn<F, size_t, T>;

    template <class F, class A, class B = A>
    concept Eq = Fn<F, bool, const A&, const B&>;

    template <class T>
    concept HasHasher = requires
    {
        typename T::Hasher;
    } && Hash<typename T::Hasher, T>;

    template <class T, class U = T>
    concept HasEq = requires
    {
        typename T::Eq;
    } && Eq<typename T::Eq, T, U>;

    template <class T>
    concept HasGetHashCode = requires(const T& t)
    {
        { t.GetHashCode() } -> std::convertible_to<size_t>;
    };

    template <HasGetHashCode T>
    auto GetHashCode(const T& value)
    {
        return value.GetHashCode();
    }

    template <class T>
    concept ExistsGetHashCode = requires(const T& t)
    {
        { GetHashCode(t) } -> std::convertible_to<size_t>;
    };

    template <class T, class U = T>
    concept HasEquals = requires(const T& t, const U& u)
    {
        { t == u } -> std::convertible_to<bool>;
    };

    template <ExistsGetHashCode T>
    struct GetHashCodeHasher
    {
        size_t operator()(const T& value) const
        {
            return GetHashCode(value);
        }
    };

    template <class T, class U = T> requires HasEquals<T, U>
    struct EqualsEq
    {
        bool operator()(const T& lhs, const U& rhs) const
        {
            return lhs == rhs;
        }
    };

    template <class T>
    struct TAsIs
    {
        using Type = T;
    };

    template <class T>
    struct TGetHasher
    {
        using Type = typename T::Hasher;
    };

    template <class T>
    struct TGetGetHashCodeHasher
    {
        using Type = GetHashCodeHasher<T>;
    };

    template <class T>
    struct TGetEq
    {
        using Type = typename T::Eq;
    };

    template <class T, class U = T>
    struct TGetEqualsEq
    {
        using Type = EqualsEq<T, U>;
    };

    template <class T>
    using SelectHasher = typename std::conditional_t<
        HasHasher<T>,
        TGetHasher<T>,
        std::conditional_t<ExistsGetHashCode<T>, TGetGetHashCodeHasher<T>, TAsIs<std::hash<T>>>
    >::Type;

    template <class T>
    using SelectEq = typename std::conditional_t<
        HasEq<T>, TGetEq<T>,
        std::conditional_t<HasEquals<T>, TGetEqualsEq<T>, TAsIs<std::equal_to<T>>>
    >::Type;
}
