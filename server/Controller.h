//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_CONTROLLER_H
#define PROJ3_CONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "SerdeSpecs.h"

struct Controller : public drogon::HttpController<Controller>
{
    PATH_LIST_BEGIN
        // METHOD_ADD(Controller::getAllWorkersHandler, "/get_all_workers", drogon::Get, "Utils::JWTAuthFilter");
        // PATH_ADD("/get_all_workers", drogon::Get, "Utils::JWTAuthFilter");
        // ADD_METHOD_TO(Controller::getAllWorkersHandler, "/get_all_workers", drogon::Get);
        METHOD_ADD(Controller::getAllWorkersHandler, "/get_all_workers", drogon::Get, "Utils::JWTAuthFilter");
        // PATH_ADD("/get_all_workers", drogon::Get, "Utils::JWTAuthFilter");
        /*ADD_METHOD_TO(Controller::addWorkerHandler, "/add_worker", drogon::Post);
        ADD_METHOD_TO(Controller::addStorageHandler, "/add_storage", drogon::Post);
        ADD_METHOD_TO(Controller::getAllWorkersHandler, "/get_all_workers", drogon::Get);
        ADD_METHOD_TO(Controller::getAllStoragesHandler, "/get_all_storages", drogon::Get);
        ADD_METHOD_TO(Controller::deleteWorkerHandler, "/workers/{id}", drogon::Delete);
        ADD_METHOD_TO(Controller::deleteStorageHandler, "/storages/{id}", drogon::Delete);
        ADD_METHOD_TO(Controller::authHandler, "/auth", drogon::Get);*/
    PATH_LIST_END

    using callback_t = std::function<void(const drogon::HttpResponsePtr&)>;

    static void sendResponse(const std::string& message, drogon::HttpStatusCode statusCode,
                             const callback_t& callback, const std::string& column = "message") noexcept
    {
        Json::Value jsonBody;

        jsonBody[column] = message;

        auto response = drogon::HttpResponse::newHttpJsonResponse(jsonBody);
        response->setStatusCode(statusCode);
        callback(response);
    }

    void addWorkerHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
    {
        auto dbClient = drogon::app().getDbClient();

        Json::Value jsonBody;
        auto requestBody = request->getJsonObject();

        std::string deserLog;
        Worker value;
        SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

        dbClient->execSqlAsync("INSERT INTO staff (id, name, surname, patronymic, role, storage_id, login, password) "
                               "VALUES (DEFAULT, $1, $2, $3, $4, $5, $6, $7) RETURNING id, name",
                               [callback](const drogon::orm::Result &result) {
                                   if (!result.empty())
                                   {
                                       std::cout << "Inserted worker ID: " << result[0]["id"].as<int>() << std::endl;
                                       sendResponse("Worker '" + result[0]["name"].as<std::string>() + "' added!",
                                                    drogon::HttpStatusCode::k200OK, callback
                                       );
                                   }
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Error inserting worker: " << e.base().what() << std::endl;
                                   sendResponse("Error inserting worker!", drogon::HttpStatusCode::k400BadRequest, callback);
                               },
                               value.m_name,
                               value.m_surname,
                               value.m_patronymic,
                               value.m_role,
                               value.m_storageID,
                               value.m_login,
                               value.m_password);
    }

    void addStorageHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
    {
        auto dbClient = drogon::app().getDbClient();

        Json::Value jsonBody;
        auto requestBody = request->getJsonObject();

        std::string deserLog;
        Storage value;
        SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

        dbClient->execSqlAsync("INSERT INTO storages (id, address) "
                               "VALUES (DEFAULT, $1) RETURNING id",
                               [callback](const drogon::orm::Result &result) {
                                   if (!result.empty())
                                   {
                                       std::cout << "Inserted storage ID: " << result[0]["id"].as<int>() << std::endl;
                                       sendResponse("Inserted storage ID: " + std::to_string(result[0]["id"].as<int>()),
                                                    drogon::HttpStatusCode::k200OK, callback
                                       );
                                   }
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Error inserting storage: " << e.base().what() << std::endl;
                                   sendResponse("Error inserting storage!", drogon::HttpStatusCode::k400BadRequest,
                                                callback
                                   );

                               },
                               value.m_address);
    }

