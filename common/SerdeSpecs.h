//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_SERDESPECS_H
#define PROJ3_SERDESPECS_H

#include <SGCore/Serde/Common.h>
#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>

#include "Models/Worker.h"
#include "Models/Storage.h"
#include "Models/ItemTypeInfo.h"
#include "Models/Item.h"
#include "Models/Provider.h"

#include "Reflection/GetTypeName.h"
#include "Reflection/Reflection.h"

namespace SGCore::Serde
{
    template<typename T, FormatType TFormatType>
    requires(std::is_aggregate_v<T>)
    struct SerdeSpec<T, TFormatType> : Serde::BaseTypes<>, SGCore::Serde::DerivedTypes<>
    {
        static inline const std::string type_name = std::string { getUnMangledTypeName<T>() };
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SGCore::Serde::SerializableValueView<T, TFormatType>& valueView) noexcept
        {
            auto meta = makeMetaInfo(*valueView.m_data);

            meta.iterateThroughMembers([&valueView](auto memberInfo) {
                valueView.getValueContainer().addMember(std::string { memberInfo.unmangled_name }, memberInfo.value);
            });
        }

        static void deserialize(SGCore::Serde::DeserializableValueView<T, TFormatType>& valueView) noexcept
        {
            auto meta = makeMetaInfo(*valueView.m_data);

            meta.iterateThroughMembers([&valueView](auto memberInfo) {
                using member_t = decltype(memberInfo)::member_t;

                auto deserializedValue = valueView.getValueContainer().template getMember<member_t>(std::string { memberInfo.unmangled_name });

                if(deserializedValue)
                {
                    memberInfo.value = *deserializedValue;
                }
            });
        }
    };
}

#endif //PROJ3_SERDESPECS_H
