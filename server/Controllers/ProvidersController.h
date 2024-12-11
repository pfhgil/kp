//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_PROVIDERSCONTROLLER_H
#define PROJ3_PROVIDERSCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Common.h"

namespace api
{
    struct providers : public drogon::HttpController<providers, false>
    {
        PATH_LIST_BEGIN
        METHOD_ADD(providers::getAllProvidersHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
        METHOD_ADD(providers::addProvidersHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
        METHOD_ADD(providers::deleteProviderByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
        METHOD_ADD(providers::updateProviderByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
        METHOD_ADD(providers::getProviderByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllProvidersHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Provider>(request, std::forward<Utils::callback_t>(callback));
        }

        void addProvidersHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Provider, 1>
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO providers (id, name) "
                     "VALUES (DEFAULT, $1) RETURNING id");
        }

        void deleteProviderByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Provider>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateProviderByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Provider, 1>(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE providers SET name = $1 WHERE id = $2", id);
        }

        void getProviderByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<Provider>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_PROVIDERSCONTROLLER_H
