//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_UPDATEORDERWINDOW_H
#define PROJ3_UPDATEORDERWINDOW_H

#include "Models/Order.h"
#include "Models/Provider.h"
#include "Models/ItemTypeInfo.h"

#include "UpdateRecordWindow.h"

struct UpdateOrderWindow : public UpdateRecordWindow<Order>
{
    void renderBody() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    std::string m_selectedProviderName;
    std::string m_selectedItemTypeInfoName;

    std::vector<Provider> m_tmpProviders;
    std::vector<ItemTypeInfo> m_tmpItemTypeInfo;

    void submit() noexcept final;
};

#endif //PROJ3_UPDATEORDERWINDOW_H
