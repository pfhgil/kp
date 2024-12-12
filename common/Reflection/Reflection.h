//
// Created by Ilya on 12.05.2024.
//

#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

#include <iostream>
#include <vector>
#include <optional>
#include <cassert>
#include <array>

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

template<auto>
struct always_false_obj : std::false_type { };

template<size_t Size, typename CharT>
struct basic_constexpr_string
{
    using char_t = CharT;
    static constexpr inline size_t size_v = Size;

    CharT data[Size] { };

    consteval basic_constexpr_string(const CharT (&str)[Size])
    {
        std::copy_n(str, Size, data);
    }

    consteval bool operator==(const basic_constexpr_string<Size, CharT> str) const
    {
        return std::equal(str.data, str.data + Size, data);
    }

    template<std::size_t N2>
    consteval bool operator==(const basic_constexpr_string<N2, CharT> s) const
    {
        return false;
    }

    template<std::size_t Size2>
    consteval basic_constexpr_string<Size + Size2 - 1, CharT> operator+(const basic_constexpr_string<Size2, CharT> str) const
    {
        CharT newBuf[Size + Size2 - 1] {};
        std::copy_n(data, Size - 1, newBuf);
        std::copy_n(str.data, Size2, newBuf + Size - 1);
        return newBuf;
    }

    consteval char operator[](std::size_t n) const
    {
        return data[n];
    }

    [[nodiscard]] consteval std::size_t size() const
    {
        return Size - 1;
    }
};

template<std::size_t Size1, std::size_t Size2, typename CharT>
consteval auto operator+(basic_constexpr_string<Size1, CharT> fs, const CharT (&str) [Size2])
{
    return fs + basic_constexpr_string<Size2, CharT>(str);
}

template<std::size_t Size1, std::size_t Size2, typename CharT>
consteval auto operator+(const CharT (&str) [Size2], basic_constexpr_string<Size1, CharT> fs)
{
    return basic_constexpr_string<Size2, CharT>(str) + fs;
}

template<std::size_t Size1, std::size_t Size2, typename CharT>
consteval auto operator==(basic_constexpr_string<Size1, CharT> fs, const CharT (&str) [Size2])
{
    return fs == basic_constexpr_string<Size2, CharT>(str);
}

template<std::size_t Size1, std::size_t Size2, typename CharT>
consteval auto operator==(const CharT (&str) [Size2], basic_constexpr_string<Size1, CharT> fs)
{
    return basic_constexpr_string<Size2, CharT>(str) == fs;
}

template<std::size_t Size, typename CharT>
consteval auto operator==(basic_constexpr_string<Size, CharT> fs, const std::basic_string_view<CharT>& sv)
{
    return sv == fs.data;
}

template<std::size_t Size, typename CharT>
consteval auto operator==(const std::basic_string_view<CharT>& sv, basic_constexpr_string<Size, CharT> fs)
{
    return sv == fs.data;
}

