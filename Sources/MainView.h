//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_MAINVIEW_H
#define PROJ3_MAINVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "SGE/TopToolbarView.h"
#include "SGE/FileCreateDialog.h"
#include "TablesView.h"
#include "LoginWindow.h"

struct MainView : SGCore::ImGuiWrap::IView
{
    MainView() noexcept;

    void renderBody() noexcept final;

    [[nodiscard]] SGCore::Ref<TablesView> getTablesView() const noexcept;
    [[nodiscard]] SGCore::Ref<SGE::FileCreateDialog> getFileCreateDialog() const noexcept;

private:
    ImGuiID m_dockID = 0;

    SGCore::Ref<SGE::TopToolbarView> m_toolbarView;
    SGCore::Ref<TablesView> m_tablesView;
    SGCore::Ref<LoginWindow> m_loginWindow;
    SGCore::Ref<SGE::FileCreateDialog> m_fileCreateDialog;
};

#endif //PROJ3_MAINVIEW_H
