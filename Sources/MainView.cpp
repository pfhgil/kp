//
// Created by stuka on 01.12.2024.
//
#include "MainView.h"

MainView::MainView() noexcept
{
    m_toolbarView = SGCore::MakeRef<SGE::TopToolbarView>();
    m_toolbarView->m_name = "TOOLBAR";

    m_loginWindow = SGCore::MakeRef<LoginWindow>();
    m_loginWindow->m_name = "Login";

    m_tablesView = SGCore::MakeRef<TablesView>();
    m_tablesView->m_name = "TABLES";

    m_fileCreateDialog = SGCore::MakeRef<SGE::FileCreateDialog>();
    m_fileCreateDialog->setActive(false);
    m_fileCreateDialog->m_name = "FILE_CREATE";

    addChild(m_tablesView);
    addChild(m_loginWindow);
    addChild(m_toolbarView);
    addChild(m_fileCreateDialog);
}

void MainView::renderBody() noexcept
{
    m_dockID = ImGui::DockSpaceOverViewport(m_dockID, ImGui::GetMainViewport());
}

SGCore::Ref<TablesView> MainView::getTablesView() const noexcept
{
    return m_tablesView;
}

SGCore::Ref<SGE::FileCreateDialog> MainView::getFileCreateDialog() const noexcept
{
    return m_fileCreateDialog;
}

