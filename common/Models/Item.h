//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ITEM_H
#define PROJ3_ITEM_H

#include <string>

struct Item
{
    static const inline std::string s_parentTableName = "items";

    std::int64_t id = 0;
    // FK FROM PROVIDERS
    std::int64_t provider_id = -1;
    // FK FROM item_type_info
    std::int64_t type_info_id = -1;
    // FK FROM STORAGES
    std::int64_t storage_id = -1;
};

#endif //PROJ3_ITEM_H
