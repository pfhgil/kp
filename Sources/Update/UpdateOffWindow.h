//
// Created by Ilya on 10.12.2024.
//

#ifndef PROJ3_UPDATEOFFWINDOW_H
#define PROJ3_UPDATEOFFWINDOW_H

#include "UpdateRecordWindow.h"
#include "Models/Offs.h"
#include "Models/Worker.h"
#include "Models/Storage.h"
#include "Models/Item.h"
#include "Models/ItemTypeInfo.h"

struct UpdateOffWindow : public UpdateRecordWindow<Offs>
{
    void renderBody() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    std::string m_selectedWorkerLogin;
    std::string m_selectedStorageAddress;

    std::vector<Item> m_tmpItems;
    // first - id
    std::unordered_map<std::int32_t, ItemTypeInfo> m_tmpItemsTypeInfo;
    std::vector<Worker> m_tmpWorkers;
    std::vector<Storage> m_tmpStorages;

    void submit() noexcept final;
};

#endif //PROJ3_UPDATEOFFWINDOW_H
