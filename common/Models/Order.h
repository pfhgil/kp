//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ORDER_H
#define PROJ3_ORDER_H

#include <string>

struct Order
{
    std::int32_t m_id = 0;
    // FK FROM PROVIDERS
    std::int32_t m_providerID = -1;
    // FK FROM item_type_info
    std::int32_t m_itemTypeInfoID = -1;
    std::int32_t m_orderedCount = 0;
    float m_cost = 0.0f;
};

#endif //PROJ3_ORDER_H
