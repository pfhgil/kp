//
// Created by Ilya on 19.07.2024.
//
#include "Dark.h"

#include <imgui.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Graphics/API/IRenderer.h>

#include <lunasvg.h>
#include <SGCore/Memory/Assets/SVGImage.h>

void SGE::EditorStyles::Dark::apply()
{
    IStyle::apply();

    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_Border]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.15f, 0.15f, 0.15f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.15f, 0.15f, 0.15f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1.0f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 0.7f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 0.5f);
    colors[ImGuiCol_Separator]              = ImVec4(0.60f, 0.60f, 0.60f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(171 / 255.0f, 205.0f / 255.0f, 239.0f / 255.0f, 0.2f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.50f, 0.50f, 0.50f, 0.2f);

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding                     = ImVec2(8.00f, 8.00f);
    style.FramePadding                      = ImVec2(5.00f, 2.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 15;
    style.GrabMinSize                       = 10;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 1;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 0;
    style.ChildRounding                     = 0;
    style.FrameRounding                     = 0;
    style.PopupRounding                     = 0;
    style.ScrollbarRounding                 = 0;
    style.GrabRounding                      = 0;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 0;

    fillInitialColors();

    // bebas_kai
    // roboto_flex
    // THE STANDARD OF LOADING FONTS
    SGCore::ImGuiWrap::ImGuiLayer::destroyDeviceObjects();

    auto& io = ImGui::GetIO();

    ImFontConfig fontConfig;

    io.Fonts->Clear();

    m_fonts["default_16"] = io.Fonts->AddFontFromFileTTF(
            SGCore::Utils::toUTF8(
                    SGCore::InterpolatedPath("${projectPath}/Resources/fonts/roboto_flex.ttf").resolved().u16string()).c_str(),
            16, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

    m_fonts["default_18"] = io.Fonts->AddFontFromFileTTF(
            SGCore::Utils::toUTF8(
                    SGCore::InterpolatedPath("${projectPath}/Resources/fonts/roboto_flex.ttf").resolved().u16string()).c_str(),
            18, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

    io.Fonts->Build();

    // loading icons
    {

        m_chevronRightIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_chevron_right_icon",
                "${projectPath}/Resources/icons/dark_style/chevron_right.svg");

        m_chevronDownIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_chevron_down_icon",
                "${projectPath}/Resources/icons/dark_style/chevron_down.svg");

        m_dummyIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_dummy_icon",
                "${projectPath}/Resources/icons/dark_style/dummy.svg");

        m_crossIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_cross_icon",
                "${projectPath}/Resources/icons/dark_style/cross.svg");

        m_plusIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_plus_icon",
                "${projectPath}/Resources/icons/dark_style/plus.svg");

        m_minusIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_minus_icon",
                "${projectPath}/Resources/icons/dark_style/minus.svg");

        m_moreIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_more_icon",
                "${projectPath}/Resources/icons/dark_style/more.svg");

        m_reloadIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_reload_icon",
                "${projectPath}/Resources/icons/dark_style/reload.svg");

        m_pencilIcon = SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::SVGImage>(
                "white_pencil_icon",
                "${projectPath}/Resources/icons/dark_style/pencil.svg");
    }
}

