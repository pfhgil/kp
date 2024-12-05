//
// Created by Ilya on 05.12.2024.
//

#include "UpdateStorageWindow.h"
#include "imgui_stdlib.h"
#include "Client.h"
#include "TableType.h"
#include "main.h"

void UpdateStorageWindow::renderBody() noexcept
{
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Address");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Address", &m_record.m_address);
        }

        ImGui::EndTable();
    }
}

void UpdateStorageWindow::submit() noexcept
{
    if(m_record.m_address.empty())
    {
        m_errorMessage = "Field 'name' should not be empty";
        return;
    }

    switch(getTableUpdateType())
    {
        case TableUpdateType::ADD:
            Client::addStorage(m_record);
            break;
        case TableUpdateType::UPDATE:
            Client::updateStorageByID(m_record.m_id, m_record);
            break;
    }

    m_record = { };

    Main::getMainView()->getTablesView()->reloadTable(TableType::STORAGES);

    m_errorMessage = "";
    setActive(false);
}
