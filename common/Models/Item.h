//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ITEM_H
#define PROJ3_ITEM_H

#include <string>

struct Item
{
    std::int64_t m_id = 0;
    // FK FROM PROVIDERS
    std::int64_t m_providerID = 0;
    // FK FROM item_type_info
    std::int64_t m_typeInfoID = 0;
    // FK FROM STORAGES
    std::int64_t m_storageID = 0;
};

#endif //PROJ3_ITEM_H
