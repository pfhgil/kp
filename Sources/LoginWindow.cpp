//
// Created by Ilya on 03.12.2024.
//

#include "LoginWindow.h"
#include <SGCore/Input/InputManager.h>

LoginWindow::LoginWindow() noexcept
{
    m_isPopupWindow = true;
    m_bodyMinSize = { 350.0f, 100.0f };

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
                      .m_text = "Quit",
                      .m_name = "QuitButton",
                      .isFastClicked = [](auto& self) -> bool {
                          return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE);
                      },
                      .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView) {
                          // cancel();
                          quit();
                      },
                      // .m_size = buttonsSize
              });
}

void LoginWindow::renderBody() noexcept
{

}

void LoginWindow::onActiveChangedListener() noexcept
{
    if(!isActive())
    {
        quit();
    }
}

void LoginWindow::submit() noexcept
{

}

void LoginWindow::quit() noexcept
{
    std::exit(0);
}
