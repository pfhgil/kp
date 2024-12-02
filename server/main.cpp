#include <iostream>

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>

#include "SerdeSpecs.h"

using callback_t = std::function<void(const drogon::HttpResponsePtr&)>;

void sendResponse(const std::string& message, drogon::HttpStatusCode statusCode, const callback_t& callback, const std::string& column = "message") noexcept
{
    Json::Value jsonBody;

    jsonBody[column] = message;

    auto response = drogon::HttpResponse::newHttpJsonResponse(jsonBody);
    response->setStatusCode(statusCode);
    callback(response);
}

void addWorkerHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
{
    auto dbClient = drogon::app().getDbClient();

    Json::Value jsonBody;
    auto requestBody = request->getJsonObject();

    std::string deserLog;
    Worker value;
    SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

    dbClient->execSqlAsync("INSERT INTO staff (id, name, surname, patronymic, role, storage_id, login, password) "
                           "VALUES (DEFAULT, $1, $2, $3, $4, $5, $6, $7) RETURNING id, name",
                           [callback](const drogon::orm::Result &result) {
                               if (!result.empty())
                               {
                                   std::cout << "Inserted worker ID: " << result[0]["id"].as<int>() << std::endl;
                                   sendResponse("Worker '" + result[0]["name"].as<std::string>() + "' added!",
                                                drogon::HttpStatusCode::k200OK, callback
                                   );
                               }
                           },
                           [callback](const drogon::orm::DrogonDbException& e) {
                               std::cerr << "Error inserting worker: " << e.base().what() << std::endl;
                               sendResponse("Error inserting worker!", drogon::HttpStatusCode::k400BadRequest, callback);
                           },
                           value.m_name,
                           value.m_surname,
                           value.m_patronymic,
                           value.m_role,
                           value.m_storageID,
                           value.m_login,
                           value.m_password);
}

void addStorageHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
{
    auto dbClient = drogon::app().getDbClient();

    Json::Value jsonBody;
    auto requestBody = request->getJsonObject();

    std::string deserLog;
    Storage value;
    SGCore::Serde::Serializer::fromFormat(requestBody->get("value", {}).asString(), value, deserLog);

    dbClient->execSqlAsync("INSERT INTO storages (id, address) "
                           "VALUES (DEFAULT, $1) RETURNING id",
                           [callback](const drogon::orm::Result &result) {
                               if (!result.empty())
                               {
                                   std::cout << "Inserted storage ID: " << result[0]["id"].as<int>() << std::endl;
                                   sendResponse("Inserted storage ID: " + std::to_string(result[0]["id"].as<int>()),
                                                drogon::HttpStatusCode::k200OK, callback
                                   );
                               }
                           },
                           [callback](const drogon::orm::DrogonDbException& e) {
                               std::cerr << "Error inserting storage: " << e.base().what() << std::endl;
                               sendResponse("Error inserting storage!", drogon::HttpStatusCode::k400BadRequest,
                                            callback
                               );

                           },
                           value.m_address);
}

void getAllWorkersHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync("SELECT * FROM staff",
                           [callback](const drogon::orm::Result &result) {
                               std::vector<Worker> workers;

                               for (const auto& row: result)
                               {
                                   Worker worker;

                                   worker.m_id = row["id"].as<int>();
                                   worker.m_name = row["name"].as<std::string>();
                                   worker.m_surname = row["surname"].as<std::string>();
                                   worker.m_patronymic = row["patronymic"].as<std::string>();
                                   worker.m_role = row["role"].as<std::string>();
                                   worker.m_storageID = row["storage_id"].as<int>();
                                   worker.m_login = row["login"].as<std::string>();
                                   worker.m_password = row["password"].as<std::string>();

                                   workers.push_back(worker);
                               }

                               const std::string req = "Selected " + std::to_string(result.size()) + " workers.";
                               std::cout << req << std::endl;
                               sendResponse(SGCore::Serde::Serializer::toFormat(workers),
                                            drogon::HttpStatusCode::k200OK, callback, "value");
                           },
                           [callback](const drogon::orm::DrogonDbException& e) {
                               std::cerr << "Error selecting workers: " << e.base().what() << std::endl;
                               sendResponse("Error selecting workers! " + std::string(e.base().what()), drogon::HttpStatusCode::k400BadRequest, callback);
                           });
}

