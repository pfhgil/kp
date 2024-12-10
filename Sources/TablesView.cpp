//
// Created by stuka on 01.12.2024.
//

#include "TablesView.h"
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
                {
                    auto editableOffTypeInfo = Client::getRecordByID<Offs>(m_rightClickedRowID).get();
                    m_updateOffWindow->setTableUpdateType(TableUpdateType::UPDATE);
                    m_updateOffWindow->m_record = editableOffTypeInfo;
                    m_updateOffWindow->setActive(true);

                    break;
                }
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
                {
                    auto editableItemTypeInfo = Client::getRecordByID<ItemTypeInfo>(m_rightClickedRowID).get();
                    m_updateItemTypeInfoWindow->setTableUpdateType(TableUpdateType::UPDATE);
                    m_updateItemTypeInfoWindow->m_record = editableItemTypeInfo;
                    m_updateItemTypeInfoWindow->setActive(true);

                    break;
                }
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

    m_updateItemTypeInfoWindow = SGCore::MakeRef<UpdateItemTypeInfoWindow>();
    m_updateItemTypeInfoWindow->setActive(false);

    addChild(m_updateItemTypeInfoWindow);

    m_updateOffWindow = SGCore::MakeRef<UpdateOffWindow>();
    m_updateOffWindow->setActive(false);

    addChild(m_updateOffWindow);

    m_selectedRows.resize(static_cast<int>(TableType::COUNT));

    reloadAllTables();

    initializeSortingSpecsForWorkers();
    initializeSortingSpecsForStorages();
    initializeSortingSpecsForItemsTypeInfo();
    initializeSortingSpecsForOffs();
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
            drawTable(m_workers);
            break;
        }
        case TableType::OFFS:
        {
            drawTable(m_offs);
            break;
        }
        case TableType::STORAGES:
        {
            drawTable(m_storages);
            break;
        }
        case TableType::SHIPMENTS:break;
        case TableType::ITEM_TYPE_INFO:
        {
            drawTable(m_itemsTypeInfo);
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
        case TableType::OFFS:
        {
            m_offs = Client::getAllRecords<Offs>().get();
            break;
        }
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
        {
            for (const auto& row: tableSelectedRowsMapping)
            {
                if (row.second) Client::deleteRecord<Offs>(row.first);
            }

            m_offs = Client::getAllRecords<Offs>().get();
            break;
        }
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

void TablesView::initializeSortingSpecsForWorkers() const noexcept
{
    SortingSpecs<Worker>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.id, t1.id };
    };

    SortingSpecs<Worker>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.name.size(), t1.name.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[2] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.surname.size(), t1.surname.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[3] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.patronymic.size(), t1.patronymic.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[4] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { static_cast<std::int32_t>(t0.role), static_cast<std::int32_t>(t1.role) };
    };

    SortingSpecs<Worker>::s_sortingFunctions[5] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.storage_id, t1.storage_id };
    };

    SortingSpecs<Worker>::s_sortingFunctions[6] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.login.size(), t1.login.size() };
    };

    SortingSpecs<Worker>::s_sortingFunctions[7] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.password.size(), t1.password.size() };
    };
}

void TablesView::initializeSortingSpecsForStorages() const noexcept
{
    SortingSpecs<Storage>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.id, t1.id };
    };

    SortingSpecs<Storage>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.address.size(), t1.address.size() };
    };
}

void TablesView::initializeSortingSpecsForItemsTypeInfo() const noexcept
{
    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.id, t1.id };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.name.size(), t1.name.size() };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[2] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.count, t1.count };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[3] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { SGCore::Utils::getStringAsTime(t0.date_of_receipt).time_since_epoch().count(),
                           SGCore::Utils::getStringAsTime(t1.date_of_receipt).time_since_epoch().count() };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[4] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { SGCore::Utils::getStringAsTime(t0.expiration_date).time_since_epoch().count(),
                           SGCore::Utils::getStringAsTime(t1.expiration_date).time_since_epoch().count() };
    };

    SortingSpecs<ItemTypeInfo>::s_sortingFunctions[5] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { SGCore::Utils::getStringAsTime(t0.production_date).time_since_epoch().count(),
                           SGCore::Utils::getStringAsTime(t1.production_date).time_since_epoch().count() };
    };
}

void TablesView::initializeSortingSpecsForOffs() const noexcept
{
    SortingSpecs<Offs>::s_sortingFunctions[0] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.id, t1.id };
    };

    SortingSpecs<Offs>::s_sortingFunctions[1] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.item_id, t1.item_id };
    };

    SortingSpecs<Offs>::s_sortingFunctions[2] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.worker_id, t1.worker_id };
    };

    SortingSpecs<Offs>::s_sortingFunctions[3] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.reason.size(), t1.reason.size() };
    };

    SortingSpecs<Offs>::s_sortingFunctions[4] = [](const auto& t0, const auto& t1) noexcept {
        return std::pair { t0.storage_id, t1.storage_id };
    };
}

