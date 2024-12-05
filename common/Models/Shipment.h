//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_SHIPMENT_H
#define PROJ3_SHIPMENT_H

#include <string>

struct Shipment
{
    std::int32_t m_id = 0;
    // FK FROM STORAGES
    std::int32_t m_storageID = -1;
    // FK FROM STAFF
    std::int32_t m_workerID = -1;
    // FK FROM ORDERS
    std::int32_t m_orderID = -1;
};

#endif //PROJ3_SHIPMENT_H
