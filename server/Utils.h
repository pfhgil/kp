//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_UTILS_H
#define PROJ3_UTILS_H

#include <drogon/HttpFilter.h>
#include <jwt-cpp/jwt.h>
#include "Models/Worker.h"
#include "SerdeSpecs.h"

struct Utils
{
    using callback_t = std::function<void(const drogon::HttpResponsePtr&)>;

    template<typename T>
    static void sendResponse(const T& message, drogon::HttpStatusCode statusCode,
                             const callback_t& callback, const std::string& column = "message") noexcept
    {
        Json::Value jsonBody;

        jsonBody[column] = message;

        auto response = drogon::HttpResponse::newHttpJsonResponse(jsonBody);
        response->setStatusCode(statusCode);
        callback(response);
    }

    [[nodiscard]] static std::string generateJWT(const Worker& worker)
    {
        auto token = jwt::create()
                .set_issuer("auth_service") // источник токена
                .set_type("JWT")
                .set_subject(SGCore::Serde::Serializer::toFormat(worker))      // сохраняем пользователя
                .set_issued_at(std::chrono::system_clock::now())
                .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(1)) // срок действия ключа
                .sign(jwt::algorithm::hs256 { s_jwtSecretKey }); // подписываем токен секретнымключём
        return token;
    }

    static bool validateJWT(const std::string& token, Worker* worker)
    {
        try
        {
            jwt::verify()
                    .allow_algorithm(jwt::algorithm::hs256 { s_jwtSecretKey })
                    .with_issuer("auth_service")                   // проверяем источник
                    .verify(jwt::decode(token));

            std::string deserLog;
            if(worker)
            {
                // берём пользователя из токена
                SGCore::Serde::Serializer::fromFormat(jwt::decode(token).get_subject(), *worker, deserLog);
            }

            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "JWT validation error: " << e.what() << std::endl;
            return false;
        }
    }

    struct JWTAuthFilter : public drogon::HttpFilter<JWTAuthFilter>
    {
        void doFilter(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                      std::function<void()>&& next) override
        {
            std::string token = req->getHeader("Authorization");
            std::cout << "token: " << token << std::endl;

            if (token.rfind("Bearer ", 0) == 0)
            {
                token = token.substr(7);
            }

            Worker worker;

            if (validateJWT(token, &worker))
            {
                // req->attributes()->insert("username", username); // Сохраняем имя пользователя
                next(); // Передаем управление следующему обработчику
            }
            else
            {
                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::k401Unauthorized);
                resp->setBody("Unauthorized");
                callback(resp);
            }
        }
    };

private:
    static const inline std::string s_jwtSecretKey = "topchik";
};

#endif //PROJ3_UTILS_H
