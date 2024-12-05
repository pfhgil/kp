//
// Created by Ilya on 05.12.2024.
//

#ifndef PROJ3_ITEMTYPEINFOCONTROLLER_H
#define PROJ3_ITEMTYPEINFOCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "SerdeSpecs.h"

namespace api
{
    struct item_type_info : public drogon::HttpController<item_type_info, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(item_type_info::getAllItemsTypeInfoHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(item_type_info::addItemTypeInfoHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(item_type_info::deleteItemTypeInfoByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(item_type_info::updateItemTypeInfoByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(item_type_info::getItemTypeInfoByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllItemsTypeInfoHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM item_type_info",
                                   [callback](const drogon::orm::Result& result) {
                                       std::vector<ItemTypeInfo> itemsTypeInfo;

                                       for (const auto& row: result)
                                       {
                                           ItemTypeInfo itemTypeInfo;

                                           itemTypeInfo.m_id = row["id"].as<int>();
                                           itemTypeInfo.m_name = row["name"].as<std::string>();
                                           itemTypeInfo.m_count = row["count"].as<std::int64_t>();
                                           itemTypeInfo.m_dateOfReceipt = row["date_of_receipt"].as<std::string>();
                                           itemTypeInfo.m_expirationDate = row["expiration_date"].as<std::string>();
                                           itemTypeInfo.m_productionDate = row["production_date"].as<std::string>();

                                           itemsTypeInfo.push_back(itemTypeInfo);
                                       }

                                       const std::string req =
                                               "Selected " + std::to_string(result.size()) + " items type info.";
                                       std::cout << req << std::endl;
                                       Utils::sendResponse(SGCore::Serde::Serializer::toFormat(itemsTypeInfo),
                                                           drogon::HttpStatusCode::k200OK, callback, "value");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error selecting items type info: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error selecting items type info!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   });
        }

        void addItemTypeInfoHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            std::string deserLog;
            ItemTypeInfo value;
            SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

            dbClient->execSqlAsync("INSERT INTO item_type_info (id, name, count, date_of_receipt, expiration_date, production_date) "
                                   "VALUES (DEFAULT, $1, $2, $3, $4, $5) RETURNING id",
                                   [callback](const drogon::orm::Result& result) {
                                       if (!result.empty())
                                       {
                                           std::cout << "Inserted item type info ID: " << result[0]["id"].as<int>()
                                                     << std::endl;
                                           Utils::sendResponse(
                                                   "Inserted item type info ID: " + std::to_string(result[0]["id"].as<int>()),
                                                   drogon::HttpStatusCode::k200OK, callback
                                           );
                                       }

                                       Utils::sendResponse(
                                               "Error inserting item type info!",
                                               drogon::HttpStatusCode::k400BadRequest, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error inserting item type info: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error inserting item type info!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback
                                       );

                                   },
                                   value.m_name,
                                   value.m_count,
                                   value.m_dateOfReceipt,
                                   value.m_expirationDate,
                                   value.m_productionDate);
        }

        void deleteItemTypeInfoByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("DELETE FROM item_type_info WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                       std::cout << "Delete item type info with ID: " << id << std::endl;
                                       Utils::sendResponse("Item type info with ID '" + std::to_string(id) + "' deleted!",
                                                           drogon::HttpStatusCode::k200OK, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error deleting item type info: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error deleting item type info!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   id);
        }

        void updateItemTypeInfoByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            std::string deserLog;
            ItemTypeInfo value;
            SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

            dbClient->execSqlAsync("UPDATE item_type_info SET name = $1, count = $2, date_of_receipt = $3, "
                                   "expiration_date = $4, production_date = $5 WHERE id = $6",
                                   [callback, id](const drogon::orm::Result& result) {
                                       std::cout << "Updated item type info with ID: " << id << std::endl;
                                       Utils::sendResponse("Item type info with ID '" + std::to_string(id) + "' updated!",
                                                           drogon::HttpStatusCode::k200OK, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error updating item type info: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error updating item type info!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   value.m_name,
                                   value.m_count,
                                   value.m_dateOfReceipt,
                                   value.m_expirationDate,
                                   value.m_productionDate,
                                   id);
        }

        void getItemTypeInfoByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM item_type_info WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                       if(result.empty())
                                       {
                                           Utils::sendResponse("{}",drogon::HttpStatusCode::k204NoContent, callback);
                                           return;
                                       }

                                       ItemTypeInfo gotItemTypeInfo;
                                       gotItemTypeInfo.m_id = result[0]["id"].as<int>();
                                       gotItemTypeInfo.m_name = result[0]["name"].as<std::string>();
                                       gotItemTypeInfo.m_count = result[0]["count"].as<std::int64_t>();
                                       gotItemTypeInfo.m_dateOfReceipt = result[0]["date_of_receipt"].as<std::string>();
                                       gotItemTypeInfo.m_expirationDate = result[0]["expiration_date"].as<std::string>();
                                       gotItemTypeInfo.m_productionDate = result[0]["production_date"].as<std::string>();

                                       std::cout << "Got item type info by ID: " << id << std::endl;

                                       Utils::sendResponse(SGCore::Serde::Serializer::toFormat(gotItemTypeInfo),
                                                           drogon::HttpStatusCode::k200OK, callback, "value"
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error getting item type info: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error getting item type info!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   id);
        }
    };
}

#endif //PROJ3_ITEMTYPEINFOCONTROLLER_H
