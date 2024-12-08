//
// Created by Ilya on 12.05.2024.
//

#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

#include <iostream>
#include <vector>
#include <optional>
#include <cassert>

/*
MIT License

Copyright (c) 2023 Pixelfield

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// некоторые фокусы

template<class T, template <class...> class Template>
struct is_specialization : std::false_type {};

template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};

struct Any final
{
#if defined(__clang__)
    #pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
         template <class T>
         requires(!std::same_as<T, const char*> && !std::same_as<T, std::nullptr_t>)
         [[maybe_unused]] constexpr operator T();
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
    template <class T>
    requires(!std::same_as<T, const char*> && !std::same_as<T, std::nullptr_t>)
    [[maybe_unused]] constexpr operator T();

    template <class T>
    requires(is_specialization<T, std::optional>::value)
    [[maybe_unused]] constexpr operator T()
    {
        return std::nullopt;
    }
#else
    #pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
         template <class T>
         requires(!std::same_as<T, const char*> && !std::same_as<T, std::nullptr_t>)
         [[maybe_unused]] constexpr operator T();
#pragma GCC diagnostic pop
#endif

    [[maybe_unused]] constexpr operator std::string_view() { return {}; }
};

static Any global_any;

template <class T, class... Args>
requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
inline constexpr auto count_members = []
{
    using V = std::remove_cvref_t<T>;

    if constexpr(!requires {
        V {
                { Args { } }...,
                { Any  { } }
        };
    })
    {
        return sizeof...(Args);
    }
    else
    {
        return count_members<V, Args..., Any>;
    }
}();

template<class ClsT, size_t MembersCnt = count_members<ClsT>>
requires(MembersCnt <= 20)
constexpr decltype(auto) toTuple(ClsT&& obj) noexcept
{
    if constexpr(MembersCnt == 0)
    {
        return std::tuple {};
    }
    else if constexpr (MembersCnt == 1)
    {
        auto& [p0] = obj;
        return std::tie(p0);
    }
    else if constexpr (MembersCnt == 2)
    {
        auto& [p0, p1] = obj;
        return std::tie(p0, p1);
    }
    else if constexpr (MembersCnt == 3)
    {
        auto& [p0, p1, p2] = obj;
        return std::tie(p0, p1, p2);
    }
    else if constexpr (MembersCnt == 4)
    {
        auto& [p0, p1, p2, p3] = obj;
        return std::tie(p0, p1, p2, p3);
    }
    else if constexpr (MembersCnt == 5)
    {
        auto& [p0, p1, p2, p3, p4] = obj;
        return std::tie(p0, p1, p2, p3, p4);
    }
    else if constexpr (MembersCnt == 6)
    {
        auto& [p0, p1, p2, p3, p4, p5] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5);
    }
    else if constexpr (MembersCnt == 7)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6);
    }
    else if constexpr (MembersCnt == 8)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7);
    }
    else if constexpr (MembersCnt == 9)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    }
    else if constexpr (MembersCnt == 10)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    else if constexpr (MembersCnt == 11)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    else if constexpr (MembersCnt == 12)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    else if constexpr (MembersCnt == 13)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    else if constexpr (MembersCnt == 14)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    else if constexpr (MembersCnt == 15)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
    else if constexpr (MembersCnt == 16)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    else if constexpr (MembersCnt == 17)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
    }
    else if constexpr (MembersCnt == 18)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17);
    }
    else if constexpr (MembersCnt == 19)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18);
    }
    else if constexpr (MembersCnt == 20)
    {
        auto& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19] = obj;
        return std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19);
    }
}

#include "GetTypeName.h"

template<class ObjT>
struct RuntimeMemberInfo
{
    template<class ClassT, class MemberT, size_t Idx>
    friend struct MemberInfo;

    using class_t = ObjT;

    RuntimeMemberInfo() = default;

    const size_t index = 0;

    const bool is_const = false;
    const bool is_volatile = false;
    const bool is_pointer = false;
    const bool is_reference = false;

    const std::string_view mangled_type_name = std::string_view("not_provided");
    const std::string_view unmangled_type_name = std::string_view("not_provided");
    const std::string_view mangled_name = std::string_view("not_provided");
    const std::string_view unmangled_name = std::string_view("not_provided");

    template<typename T>
    void setValue(const T& val) noexcept
    {
        if(!is_const)
        {
            *((T*) value) = val;
        }
        else
        {
            // MAYBE ASSERT?? OR NO
        }
    }

    template<typename T>
    const T* getValue() const noexcept
    {
        return (T*) value;
    }

    bool operator==(const RuntimeMemberInfo& other) noexcept
    {
        return index == other.index;
    }

    bool operator!=(const RuntimeMemberInfo& other) noexcept
    {
        return !(*this == other);
    }

private:
    void* value = nullptr;

    RuntimeMemberInfo(size_t idx, bool isConst, bool isVolatile, bool isPtr, bool isRef,
                      std::string_view mangledTypeName,
                      std::string_view unmangledTypeName,
                      std::string_view mangledName,
                      std::string_view unmangledName,
                      void* val) noexcept :
            index(idx), is_const(isConst), is_volatile(isVolatile), is_pointer(isPtr), is_reference(isRef),
            mangled_type_name(mangledTypeName),
            unmangled_type_name(unmangledTypeName),
            mangled_name(mangledName),
            unmangled_name(unmangledName),
            value(val) { }
};

static const auto null_member = RuntimeMemberInfo<void>();

template<class ClassT, class MemberT, size_t Idx>
struct MemberInfo
{
    using member_t = MemberT;
    using class_t = ClassT;

    static constexpr size_t index = Idx;

    static constexpr bool is_const = std::is_const_v<member_t>;
    static constexpr bool is_volatile = std::is_volatile_v<member_t>;
    static constexpr bool is_pointer = std::is_pointer_v<member_t>;
    static constexpr bool is_reference = std::is_reference_v<member_t>;

    static constexpr auto mangled_type_name = getMangledTypeName<member_t>();
    static constexpr auto unmangled_type_name = getUnMangledTypeName<member_t>();
    static constexpr auto mangled_name = getMemberMangledName<Idx, std::remove_pointer_t<std::remove_reference_t<class_t>>>;
    static constexpr auto unmangled_name = getMemberUnMangledName<Idx, std::remove_pointer_t<std::remove_reference_t<class_t>>>();

    MemberT& value = (MemberT&) global_any;

    constexpr RuntimeMemberInfo<class_t> asRuntime() const noexcept
    {
        return RuntimeMemberInfo<class_t>(
                index,
                is_const,
                is_volatile,
                is_pointer,
                is_reference,

                mangled_type_name,
                unmangled_type_name,
                mangled_name,
                unmangled_name,

                (void*) &value
        );
    }
};

template<size_t Idx, typename ClsT>
constexpr auto getMemberInfo(ClsT& obj) noexcept
{
    using V = std::remove_pointer_t<std::remove_reference_t<ClsT>>;

    constexpr bool classIsPointer = std::is_pointer_v<std::remove_reference_t<ClsT>>;

    constexpr bool providesCustomInfo = requires { V::info::value; };

    if constexpr(providesCustomInfo)
    {
        if constexpr(classIsPointer)
        {
            auto& member = obj->*(std::get<Idx>(V::info::value));
            MemberInfo<ClsT, std::remove_reference_t<decltype(member)>, Idx> memberInfo { member };

            return memberInfo;
        }
        else
        {
            auto& member = obj.*(std::get<Idx>(V::info::value));
            MemberInfo<ClsT, std::remove_reference_t<decltype(member)>, Idx> memberInfo { member };

            return memberInfo;
        }
    }
    else
    {
        if constexpr(classIsPointer)
        {
            auto objAsTuple = toTuple(*obj);
            auto& member = std::get<Idx>(objAsTuple);
            MemberInfo<ClsT, std::remove_reference_t<decltype(member)>, Idx> memberInfo {member};

            return memberInfo;
        }
        else
        {
            auto objAsTuple = toTuple(obj);
            auto& member = std::get<Idx>(objAsTuple);
            MemberInfo<ClsT, std::remove_reference_t<decltype(member)>, Idx> memberInfo {member};

            return memberInfo;
        }
    }
}

// ==================================================================
// ==================================================================
// ==================================================================

template<class ClsT, bool>
struct RawMembersTupleTImpl;

template<class ClsT>
struct RawMembersTupleTImpl<ClsT, true>
{
    using tuple_t = std::remove_const_t<decltype(std::remove_pointer_t<std::remove_reference_t<ClsT>>::info::value)>;
};

template<class ClsT>
struct RawMembersTupleTImpl<ClsT, false>
{
    using tuple_t = std::remove_const_t<decltype(toTuple(not_const_external<std::remove_pointer_t<std::remove_reference_t<ClsT>>>))>;
};

template<class ClsT>
using raw_members_tuple_type = RawMembersTupleTImpl<ClsT, requires { std::remove_pointer_t<std::remove_reference_t<ClsT>>::info::value; }>;

template<class ClsT, size_t CurIdx, size_t MaxIdx, class... Infos>
struct MembersTupleTImpl;

template<class ClsT, size_t MaxIdx, class... Infos>
struct MembersTupleTImpl<ClsT, MaxIdx, MaxIdx, Infos...>
{
    using tuple_t = decltype(std::tuple<
            decltype(not_const_external<Infos>)
            ...
    >());
};

template<class ClsT, size_t CurIdx, size_t MaxIdx, class... Infos>
struct MembersTupleTImpl
{
    using tuple_t = MembersTupleTImpl<ClsT, CurIdx + 1, std::tuple_size_v<typename raw_members_tuple_type<ClsT>::tuple_t>,
            Infos...,
            decltype(getMemberInfo<CurIdx, ClsT>(not_const_external<ClsT>))>::tuple_t;
};

template<class ClsT>
using members_tuple = MembersTupleTImpl<ClsT, 0, std::tuple_size_v<typename raw_members_tuple_type<ClsT>::tuple_t>>;

// ==================================================================
// ==================================================================
// ==================================================================

template<class ClsT, bool>
struct MembersInfoTupleTImpl;

template<class ClsT>
struct MembersInfoTupleTImpl<ClsT, true>
{
    using tuple_t = members_tuple<ClsT>::tuple_t;
};

template<class ClsT>
struct MembersInfoTupleTImpl<ClsT, false>
{
    using tuple_t = members_tuple<ClsT>::tuple_t;
};

template<class ClsT>
using members_info_tuple_type = MembersInfoTupleTImpl<ClsT, requires { ClsT::info::value; }>;

// ==================================================================
// ==================================================================
// ==================================================================

template<class>
struct MetaInfo;

template<class ClsT, size_t... Indices>
constexpr auto makeMetaInfoImpl(ClsT& obj, std::index_sequence<Indices...>) noexcept
{
    using V = std::remove_pointer_t<std::remove_reference_t<ClsT>>;

    constexpr bool classIsPointer = std::is_pointer_v<std::remove_reference_t<ClsT>>;

    constexpr bool providesCustomInfo = requires { V::info::value; };

    if constexpr (providesCustomInfo)
    {
        if constexpr (classIsPointer)
        {
            return MetaInfo<ClsT> {
                    .members {
                            { obj->*(std::get<Indices>(V::info::value)) }...
                    }
            };
        }
        else
        {
            return MetaInfo<ClsT> {
                    .members {
                            { obj.*(std::get<Indices>(V::info::value)) }...
                    }
            };
        }
    }
    else
    {
        if constexpr (classIsPointer)
        {
            return MetaInfo<ClsT> {
                    .members {
                            { std::get<Indices>(toTuple(*obj)) }...
                    }
            };
        }
        else
        {
            return MetaInfo<ClsT> {
                    .members {
                            { std::get<Indices>(toTuple(obj)) }...
                    }
            };
        }
    }
}

template<class ClsT>
constexpr auto makeMetaInfo(ClsT& obj) noexcept
{
    return makeMetaInfoImpl(obj, std::make_index_sequence<std::tuple_size_v<typename raw_members_tuple_type<ClsT>::tuple_t>>());
}

template<class ClsT>
struct RuntimeMetaInfo
{
    using class_t = ClsT;
    using meta_info_t = MetaInfo<ClsT>;

    std::vector<RuntimeMemberInfo<ClsT>> members;

    RuntimeMemberInfo<ClsT>* findMember(std::string_view name) noexcept
    {
        for(auto& member : members)
        {
            if(member.unmangled_name == name)
            {
                return &member;
            }
        }

        return nullptr;
    }
};

template<class ClsT>
struct MetaInfo
{
    using class_t = ClsT;
    using as_tuple_t = raw_members_tuple_type<ClsT>::tuple_t;
    using as_members_tuple_t = members_info_tuple_type<ClsT>::tuple_t;

    static constexpr size_t members_count = std::tuple_size_v<as_members_tuple_t>;

    static constexpr bool is_provides_custom_info = requires { ClsT::info::value; };

    as_members_tuple_t members;

    template<size_t Idx>
    constexpr auto& get() const noexcept
    {
        return std::get<Idx>(members);
    }

    template<typename Func>
    constexpr void iterateThroughMembers(const Func& func) const
    {
        iterateThroughMembersImpl<Func, 0>(func);
    }

    auto asRuntime() const noexcept
    {
        RuntimeMetaInfo<class_t> runtimeMetaInfo;

        iterateThroughMembers([&runtimeMetaInfo](auto& metaInfo) {
            runtimeMetaInfo.members.emplace_back(metaInfo.asRuntime());
        });

        return runtimeMetaInfo;
    }

private:
    template<typename Func, size_t CurIdx>
    void iterateThroughMembersImpl(const Func& func) const
    {
        if constexpr(CurIdx == members_count)
        {

        }
        else
        {
            func(std::get<CurIdx>(members));
            iterateThroughMembersImpl<Func, CurIdx + 1>(func);
        }
    }
};

#endif //REFLECTION_REFLECTION_H
