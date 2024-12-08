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
            METHOD_ADD(storages::updateStorageByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(storages::getStorageByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllStoragesHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Storage>(request, std::forward<Utils::callback_t>(callback));
        }

        void addStorageHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Storage, 1>
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO storages (id, address) "
                     "VALUES (DEFAULT, $1) RETURNING id");
        }

        void deleteStorageByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Storage>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateStorageByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Storage, 1>(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE storages SET address = $1 WHERE id = $2", id);
        }

        void getStorageByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<Storage>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_STORAGESCONTROLLER_H
