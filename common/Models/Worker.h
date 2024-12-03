//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_EMPLOYEE_H
#define PROJ3_EMPLOYEE_H

#include <string>

struct Worker
{
    std::int32_t m_id = 0;
    std::string m_name;
    std::string m_surname;
    std::string m_patronymic;
    std::string m_role;
    // FK FROM STORAGES
    std::int32_t m_storageID = -1;
    std::string m_login;
    std::string m_password;
};

#endif //PROJ3_EMPLOYEE_H
