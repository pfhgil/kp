//
// Created by stuka on 01.12.2024.
//

#include "Client.h"

void Client::start() noexcept
{
    s_httpClient = drogon::HttpClient::newHttpClient("http://127.0.0.1:3456");

    s_clientThread = SGCore::Threading::Thread::create();
    auto task = SGCore::MakeRef<SGCore::Threading::Task>();
    task->setOnExecuteCallback([]() {
        drogon::app().run();
    });
    s_clientThread->addTask(task);
    s_clientThread->start();
}

std::future<Worker> Client::getWorkerByLogin(const std::string& login) noexcept
{
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/api/staff/get/login=" + login);
    req->setMethod(drogon::HttpMethod::Get);
    req->addHeader("Authorization", "Bearer " + s_jwtToken);

    auto valuePromise = std::make_shared<std::promise<Worker>>();
    auto valueFuture = valuePromise->get_future();

    s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response)  {
        if (result == drogon::ReqResult::Ok &&
            response->getStatusCode() == drogon::HttpStatusCode::k200OK)
        {
            Worker worker;
            std::string deserLog;
            SGCore::Serde::Serializer::fromFormat((*response->getJsonObject())["value"].asString(), worker, deserLog);
            std::cout << "Got worker with login " << worker.login << ", status: " << response->getStatusCode() << std::endl;

            valuePromise->set_value(worker);
        }
        else
        {
            std::cerr << "Request failed, request code: " << static_cast<int>(result)
                      << ", and status code: " << response->getStatusCode() << std::endl;
            valuePromise->set_value({ });
        }
    });

    return valueFuture;
}

void Client::auth(const std::string& login, const std::string& password) noexcept
{
    Json::Value json;
    json["login"] = login;
    json["password"] = password;

    auto req = drogon::HttpRequest::newHttpJsonRequest(json);
    req->setPath("/api/staff/auth");
    req->setMethod(drogon::HttpMethod::Get);

    auto valuePromise = std::make_shared<std::promise<bool>>();
    auto valueFuture = valuePromise->get_future();

    s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok &&
            response->getStatusCode() == drogon::HttpStatusCode::k200OK)
        {
            auto jsonObj = response->getJsonObject();

            if(jsonObj)
            {
                s_jwtToken = (*jsonObj).isMember("token") ? (*jsonObj)["token"].asString() : "";
            }
            else
            {
                s_jwtToken = "";
            }

            valuePromise->set_value(true);

            std::cout << "Auth response: " << response->body() << ", status: " << response->getStatusCode() << std::endl;
        }
        else
        {
            std::cerr << "Request failed, request code: " << static_cast<int>(result)
                      << ", and status code: " << response->getStatusCode() << std::endl;

            valuePromise->set_value(false);
        }
    });

    valueFuture.get();
}

std::future<bool> Client::isAuthValid(const std::string& token) noexcept
{
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/api/staff/validate_auth/tok=" + token);
    req->setMethod(drogon::HttpMethod::Get);

    auto valuePromise = std::make_shared<std::promise<bool>>();
    auto valueFuture = valuePromise->get_future();

    s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok &&
            response->getStatusCode() == drogon::HttpStatusCode::k200OK)
        {
            auto jsonObj = response->getJsonObject();

            valuePromise->set_value(jsonObj &&
                                    (*jsonObj).isMember("value") &&
                                    (*jsonObj)["value"].asBool());

            std::cout << "Auth response: " << response->body() << ", status: " << response->getStatusCode() << std::endl;
        }
        else
        {
            std::cerr << "Request failed, request code: " << static_cast<int>(result)
                      << ", and status code: " << response->getStatusCode() << std::endl;

            valuePromise->set_value(false);
        }
    });

    return valueFuture;
}

const std::string& Client::getJWTToken() noexcept
{
    return s_jwtToken;
}
