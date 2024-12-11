//
// Created by Ilya on 11.12.2024.
//

#include "UpdateProviderWindow.h"
#include "Client.h"
#include "main.h"
#include "TableType.h"

#include <imgui_stdlib.h>

void UpdateProviderWindow::renderBody() noexcept
{
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Name");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##ProviderName", &m_record.name);
        }

        ImGui::EndTable();
    }
}

void UpdateProviderWindow::submit() noexcept
{
    if(m_record.name.empty())
    {
        m_errorMessage = "Name should not be empty!";
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

    Main::getMainView()->getTablesView()->reloadTable(TableType::PROVIDERS);

    m_errorMessage = "";
    setActive(false);
}
