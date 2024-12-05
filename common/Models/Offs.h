//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_OFFS_H
#define PROJ3_OFFS_H

#include <string>

struct Offs
{
    // PK
    std::int32_t m_id = 0;
    // FK FROM ITEMS
    std::int32_t m_itemID = -1;
    // FK FROM STAFF
    std::int32_t m_workerID = -1;
    std::string m_reason;
    // FK FROM STORAGES
    std::int32_t m_storageID = -1;
};

#endif //PROJ3_OFFS_H
