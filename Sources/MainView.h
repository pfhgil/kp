//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_MAINVIEW_H
#define PROJ3_MAINVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "SGE/TopToolbarView.h"
#include "TablesView.h"
#include "LoginWindow.h"

struct MainView : SGCore::ImGuiWrap::IView
{
    MainView() noexcept;

    void renderBody() noexcept final;

    [[nodiscard]] SGCore::Ref<TablesView> getTablesView() const noexcept;

private:
    ImGuiID m_dockID = 0;

    SGCore::Ref<SGE::TopToolbarView> m_toolbarView;
    SGCore::Ref<TablesView> m_tablesView;
    SGCore::Ref<LoginWindow> m_loginWindow;
};

#endif //PROJ3_MAINVIEW_H
