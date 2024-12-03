//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_ROLES_H
#define PROJ3_ROLES_H

#include <string>

enum class Role
{
    DIRECTOR,       // ЗАВЕДУЮЩИЙ
    STOREKEEPER,    // КЛАДОВЩИК
    WORKER          // РАБОЧИЙ
};

static std::string roleToString(Role role) noexcept
{
    switch (role)
    {
        case Role::DIRECTOR:
            return "Director";
        case Role::STOREKEEPER:
            return "Storekeeper";
        case Role::WORKER:
            return "Worker";
    }

    return "";
}

#endif //PROJ3_ROLES_H
