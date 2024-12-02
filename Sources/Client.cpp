//
// Created by stuka on 01.12.2024.
//

#include "Client.h"
#include "SerdeSpecs.h"

void Client::start() noexcept
{
    s_httpClient = drogon::HttpClient::newHttpClient("http://127.0.0.1:3000");

    s_clientThread = SGCore::Threading::Thread::create();
    auto task = SGCore::MakeRef<SGCore::Threading::Task>();
    task->setOnExecuteCallback([]() {
        drogon::app().run();
    });
    s_clientThread->addTask(task);
    s_clientThread->start();
}

void Client::addWorker(const Worker& value) noexcept
{
    Json::Value json;
    json["value"] = SGCore::Serde::Serializer::toFormat(value);
    auto req = drogon::HttpRequest::newHttpJsonRequest(json);
    req->setPath("/add_worker");
    req->setMethod(drogon::HttpMethod::Post);

    s_httpClient->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok)
        {
            std::cout << "Response received: " << response->body() << ", status: " << response->getStatusCode() << std::endl;
        }
        else
        {
            std::cerr << "Request failed, error code: " << static_cast<int>(result) << std::endl;
        }
    });
}

std::future<std::vector<Worker>> Client::getAllWorkers() noexcept
{
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/get_all_workers");
    req->setMethod(drogon::HttpMethod::Get);

    auto valuePromise = std::make_shared<std::promise<std::vector<Worker>>>();
    auto valueFuture = valuePromise->get_future();

    s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response)  {
        if (result == drogon::ReqResult::Ok)
        {
            std::vector<Worker> workers;
            std::string deserLog;
            SGCore::Serde::Serializer::fromFormat((*response->getJsonObject())["value"].asString(), workers, deserLog);
            std::cout << "Got " << workers.size() << " workers, status: " << response->getStatusCode() << std::endl;

            valuePromise->set_value(workers);
        }
        else
        {
            std::cerr << "Request failed, error code: " << static_cast<int>(result) << std::endl;
        }
    });

    return valueFuture;
}

void Client::addStorage(const Storage& value) noexcept
{
    Json::Value json;
    json["value"] = SGCore::Serde::Serializer::toFormat(value);
    auto req = drogon::HttpRequest::newHttpJsonRequest(json);
    req->setPath("/add_storage");
    req->setMethod(drogon::HttpMethod::Post);

    s_httpClient->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok)
        {
            std::cout << "Response received: " << response->body() << ", status: " << response->getStatusCode() << std::endl;
        }
        else
        {
            std::cerr << "Request failed, error code: " << static_cast<int>(result) << std::endl;
        }
    });
}

std::future<std::vector<Storage>> Client::getAllStorages() noexcept
{
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/get_all_storages");
    req->setMethod(drogon::HttpMethod::Get);

    auto valuePromise = std::make_shared<std::promise<std::vector<Storage>>>();
    auto valueFuture = valuePromise->get_future();

    s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response)  {
        if (result == drogon::ReqResult::Ok)
        {
            std::vector<Storage> storages;
            std::string deserLog;
            SGCore::Serde::Serializer::fromFormat((*response->getJsonObject())["value"].asString(), storages, deserLog);
            std::cout << "Got " << storages.size() << " storages, status: " << response->getStatusCode() << std::endl;

            valuePromise->set_value(storages);
        }
        else
        {
            std::cerr << "Request failed, error code: " << static_cast<int>(result) << std::endl;
        }
    });

    return valueFuture;
}
