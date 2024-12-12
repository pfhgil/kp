//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_SHIPMENTSCONTROLLER_H
#define PROJ3_SHIPMENTSCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"
#include "Models/Shipment.h"
#include "Common.h"

namespace api
{
    struct shipments : public drogon::HttpController<shipments, false>
    {
        PATH_LIST_BEGIN
            METHOD_ADD(shipments::getAllShipmentsHandler, "/get_all", drogon::Get, "Utils::JWTAuthFilter");
            METHOD_ADD(shipments::addShipmentHandler, "/add", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(shipments::deleteShipmentByIDHandler, "/delete/id={id}", drogon::Delete, "Utils::JWTAuthFilter");
            METHOD_ADD(shipments::updateShipmentByIDHandler, "/update/id={id}", drogon::Post, "Utils::JWTAuthFilter");
            METHOD_ADD(shipments::getShipmentByIDHandler, "/get/id={id}", drogon::Get, "Utils::JWTAuthFilter");
        PATH_LIST_END

        void getAllShipmentsHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::getAllRecords<Shipment>(request, std::forward<Utils::callback_t>(callback));
        }

        void addShipmentHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback)
        {
            Common::addRecord<Shipment, "storage_id", "worker_id", "order_id">
                    (request, std::forward<Utils::callback_t>(callback),
                     "INSERT INTO shipments (id, storage_id, worker_id, order_id) "
                     "VALUES (DEFAULT, $1, $2, $3) RETURNING id");
        }

        void deleteShipmentByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::deleteRecordByID<Shipment>(request, std::forward<Utils::callback_t>(callback), id);
        }

        void updateShipmentByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::updateRecordByID<Shipment, "storage_id", "worker_id", "order_id">(
                    request, std::forward<Utils::callback_t>(callback),
                    "UPDATE shipments SET storage_id = $1, worker_id = $2, order_id = $3 WHERE id = $4", id);
        }

        void getShipmentByIDHandler(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, std::int32_t id)
        {
            Common::getRecordBy<Shipment>(request, std::forward<Utils::callback_t>(callback), "id", id);
        }
    };
}

#endif //PROJ3_SHIPMENTSCONTROLLER_H
