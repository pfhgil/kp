//
// Created by Ilya on 09.12.2024.
//

#ifndef PROJ3_OFFSCONTROLLER_H
#define PROJ3_OFFSCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Common.h"
#include "Utils.h"
#include "Models/Offs.h"

namespace api
{
    struct offs : public drogon::HttpController<offs, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(offs::getAllOffsHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(offs::addOffHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(offs::deleteOffByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(offs::updateOffByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(offs::getOffByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllOffsHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Offs>(request, std::forward<Utils::callback_t>(callback));
        }

        void addOffHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Offs, "item_id", "worker_id", "reason", "storage_id">
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO offs (id, item_id, worker_id, reason, storage_id) "
                     "VALUES (DEFAULT, $1, $2, $3, $4) RETURNING id");
        }

        void deleteOffByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Offs>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateOffByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Offs, "item_id", "worker_id", "reason", "storage_id">(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE offs SET item_id = $1, worker_id = $2, reason = $3, storage_id = $4 WHERE id = $5", id);
        }

        void getOffByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<Offs>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_OFFSCONTROLLER_H
