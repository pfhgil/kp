//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_CLIENT_H
#define PROJ3_CLIENT_H

#include <SGCore/Threading/Thread.h>
#include <SGCore/Main/CoreGlobals.h>
#include <drogon/drogon.h>
#include "Models/Worker.h"
#include "Models/Storage.h"
#include "Models/ItemTypeInfo.h"
#include "SerdeSpecs.h"

struct Client
{
    static void start() noexcept;

    template<typename T>
    static void addRecord(const T& value) noexcept
    {
        Json::Value json;
        json["value"] = SGCore::Serde::Serializer::toFormat(value);

        auto req = drogon::HttpRequest::newHttpJsonRequest(json);
        req->setPath("/api/" + T::s_parentTableName + "/add");
        req->setMethod(drogon::HttpMethod::Post);
        req->addHeader("Authorization", "Bearer " + s_jwtToken);

        s_httpClient->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok &&
                response->getStatusCode() == drogon::HttpStatusCode::k200OK)
            {
                std::cout << "Response received: " << response->body() << ", status: "
                          << response->getStatusCode() << std::endl;
            }
            else
            {
                std::cerr << "Request failed, request code: " << static_cast<int>(result)
                          << ", and status code: " << response->getStatusCode() << std::endl;
            }
        });
    }

    template<typename T>
    static void updateRecord(const std::int32_t& id, const T& value) noexcept
    {
        Json::Value json;
        json["value"] = SGCore::Serde::Serializer::toFormat(value);

        auto req = drogon::HttpRequest::newHttpJsonRequest(json);
        req->setPath("/api/" + T::s_parentTableName + "/update/id=" + std::to_string(id));
        req->setMethod(drogon::HttpMethod::Post);
        req->addHeader("Authorization", "Bearer " + s_jwtToken);

        s_httpClient->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok &&
                response->getStatusCode() == drogon::HttpStatusCode::k200OK)
            {
                std::cout << "Response received: " << response->body() << ", status: "
                          << response->getStatusCode() << std::endl;
            }
            else
            {
                std::cerr << "Request failed, request code: " << static_cast<int>(result)
                          << ", and status code: " << response->getStatusCode() << std::endl;
            }
        });
    }

    template<typename T>
    static void deleteRecord(const std::int32_t& id) noexcept
    {
        auto req = drogon::HttpRequest::newHttpRequest();
        req->setPath("/api/" + T::s_parentTableName + "/delete/id=" + std::to_string(id));
        req->setMethod(drogon::HttpMethod::Delete);
        req->addHeader("Authorization", "Bearer " + s_jwtToken);

        s_httpClient->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok &&
                response->getStatusCode() == drogon::HttpStatusCode::k200OK)
            {
                std::cout << "Response received: " << response->body() << ", status: " << response->getStatusCode()
                          << std::endl;
            }
            else
            {
                std::cerr << "Request failed, request code: " << static_cast<int>(result)
                          << ", and status code: " << response->getStatusCode() << std::endl;
            }
        });
    }

    template<typename T>
    [[nodiscard]] static std::future<T> getRecordByID(const std::int32_t& id) noexcept
    {
        auto req = drogon::HttpRequest::newHttpRequest();
        req->setPath("/api/" + T::s_parentTableName + "/get/id=" + std::to_string(id));
        req->setMethod(drogon::HttpMethod::Get);
        req->addHeader("Authorization", "Bearer " + s_jwtToken);

        auto valuePromise = std::make_shared<std::promise<T>>();
        auto valueFuture = valuePromise->get_future();

        s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response)  {
            if (result == drogon::ReqResult::Ok &&
                response->getStatusCode() == drogon::HttpStatusCode::k200OK)
            {
                T value;
                std::string deserLog;
                SGCore::Serde::Serializer::fromFormat((*response->getJsonObject())["value"].asString(), value, deserLog);
                std::cout << "Got record with id " << value.m_id
                          << " from table '" + T::s_parentTableName + "', status: "
                          << response->getStatusCode() << std::endl;

                valuePromise->set_value(value);
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

    template<typename T>
    [[nodiscard]] static std::future<std::vector<T>> getAllRecords() noexcept
    {
        auto req = drogon::HttpRequest::newHttpRequest();
        req->setPath("/api/" + T::s_parentTableName + "/get_all");
        req->setMethod(drogon::HttpMethod::Get);
        req->addHeader("Authorization", "Bearer " + s_jwtToken);

        auto valuePromise = std::make_shared<std::promise<std::vector<T>>>();
        auto valueFuture = valuePromise->get_future();

        s_httpClient->sendRequest(req, [valuePromise](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok &&
                response->getStatusCode() == drogon::HttpStatusCode::k200OK)
            {
                std::vector<T> values;
                std::string deserLog;
                SGCore::Serde::Serializer::fromFormat((*response->getJsonObject())["value"].asString(), values, deserLog);
                std::cout << "Got " << values.size() << " records from table '" + T::s_parentTableName + "', status: "
                          << response->getStatusCode() << std::endl;

                valuePromise->set_value(values);
            }
            else
            {
                std::cerr << "Request failed, request code: " << static_cast<int>(result)
                          << ", and status code: " << response->getStatusCode() << std::endl;
                valuePromise->set_value({});
            }
        });

        return valueFuture;
    }

    [[nodiscard]] static std::future<Worker> getWorkerByLogin(const std::string& login) noexcept;

    static void auth(const std::string& login, const std::string& password) noexcept;
    static std::future<bool> isAuthValid(const std::string& token) noexcept;

    static const std::string& getJWTToken() noexcept;

private:
    static inline SGCore::Ref<SGCore::Threading::Thread> s_clientThread;
    static inline std::shared_ptr<drogon::HttpClient> s_httpClient;

    static inline std::string s_jwtToken;
};

#endif //PROJ3_CLIENT_H
