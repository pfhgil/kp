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
                    auto editableWorker = Client::getRecordByID<Worker>(m_rightClickedRowID).get();
                    m_updateWorkerWindow->setTableUpdateType(TableUpdateType::UPDATE);
                    m_updateWorkerWindow->m_record = editableWorker;
                    m_updateWorkerWindow->setActive(true);

                    break;
                }
                case TableType::OFFS:
                    break;
                case TableType::STORAGES:
                {
                    auto editableStorage = Client::getRecordByID<Storage>(m_rightClickedRowID).get();
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
    initializeSortingSpecsForItemsTypeInfo();
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

    ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, m_error.c_str());

    ImGui::BeginChildFrame(ImGui::GetID("TableChild"), ImGui::GetContentRegionAvail());

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
        case TableType::ITEM_TYPE_INFO:
        {
            drawItemsTypeInfoTable();
            break;
        }
        case TableType::ITEMS:break;
        case TableType::ORDERS:break;
        case TableType::PROVIDERS:break;
    }

    ImGui::EndChildFrame();

    ImGui::End();
}

void TablesView::reloadTable(TableType tableType) noexcept
{
    switch(tableType)
    {
        case TableType::STAFF:
        {
            m_workers = Client::getAllRecords<Worker>().get();
            break;
        }
        case TableType::OFFS:break;
        case TableType::STORAGES:
        {
            m_storages = Client::getAllRecords<Storage>().get();
            break;
        }
        case TableType::SHIPMENTS:break;
        case TableType::ITEM_TYPE_INFO:
        {
            m_itemsTypeInfo = Client::getAllRecords<ItemTypeInfo>().get();
            break;
        }
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
                if(row.second) Client::deleteRecord<Worker>(row.first);
            }

            m_workers = Client::getAllRecords<Worker>().get();
            break;
        }
        case TableType::OFFS:
            break;
        case TableType::STORAGES:
        {
            for(const auto& row : tableSelectedRowsMapping)
            {
                if(row.second) Client::deleteRecord<Storage>(row.first);
            }

            m_storages = Client::getAllRecords<Storage>().get();
            break;
        }
        case TableType::SHIPMENTS:
            break;
        case TableType::ITEM_TYPE_INFO:
        {
            for(const auto& row : tableSelectedRowsMapping)
            {
                if(row.second) Client::deleteRecord<ItemTypeInfo>(row.first);
            }

            m_itemsTypeInfo = Client::getAllRecords<ItemTypeInfo>().get();
            break;
        }
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

void TablesView::drawItemsTypeInfoTable() noexcept
{
    if(m_storages.empty())
    {
        m_error = "Table 'item_type_info' is empty!";
        return;
    }

    if(ImGui::BeginTable("ItemsTypeInfo", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
    {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Count");
        ImGui::TableSetupColumn("Date Of Receipt");
        ImGui::TableSetupColumn("Expiration Date");
        ImGui::TableSetupColumn("Production Date");
        ImGui::TableHeadersRow();

        for(auto& record : m_itemsTypeInfo)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::PushID(("ItemTypeInfoRow" + std::to_string(record.m_id)).c_str());

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
            ImGui::Text(std::to_string(record.m_count).c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_dateOfReceipt.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_expirationDate.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(record.m_productionDate.c_str());

            ImGui::PopID();

            sortTable(m_itemsTypeInfo);
        }

        ImGui::EndTable();
    }
}

void TablesView::initializeSortingSpecsForWorkers() const noexcept
{
    SortingSpecs<Worker>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_id, t1.m_id };
    };

    SortingSpecs<Worker>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_name.size(), t1.m_name.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[2] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_surname.size(), t1.m_surname.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[3] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_patronymic.size(), t1.m_patronymic.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[4] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { static_cast<std::int32_t>(t0.m_role), static_cast<std::int32_t>(t1.m_role) };
    };

    SortingSpecs<Worker>::s_sortingFunctions[5] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_storageID, t1.m_storageID };
    };

    SortingSpecs<Worker>::s_sortingFunctions[6] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_login.size(), t1.m_login.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[7] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_password.size(), t1.m_password.size() };
    };
}

void TablesView::initializeSortingSpecsForStorages() const noexcept
{
    SortingSpecs<Storage>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_id, t1.m_id };
    };

    SortingSpecs<Storage>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_address.size(), t1.m_address.size() };
    };
}

void TablesView::initializeSortingSpecsForItemsTypeInfo() const noexcept
{
    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_id, t1.m_id };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_name.size(), t1.m_name.size() };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[2] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.m_count, t1.m_count };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[3] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { SGCore::Utils::getStringAsTime(t0.m_dateOfReceipt).time_since_epoch().count(),
                           SGCore::Utils::getStringAsTime(t1.m_dateOfReceipt).time_since_epoch().count() };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[4] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { SGCore::Utils::getStringAsTime(t0.m_expirationDate).time_since_epoch().count(),
                           SGCore::Utils::getStringAsTime(t1.m_expirationDate).time_since_epoch().count() };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[5] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { SGCore::Utils::getStringAsTime(t0.m_productionDate).time_since_epoch().count(),
                           SGCore::Utils::getStringAsTime(t1.m_productionDate).time_since_epoch().count() };
    };
}