template<size_t Size>
using constexpr_string = basic_constexpr_string<Size, char>;

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

    template<basic_constexpr_string Name>
    constexpr auto& getByName() const noexcept
    {
        return getByNameImpl<Name, 0>();
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
    template<basic_constexpr_string Name, size_t MemberIdx>
    constexpr auto& getByNameImpl() const noexcept
    {
        if constexpr(MemberIdx == members_count)
        {
            static_assert(always_false_obj<MemberIdx>::value, "Can not find member with this name!");
        }
        else
        {
            if constexpr(Name == std::remove_reference_t<decltype(get<MemberIdx>())>::unmangled_name)
            {
                return get<MemberIdx>();
            }
            else
            {
                return getByNameImpl<Name, MemberIdx + 1>();
            }
        }
    }

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

struct enum_reflect
{
    template<size_t Index, auto EnumV>
    struct EnumValueInfo
    {
        using enum_t = decltype(EnumV);

        static constexpr enum_t value = EnumV;
        static constexpr size_t index = Index;
        static constexpr std::string_view unmangled_name = getUnMangledValueName<EnumV>();
        static constexpr std::string_view mangled_name = getMangledTypeName<EnumV>();
    };

    template<typename EnumT>
    static consteval auto getEnumMembers()
    {
        return getEnumMembersImpl<EnumT>({});
    }

    template<typename EnumT>
    static consteval auto getEnumMembersCount()
    {
        return getEnumMembers<EnumT>().size();
    }

    template<typename EnumT>
    static consteval auto getEnumMembersInfo()
    {
        constexpr auto members = getEnumMembers<EnumT>();

        if constexpr(members.size() == 0)
        {
            return std::tuple<>();
        }
        else
        {
            auto tup = std::make_tuple(EnumValueInfo<members.size() - 1, members[members.size() - 1]>());

            return getEnumMembersInfoImpl<members.size() - 2, members>(tup);
        }
    }

    template<typename EnumT, typename Func>
    static constexpr void iterateThroughEnumMembers(const Func& func)
    {
        constexpr auto membersInfo = getEnumMembersInfo<EnumT>();
        if constexpr(std::tuple_size_v<decltype(membersInfo)> > 0)
        {
            iterateThroughEnumMembersImpl<0>(func, membersInfo);
        }
    }

    template<typename EnumT>
    static std::array<std::string, getEnumMembersCount<EnumT>()> generateRuntimeNamesMarkup() noexcept
    {
        static auto markup = []() {
            std::array<std::string, getEnumMembersCount<EnumT>()> markup;

            size_t idx = 0;
            iterateThroughEnumMembers<EnumT>([&markup, &idx](auto memberInfo) {
                markup[idx] = memberInfo.unmangled_name;
                ++idx;
            });

            return markup;
        }();

        return markup;
    }

private:
    template<size_t MemberIdx,  typename Func>
    static constexpr void iterateThroughEnumMembersImpl(const Func& func, auto membersInfo)
    {
        if constexpr(MemberIdx == std::tuple_size_v<decltype(membersInfo)>)
        {

        }
        else
        {
            func(std::get<MemberIdx>(membersInfo));
            iterateThroughEnumMembersImpl<MemberIdx + 1>(func, membersInfo);
        }
    }

    template<size_t MemberIdx, auto MembersArr>
    static consteval auto getEnumMembersInfoImpl(auto membersTuple)
    {
        if constexpr(MemberIdx == -1)
        {
            return membersTuple;
        }
        else
        {
            auto newTuple = std::tuple_cat(std::make_tuple(EnumValueInfo<MemberIdx, MembersArr[MemberIdx]>()), membersTuple);
            return getEnumMembersInfoImpl<MemberIdx - 1, MembersArr>(newTuple);
        }
    }

    template<typename EnumT, std::int64_t ValidCount = 0, std::underlying_type_t<EnumT> EnumIdx = -127>
    static consteval auto getEnumMembersImpl(const std::array<EnumT, ValidCount>& lastArr)
    {
        using underlying_t = std::underlying_type_t<EnumT>;

        if constexpr (EnumIdx == 127)
        {
            return lastArr;
        }
        else
        {
            if constexpr (getUnMangledValueName<(EnumT) EnumIdx>().contains("("))
            {
                return getEnumMembersImpl<EnumT, ValidCount, EnumIdx + 1>(lastArr);
            }
            else
            {
                std::array<EnumT, ValidCount + 1> newArr = concatArrays(lastArr, std::array<EnumT, 1> {});

                newArr[ValidCount] = (EnumT) EnumIdx;

                return getEnumMembersImpl<EnumT, ValidCount + 1, EnumIdx + 1>(newArr);
            }
        }
    }

    template <typename T, std::size_t N1, std::size_t N2>
    static constexpr std::array<T, N1 + N2> concatArrays(std::array<T, N1> lhs, std::array<T, N2> rhs)
    {
        std::array<T, N1 + N2> result { };
        std::size_t index = 0;

        for (auto& el : lhs)
        {
            result[index] = std::move(el);
            ++index;
        }

        for (auto& el : rhs)
        {
            result[index] = std::move(el);
            ++index;
        }

        return result;
    }
};

#endif //REFLECTION_REFLECTION_H