void getAllStoragesHandler(const drogon::HttpRequestPtr& request, callback_t&& callback)
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync("SELECT id, address FROM storages",
                           [callback](const drogon::orm::Result& result) {
                               std::vector<Storage> storages;

                               for (const auto& row: result)
                               {
                                   Storage storage;

                                   storage.m_id = row["id"].as<int>();
                                   storage.m_address = row["address"].as<std::string>();

                                   storages.push_back(storage);
                               }

                               const std::string req = "Selected " + std::to_string(result.size()) + " storages.";
                               std::cout << req << std::endl;
                               sendResponse(SGCore::Serde::Serializer::toFormat(storages),
                                            drogon::HttpStatusCode::k200OK, callback, "value");
                           },
                           [callback](const drogon::orm::DrogonDbException& e) {
                               std::cerr << "Error selecting storages: " << e.base().what() << std::endl;
                               sendResponse("Error selecting storages!", drogon::HttpStatusCode::k400BadRequest, callback);
                           });
}

void deleteWorkerHandler(const drogon::HttpRequestPtr& request, callback_t&& callback, std::int32_t id)
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync("DELETE FROM staff WHERE id = $1",
                           [callback, id](const drogon::orm::Result &result) {
                               std::cout << "Delete worker with ID: " << id << std::endl;
                               sendResponse("Worker with ID '" + std::to_string(id) + "' deleted!",
                                            drogon::HttpStatusCode::k200OK, callback
                               );
                           },
                           [callback](const drogon::orm::DrogonDbException& e) {
                               std::cerr << "Error deleting worker: " << e.base().what() << std::endl;
                               sendResponse("Error deleting worker!", drogon::HttpStatusCode::k400BadRequest, callback);
                           },
                           id);

    std::cout << "workerrrrrrrr " << id << std::endl;
}

void deleteStorageHandler(const drogon::HttpRequestPtr& request, callback_t&& callback, std::int32_t id)
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync("DELETE FROM storages WHERE id = $1",
                           [callback, id](const drogon::orm::Result &result) {
                               std::cout << "Delete storage with ID: " << id << std::endl;
                               sendResponse("Storage with ID '" + std::to_string(id) + "' deleted!",
                                            drogon::HttpStatusCode::k200OK, callback
                               );
                           },
                           [callback](const drogon::orm::DrogonDbException& e) {
                               std::cerr << "Error deleting storage: " << e.base().what() << std::endl;
                               sendResponse("Error deleting storage!", drogon::HttpStatusCode::k400BadRequest, callback);
                           },
                           id);
}

int main()
{
    std::system("chcp 65001");

    drogon::app()
            .createDbClient("postgresql", "localhost", 4567, "warehouse", "postgres", "12345");

    // drogon::app().setDb

    drogon::app()
            .setLogLevel(trantor::Logger::LogLevel::kTrace)
            .registerHandler("/add_worker", &addWorkerHandler, { drogon::Post })
            .registerHandler("/add_storage", &addStorageHandler, { drogon::Post })
            .registerHandler("/get_all_workers", &getAllWorkersHandler, { drogon::Get })
            .registerHandler("/get_all_storages", &getAllStoragesHandler, { drogon::Get })
            .registerHandler("/workers/{id}", &deleteWorkerHandler, { drogon::Delete })
            .registerHandler("/storages/{id}", &deleteStorageHandler, { drogon::Delete })
            .addListener("127.0.0.1", 3456)
            .setThreadNum(8)
            .enableServerHeader(false)
            .run();

    std::cout << "sddssd" << std::endl;

    return 0;
}
