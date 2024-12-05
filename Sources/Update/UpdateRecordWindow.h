//
// Created by Ilya on 05.12.2024.
//

#ifndef PROJ3_UPDATERECORDWINDOW_H
#define PROJ3_UPDATERECORDWINDOW_H

#include <string>
#include "TableUpdateType.h"
#include "SGE/Window.h"
#include <SGCore/Input/InputManager.h>

template<typename RecordT>
struct UpdateRecordWindow : public SGE::Window
{
    RecordT m_record { };

    UpdateRecordWindow() noexcept
    {
        m_isPopupWindow = true;
        m_bodyMinSize = { 350.0f, 275.0f };
        setTableUpdateType(m_tableUpdateType);

        addButton({
                          .m_text = "OK",
                          .m_name = "OKButton",
                          .isFastClicked = [](auto& self) -> bool {
                              return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER);
                          },
                          .onClicked = [this](auto& self, SGCore::ImGuiWrap::IView* parentView) {
                              submit();
                          },
                          .m_color = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1),
                          .m_hoveredColor = ImVec4(0 / 255.0f, 70 / 255.0f, 110 / 255.0f, 1),
                          .m_borderColor = { 0, 0, 0, 0 },
                          .m_borderShadowColor = { 0, 0, 0, 0 },
                          // .m_size = buttonsSize
                  });

        addButton({
                          .m_text = "Cancel",
                          .m_name = "CancelButton",
                          .isFastClicked = [](auto& self) -> bool {
                              return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE);
                          },
                          .onClicked = [this](auto& self, SGCore::ImGuiWrap::IView* parentView) {
                              cancel();
                          },
                          // .m_size = buttonsSize
                  });
    }

    void footerRender() noexcept override
    {
        if(!m_errorMessage.empty())
        {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), m_errorMessage.c_str());
        }
    }

    void setTableUpdateType(TableUpdateType tableUpdateType) noexcept
    {
        m_tableUpdateType = tableUpdateType;
        switch(m_tableUpdateType)
        {
            case TableUpdateType::ADD:
                m_name = "Add Record";
                break;
            case TableUpdateType::UPDATE:
                m_name = "Update Record";
                break;
        }
    }

    [[nodiscard]] const TableUpdateType& getTableUpdateType() const noexcept
    {
        return m_tableUpdateType;
    }

    void onActiveChangedListener() noexcept override
    {
        if(!isActive())
        {
            m_record = {};
            m_name = "";
        }
    }

protected:
    virtual void submit() noexcept = 0;
    virtual void cancel() noexcept
    {
        m_record = { };
        m_errorMessage = "";
        setActive(false);
    }

    std::string m_errorMessage;

private:
    TableUpdateType m_tableUpdateType = TableUpdateType::ADD;
};

#endif //PROJ3_UPDATERECORDWINDOW_H
