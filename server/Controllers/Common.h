//
// Created by Ilya on 08.12.2024.
//

#ifndef PROJ3_CONTROLLERSCOMMON_H
#define PROJ3_CONTROLLERSCOMMON_H

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>

#include "Utils.h"

namespace api
{
    struct Common
    {
        template<typename T>
        static void deleteRecordByID(const drogon::HttpRequestPtr& request, Utils::callback_t&& callback, const std::int32_t& id) noexcept
        {
            auto dbClient = drogon::app().getDbClient();

            dbClient->execSqlAsync("DELETE FROM " + T::s_parentTableName + " WHERE id = $1",
                                   [callback, id](const drogon::orm::Result& result) {
                                       std::cout << "Delete record from '" + T::s_parentTableName + "' with ID: " << id << std::endl;
                                       Utils::sendResponse("Record with ID '" + std::to_string(id) + "' deleted from table '" + T::s_parentTableName + "'.",
                                                           drogon::HttpStatusCode::k200OK, callback
                                       );
                                   },
                                   [callback](const drogon::orm::DrogonDbException& e) {
                                       std::cerr << "Error deleting record from table '" + T::s_parentTableName + "': "
                                                 << e.base().what() << std::endl;
                                       Utils::sendResponse("Error deleting item type info!",
                                                           drogon::HttpStatusCode::k400BadRequest,
                                                           callback);
                                   },
                                   id);
        }
    };
}

#endif // PROJ3_CONTROLLERSCOMMON_H
