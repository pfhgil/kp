//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_EMPLOYEE_H
#define PROJ3_EMPLOYEE_H

#include <string>
#include "WorkerRole.h"

struct Worker
{
    static const inline std::string s_parentTableName = "staff";

    std::int32_t id = -1;
    std::string name;
    std::string surname;
    std::string patronymic;
    WorkerRole role = WorkerRole::WORKER;
    // FK FROM STORAGES
    std::int32_t storage_id = -1;
    std::string login;
    std::string password;
};

#endif //PROJ3_EMPLOYEE_H
