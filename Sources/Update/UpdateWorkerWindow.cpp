//
// Created by Ilya on 03.12.2024.
//

#include <SGCore/Input/InputManager.h>
#include "imgui_stdlib.h"

#include "UpdateWorkerWindow.h"
#include "Client.h"

#include "main.h"

UpdateWorkerWindow::UpdateWorkerWindow() noexcept
{
    m_isPopupWindow = true;
    m_bodyMinSize = { 350.0f, 300.0f };
    m_name = "Add Worker";

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

void UpdateWorkerWindow::renderBody() noexcept
{
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Name");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Name", &m_tmpWorker.m_name);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Surname");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Surname", &m_tmpWorker.m_surname);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Patronymic");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Patronymic", &m_tmpWorker.m_patronymic);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Role");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##RoleCombo", m_tmpWorker.m_role.c_str()))
            {
                drawRoleSelectable(Role::DIRECTOR);
                drawRoleSelectable(Role::STOREKEEPER);
                drawRoleSelectable(Role::WORKER);

                ImGui::EndCombo();
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Storage");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            auto workerStorage = Client::getStorageByID(m_tmpWorker.m_storageID).get();
            if(ImGui::BeginCombo("##StorageCombo", workerStorage.m_address.c_str()))
            {
                for(const auto& storage : m_tmpStorages)
                {
                    drawStorageSelectable(storage);
                }

                ImGui::EndCombo();
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Login");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Login", &m_tmpWorker.m_login);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Password");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Password", &m_tmpWorker.m_password);
        }

        ImGui::EndTable();
    }
}

void UpdateWorkerWindow::footerRender() noexcept
{
    if(!m_errorMessage.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_errorMessage.c_str());
    }
}

void UpdateWorkerWindow::onActiveChangedListener() noexcept
{
    m_tmpStorages = Client::getAllStorages().get();
}

void UpdateWorkerWindow::submit() noexcept
{
    if(m_tmpWorker.m_name.empty())
    {
        m_errorMessage = "Field 'name' should not be empty";
        return;
    }

    if(m_tmpWorker.m_surname.empty())
    {
        m_errorMessage = "Field 'surname' should not be empty";
        return;
    }

    if(m_tmpWorker.m_role.empty())
    {
        m_errorMessage = "Please, select role (field 'Role')";
        return;
    }

    if(m_tmpWorker.m_storageID == -1)
    {
        m_errorMessage = "Please, select storage (field 'Storage')";
        return;
    }

    if(m_tmpWorker.m_login.empty())
    {
        m_errorMessage = "Field 'login' should not be empty";
        return;
    }

    if(m_tmpWorker.m_password.empty())
    {
        m_errorMessage = "Field 'password' should not be empty";
        return;
    }

    if(m_tmpWorker.m_login.empty())
    {
        m_errorMessage = "Field 'login' should not be empty";
        return;
    }

    if(Client::getWorkerByLogin(m_tmpWorker.m_login).get().m_id != -1)
    {
        m_errorMessage = "Worker with this login is already exists";
        return;
    }

    m_tmpWorker.m_password = std::to_string(SGCore::hashString(m_tmpWorker.m_password));
    Client::addWorker(m_tmpWorker);

    Main::getMainView()->getTablesView()->reloadTable(TableType::STAFF);

    m_tmpWorker = { };
    m_errorMessage = "";
    setActive(false);
}

void UpdateWorkerWindow::cancel() noexcept
{
    m_tmpWorker = { };
    m_errorMessage = "";
    setActive(false);
}

void UpdateWorkerWindow::drawRoleSelectable(Role role) noexcept
{
    const auto roleStr = roleToString(role);

    if(ImGui::Selectable(roleToString(role).c_str(),
                         m_tmpWorker.m_role == roleStr))
    {
        m_tmpWorker.m_role = roleStr;
    }

    if(m_tmpWorker.m_role == roleStr)
    {
        ImGui::SetItemDefaultFocus();
    }
}

void UpdateWorkerWindow::drawStorageSelectable(const Storage& storage) noexcept
{
    if(ImGui::Selectable(storage.m_address.c_str(),
                         m_tmpWorker.m_storageID == storage.m_id))
    {
        m_tmpWorker.m_storageID = storage.m_id;
    }

    if(m_tmpWorker.m_storageID == storage.m_id)
    {
        ImGui::SetItemDefaultFocus();
    }
}
