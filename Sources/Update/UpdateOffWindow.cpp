//
// Created by Ilya on 10.12.2024.
//

#include "UpdateOffWindow.h"
#include "Client.h"
#include "main.h"
#include "TableType.h"

#include <imgui_stdlib.h>

void UpdateOffWindow::renderBody() noexcept
{
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Worker");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##OffWorker", m_selectedWorkerLogin.c_str()))
            {
                for(const auto& worker : m_tmpWorkers)
                {
                    if (ImGui::Selectable(worker.login.c_str(),
                                          m_selectedWorkerLogin == worker.login))
                    {
                        m_record.worker_id = worker.id;
                        m_selectedWorkerLogin = worker.login;
                    }

                    if (m_selectedWorkerLogin == worker.login)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Reason");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##OffReason", &m_record.reason);
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Storage");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::BeginCombo("##OffStorage", m_selectedStorageAddress.c_str()))
            {
                for(const auto& storage : m_tmpStorages)
                {
                    if (ImGui::Selectable(storage.address.c_str(),
                                          m_selectedStorageAddress == storage.address))
                    {
                        m_record.storage_id = storage.id;
                        m_selectedStorageAddress = storage.address;
                    }

                    if (m_selectedStorageAddress == storage.address)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        ImGui::EndTable();
    }
}

void UpdateOffWindow::onActiveChangedListener() noexcept
{
    UpdateRecordWindow::onActiveChangedListener();

    if(isActive())
    {
        m_selectedWorkerLogin = Client::getRecordByID<Worker>(m_record.worker_id).get().login;
        m_selectedStorageAddress = Client::getRecordByID<Storage>(m_record.storage_id).get().address;

        m_tmpWorkers = Client::getAllRecords<Worker>().get();
        m_tmpStorages = Client::getAllRecords<Storage>().get();
    }
    else
    {
        m_selectedWorkerLogin = "";
        m_selectedStorageAddress = "";
    }
}

void UpdateOffWindow::submit() noexcept
{
    if(m_selectedWorkerLogin.empty())
    {
        m_errorMessage = "Please, select worker!";
        return;
    }

    if(m_selectedStorageAddress.empty())
    {
        m_errorMessage = "Please, select storage";
        return;
    }

    if(m_record.reason.empty())
    {
        m_errorMessage = "Please, write reason!";
        return;
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

    Main::getMainView()->getTablesView()->reloadTable(TableType::OFFS);

    m_errorMessage = "";
    setActive(false);
}
