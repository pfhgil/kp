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
    [[nodiscard]] static std::future<std::vector<Worker>> getAllWorkers() noexcept;

    static void addStorage(const Storage& value) noexcept;
    [[nodiscard]] static std::future<std::vector<Storage>> getAllStorages() noexcept;

private:
    static inline SGCore::Ref<SGCore::Threading::Thread> s_clientThread;
    static inline std::shared_ptr<drogon::HttpClient> s_httpClient;
};

#endif //PROJ3_CLIENT_H
