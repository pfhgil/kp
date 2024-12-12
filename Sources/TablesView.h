//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_TABLESVIEW_H
#define PROJ3_TABLESVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "TableType.h"

#include "Models/Storage.h"
#include "Models/Worker.h"
#include "Models/Offs.h"
#include "Models/Provider.h"
#include "Models/ItemTypeInfo.h"
#include "Models/Item.h"
#include "Models/Order.h"

#include "Update/UpdateWorkerWindow.h"
#include "Update/UpdateStorageWindow.h"
#include "Update/UpdateItemTypeInfoWindow.h"
#include "Update/UpdateOffWindow.h"
#include "Update/UpdateProviderWindow.h"
#include "Update/UpdateOrderWindow.h"
#include "Update/UpdateShipmentWindow.h"

#include "SGE/Popup.h"
#include "Reflection/Reflection.h"
#include "Update/UpdateItemWindow.h"

struct TablesView : public SGCore::ImGuiWrap::IView
{
    TablesView() noexcept;

    void renderBody() noexcept final;

    TableType m_tableType = TableType::STAFF;

    std::vector<Worker> m_workers;
    std::vector<Storage> m_storages;
    std::vector<ItemTypeInfo> m_itemsTypeInfo;
    std::vector<Offs> m_offs;
    std::vector<Provider> m_providers;
    std::vector<Item> m_items;
    std::vector<Order> m_orders;
    std::vector<Shipment> m_shipments;

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

    [[nodiscard]] auto getUpdateItemTypeInfoWindow() const noexcept
    {
        return m_updateItemTypeInfoWindow;
    }

    [[nodiscard]] auto getUpdateOffWindow() const noexcept
    {
        return m_updateOffWindow;
    }

    [[nodiscard]] auto getUpdateProviderWindow() const noexcept
    {
        return m_updateProviderWindow;
    }

    [[nodiscard]] auto getUpdateItemWindow() const noexcept
    {
        return m_updateItemWindow;
    }

    [[nodiscard]] auto getUpdateOrderWindow() const noexcept
    {
        return m_updateOrderWindow;
    }

    [[nodiscard]] auto getUpdateShipmentWindow() const noexcept
    {
        return m_updateShipmentWindow;
    }

private:
    std::string m_error;

    // every element in vector is map for every table type. in map: first - id of record
    std::vector<std::unordered_map<std::int32_t, bool>> m_selectedRows;

    std::int32_t m_rightClickedRowID = -1;
    bool m_openPopup = false;

    SGCore::Ref<UpdateWorkerWindow> m_updateWorkerWindow;
    SGCore::Ref<UpdateStorageWindow> m_updateStorageWindow;
    SGCore::Ref<UpdateItemTypeInfoWindow> m_updateItemTypeInfoWindow;
    SGCore::Ref<UpdateOffWindow> m_updateOffWindow;
    SGCore::Ref<UpdateProviderWindow> m_updateProviderWindow;
    SGCore::Ref<UpdateItemWindow> m_updateItemWindow;
    SGCore::Ref<UpdateOrderWindow> m_updateOrderWindow;
    SGCore::Ref<UpdateShipmentWindow> m_updateShipmentWindow;

    SGE::Popup m_rowPopup;

    template<typename T>
    requires(requires { T::id; } && std::is_integral_v<decltype(T::id)> &&
    std::remove_reference_t<decltype(makeMetaInfo<const T>(T { }).template get<0>())>::unmangled_name == "id")
    void drawTable(std::vector<T>& records) noexcept
    {
        if(records.empty())
        {
            m_error = "Table '" + T::s_parentTableName + "' is empty!";
            return;
        }

        auto exampleMeta = makeMetaInfo(records[0]);

        if(ImGui::BeginTable(T::s_parentTableName.c_str(), exampleMeta.members_count,
                             ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
        {
            exampleMeta.iterateThroughMembers([](auto memberInfo) {
                ImGui::TableSetupColumn(std::string { memberInfo.unmangled_name }.c_str());
            });

            ImGui::TableHeadersRow();

            for(auto& record : records)
            {
                ImGui::TableNextRow();
                // =====================================

                auto recordMetaInfo = makeMetaInfo(record);

                recordMetaInfo.iterateThroughMembers([&recordMetaInfo, this](auto memberInfo) {
                    using member_t = typename decltype(memberInfo)::member_t;

                    ImGui::TableNextColumn();

                    if constexpr(memberInfo.unmangled_name == "id")
                    {
                        ImGui::PushID((T::s_parentTableName + std::to_string(memberInfo.value)).c_str());

                        auto& currentSelectedRowsMap = m_selectedRows[static_cast<int>(m_tableType)];
                        auto& isSelected = currentSelectedRowsMap[memberInfo.value];
                        auto tmpIsSelected = isSelected;

                        if(ImGui::Selectable(
                                "", &tmpIsSelected, ImGuiSelectableFlags_SpanAllColumns))
                        {
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
                                currentSelectedRowsMap[memberInfo.value] = true;
                            }
                        }

                        if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        {
                            m_openPopup = true;
                            m_rightClickedRowID = memberInfo.value;
                        }

                        ImGui::SameLine();
                        ImGui::Text(std::to_string(memberInfo.value).c_str());
                    }
                    else
                    {
                        if constexpr(std::is_integral_v<member_t> || std::is_floating_point_v<member_t>)
                        {
                            ImGui::Text(std::to_string(memberInfo.value).c_str());
                        }
                        else if constexpr(std::is_same_v<std::basic_string<char>, member_t>)
                        {
                            ImGui::Text(memberInfo.value.c_str());
                        }
                        else if constexpr(std::is_enum_v<member_t>)
                        {
                            ImGui::Text(enum_reflect::generateRuntimeNamesMarkup<member_t>()[std::to_underlying(memberInfo.value)].c_str());
                        }
                    }
                });

                ImGui::PopID();

                sortTable(records);
            }

            ImGui::EndTable();
        }
    }

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
    void initializeSortingSpecsForOffs() const noexcept;
    void initializeSortingSpecsForProviders() const noexcept;
    void initializeSortingSpecsForItems() const noexcept;
    void initializeSortingSpecsForOrders() const noexcept;
    void initializeSortingSpecsForShipments() const noexcept;

    template<typename T>
    struct SortingSpecs
    {
        static inline std::unordered_map<std::uint16_t, std::function<std::pair<float, float>(const T& t0, const T& t1)>> s_sortingFunctions;
    };
};

#endif //PROJ3_TABLESVIEW_H
