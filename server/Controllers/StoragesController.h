//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_STORAGESCONTROLLER_H
#define PROJ3_STORAGESCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "SerdeSpecs.h"

namespace api
{
    struct storages : public drogon::HttpController<storages, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(storages::getAllStoragesHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(storages::addStorageHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(storages::deleteStorageByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(storages::getStorageByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllStoragesHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT id, address FROM storages",
                                   [callback](const drogon::orm::Result& result) {
                                       std::vector<Storage> storages;

                                       for (const auto& row: result)
                                       {
                                           Storage storage;

                                           storage.m_id = row["id"].as<int>();
                                           storage.m_address = row["address"].as<std::string>();

                                           storages.push_back(storage);
                                       }

                                       const std::string req =
                                               "Selected " + std::to_string(result.size()) + " storages.";
                                       std::cout << req << std::endl;
                                       Utils::sendResponse(SGCore::Serde::Serializer::toFormat(storages),
                                                           drogon::HttpStatusCode::k200OK, callback, "value");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error selecting storages: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error selecting storages!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   });
        }

        void addStorageHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            std::string deserLog;
            Storage value;
            SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

            dbClient->execSqlAsync("INSERT INTO storages (id, address) "
                                   "VALUES (DEFAULT, $1) RETURNING id",
                                   [callback](const drogon::orm::Result& result) {
                                       if (!result.empty())
                                       {
                                           std::cout << "Inserted storage ID: " << result[0]["id"].as<int>()
                                                     << std::endl;
                                           Utils::sendResponse(
                                                   "Inserted storage ID: " + std::to_string(result[0]["id"].as<int>()),
                                                   drogon::HttpStatusCode::k200OK, callback
                                           );
                                       }

                                       Utils::sendResponse(
                                                   "Error inserting storage!",
                                                   drogon::HttpStatusCode::k400BadRequest, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error inserting storage: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error inserting storage!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback
                                       );

                                   },
                                   value.m_address);
        }

        void deleteStorageByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("DELETE FROM storages WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                       std::cout << "Delete storage with ID: " << id << std::endl;
                                       Utils::sendResponse("Storage with ID '" + std::to_string(id) + "' deleted!",
                                                           drogon::HttpStatusCode::k200OK, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error deleting storage: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error deleting storage!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   id);
        }

        void getStorageByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM storages WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                        if(result.empty())
                                        {
                                            Utils::sendResponse("{}",drogon::HttpStatusCode::k204NoContent, callback);
                                            return;
                                        }

                                        Storage gotStorage;
                                        gotStorage.m_id = result[0]["id"].as<int>();
                                        gotStorage.m_address = result[0]["address"].as<std::string>();

                                        std::cout << "Got storage by ID: " << id << std::endl;

                                        Utils::sendResponse(SGCore::Serde::Serializer::toFormat(gotStorage),
                                                            drogon::HttpStatusCode::k200OK, callback, "value"
                                        );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error getting storage: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error getting storage!", drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   id);
        }
    };
}

#endif //PROJ3_STORAGESCONTROLLER_H