    void getAllWorkersHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
    {
        auto dbClient = drogon::app().getDbClient();

        dbClient->execSqlAsync("SELECT * FROM staff",
                               [callback](const drogon::orm::Result &result) {
                                   std::vector<Worker> workers;

                                   for (const auto& row: result)
                                   {
                                       Worker worker;

                                       worker.m_id = row["id"].as<int>();
                                       worker.m_name = row["name"].as<std::string>();
                                       worker.m_surname = row["surname"].as<std::string>();
                                       worker.m_patronymic = row["patronymic"].as<std::string>();
                                       worker.m_role = row["role"].as<std::string>();
                                       worker.m_storageID = row["storage_id"].as<int>();
                                       worker.m_login = row["login"].as<std::string>();
                                       worker.m_password = row["password"].as<std::string>();

                                       workers.push_back(worker);
                                   }

                                   const std::string req = "Selected " + std::to_string(result.size()) + " workers.";
                                   std::cout << req << std::endl;
                                   sendResponse(SGCore::Serde::Serializer::toFormat(workers),
                                                drogon::HttpStatusCode::k200OK, callback, "value");
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Error selecting workers: " << e.base().what() << std::endl;
                                   sendResponse("Error selecting workers! " + std::string(e.base().what()), drogon::HttpStatusCode::k400BadRequest, callback);
                               });
    }

    void getAllStoragesHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
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

                                   const std::string req = "Selected " + std::to_string(result.size()) + " storages.";
                                   std::cout << req << std::endl;
                                   sendResponse(SGCore::Serde::Serializer::toFormat(storages),
                                                drogon::HttpStatusCode::k200OK, callback, "value");
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Error selecting storages: " << e.base().what() << std::endl;
                                   sendResponse("Error selecting storages!", drogon::HttpStatusCode::k400BadRequest, callback);
                               });
    }

    void deleteWorkerHandler(const drogon::HttpRequestPtr& request, callback_t&& callback, std::int32_t id)
    {
        auto dbClient = drogon::app().getDbClient();

        dbClient->execSqlAsync("DELETE FROM staff WHERE id = $1",
                               [callback, id](const drogon::orm::Result &result) {
                                   std::cout << "Delete worker with ID: " << id << std::endl;
                                   sendResponse("Worker with ID '" + std::to_string(id) + "' deleted!",
                                                drogon::HttpStatusCode::k200OK, callback
                                   );
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Error deleting worker: " << e.base().what() << std::endl;
                                   sendResponse("Error deleting worker!", drogon::HttpStatusCode::k400BadRequest, callback);
                               },
                               id);

        std::cout << "workerrrrrrrr " << id << std::endl;
    }

    void deleteStorageHandler(const drogon::HttpRequestPtr& request, callback_t&& callback, std::int32_t id)
    {
        auto dbClient = drogon::app().getDbClient();

        dbClient->execSqlAsync("DELETE FROM storages WHERE id = $1",
                               [callback, id](const drogon::orm::Result &result) {
                                   std::cout << "Delete storage with ID: " << id << std::endl;
                                   sendResponse("Storage with ID '" + std::to_string(id) + "' deleted!",
                                                drogon::HttpStatusCode::k200OK, callback
                                   );
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Error deleting storage: " << e.base().what() << std::endl;
                                   sendResponse("Error deleting storage!", drogon::HttpStatusCode::k400BadRequest, callback);
                               },
                               id);
    }

    void authHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
    {
        auto dbClient = drogon::app().getDbClient();

        Json::Value jsonBody;
        auto requestBody = request->getJsonObject();

        const std::string gotLogin = (*requestBody)["login"].asString();
        const std::string gotPassword = (*requestBody)["password"].asString();

        dbClient->execSqlAsync("SELECT * FROM staff WHERE login = $1 AND password = $2",
                               [callback](const drogon::orm::Result &result) {
                                   if (!result.empty())
                                   {
                                       Worker gotWorker;
                                       gotWorker.m_login = result[0]["login"].as<std::string>();
                                       gotWorker.m_role = result[0]["role"].as<std::string>();
                                       gotWorker.m_id = result[0]["id"].as<int>();
                                       // generating jwt token
                                       const std::string jwtToken = Utils::generateJWT(gotWorker);

                                       std::cout << "Authorized worker: " << gotWorker.m_id << ", token: " << jwtToken << std::endl;
                                       sendResponse(jwtToken,
                                                    drogon::HttpStatusCode::k200OK, callback, "token");
                                   }
                               },
                               [callback](const drogon::orm::DrogonDbException& e) {
                                   std::cerr << "Auth error: " << e.base().what() << std::endl;
                                   sendResponse("Auth error!", drogon::HttpStatusCode::k400BadRequest,
                                                callback
                                   );

                               },
                               gotLogin,
                               gotPassword);
    }
};

#endif //PROJ3_CONTROLLER_H
