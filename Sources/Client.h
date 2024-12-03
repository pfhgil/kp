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

struct Client
{
    static void start() noexcept;

    static void addWorker(const Worker& value) noexcept;
    static void deleteWorkerByID(const std::int32_t& id) noexcept;
    [[nodiscard]] static std::future<std::vector<Worker>> getAllWorkers() noexcept;
    [[nodiscard]] static std::future<Worker> getWorkerByLogin(const std::string& login) noexcept;

    static void addStorage(const Storage& value) noexcept;
    static void deleteStorageByID(const std::int32_t& id) noexcept;
    static std::future<Storage> getStorageByID(const std::int32_t& id) noexcept;
    [[nodiscard]] static std::future<std::vector<Storage>> getAllStorages() noexcept;

    static void auth(const std::string& login, const std::string& password) noexcept;
    static std::future<bool> isAuthValid(const std::string& token) noexcept;

    static const std::string& getJWTToken() noexcept;

private:
    static inline SGCore::Ref<SGCore::Threading::Thread> s_clientThread;
    static inline std::shared_ptr<drogon::HttpClient> s_httpClient;

    static inline std::string s_jwtToken;
};

#endif //PROJ3_CLIENT_H
