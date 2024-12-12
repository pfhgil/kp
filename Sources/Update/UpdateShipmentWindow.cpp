//
// Created by Ilya on 12.12.2024.
//

#include "UpdateShipmentWindow.h"
#include "Client.h"
#include "TableType.h"
#include "main.h"

void UpdateShipmentWindow::renderBody() noexcept
{
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Storage");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##ShipmentStorage", m_selectedStorageAddress.c_str()))
            {
                for(const auto& storage : m_tmpStorages)
                {
                    if (ImGui::Selectable(storage.address.c_str(),
                                          m_selectedStorageAddress == storage.address))
                    {
                        m_record.storage_id = storage.id;
                        m_selectedStorageAddress = storage.address;
                    }

                    if (m_selectedStorageAddress == storage.address)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Worker");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##ShipmentWorker", m_selectedWorkerLogin.c_str()))
            {
                for(const auto& worker : m_tmpWorkers)
                {
                    if (ImGui::Selectable(worker.login.c_str(),
                                          m_selectedWorkerLogin == worker.login))
                    {
                        m_record.worker_id = worker.id;
                        m_selectedWorkerLogin = worker.login;
                    }

                    if (m_selectedWorkerLogin == worker.login)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Order");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::BeginCombo("##ShipmentOrder", m_selectedOrderName.c_str()))
            {
                for (const auto& order : m_tmpOrders)
                {
                    const auto& itemTypeInfo = m_tmpItemsTypeInfo[order.item_type_info_id];

                    if (ImGui::Selectable(fmt::format("ID: {}, Item Type: '{}', Count: {}, Cost: {}",
                                                      order.id,
                                                      itemTypeInfo.name,
                                                      itemTypeInfo.count,
                                                      order.cost).c_str(), m_record.order_id == order.id))
                    {
                        m_selectedOrderName = fmt::format("ID: {}, Item Type: '{}', Count: {}, Cost: {}",
                                                          order.id,
                                                          itemTypeInfo.name,
                                                          itemTypeInfo.count,
                                                          order.cost);
                        m_record.order_id = order.id;
                    }

                    if (m_record.order_id == order.id)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        ImGui::EndTable();
    }
}

void UpdateShipmentWindow::onActiveChangedListener() noexcept
{
    UpdateRecordWindow::onActiveChangedListener();

    m_tmpItemsTypeInfo = { };

    if(isActive())
    {
        m_selectedWorkerLogin = Client::getRecordByID<Worker>(m_record.worker_id).get().login;
        m_selectedStorageAddress = Client::getRecordByID<Storage>(m_record.storage_id).get().address;

        auto itemsTypeInfo = Client::getAllRecords<ItemTypeInfo>().get();
        for(auto&& itemTypeInfo : itemsTypeInfo)
        {
            m_tmpItemsTypeInfo[itemTypeInfo.id] = std::move(itemTypeInfo);
        }
        m_tmpWorkers = Client::getAllRecords<Worker>().get();
        m_tmpStorages = Client::getAllRecords<Storage>().get();
        m_tmpOrders = Client::getAllRecords<Order>().get();

        if (m_record.order_id != -1)
        {
            auto selectedOrder = Client::getRecordByID<Order>(m_record.order_id).get();
            const auto& itemTypeInfo = m_tmpItemsTypeInfo[selectedOrder.item_type_info_id];


            m_selectedOrderName = fmt::format("ID: {}, Item Type: '{}', Count: {}, Cost: {}",
                                              selectedOrder.id,
                                              itemTypeInfo.name,
                                              itemTypeInfo.count,
                                              selectedOrder.cost);
        }
    }
    else
    {
        m_selectedWorkerLogin = "";
        m_selectedStorageAddress = "";
        m_selectedOrderName = "";
    }
}

void UpdateShipmentWindow::submit() noexcept
{
    if(m_selectedStorageAddress.empty())
    {
        m_errorMessage = "Please, select storage!";
        return;
    }

    if(m_selectedWorkerLogin.empty())
    {
        m_errorMessage = "Please, select worker!";
        return;
    }

    if(m_selectedOrderName.empty())
    {
        m_errorMessage = "Please, select order!";
        return;
    }

    switch(getTableUpdateType())
    {
        case TableUpdateType::ADD:
            Client::addRecord(m_record);
            break;
        case TableUpdateType::UPDATE:
            Client::updateRecord(m_record.id, m_record);
            break;
    }

    m_record = { };

    Main::getMainView()->getTablesView()->reloadTable(TableType::SHIPMENTS);

    m_errorMessage = "";
    setActive(false);
}
