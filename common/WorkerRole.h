//
// Created by Ilya on 05.12.2024.
//

#ifndef PROJ3_WORKERROLE_H
#define PROJ3_WORKERROLE_H

#include <string>

enum class WorkerRole
{
    DIRECTOR,       // ЗАВЕДУЮЩИЙ
    STOREKEEPER,    // КЛАДОВЩИК
    WORKER          // РАБОЧИЙ
};

static std::string roleToString(WorkerRole role) noexcept
{
    switch (role)
    {
        case WorkerRole::DIRECTOR:
            return "Director";
        case WorkerRole::STOREKEEPER:
            return "Storekeeper";
        case WorkerRole::WORKER:
            return "Worker";
    }

    return "";
}

#endif //PROJ3_WORKERROLE_H
