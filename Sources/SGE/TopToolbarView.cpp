//
// Created by ilya on 27.03.24.
//

#include <imgui.h>

#include "TopToolbarView.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"
#include "main.h"
#include "TablesView.h"

SGE::TopToolbarView::TopToolbarView()
{
    m_tablesButtonPopup = Popup {
            "ToolbarTablesActions",
            {
                    {
                            .m_text = "Tables",
                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                       18)->getTexture(),
                            .m_elements = {
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Staff",
                                            .m_ID = "Tables/Staff",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Offs",
                                            .m_ID = "Tables/Offs",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Storages",
                                            .m_ID = "Tables/Storages",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Shipments",
                                            .m_ID = "Tables/Shipments",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Items Type Info",
                                            .m_ID = "Tables/ItemsTypeInfo",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Items",
                                            .m_ID = "Tables/Items",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Orders",
                                            .m_ID = "Tables/Orders",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Providers",
                                            .m_ID = "Tables/Providers",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    })
                            }
                    }
            }
    };

    m_editButtonPopup = Popup {
            "ToolbarTableActions",
            {
                    {
                            .m_text = "Create Row",
                            .m_ID = "Edit/CreateRow",
                            .m_icon = StylesManager::getCurrentStyle()->m_plusIcon->getSpecialization(18,
                                                                                                      18)->getTexture()
                    },
                    {
                            .m_text = "Delete Row",
                            .m_ID = "Edit/DeleteRow",
                            .m_icon = StylesManager::getCurrentStyle()->m_minusIcon->getSpecialization(18,
                                                                                                       18)->getTexture()
                    },
                    {
                            .m_text = "Reload",
                            .m_ID = "Edit/ReloadTable",
                            .m_icon = StylesManager::getCurrentStyle()->m_reloadIcon->getSpecialization(18,
                                                                                                        18)->getTexture()
                    },
                    {
                            .m_text = "Export",
                            .m_ID = "Edit/Export",
                            .m_icon = StylesManager::getCurrentStyle()->m_exportIcon->getSpecialization(18,
                                                                                                        18)->getTexture()
                    },
                    {
                            .m_text = "Import",
                            .m_ID = "Edit/Import",
                            .m_icon = StylesManager::getCurrentStyle()->m_importIcon->getSpecialization(18,
                                                                                                        18)->getTexture()
                    }
            }
    };

    m_tablesButtonPopup.onElementClicked += [](const SGCore::Ref<PopupElement>& element) {
        if(element->m_ID == "Tables/Staff")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::STAFF;
        }
        else if(element->m_ID == "Tables/Storages")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::STORAGES;
        }
        else if(element->m_ID == "Tables/ItemsTypeInfo")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::ITEM_TYPE_INFO;
        }
        else if(element->m_ID == "Tables/Offs")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::OFFS;
        }
        else if(element->m_ID == "Tables/Providers")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::PROVIDERS;
        }
        else if(element->m_ID == "Tables/Items")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::ITEMS;
        }
        else if(element->m_ID == "Tables/Orders")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::ORDERS;
        }
        else if(element->m_ID == "Tables/Shipments")
        {
            Main::getMainView()->getTablesView()->m_tableType = TableType::SHIPMENTS;
        }
    };

    m_editButtonPopup.onElementClicked += [](const SGCore::Ref<PopupElement>& element) {
        if(element->m_ID == "Edit/CreateRow")
        {
            switch(Main::getMainView()->getTablesView()->m_tableType)
            {
                case TableType::STAFF:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateWorkerWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::OFFS:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateOffWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::STORAGES:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateStorageWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::SHIPMENTS:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateShipmentWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::ITEM_TYPE_INFO:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateItemTypeInfoWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::ITEMS:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateItemWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::ORDERS:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateOrderWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::PROVIDERS:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateProviderWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
            }
        }
        else if(element->m_ID == "Edit/DeleteRow")
        {
            Main::getMainView()->getTablesView()->deleteSelectedRows();
        }
        else if(element->m_ID == "Edit/ReloadTable")
        {
            Main::getMainView()->getTablesView()->reloadTable(Main::getMainView()->getTablesView()->m_tableType);
        }
        else if(element->m_ID == "Edit/Export")
        {
            auto fileCreateDialog = Main::getMainView()->getFileCreateDialog();

            fileCreateDialog->m_name = "Export Table";
            fileCreateDialog->m_mode = FileOpenMode::CREATE;
            fileCreateDialog->m_ext = ".json";
            fileCreateDialog->setActive(true);

            fileCreateDialog->onOperationEnd = [](const std::filesystem::path& byPath, bool canceled) {
                if(canceled) return;

                std::string jsonContent;

                switch(Main::getMainView()->getTablesView()->m_tableType)
                {
                    case TableType::STAFF:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_workers);
                        break;
                    }
                    case TableType::OFFS:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_offs);
                        break;
                    }
                    case TableType::STORAGES:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_storages);
                        break;
                    }
                    case TableType::SHIPMENTS:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_shipments);
                        break;
                    }
                    case TableType::ITEM_TYPE_INFO:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_itemsTypeInfo);
                        break;
                    }
                    case TableType::ITEMS:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_items);
                        break;
                    }
                    case TableType::ORDERS:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_orders);
                        break;
                    }
                    case TableType::PROVIDERS:
                    {
                        jsonContent =
                                SGCore::Serde::Serializer::toFormat(Main::getMainView()->getTablesView()->m_providers);
                        break;
                    }
                }

                SGCore::FileUtils::writeToFile(byPath, jsonContent, false, true);
            };
        }
        else if(element->m_ID == "Edit/Import")
        {
            auto fileCreateDialog = Main::getMainView()->getFileCreateDialog();

            fileCreateDialog->m_name = "Import Table";
            fileCreateDialog->m_mode = FileOpenMode::OPEN;
            fileCreateDialog->setActive(true);

            fileCreateDialog->onOperationEnd = [](const std::filesystem::path& byPath, bool canceled) {
                if (canceled) return;

                const std::string jsonContent = SGCore::FileUtils::readFile(byPath);

                switch(Main::getMainView()->getTablesView()->m_tableType)
                {
                    case TableType::STAFF:
                    {
                        importData<Worker>(jsonContent);

                        break;
                    }
                    case TableType::OFFS:
                    {
                        importData<Offs>(jsonContent);

                        break;
                    }
                    case TableType::STORAGES:
                    {
                        importData<Storage>(jsonContent);

                        break;
                    }
                    case TableType::SHIPMENTS:
                    {
                        importData<Shipment>(jsonContent);

                        break;
                    }
                    case TableType::ITEM_TYPE_INFO:
                    {
                        importData<ItemTypeInfo>(jsonContent);

                        break;
                    }
                    case TableType::ITEMS:
                    {
                        importData<Item>(jsonContent);

                        break;
                    }
                    case TableType::ORDERS:
                    {
                        importData<Order>(jsonContent);

                        break;
                    }
                    case TableType::PROVIDERS:
                    {
                        importData<Provider>(jsonContent);

                        break;
                    }
                }

                Main::getMainView()->getTablesView()->reloadTable(Main::getMainView()->getTablesView()->m_tableType);
            };
        }
    };

    m_moreIcon = StylesManager::getCurrentStyle()->m_moreIcon
            ->getSpecialization(24, 24)
            ->getTexture();

    m_tablesButtonPopup.m_isCustomPosition = true;
    m_editButtonPopup.m_isCustomPosition = true;

    m_tablesButtonPopup.m_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
    m_editButtonPopup.m_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
}

