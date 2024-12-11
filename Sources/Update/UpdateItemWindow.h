//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_UPDATEITEMWINDOW_H
#define PROJ3_UPDATEITEMWINDOW_H

#include "Models/Item.h"
#include "Models/Provider.h"
#include "Models/Storage.h"
#include "Models/ItemTypeInfo.h"

#include "UpdateRecordWindow.h"

struct UpdateItemWindow : public UpdateRecordWindow<Item>
{
    void renderBody() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    std::string m_selectedProviderName;
    std::string m_selectedItemTypeInfoName;
    std::string m_selectedStorageAddress;

    std::vector<Provider> m_tmpProviders;
    std::vector<ItemTypeInfo> m_tmpItemsTypeInfo;
    std::vector<Storage> m_tmpStorages;

    void submit() noexcept final;
};

#endif //PROJ3_UPDATEITEMWINDOW_H
