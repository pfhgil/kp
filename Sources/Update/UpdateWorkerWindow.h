//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_UPDATEWORKERWINDOW_H
#define PROJ3_UPDATEWORKERWINDOW_H

#include "SGE/Window.h"
#include "Models/Worker.h"
#include "Models/Storage.h"
#include "WorkerRole.h"
#include "TableUpdateType.h"
#include "UpdateRecordWindow.h"

struct UpdateWorkerWindow : public UpdateRecordWindow<Worker>
{
    void renderBody() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    void submit() noexcept final;

    void drawRoleSelectable(WorkerRole role) noexcept;
    void drawStorageSelectable(const Storage& storage) noexcept;

    std::vector<Storage> m_tmpStorages;
};

#endif //PROJ3_UPDATEWORKERWINDOW_H
