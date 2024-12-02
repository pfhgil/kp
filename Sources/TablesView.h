//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_TABLESVIEW_H
#define PROJ3_TABLESVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "TableType.h"
#include "Models/Storage.h"
#include "Models/Worker.h"

struct TablesView : public SGCore::ImGuiWrap::IView
{
    TablesView() noexcept;

    void renderBody() noexcept final;

    TableType m_tableType = TableType::STAFF;

    std::vector<Worker> m_workers;
    std::vector<Storage> m_storages;

    void reloadTable(TableType tableType) noexcept;
    void reloadAllTables() noexcept;

    void deleteSelectedRows() noexcept;

private:
    std::string m_error;

    // every element in vector is map for every table type. in map: first - id of record
    std::vector<std::unordered_map<std::int32_t, bool>> m_selectedRows;

    void drawStaffTable() noexcept;
    void drawStoragesTable() noexcept;
};

#endif //PROJ3_TABLESVIEW_H