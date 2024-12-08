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
            Common::getAllRecords<ItemTypeInfo>(request, std::forward<Utils::callback_t>(callback));
        }

        void addItemTypeInfoHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<ItemTypeInfo, 1, 2, 3, 4, 5>
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO item_type_info (id, name, count, date_of_receipt, expiration_date, production_date) "
                     "VALUES (DEFAULT, $1, $2, $3, $4, $5) RETURNING id");
        }

        void deleteItemTypeInfoByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<ItemTypeInfo>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateItemTypeInfoByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<ItemTypeInfo, 1, 2, 3, 4, 5>(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE item_type_info SET name = $1, count = $2, date_of_receipt = $3, "
                    "expiration_date = $4, production_date = $5 WHERE id = $6", id);
        }

        void getItemTypeInfoByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<ItemTypeInfo>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_ITEMTYPEINFOCONTROLLER_H
