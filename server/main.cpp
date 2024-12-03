#include <iostream>

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>
#include <jwt-cpp/jwt.h>
#include "Controller.h"

#include "Utils.h"
#include "SerdeSpecs.h"

int main()
{
    std::system("chcp 65001");

    drogon::app()
            .createDbClient("postgresql", "localhost", 4567, "warehouse", "postgres", "12345");

    // drogon::app().setDb
    drogon::app()
            .setLogLevel(trantor::Logger::LogLevel::kTrace)
            /*.registerHandler("/add_worker", &addWorkerHandler, { drogon::Post })
            .registerHandler("/add_storage", &addStorageHandler, { drogon::Post })
            .registerHandler("/get_all_workers", &getAllWorkersHandler, { drogon::Get })
            .registerHandler("/get_all_storages", &getAllStoragesHandler, { drogon::Get })
            .registerHandler("/workers/{id}", &deleteWorkerHandler, { drogon::Delete })
            .registerHandler("/storages/{id}", &deleteStorageHandler, { drogon::Delete })
            .registerHandler("/auth", &authHandler, { drogon::Get })*/
            .addListener("127.0.0.1", 3456)
            .setThreadNum(8)
            .enableServerHeader(false)
            .run();

    std::cout << "sddssd" << std::endl;

    return 0;
}
