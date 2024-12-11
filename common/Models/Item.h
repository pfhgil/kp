//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ITEM_H
#define PROJ3_ITEM_H

#include <string>

struct Item
{
    static const inline std::string s_parentTableName = "items";

    std::int32_t id = -1;
    // FK FROM PROVIDERS
    std::int32_t provider_id = -1;
    // FK FROM item_type_info
    std::int32_t type_info_id = -1;
    // FK FROM STORAGES
    std::int32_t storage_id = -1;
};

#endif //PROJ3_ITEM_H
