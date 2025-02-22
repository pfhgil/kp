//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_PROVIDER_H
#define PROJ3_PROVIDER_H

#include <string>

struct Provider
{
    static const inline std::string s_parentTableName = "providers";

    std::int32_t id = 0;
    std::string name;
};

#endif //PROJ3_PROVIDER_H
