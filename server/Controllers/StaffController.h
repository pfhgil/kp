//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_STAFFCONTROLLER_H
#define PROJ3_STAFFCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "SerdeSpecs.h"

namespace api
{
    struct staff : public drogon::HttpController<staff, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(staff::getAllWorkersHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::getWorkerByLoginHandler, "/get/login={login}", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::addWorkerHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::authHandler, "/auth", drogon::Get);
            METHOD_ADD(staff::deleteWorkerHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::isAuthValid, "/validate_auth/tok={token}", drogon::Get);
        PATH_LIST_END

        using callback_t = std::function<void(const drogon::HttpResponsePtr&)>;

        void addWorkerHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            std::string deserLog;
            Worker value;
            SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

            dbClient->execSqlAsync(
                    "INSERT INTO staff (id, name, surname, patronymic, role, storage_id, login, password) "
                    "VALUES (DEFAULT, $1, $2, $3, $4, $5, $6, $7) RETURNING id, name",
                    [callback](const drogon::orm::Result& result) {
                        if (!result.empty())
                        {
                            std::cout << "Inserted worker ID: " << result[0]["id"].as<int>() << std::endl;
                            Utils::sendResponse("Worker '" + result[0]["name"].as<std::string>() + "' added!",
                                         drogon::HttpStatusCode::k200OK, callback
                            );
                        }
                    },
                    [callback](const drogon::orm::DrogonDbException& e) {
                        std::cerr << "Error inserting worker: " << e.base().what() << std::endl;
                        Utils::sendResponse("Error inserting worker!", drogon::HttpStatusCode::k400BadRequest, callback);
                    },
                    value.m_name,
                    value.m_surname,
                    value.m_patronymic,
                    value.m_role,
                    value.m_storageID,
                    value.m_login,
                    value.m_password);
        }

        void getAllWorkersHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM staff",
                                   [callback](const drogon::orm::Result& result) {
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

                                       const std::string req =
                                               "Selected " + std::to_string(result.size()) + " workers.";
                                       std::cout << req << std::endl;
                                       Utils::sendResponse(SGCore::Serde::Serializer::toFormat(workers),
                                                    drogon::HttpStatusCode::k200OK, callback, "value");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error selecting workers: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error selecting workers! " + std::string(e.base().what()),
                                                    drogon::HttpStatusCode::k400BadRequest, callback);
                                   });
        }

        void getWorkerByLoginHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, const std::string& login)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("SELECT * FROM staff WHERE login = $1",
                                   [callback](const drogon::orm::Result& result) {
                                        if(result.empty())
                                        {
                                            Utils::sendResponse(SGCore::Serde::Serializer::toFormat(Worker { .m_id = -1 }),
                                                                drogon::HttpStatusCode::k200OK, callback, "value");
                                            return;
                                        }

                                        Worker worker;

                                        worker.m_id = result[0]["id"].as<int>();
                                        worker.m_name = result[0]["name"].as<std::string>();
                                        worker.m_surname = result[0]["surname"].as<std::string>();
                                        worker.m_patronymic = result[0]["patronymic"].as<std::string>();
                                        worker.m_role = result[0]["role"].as<std::string>();
                                        worker.m_storageID = result[0]["storage_id"].as<int>();
                                        worker.m_login = result[0]["login"].as<std::string>();
                                        worker.m_password = result[0]["password"].as<std::string>();

                                        const std::string req =
                                                "Selected workers by login " + worker.m_login;

                                        std::cout << req << std::endl;

                                        Utils::sendResponse(SGCore::Serde::Serializer::toFormat(worker),
                                                            drogon::HttpStatusCode::k200OK, callback, "value");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error selecting worker: " << e.base().what() << std::endl;
                                       Utils::sendResponse("{ }",
                                                           drogon::HttpStatusCode::k400BadRequest, callback);
                                   },
                                   login);
        }

        void deleteWorkerHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("DELETE FROM staff WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                       std::cout << "Delete worker with ID: " << id << std::endl;
                                       Utils::sendResponse("Worker with ID '" + std::to_string(id) + "' deleted!",
                                                    drogon::HttpStatusCode::k200OK, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error deleting worker: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Error deleting worker!", drogon::HttpStatusCode::k400BadRequest,
                                                    callback);
                                   },
                                   id);

            std::cout << "workerrrrrrrr " << id << std::endl;
        }

        void authHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            auto dbClient = drogon::app().getDbClient();

            Json::Value jsonBody;
            auto requestBody = request->getJsonObject();

            const std::string gotLogin = (*requestBody)["login"].asString();
            const std::string gotPassword = (*requestBody)["password"].asString();

            dbClient->execSqlAsync("SELECT * FROM staff WHERE login = $1 AND password = $2",
                                   [callback](const drogon::orm::Result& result) {
                                       if (result.empty())
                                       {
                                           Utils::sendResponse("",
                                                               drogon::HttpStatusCode::k200OK, callback, "token");
                                           return;
                                       }

                                       Worker gotWorker;
                                       gotWorker.m_login = result[0]["login"].as<std::string>();
                                       gotWorker.m_role = result[0]["role"].as<std::string>();
                                       gotWorker.m_id = result[0]["id"].as<int>();
                                       // generating jwt token
                                       const std::string jwtToken = Utils::generateJWT(gotWorker);

                                       std::cout << "Authorized worker: " << gotWorker.m_id << ", token: "
                                                 << jwtToken << std::endl;
                                       Utils::sendResponse(jwtToken,
                                                           drogon::HttpStatusCode::k200OK, callback, "token");
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Auth error: " << e.base().what() << std::endl;
                                       Utils::sendResponse("Auth error!", drogon::HttpStatusCode::k400BadRequest,
                                                    callback
                                       );

                                   },
                                   gotLogin,
                                   gotPassword);
        }

        void isAuthValid(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback,
                         const std::string& token)
        {
            Utils::sendResponse(Utils::validateJWT(token, nullptr),
                                drogon::HttpStatusCode::k200OK, callback, "value");
        }
    };
}

#endif //PROJ3_STAFFCONTROLLER_H
