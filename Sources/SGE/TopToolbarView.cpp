//
// Created by ilya on 27.03.24.
//

#include <imgui.h>

#include "TopToolbarView.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"
#include "main.h"
#include "TablesView.h"
#include "Client.h"

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
                case TableType::OFFS:break;
                case TableType::STORAGES:
                {
                    auto wnd = Main::getMainView()->getTablesView()->getUpdateStorageWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);

                    break;
                }
                case TableType::SHIPMENTS:break;
                case TableType::ITEM_TYPE_INFO:
                {
                    /*auto wnd = Main::getMainView()->getTablesView()->getUpdateStorageWindow();

                    wnd->setTableUpdateType(TableUpdateType::ADD);
                    wnd->setActive(true);*/

                    ItemTypeInfo itemTypeInfo {
                        .m_name = "Металл",
                        .m_dateOfReceipt = "2023-10-24",
                        .m_expirationDate = "2101-11-10",
                        .m_productionDate = "2001-11-10"
                    };

                    Client::addItemTypeInfo(itemTypeInfo);

                    break;
                }
                case TableType::ITEMS:break;
                case TableType::ORDERS:break;
                case TableType::PROVIDERS:break;
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
