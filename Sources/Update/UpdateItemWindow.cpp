//
// Created by Ilya on 11.12.2024.
//

#include "UpdateItemWindow.h"

#include "Client.h"
#include "main.h"
#include "TableType.h"

void UpdateItemWindow::renderBody() noexcept
{
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Provider");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##ItemProvider", m_selectedProviderName.c_str()))
            {
                for(const auto& provider : m_tmpProviders)
                {
                    if (ImGui::Selectable(provider.name.c_str(),
                                          m_selectedProviderName == provider.name))
                    {
                        m_record.provider_id = provider.id;
                        m_selectedProviderName = provider.name;
                    }

                    if (m_selectedProviderName == provider.name)
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
            ImGui::Text("Item Type Info");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##Item_ItemTypeInfo", m_selectedItemTypeInfoName.c_str()))
            {
                for(const auto& itemTypeInfo : m_tmpItemsTypeInfo)
                {
                    if (ImGui::Selectable(itemTypeInfo.name.c_str(),
                                          m_selectedItemTypeInfoName == itemTypeInfo.name))
                    {
                        m_record.type_info_id = itemTypeInfo.id;
                        m_selectedItemTypeInfoName = itemTypeInfo.name;
                    }

                    if (m_selectedItemTypeInfoName == itemTypeInfo.name)
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
            ImGui::Text("Storage");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##ProviderStorage", m_selectedStorageAddress.c_str()))
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

        ImGui::EndTable();
    }
}

void UpdateItemWindow::onActiveChangedListener() noexcept
{
    UpdateRecordWindow::onActiveChangedListener();

    if(isActive())
    {
        m_selectedProviderName = Client::getRecordByID<Provider>(m_record.provider_id).get().name;
        m_selectedItemTypeInfoName = Client::getRecordByID<ItemTypeInfo>(m_record.type_info_id).get().name;
        m_selectedStorageAddress = Client::getRecordByID<Storage>(m_record.storage_id).get().address;

        m_tmpProviders = Client::getAllRecords<Provider>().get();
        m_tmpItemsTypeInfo = Client::getAllRecords<ItemTypeInfo>().get();
        m_tmpStorages = Client::getAllRecords<Storage>().get();
    }
    else
    {
        m_selectedProviderName = "";
        m_selectedItemTypeInfoName = "";
        m_selectedStorageAddress = "";
    }
}

void UpdateItemWindow::submit() noexcept
{
    if(m_selectedProviderName.empty())
    {
        m_errorMessage = "Please, select provider!";
        return;
    }

    if(m_selectedItemTypeInfoName.empty())
    {
        m_errorMessage = "Please, select item type info!";
        return;
    }

    if(m_selectedStorageAddress.empty())
    {
        m_errorMessage = "Please, select storage!";
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

    Main::getMainView()->getTablesView()->reloadTable(TableType::ITEMS);

    m_errorMessage = "";
    setActive(false);
}
