//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_ITEMCONTROLLER_H
#define PROJ3_ITEMCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "Models/Item.h"
#include "Common.h"

namespace api
{
    struct items : public drogon::HttpController<items, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(items::getAllItemsHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(items::addItemHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(items::deleteItemByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(items::updateItemByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(items::getItemByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllItemsHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Item>(request, std::forward<Utils::callback_t>(callback));
        }

        void addItemHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Item, 1, 2, 3>
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO items (id, provider_id, type_info_id, storage_id) "
                     "VALUES (DEFAULT, $1, $2, $3) RETURNING id");
        }

        void deleteItemByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Item>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateItemByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Item, 1, 2, 3>(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE items SET provider_id = $1, type_info_id = $2, storage_id = $3 WHERE id = $4", id);
        }

        void getItemByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<Item>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_ITEMCONTROLLER_H
