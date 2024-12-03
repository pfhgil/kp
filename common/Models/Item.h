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
    std::int64_t m_providerID = -1;
    // FK FROM item_type_info
    std::int64_t m_typeInfoID = -1;
    // FK FROM STORAGES
    std::int64_t m_storageID = -1;
};

#endif //PROJ3_ITEM_H
