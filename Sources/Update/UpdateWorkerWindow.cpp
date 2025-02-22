//
// Created by Ilya on 03.12.2024.
//

#include <SGCore/Input/InputManager.h>
#include "imgui_stdlib.h"

#include "UpdateWorkerWindow.h"
#include "Client.h"

#include "main.h"

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
            ImGui::InputText("##Name", &m_record.name);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Surname");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Surname", &m_record.surname);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Patronymic");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Patronymic", &m_record.patronymic);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Role");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##RoleCombo", roleToString(m_record.role).c_str()))
            {
                drawRoleSelectable(WorkerRole::DIRECTOR);
                drawRoleSelectable(WorkerRole::STOREKEEPER);
                drawRoleSelectable(WorkerRole::WORKER);

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

            if(ImGui::BeginCombo("##StorageCombo", m_workerStorage.address.c_str()))
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
            ImGui::InputText("##Login", &m_record.login);
        }

        if(getTableUpdateType() != TableUpdateType::UPDATE)
        {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                ImGui::Text("Password");

                ImGui::TableNextColumn();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputText("##Password", &m_record.password);
            }
        }

        ImGui::EndTable();
    }
}

void UpdateWorkerWindow::onActiveChangedListener() noexcept
{
    UpdateRecordWindow::onActiveChangedListener();

    m_workerStorage = Client::getRecordByID<Storage>(m_record.storage_id).get();

    m_tmpStorages = Client::getAllRecords<Storage>().get();
}

void UpdateWorkerWindow::submit() noexcept
{
    if(m_record.name.empty())
    {
        m_errorMessage = "Field 'name' should not be empty";
        return;
    }

    if(m_record.surname.empty())
    {
        m_errorMessage = "Field 'surname' should not be empty";
        return;
    }

    if(m_record.storage_id == -1)
    {
        m_errorMessage = "Please, select storage (field 'Storage')";
        return;
    }

    if(m_record.login.empty())
    {
        m_errorMessage = "Field 'login' should not be empty";
        return;
    }

    if(m_record.password.empty())
    {
        m_errorMessage = "Field 'password' should not be empty";
        return;
    }

    if(m_record.login.empty())
    {
        m_errorMessage = "Field 'login' should not be empty";
        return;
    }

    auto workerWithThatLogin = Client::getWorkerByLogin(m_record.login).get();
    if(workerWithThatLogin.id != -1 && workerWithThatLogin.id != m_record.id)
    {
        m_errorMessage = "Worker with this login is already exists";
        return;
    }

    if(getTableUpdateType() != TableUpdateType::UPDATE)
    {
        m_record.password = std::to_string(SGCore::hashString(m_record.password));
    }

    switch(getTableUpdateType())
    {
        case TableUpdateType::ADD:
            Client::addRecord(m_record);
            break;
        case TableUpdateType::UPDATE:
            Client::updateRecord(m_record.id, m_record);
            break;
    }

    m_record = { };

    Main::getMainView()->getTablesView()->reloadTable(TableType::STAFF);

    m_errorMessage = "";
    setActive(false);
}

void UpdateWorkerWindow::drawRoleSelectable(WorkerRole role) noexcept
{
    if(ImGui::Selectable(roleToString(role).c_str(),
                         m_record.role == role))
    {
        m_record.role = role;
    }

    if(m_record.role == role)
    {
        ImGui::SetItemDefaultFocus();
    }
}

void UpdateWorkerWindow::drawStorageSelectable(const Storage& storage) noexcept
{
    if(ImGui::Selectable(storage.address.c_str(),
                         m_record.storage_id == storage.id))
    {
        m_record.storage_id = storage.id;
        m_workerStorage = Client::getRecordByID<Storage>(m_record.storage_id).get();
    }

    if(m_record.storage_id == storage.id)
    {
        ImGui::SetItemDefaultFocus();
    }
}
