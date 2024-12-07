//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_STORAGE_H
#define PROJ3_STORAGE_H

#include <string>

struct Storage
{
    static const inline std::string s_parentTableName = "storages";

    std::int32_t m_id = -1;
    std::string m_address;
};

#endif //PROJ3_STORAGE_H
