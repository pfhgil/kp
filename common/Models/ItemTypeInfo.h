//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ITEMTYPEINFO_H
#define PROJ3_ITEMTYPEINFO_H

#include <string>
#include <chrono>

struct ItemTypeInfo
{
    std::int64_t m_id = 0;
    std::string m_name;
    std::int64_t m_count;
    std::string m_dateOfReceipt;
    std::string m_expirationDate;
    std::string m_productionDate;
};

#endif //PROJ3_ITEMTYPEINFO_H