bool SGE::TopToolbarView::begin()
{
    return true;
}

void SGE::TopToolbarView::renderBody()
{
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther |
            ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { (!m_showToolsButtons ? 10.0f : 0.0f), 10.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { (!m_showToolsButtons ? 10.0f : 0.0f), 0 });
    ImGui::Begin(m_name.getName().c_str(), nullptr,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoResize);

    if(!m_isAnyButtonHovered)
    {
        m_areaToHighlightPos = { 0, 0 };
        m_areaToHighlightSize = { 0, 0 };
    }

    m_isAnyButtonHovered = false;

    ImGui::GetWindowDrawList()->AddRectFilled(m_areaToHighlightPos,
                                              {m_areaToHighlightPos.x + m_areaToHighlightSize.x,
                                               m_areaToHighlightPos.y + m_areaToHighlightSize.y},
                                              ImGui::ColorConvertFloat4ToU32({0.3f, 0.3f, 0.3f, 0.3f}));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
    // drawing icon that will open bars
    if(!m_showToolsButtons && ImGuiUtils::ImageButton(m_moreIcon->getTextureNativeHandler(),
                               {27.0f, 27.0f},
                               {24.0f, 24.0f}
    ).m_isLMBClicked)
    {
        m_showToolsButtons = true;
    }

    ImGui::SameLine();

    if(m_showToolsButtons)
    {
        // pushing style variables for buttons
        ImGui::PushStyleColor(ImGuiCol_Border, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        // ====================

        drawButtonWithPopup("Tables", m_tablesButtonPopup);
        drawButtonWithPopup("Actions", m_editButtonPopup);

        // ====================
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1);
    }

    if(!ImGui::IsWindowFocused() && !m_isAnyButtonHovered)
    {
        m_showToolsButtons = false;
    }

    // drawing popups
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 0 });
    m_tablesButtonPopup.draw();
    m_editButtonPopup.draw();
    ImGui::PopStyleVar(1);

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void SGE::TopToolbarView::end()
{
    IView::end();
}

void SGE::TopToolbarView::drawButtonWithPopup(const std::string& name, SGE::Popup& buttonPopup) noexcept
{
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    ImVec2 popupPos = ImGui::GetCursorScreenPos();
    popupPos.y += ImGui::GetWindowHeight() - popupPos.y;

    if(buttonPopup.isOpened())
    {
        m_isAnyButtonHovered = true;
        m_areaToHighlightPos = ImGui::GetCursorPos();
    }

    ImGui::Button(name.c_str(), ImVec2(0, ImGui::GetWindowHeight()));
    ImGui::SameLine();

    if(buttonPopup.isOpened())
    {
        m_areaToHighlightSize = ImGui::GetItemRectSize();
    }

    if(ImGui::IsItemHovered())
    {
        buttonPopup.m_customPosition = popupPos;
        buttonPopup.setOpened(true);
    }
}
