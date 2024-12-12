//
// Created by Ilya on 12.12.2024.
//

#ifndef PROJ3_UPDATESHIPMENTWINDOW_H
#define PROJ3_UPDATESHIPMENTWINDOW_H

#include "Models/Shipment.h"
#include "Models/ItemTypeInfo.h"
#include "Models/Worker.h"
#include "Models/Storage.h"
#include "Models/Order.h"
#include "UpdateRecordWindow.h"

struct UpdateShipmentWindow : public UpdateRecordWindow<Shipment>
{
    void renderBody() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    std::string m_selectedStorageAddress;
    std::string m_selectedWorkerLogin;
    std::string m_selectedOrderName;

    // first - id
    std::unordered_map<std::int32_t, ItemTypeInfo> m_tmpItemsTypeInfo;
    std::vector<Worker> m_tmpWorkers;
    std::vector<Storage> m_tmpStorages;
    std::vector<Order> m_tmpOrders;

    void submit() noexcept final;
};

#endif //PROJ3_UPDATESHIPMENTWINDOW_H
