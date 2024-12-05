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
    m_rowPopup = SGE::Popup {
            "TableRowActions",
            {
                    {
                            .m_text = "Edit Row",
                            .m_ID = "EditRow",
                            .m_icon = SGE::StylesManager::getCurrentStyle()->m_pencilIcon->getSpecialization(18,
                                                                                                             18)->getTexture()
                    }
            }
    };

    m_rowPopup.onElementClicked += [this](const SGCore::Ref<SGE::PopupElement>& elem) {
        if(elem->m_ID == "EditRow")
        {
            switch(m_tableType)
            {
                case TableType::STAFF:
                {
                    auto editableWorker = Client::getWorkerByID(m_rightClickedRowID).get();
                    m_updateWorkerWindow->setTableUpdateType(TableUpdateType::UPDATE);
                    m_updateWorkerWindow->m_record = editableWorker;
                    m_updateWorkerWindow->setActive(true);

                    break;
                }
                case TableType::OFFS:
                    break;
                case TableType::STORAGES:
                {
                    auto editableStorage = Client::getStorageByID(m_rightClickedRowID).get();
                    m_updateStorageWindow->setTableUpdateType(TableUpdateType::UPDATE);
                    m_updateStorageWindow->m_record = editableStorage;
                    m_updateStorageWindow->setActive(true);

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
        }
    };

    m_updateWorkerWindow = SGCore::MakeRef<UpdateWorkerWindow>();
    m_updateWorkerWindow->setActive(false);

    addChild(m_updateWorkerWindow);

    m_updateStorageWindow = SGCore::MakeRef<UpdateStorageWindow>();
    m_updateStorageWindow->setActive(false);

    addChild(m_updateStorageWindow);

    m_selectedRows.resize(static_cast<int>(TableType::COUNT));

    reloadAllTables();

    initializeSortingSpecsForWorkers();
    initializeSortingSpecsForStorages();
}

void TablesView::renderBody() noexcept
{
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    if(m_openPopup)
    {
        m_rowPopup.setOpened(true);
        m_openPopup = false;
    }

    m_rowPopup.draw();

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

    if(ImGui::BeginTable("Staff", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
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

            if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                m_openPopup = true;
                m_rightClickedRowID = record.m_id;
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
            ImGui::Text(roleToString(record.m_role).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(std::to_string(record.m_storageID).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_login.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_password.c_str());

            ImGui::PopID();

            sortTable(m_workers);
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

            ImGui::PushID(("StoragesRow" + std::to_string(record.m_id)).c_str());

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

            if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                m_openPopup = true;
                m_rightClickedRowID = record.m_id;
            }

            ImGui::SameLine();
            ImGui::Text(std::to_string(record.m_id).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_address.c_str());

            ImGui::PopID();

            sortTable(m_storages);
        }

        ImGui::EndTable();
    }
}

void TablesView::initializeSortingSpecsForWorkers() const noexcept
{
    SortingSpecs<Worker>::s_sortingFunctions[0] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_id, t1.m_id };
    };

    SortingSpecs<Worker>::s_sortingFunctions[1] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_name.size(), t1.m_name.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[2] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_surname.size(), t1.m_surname.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[3] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_patronymic.size(), t1.m_patronymic.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[4] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { static_cast<std::int32_t>(t0.m_role), static_cast<std::int32_t>(t1.m_role) };
    };

    SortingSpecs<Worker>::s_sortingFunctions[5] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_storageID, t1.m_storageID };
    };

    SortingSpecs<Worker>::s_sortingFunctions[6] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_login.size(), t1.m_login.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[7] = [](const Worker& t0, const Worker& t1) noexcept {
        return std::pair { t0.m_password.size(), t1.m_password.size() };
    };
}

void TablesView::initializeSortingSpecsForStorages() const noexcept
{
    SortingSpecs<Storage>::s_sortingFunctions[0] = [](const Storage& t0, const Storage& t1) noexcept {
        return std::pair { t0.m_id, t1.m_id };
    };

    SortingSpecs<Storage>::s_sortingFunctions[1] = [](const Storage& t0, const Storage& t1) noexcept {
        return std::pair { t0.m_address.size(), t1.m_address.size() };
    };
}

