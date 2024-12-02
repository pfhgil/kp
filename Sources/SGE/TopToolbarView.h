//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
#define SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "SGE/Popup.h"

namespace SGE
{
    struct TopToolbarView : public SGCore::ImGuiWrap::IView
    {
        TopToolbarView();
        
        bool begin() final;
        void renderBody() final;
        void end() final;

    private:
        void drawButtonWithPopup(const std::string& name, SGE::Popup& buttonPopup) noexcept;

        ImGuiID m_dockID = 0;

        SGCore::AssetRef<SGCore::ITexture2D> m_moreIcon;

        Popup m_tablesButtonPopup;
        Popup m_editButtonPopup;

        ImVec2 m_areaToHighlightPos { 0, 0 };
        ImVec2 m_areaToHighlightSize { 0, 0 };

        bool m_isAnyButtonHovered = false;

        bool m_showToolsButtons = false;
    };
}

#endif //SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
