//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_SHIPMENT_H
#define PROJ3_SHIPMENT_H

#include <string>

struct Shipment
{
    static const inline std::string s_parentTableName = "shipments";

    std::int32_t id = 0;
    // FK FROM STORAGES
    std::int32_t storage_id = -1;
    // FK FROM STAFF
    std::int32_t worker_id = -1;
    // FK FROM ORDERS
    std::int32_t order_id = -1;
};

#endif //PROJ3_SHIPMENT_H
