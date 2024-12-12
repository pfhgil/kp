//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_ORDERSCONTROLLER_H
#define PROJ3_ORDERSCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "Models/Order.h"
#include "Common.h"

namespace api
{
    struct orders : public drogon::HttpController<orders, false>
    {
        PATH_LIST_BEGIN
        METHOD_ADD(orders::getAllOrdersHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
        METHOD_ADD(orders::addOrderHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
        METHOD_ADD(orders::deleteOrderByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
        METHOD_ADD(orders::updateOrderByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
        METHOD_ADD(orders::getOrderByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllOrdersHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Order>(request, std::forward<Utils::callback_t>(callback));
        }

        void addOrderHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Order, "provider_id", "item_type_info_id", "ordered_count", "cost">
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO orders (id, provider_id, item_type_info_id, ordered_count, cost) "
                     "VALUES (DEFAULT, $1, $2, $3, $4) RETURNING id");
        }

        void deleteOrderByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Order>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateOrderByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Order, "provider_id", "item_type_info_id", "ordered_count", "cost">(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE orders SET provider_id = $1, item_type_info_id = $2, "
                    "ordered_count = $3, cost = $4 WHERE id = $5", id);
        }

        void getOrderByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<Order>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_ORDERSCONTROLLER_H
