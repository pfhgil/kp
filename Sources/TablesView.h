//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_TABLESVIEW_H
#define PROJ3_TABLESVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "TableType.h"
#include "Models/Storage.h"
#include "Models/Worker.h"
#include "Update/UpdateWorkerWindow.h"
#include "Update/UpdateStorageWindow.h"
#include "SGE/Popup.h"
#include "Models/ItemTypeInfo.h"

struct TablesView : public SGCore::ImGuiWrap::IView
{
    TablesView() noexcept;

    void renderBody() noexcept final;

    TableType m_tableType = TableType::STAFF;

    std::vector<Worker> m_workers;
    std::vector<Storage> m_storages;
    std::vector<ItemTypeInfo> m_itemsTypeInfo;

    void reloadTable(TableType tableType) noexcept;
    void reloadAllTables() noexcept;

    void deleteSelectedRows() noexcept;

    [[nodiscard]] auto getUpdateWorkerWindow() const noexcept
    {
        return m_updateWorkerWindow;
    }

    [[nodiscard]] auto getUpdateStorageWindow() const noexcept
    {
        return m_updateStorageWindow;
    }

private:
    std::string m_error;

    // every element in vector is map for every table type. in map: first - id of record
    std::vector<std::unordered_map<std::int32_t, bool>> m_selectedRows;

    std::int32_t m_rightClickedRowID = -1;
    bool m_openPopup = false;

    SGCore::Ref<UpdateWorkerWindow> m_updateWorkerWindow;
    SGCore::Ref<UpdateStorageWindow> m_updateStorageWindow;

    SGE::Popup m_rowPopup;

    void drawStaffTable() noexcept;
    void drawStoragesTable() noexcept;
    void drawItemsTypeInfoTable() noexcept;

    template<typename T>
    void sortTable(std::vector<T>& collection)
    {
        if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
        {
            if (sort_specs->SpecsDirty)
            {
                // Данные сортировки изменились, необходимо пересортировать
                const ImGuiTableColumnSortSpecs* sort_column = sort_specs->Specs;

                // Пример сортировки данных:
                if (sort_column)
                {
                    std::sort(collection.begin(), collection.end(), [sort_column](const T& a, const T& b) {
                        auto values = SortingSpecs<T>::s_sortingFunctions[sort_column->ColumnIndex](a, b);

                        if (sort_column->SortDirection == ImGuiSortDirection_Ascending)
                        {
                            return values.first < values.second;
                        }
                        else
                        {
                            return values.first > values.second;
                        }
                    });

                    sort_specs->SpecsDirty = false;
                }
            }
        }
    }

    void initializeSortingSpecsForWorkers() const noexcept;
    void initializeSortingSpecsForStorages() const noexcept;
    void initializeSortingSpecsForItemsTypeInfo() const noexcept;

    template<typename T>
    struct SortingSpecs
    {
        static inline std::unordered_map<std::uint16_t, std::function<std::pair<std::int64_t, std::int64_t>(const T& t0, const T& t1)>> s_sortingFunctions;
    };
};

#endif //PROJ3_TABLESVIEW_H
