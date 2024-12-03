//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_UPDATEWORKERWINDOW_H
#define PROJ3_UPDATEWORKERWINDOW_H

#include "SGE/Window.h"
#include "Models/Worker.h"
#include "Models/Storage.h"
#include "Roles.h"

struct UpdateWorkerWindow : public SGE::Window
{
    UpdateWorkerWindow() noexcept;

    void renderBody() noexcept final;
    void footerRender() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    void submit() noexcept;
    void cancel() noexcept;

    void drawRoleSelectable(Role role) noexcept;
    void drawStorageSelectable(const Storage& storage) noexcept;

    std::string m_errorMessage;

    std::vector<Storage> m_tmpStorages;
    Worker m_tmpWorker { };
};

#endif //PROJ3_UPDATEWORKERWINDOW_H
