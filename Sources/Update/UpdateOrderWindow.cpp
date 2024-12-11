//
// Created by Ilya on 11.12.2024.
//

#include "UpdateOrderWindow.h"
#include "Client.h"
#include "main.h"
#include "TableType.h"

void UpdateOrderWindow::renderBody() noexcept
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
            if(ImGui::BeginCombo("##OrderProvider", m_selectedProviderName.c_str()))
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
            if(ImGui::BeginCombo("##OrdeItemTypeInfo", m_selectedItemTypeInfoName.c_str()))
            {
                for(const auto& itemTypeInfo : m_tmpItemTypeInfo)
                {
                    if (ImGui::Selectable(itemTypeInfo.name.c_str(),
                                          m_selectedItemTypeInfoName == itemTypeInfo.name))
                    {
                        m_record.item_type_info_id = itemTypeInfo.id;
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
            ImGui::Text("Ordered Count");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::InputInt("##OrderedCount", &m_record.ordered_count))
            {
                m_record.ordered_count = std::max(m_record.ordered_count, 0);
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Cost");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::DragFloat("##OrderCost", &m_record.cost, 1.0f, 0.0f, std::numeric_limits<float>::max());
        }

        ImGui::EndTable();
    }
}

void UpdateOrderWindow::onActiveChangedListener() noexcept
{
    UpdateRecordWindow::onActiveChangedListener();

    if(isActive())
    {
        m_selectedProviderName = Client::getRecordByID<Provider>(m_record.provider_id).get().name;
        m_selectedItemTypeInfoName = Client::getRecordByID<ItemTypeInfo>(m_record.item_type_info_id).get().name;

        m_tmpProviders = Client::getAllRecords<Provider>().get();
        m_tmpItemTypeInfo = Client::getAllRecords<ItemTypeInfo>().get();
    }
    else
    {
        m_selectedProviderName = "";
        m_selectedItemTypeInfoName = "";
    }
}

void UpdateOrderWindow::submit() noexcept
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

    Main::getMainView()->getTablesView()->reloadTable(TableType::ORDERS);

    m_errorMessage = "";
    setActive(false);
}
