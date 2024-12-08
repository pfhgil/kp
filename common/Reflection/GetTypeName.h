//
// Created by Ilya on 11.05.2024.
//

#ifndef REFLECTION_GETTYPENAME_H
#define REFLECTION_GETTYPENAME_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string_view>

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

#if defined _MSC_VER
#   define GENERATOR_PRETTY_FUNCTION __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
#   define GENERATOR_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#ifndef __FUNCTION_NAME__
#ifdef WIN32   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__
#else          //*NIX
#define __FUNCTION_NAME__   __func__
#endif
#endif

template <auto Ptr>
[[nodiscard]] consteval auto getMangledTypeName()
{
    return GENERATOR_PRETTY_FUNCTION;
}

template<typename T>
consteval std::string_view getMangledTypeName() noexcept
{
    return GENERATOR_PRETTY_FUNCTION;
}

template<typename T>
consteval std::string_view getUnMangledTypeName() noexcept
{
    std::string_view str = GENERATOR_PRETTY_FUNCTION;

    size_t pos0 = str.find(__FUNCTION_NAME__) + sizeof(__FUNCTION_NAME__);
    size_t pos1 = str.rfind('>');
    str = str.substr(pos0, pos1 - pos0);

    return str;
}

template <class T>
struct MemberPtr final
{
    const T* ptr;
};

template <size_t N, typename T>
constexpr auto getMemberPtr(T&& t) noexcept
{
    using V = std::remove_cvref_t<T>;

    if constexpr(requires { V::info::value; })
    {
        auto& p = t.*(std::get<N>(V::info::value));
        return MemberPtr<std::remove_reference_t<decltype(p)>> { &p };
    }
    else
    {
        auto& p = std::get<N>(toTuple(t));
        return MemberPtr<std::remove_reference_t<decltype(p)>> { &p };
    }
}

template <class T>
extern const T external;

template <class T>
extern T not_const_external;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
   template <auto N, class T>
   constexpr std::string_view getMemberMangledName = getMangledTypeName<getMemberPtr<N>(external<std::remove_volatile_t<T>>)>();
#pragma clang diagnostic pop
#elif __GNUC__
template <auto N, class T>
   constexpr std::string_view getMemberMangledName = getMangledTypeName<getMemberPtr<N>(external<std::remove_volatile_t<T>>)>();
#else
template <auto N, class T>
constexpr std::string_view getMemberMangledName = getMangledTypeName<getMemberPtr<N>(external<std::remove_volatile_t<T>>)>();
#endif

template <auto N, class T>
consteval std::string_view getMemberUnMangledName()
{
    std::string_view mangledName = getMemberMangledName<N, T>;

    size_t pos0 = mangledName.find("->") + 2;
    size_t pos1 = mangledName.rfind('}');

    mangledName = mangledName.substr(pos0, pos1 - pos0);

    return mangledName;
}

#endif //REFLECTION_GETTYPENAME_H
