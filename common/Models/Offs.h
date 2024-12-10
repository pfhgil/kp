//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_OFFS_H
#define PROJ3_OFFS_H

#include <string>

struct Offs
{
    static const inline std::string s_parentTableName = "offs";

    // PK
    std::int32_t id = 0;
    // FK FROM ITEMS
    std::int32_t item_id = -1;
    // FK FROM STAFF
    std::int32_t worker_id = -1;
    std::string reason;
    // FK FROM STORAGES
    std::int32_t storage_id = -1;
};

#endif //PROJ3_OFFS_H
