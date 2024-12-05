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
            ImGui::InputText("##Name", &m_editableWorker.m_name);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Surname");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Surname", &m_editableWorker.m_surname);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Patronymic");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Patronymic", &m_editableWorker.m_patronymic);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Role");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##RoleCombo", m_editableWorker.m_role.c_str()))
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

            auto workerStorage = Client::getStorageByID(m_editableWorker.m_storageID).get();
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
            ImGui::InputText("##Login", &m_editableWorker.m_login);
        }

        if(m_tableUpdateType != TableUpdateType::UPDATE)
        {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                ImGui::Text("Password");

                ImGui::TableNextColumn();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputText("##Password", &m_editableWorker.m_password);
            }
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
    if(!isActive())
    {
        m_editableWorker = {};
    }
    m_tmpStorages = Client::getAllStorages().get();
}

void UpdateWorkerWindow::submit() noexcept
{
    if(m_editableWorker.m_name.empty())
    {
        m_errorMessage = "Field 'name' should not be empty";
        return;
    }

    if(m_editableWorker.m_surname.empty())
    {
        m_errorMessage = "Field 'surname' should not be empty";
        return;
    }

    if(m_editableWorker.m_role.empty())
    {
        m_errorMessage = "Please, select role (field 'Role')";
        return;
    }

    if(m_editableWorker.m_storageID == -1)
    {
        m_errorMessage = "Please, select storage (field 'Storage')";
        return;
    }

    if(m_editableWorker.m_login.empty())
    {
        m_errorMessage = "Field 'login' should not be empty";
        return;
    }

    if(m_editableWorker.m_password.empty())
    {
        m_errorMessage = "Field 'password' should not be empty";
        return;
    }

    if(m_editableWorker.m_login.empty())
    {
        m_errorMessage = "Field 'login' should not be empty";
        return;
    }

    auto workerWithThatLogin = Client::getWorkerByLogin(m_editableWorker.m_login).get();
    if(workerWithThatLogin.m_id != -1 && workerWithThatLogin.m_id != m_editableWorker.m_id)
    {
        m_errorMessage = "Worker with this login is already exists";
        return;
    }

    if(m_tableUpdateType != TableUpdateType::UPDATE)
    {
        m_editableWorker.m_password = std::to_string(SGCore::hashString(m_editableWorker.m_password));
    }

    switch(m_tableUpdateType)
    {
        case TableUpdateType::ADD:
            Client::addWorker(m_editableWorker);
            break;
        case TableUpdateType::UPDATE:
            Client::updateWorkerByID(m_editableWorker.m_id, m_editableWorker);
            break;
    }

    m_editableWorker = { };

    Main::getMainView()->getTablesView()->reloadTable(TableType::STAFF);

    m_errorMessage = "";
    setActive(false);
}

void UpdateWorkerWindow::cancel() noexcept
{
    m_editableWorker = { };
    m_errorMessage = "";
    setActive(false);
}

void UpdateWorkerWindow::drawRoleSelectable(Role role) noexcept
{
    const auto roleStr = roleToString(role);

    if(ImGui::Selectable(roleToString(role).c_str(),
                         m_editableWorker.m_role == roleStr))
    {
        m_editableWorker.m_role = roleStr;
    }

    if(m_editableWorker.m_role == roleStr)
    {
        ImGui::SetItemDefaultFocus();
    }
}

void UpdateWorkerWindow::drawStorageSelectable(const Storage& storage) noexcept
{
    if(ImGui::Selectable(storage.m_address.c_str(),
                         m_editableWorker.m_storageID == storage.m_id))
    {
        m_editableWorker.m_storageID = storage.m_id;
    }

    if(m_editableWorker.m_storageID == storage.m_id)
    {
        ImGui::SetItemDefaultFocus();
    }
}

void UpdateWorkerWindow::setTableUpdateType(TableUpdateType tableUpdateType) noexcept
{
    m_tableUpdateType = tableUpdateType;
    switch(m_tableUpdateType)
    {
        case TableUpdateType::ADD:
            m_name = "Add Worker";
            break;
        case TableUpdateType::UPDATE:
            m_name = "Update Worker";
            break;
    }
}
