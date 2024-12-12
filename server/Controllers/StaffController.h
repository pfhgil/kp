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
#include "Common.h"

namespace api
{
    struct staff : public drogon::HttpController<staff, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(staff::getAllWorkersHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::getWorkerByLoginHandler, "/get/login={login}", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::getWorkerByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::addWorkerHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::authHandler, "/auth", drogon::Get);
            METHOD_ADD(staff::deleteWorkerHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::updateWorkerHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(staff::isAuthValid, "/validate_auth/tok={token}", drogon::Get);
        PATH_LIST_END

        void addWorkerHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Worker, "name", "surname", "patronymic", "role", "storage_id", "login", "password">
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO staff (id, name, surname, patronymic, role, storage_id, login, password) "
                     "VALUES (DEFAULT, $1, $2, $3, $4, $5, $6, $7) RETURNING id, name");
        }

        void getAllWorkersHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Worker>(request, std::forward<Utils::callback_t>(callback));
        }

        void getWorkerByLoginHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, const std::string& login)
        {
            Common::getRecordBy<Worker>(request, std::forward<Utils::callback_t>(callback), "login", login);
        }

        void getWorkerByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, const std::int32_t& id)
        {
            Common::getRecordBy<Worker>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }

        void deleteWorkerHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Worker>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateWorkerHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Worker, "name", "surname", "patronymic", "role", "storage_id", "login", "password">(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE staff SET name = $1, surname = $2, patronymic = $3, "
                    "role = $4, storage_id = $5, login = $6, password = $7 WHERE id = $8", id);
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
                                       gotWorker.login = result[0]["login"].as<std::string>();
                                       gotWorker.role = static_cast<WorkerRole>(result[0]["role"].as<std::int32_t>());
                                       gotWorker.id = result[0]["id"].as<int>();
                                       // generating jwt token
                                       const std::string jwtToken = Utils::generateJWT(gotWorker);

                                       std::cout << "Authorized worker: " << gotWorker.id << ", token: "
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
