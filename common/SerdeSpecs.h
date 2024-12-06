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

namespace SGCore::Serde
{
    template<FormatType TFormatType>
    struct SerdeSpec<Worker, TFormatType> : Serde::BaseTypes<>, SGCore::Serde::DerivedTypes<>
    {
        static inline const std::string type_name = "Worker";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SGCore::Serde::SerializableValueView<Worker, TFormatType>& valueView) noexcept
        {
            valueView.getValueContainer().addMember("m_id", valueView.m_data->m_id);
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_surname", valueView.m_data->m_surname);
            valueView.getValueContainer().addMember("m_patronymic", valueView.m_data->m_patronymic);
            valueView.getValueContainer().addMember("m_role", valueView.m_data->m_role);
            valueView.getValueContainer().addMember("m_storageID", valueView.m_data->m_storageID);
            valueView.getValueContainer().addMember("m_login", valueView.m_data->m_login);
            valueView.getValueContainer().addMember("m_password", valueView.m_data->m_password);
        }

        static void deserialize(SGCore::Serde::DeserializableValueView<Worker, TFormatType>& valueView) noexcept
        {
            auto m_id = valueView.getValueContainer().template getMember<std::int64_t>("m_id");
            if(m_id)
            {
                valueView.m_data->m_id = *m_id;
            }

            auto m_name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(m_name)
            {
                valueView.m_data->m_name = std::move(*m_name);
            }

            auto m_surname = valueView.getValueContainer().template getMember<std::string>("m_surname");
            if(m_surname)
            {
                valueView.m_data->m_surname = std::move(*m_surname);
            }

            auto m_patronymic = valueView.getValueContainer().template getMember<std::string>("m_patronymic");
            if(m_patronymic)
            {
                valueView.m_data->m_patronymic = std::move(*m_patronymic);
            }

            auto m_role = valueView.getValueContainer().template getMember<WorkerRole>("m_role");
            if(m_role)
            {
                valueView.m_data->m_role = *m_role;
            }

            auto m_storageID = valueView.getValueContainer().template getMember<std::int64_t>("m_storageID");
            if(m_storageID)
            {
                valueView.m_data->m_storageID = *m_storageID;
            }

            auto m_login = valueView.getValueContainer().template getMember<std::string>("m_login");
            if(m_login)
            {
                valueView.m_data->m_login = *m_login;
            }

            auto m_password = valueView.getValueContainer().template getMember<std::string>("m_password");
            if(m_password)
            {
                valueView.m_data->m_password = *m_password;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Storage, TFormatType> : Serde::BaseTypes<>, SGCore::Serde::DerivedTypes<>
    {
        static inline const std::string type_name = "Storage";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SGCore::Serde::SerializableValueView<Storage, TFormatType>& valueView) noexcept
        {
            valueView.getValueContainer().addMember("m_id", valueView.m_data->m_id);
            valueView.getValueContainer().addMember("m_address", valueView.m_data->m_address);
        }

        static void deserialize(SGCore::Serde::DeserializableValueView<Storage, TFormatType>& valueView) noexcept
        {
            auto m_id = valueView.getValueContainer().template getMember<std::int64_t>("m_id");
            if(m_id)
            {
                valueView.m_data->m_id = *m_id;
            }

            auto m_address = valueView.getValueContainer().template getMember<std::string>("m_address");
            if(m_address)
            {
                valueView.m_data->m_address = std::move(*m_address);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ItemTypeInfo, TFormatType> : Serde::BaseTypes<>, SGCore::Serde::DerivedTypes<>
    {
        static inline const std::string type_name = "ItemTypeInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SGCore::Serde::SerializableValueView<ItemTypeInfo, TFormatType>& valueView) noexcept
        {
            valueView.getValueContainer().addMember("m_id", valueView.m_data->m_id);
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_count", valueView.m_data->m_count);
            valueView.getValueContainer().addMember("m_dateOfReceipt", valueView.m_data->m_dateOfReceipt);
            valueView.getValueContainer().addMember("m_expirationDate", valueView.m_data->m_expirationDate);
            valueView.getValueContainer().addMember("m_productionDate", valueView.m_data->m_productionDate);
        }

        static void deserialize(SGCore::Serde::DeserializableValueView<ItemTypeInfo, TFormatType>& valueView) noexcept
        {
            auto m_id = valueView.getValueContainer().template getMember<std::int64_t>("m_id");
            if(m_id)
            {
                valueView.m_data->m_id = *m_id;
            }

            auto m_name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(m_name)
            {
                valueView.m_data->m_name = std::move(*m_name);
            }

            auto m_count = valueView.getValueContainer().template getMember<std::int64_t>("m_count");
            if(m_count)
            {
                valueView.m_data->m_count = *m_count;
            }

            auto m_dateOfReceipt = valueView.getValueContainer().template getMember<std::string>("m_dateOfReceipt");
            if(m_dateOfReceipt)
            {
                valueView.m_data->m_dateOfReceipt = std::move(*m_dateOfReceipt);
            }

            auto m_expirationDate = valueView.getValueContainer().template getMember<std::string>("m_expirationDate");
            if(m_expirationDate)
            {
                valueView.m_data->m_expirationDate = std::move(*m_expirationDate);
            }

            auto m_productionDate = valueView.getValueContainer().template getMember<std::string>("m_productionDate");
            if(m_productionDate)
            {
                valueView.m_data->m_productionDate = std::move(*m_productionDate);
            }
        }
    };
}

#endif //PROJ3_SERDESPECS_H
