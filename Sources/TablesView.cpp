//
// Created by stuka on 01.12.2024.
//

#include "Models/Worker.h"
#include "TablesView.h"
#include "Models/Storage.h"
#include "Client.h"
#include <coroutine>

TablesView::TablesView() noexcept
{
    m_selectedRows.resize(static_cast<int>(TableType::COUNT));

    reloadAllTables();
}

void TablesView::renderBody() noexcept
{
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    m_error = "";

    ImGui::Begin("Table");

    switch(m_tableType)
    {
        case TableType::STAFF:
        {
            drawStaffTable();
            break;
        }
        case TableType::OFFS:break;
        case TableType::STORAGES:
        {
            drawStoragesTable();
            break;
        }
        case TableType::SHIPMENTS:break;
        case TableType::ITEM_TYPE_INFO:break;
        case TableType::ITEMS:break;
        case TableType::ORDERS:break;
        case TableType::PROVIDERS:break;
    }

    ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, m_error.c_str());

    ImGui::End();
}

void TablesView::reloadTable(TableType tableType) noexcept
{
    switch(tableType)
    {
        case TableType::STAFF:
        {
            m_workers = Client::getAllWorkers().get();
            break;
        }
        case TableType::OFFS:break;
        case TableType::STORAGES:
        {
            m_storages = Client::getAllStorages().get();
            break;
        }
        case TableType::SHIPMENTS:break;
        case TableType::ITEM_TYPE_INFO:break;
        case TableType::ITEMS:break;
        case TableType::ORDERS:break;
        case TableType::PROVIDERS:break;
    }
}

void TablesView::reloadAllTables() noexcept
{
    reloadTable(TableType::STAFF);
    reloadTable(TableType::OFFS);
    reloadTable(TableType::STORAGES);
    reloadTable(TableType::SHIPMENTS);
    reloadTable(TableType::ITEM_TYPE_INFO);
    reloadTable(TableType::ITEMS);
    reloadTable(TableType::ORDERS);
    reloadTable(TableType::PROVIDERS);
}

void TablesView::deleteSelectedRows() noexcept
{
    auto& tableSelectedRowsMapping = m_selectedRows[static_cast<int>(m_tableType)];
    switch (m_tableType)
    {
        case TableType::STAFF:
        {
            for(const auto& row : tableSelectedRowsMapping)
            {
                if(row.second) Client::deleteWorkerByID(row.first);
            }
            m_workers = Client::getAllWorkers().get();
            break;
        }
        case TableType::OFFS:
            break;
        case TableType::STORAGES:
        {
            for(const auto& row : tableSelectedRowsMapping)
            {
                if(row.second) Client::deleteStorageByID(row.first);
            }
            m_storages = Client::getAllStorages().get();
            break;
        }
        case TableType::SHIPMENTS:
            break;
        case TableType::ITEM_TYPE_INFO:
            break;
        case TableType::ITEMS:
            break;
        case TableType::ORDERS:
            break;
        case TableType::PROVIDERS:
            break;
    }

    tableSelectedRowsMapping.clear();
}

void TablesView::drawStaffTable() noexcept
{
    if(m_workers.empty())
    {
        m_error = "Table 'staff' is empty!";
        return;
    }

    if(ImGui::BeginTable("Staff", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Surname");
        ImGui::TableSetupColumn("Patronymic");
        ImGui::TableSetupColumn("Role");
        ImGui::TableSetupColumn("Storage ID");
        ImGui::TableSetupColumn("Login");
        ImGui::TableSetupColumn("Password");
        ImGui::TableHeadersRow();

        for(auto& record : m_workers)
        {
            ImGui::TableNextRow();
            // =====================================

            ImGui::TableNextColumn();

            ImGui::PushID(record.m_id);

            auto& currentSelectedRowsMap = m_selectedRows[static_cast<int>(m_tableType)];
            auto& isSelected = currentSelectedRowsMap[record.m_id];
            auto tmpIsSelected = isSelected;

            if(ImGui::Selectable(
                    "", &tmpIsSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                if (ImGui::IsKeyDown(ImGuiKey_ModShift))
                {
                    isSelected = true;
                }
                else if (ImGui::IsKeyDown(ImGuiKey_ModCtrl))
                {
                    isSelected = !isSelected;
                }
                else
                {
                    // оставляем только текущую строку
                    currentSelectedRowsMap = { };
                    currentSelectedRowsMap[record.m_id] = true;
                }
            }

            ImGui::SameLine();
            ImGui::Text(std::to_string(record.m_id).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_name.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_surname.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_patronymic.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_role.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(std::to_string(record.m_storageID).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_login.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_password.c_str());

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void TablesView::drawStoragesTable() noexcept
{
    if(m_storages.empty())
    {
        m_error = "Table 'storages' is empty!";
        return;
    }

    if(ImGui::BeginTable("Storages", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
    {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Address");
        ImGui::TableHeadersRow();

        for(auto& record : m_storages)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::PushID(record.m_id);

            auto& currentSelectedRowsMap = m_selectedRows[static_cast<int>(m_tableType)];
            auto& isSelected = currentSelectedRowsMap[record.m_id];
            auto tmpIsSelected = isSelected;

            if(ImGui::Selectable(
                    "", &tmpIsSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                if (ImGui::IsKeyDown(ImGuiKey_ModShift))
                {
                    isSelected = true;
                }
                else if (ImGui::IsKeyDown(ImGuiKey_ModCtrl))
                {
                    isSelected = !isSelected;
                }
                else
                {
                    // оставляем только текущую строку
                    currentSelectedRowsMap = { };
                    currentSelectedRowsMap[record.m_id] = true;
                }
            }

            ImGui::SameLine();
            ImGui::Text(std::to_string(record.m_id).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_address.c_str());

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

