//
// Created by Ilya on 08.12.2024.
//

#ifndef PROJ3_CONTROLLERSCOMMON_H
#define PROJ3_CONTROLLERSCOMMON_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"

namespace api
{
    struct Common
    {
        template<typename T, basic_constexpr_string... FieldsNames>
        static void addRecord(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback,
                              const std::string& sql) noexcept
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            std::string deserLog;
            T value;
            SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

            auto meta = makeMetaInfo(value);

            dbClient->execSqlAsync(sql,
                                   [callback](const drogon::orm::Result& result) {
                                       if (!result.empty())
                                       {
                                           const auto req = fmt::format("Inserting record ID: {}. In table: '{}'.",
                                                                        result[0]["id"].as<int>(),
                                                                        T::s_parentTableName);

                                           std::cout << req << std::endl;
                                           Utils::sendResponse(req, drogon::HttpStatusCode::k200OK, callback);
                                       }

                                       Utils::sendResponse(
                                               "Error inserting record in table '" + T::s_parentTableName + "'!",
                                               drogon::HttpStatusCode::k400BadRequest, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       const auto req = fmt::format("Error inserting record in table '{}': {}",
                                                                    T::s_parentTableName,
                                                                    e.base().what());

                                       std::cerr << req << std::endl;
                                       Utils::sendResponse(req, drogon::HttpStatusCode::k400BadRequest, callback);
                                   },
                                   meta.template getByName<FieldsNames>().value...);
        }

        template<typename T, basic_constexpr_string... FieldsNames>
        static void updateRecordByID(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback,
                                    const std::string& sql, const std::int32_t& id) noexcept
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            std::string deserLog;
            T value;
            SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

            auto meta = makeMetaInfo(value);

            dbClient->execSqlAsync(sql,
                                   [callback, id](const drogon::orm::Result& result) {
                                       const auto req = fmt::format("Updated record with ID: {}. In table: '{}'.",
                                                                    id, T::s_parentTableName);

                                       std::cout << req << std::endl;
                                       Utils::sendResponse(req, drogon::HttpStatusCode::k200OK, callback);
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       const auto req = fmt::format("Error updating record in table '{}': {}",
                                                                    T::s_parentTableName,
                                                                    e.base().what());

                                       std::cerr << req << std::endl;
                                       Utils::sendResponse(req, drogon::HttpStatusCode::k400BadRequest, callback);
                                   },
                                   meta.template getByName<FieldsNames>().value...,
                                   id);
        }

        template<typename T>
        static void deleteRecordByID(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, const std::int32_t& id) noexcept
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("DELETE FROM " + T::s_parentTableName + " WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                        const auto req = fmt::format("Deleted record from '{}' with ID: {}",
                                                                     T::s_parentTableName, id);

                                       std::cout << req << std::endl;
                                       Utils::sendResponse(req, drogon::HttpStatusCode::k200OK, callback);
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       const auto req = fmt::format("Error deleting record from table '{}': {}",
                                                                    T::s_parentTableName, e.base().what());

                                       std::cerr << req << std::endl;
                                       Utils::sendResponse(req,
                                                           drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   id);
        }

        template<typename T>
        static void getRecordBy(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback,
                                const std::string& byFieldName, const auto& byFieldValue) noexcept
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM " + T::s_parentTableName + " WHERE " + byFieldName + " = $1",
                                   [callback, byFieldName, byFieldValue](const drogon::orm::Result& result) {
                                       if(result.empty())
                                       {
                                           Utils::sendResponse(SGCore::Serde::Serializer::toFormat(T { .id = -1 }),
                                                               drogon::HttpStatusCode::k200OK, callback, "value");
                                           return;
                                       }

                                       T value;

                                       auto meta = makeMetaInfo(value);

                                       meta.iterateThroughMembers([&result](auto member) {
                                           using member_t = decltype(member)::member_t;

                                           if constexpr(std::is_enum_v<member_t>)
                                           {
                                               member.value = static_cast<member_t>(result[0][std::string { member.unmangled_name }].template as<std::int32_t>());
                                           }
                                           else
                                           {
                                               member.value = result[0][std::string { member.unmangled_name }].template as<member_t>();
                                           }
                                       });

                                       const std::string req = fmt::format(
                                               "Selected record by '{}' equals to '{}'. From table: '{}'.",
                                               byFieldName, byFieldValue, T::s_parentTableName);

                                       std::cout << req << std::endl;

                                       Utils::sendResponse(SGCore::Serde::Serializer::toFormat(value),
                                                           drogon::HttpStatusCode::k200OK, callback, "value");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << fmt::format("Error selecting record from table '{}': {}",
                                                                T::s_parentTableName, e.base().what()) << std::endl;

                                       Utils::sendResponse("{ }",
                                                           drogon::HttpStatusCode::k400BadRequest, callback);
                                   },
                                   byFieldValue);
        }

        template<typename T>
        static void getAllRecords(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback) noexcept
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM " + T::s_parentTableName,
                                   [callback](const drogon::orm::Result& result) {
                                       if(result.empty())
                                       {
                                           Utils::sendResponse(SGCore::Serde::Serializer::toFormat(T { .id = -1 }),
                                                               drogon::HttpStatusCode::k200OK, callback, "value");
                                           return;
                                       }

                                       std::vector<T> sendValues;

                                       for(const auto& resVal : result)
                                       {
                                           T value;

                                           auto meta = makeMetaInfo(value);

                                           meta.iterateThroughMembers([&resVal](auto member) {
                                               using member_t = decltype(member)::member_t;

                                               if constexpr (std::is_enum_v<member_t>)
                                               {
                                                   member.value = static_cast<member_t>(resVal[std::string {
                                                           member.unmangled_name }].template as<std::int32_t>());
                                               }
                                               else
                                               {
                                                   member.value = resVal[std::string {
                                                           member.unmangled_name }].template as<member_t>();
                                               }
                                           });

                                           sendValues.push_back(value);
                                       }

                                       const std::string req = fmt::format("Selected {} records. From table: '{}'.",
                                                                           sendValues.size(), T::s_parentTableName);

                                       std::cout << req << std::endl;

                                       Utils::sendResponse(SGCore::Serde::Serializer::toFormat(sendValues),
                                                           drogon::HttpStatusCode::k200OK, callback, "value");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << fmt::format("Error selecting record from table '{}': {}",
                                                                T::s_parentTableName, e.base().what()) << std::endl;

                                       Utils::sendResponse("{ }",
                                                           drogon::HttpStatusCode::k400BadRequest, callback);
                                   });
        }
    };
}

#endif // PROJ3_CONTROLLERSCOMMON_H
