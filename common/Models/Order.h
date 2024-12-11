//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_ORDER_H
#define PROJ3_ORDER_H

#include <string>

struct Order
{
    static const inline std::string s_parentTableName = "orders";

    std::int32_t id = 0;
    // FK FROM PROVIDERS
    std::int32_t provider_id = -1;
    // FK FROM item_type_info
    std::int32_t item_type_info_id = -1;
    std::int32_t ordered_count = 0;
    float cost = 0.0f;
};

#endif //PROJ3_ORDER_H
