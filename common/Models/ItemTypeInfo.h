//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ITEMTYPEINFO_H
#define PROJ3_ITEMTYPEINFO_H

#include <string>
#include <chrono>

struct ItemTypeInfo
{
    static const inline std::string s_parentTableName = "item_type_info";

    std::int32_t id = -1;
    std::string name;
    std::int32_t count;
    std::string date_of_receipt;
    std::string expiration_date;
    std::string production_date;
};

#endif //PROJ3_ITEMTYPEINFO_H
