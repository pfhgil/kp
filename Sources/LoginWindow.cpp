//
// Created by Ilya on 03.12.2024.
//

#include "LoginWindow.h"
#include "Client.h"
#include <SGCore/Input/InputManager.h>
#include <imgui_stdlib.h>
#include "main.h"

LoginWindow::LoginWindow() noexcept
{
    m_isPopupWindow = true;
    m_bodyMinSize = { 350.0f, 75.0f };

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
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Login");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Login", &m_currentLogin);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Password");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Password", &m_currentPassword);
        }

        ImGui::EndTable();
    }
}

void LoginWindow::footerRender() noexcept
{
    if(!m_errorMessage.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_errorMessage.c_str());
    }
}

void LoginWindow::onActiveChangedListener() noexcept
{
    if(!isActive() && !Client::isAuthValid(Client::getJWTToken()).get())
    {
        quit();
    }
}

void LoginWindow::submit() noexcept
{
    Client::auth(m_currentLogin, std::to_string(SGCore::hashString(m_currentPassword)));
    if(Client::isAuthValid(Client::getJWTToken()).get())
    {
        m_workerRole = Client::getWorkerByLogin(m_currentLogin).get().role;

        setActive(false);
        Main::getMainView()->getTablesView()->reloadAllTables();
    }
    else
    {
        m_errorMessage = "Incorrect login or password";
    }
}

void LoginWindow::quit() noexcept
{
    std::exit(0);
}
