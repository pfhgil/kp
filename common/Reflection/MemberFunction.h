//
// Created by Ilya on 13.05.2024.
//

#ifndef REFLECTION_MEMBERFUNCTION_H
#define REFLECTION_MEMBERFUNCTION_H

#include <functional>

template <std::size_t Idx, class... Types>
class extract
{
    static_assert(Idx < sizeof...( Types ), "index out of bounds");

    template <std::size_t I, std::size_t N, class... Rest>
    struct extract_impl;

    template <std::size_t I, std::size_t N, class T, class... Rest>
    struct extract_impl<I, N, T, Rest...>
    {
        using type = typename extract_impl<I + 1, N, Rest...>::type;
    };

    template <std::size_t N, class T, class... Rest>
    struct extract_impl<N, N, T, Rest...>
    {
        using type = T;
    };
public:
    using type = typename extract_impl<0, Idx, Types...>::type;
};

template <typename> struct detail_member_function_traits;

template <typename Return, typename Object, typename... Args>
struct detail_member_function_traits<Return(Object::*)(Args...)>
{
    using return_type = Return;
    using instance_type = Object;
    typedef Object& instance_reference;
    using member_function_type = Return(Object::*)(Args...);
    using function_type = Return(Args...);

    static constexpr size_t arguments_count = sizeof...(Args);

    template<size_t Idx>
    using get_type = extract<Idx, Args...>;

    template<size_t Idx>
    using get_type_t = extract<Idx, Args...>::type;
};

template<typename>
struct MemberFunction;

template<typename RetT, typename... Args>
struct MemberFunction<RetT(Args...)>
{
    template<auto Ptr>
    constexpr static auto bind(typename detail_member_function_traits<decltype(Ptr)>::instance_type* obj) noexcept
    {
        using traits = detail_member_function_traits<decltype(Ptr)>;
        using member_func_t = MemberFunction<typename traits::function_type>;

        member_func_t memberFunc;
        memberFunc.native_function = [obj](Args&&... args) {
            (obj->*Ptr)(std::forward<Args>(args)...);
        };

        return memberFunc;
    }

    template<typename... Args0>
    RetT operator()(const Args0&... args) const noexcept
    {
        return native_function(args...);
    }
private:
    std::function<RetT(Args...)> native_function;
};

template<auto Ptr>
constexpr auto bindFunction(typename detail_member_function_traits<decltype(Ptr)>::instance_type* obj) noexcept
{
    using traits = detail_member_function_traits<decltype(Ptr)>;
    using member_func_t = MemberFunction<typename traits::function_type>;

    return member_func_t::template bind<Ptr>(obj);
}

template<auto Ptr>
struct member_func_traits
{
    using traits = detail_member_function_traits<decltype(Ptr)>;
    using func_t = MemberFunction<typename member_func_traits<Ptr>::traits::function_type>;
};

template<auto Ptr>
using member_func_t = member_func_traits<Ptr>::func_t;

#endif //REFLECTION_MEMBERFUNCTION_H
