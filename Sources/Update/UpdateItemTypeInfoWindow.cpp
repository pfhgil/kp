//
// Created by Ilya on 07.12.2024.
//

#include "UpdateItemTypeInfoWindow.h"
#include "SGE/ImGuiUtils.h"

#include <imgui_stdlib.h>
#include "Client.h"
#include "main.h"
#include "TableType.h"

UpdateItemTypeInfoWindow::UpdateItemTypeInfoWindow() noexcept
{
    m_dateIcon = SGE::StylesManager::getCurrentStyle()->m_dateIcon->getSpecialization(22,
                                                                                      22)->getTexture();
}

void UpdateItemTypeInfoWindow::renderBody() noexcept
{
    if(m_isDatePickerOpened)
    {
        m_datePicker.setOpened(m_isDatePickerOpened);
        m_isDatePickerOpened = false;
    }

    m_datePicker.draw();

    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 3, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Name");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##Name", &m_record.m_name);

            ImGui::TableNextColumn();
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Count");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if(ImGui::InputInt("##Count", &m_record.m_count))
            {
                m_record.m_count = std::max(m_record.m_count, 0);
            }

            ImGui::TableNextColumn();
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Date Of Receipt");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Text(m_tmpDateOfReceipt.c_str());

            ImGui::TableNextColumn();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
            if(SGE::ImGuiUtils::ImageButton(m_dateIcon->getTextureNativeHandler(),
                                            { m_dateIcon->getWidth() + 3.0f, m_dateIcon->getHeight()+ 3.0f },
                                            { (float) m_dateIcon->getWidth(), (float) m_dateIcon->getHeight() }).m_isLMBClicked)
            {
                m_isDatePickerOpened = true;
                m_datePicker.setDate(SGCore::Utils::getStringAsTime(m_tmpDateOfReceipt));
                m_datePicker.onDatePicked = [outputDate = &m_tmpDateOfReceipt](DatePicker* self) {
                    *outputDate = SGCore::Utils::getTimeAsString("%Y-%m-%d", self->getTime());
                };
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Expiration Date");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Text(m_tmpExpirationDate.c_str());

            ImGui::TableNextColumn();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
            if(SGE::ImGuiUtils::ImageButton(m_dateIcon->getTextureNativeHandler(),
                                            { m_dateIcon->getWidth() + 3.0f, m_dateIcon->getHeight()+ 3.0f },
                                            { (float) m_dateIcon->getWidth(), (float) m_dateIcon->getHeight() }).m_isLMBClicked)
            {
                m_isDatePickerOpened = true;
                m_datePicker.setDate(SGCore::Utils::getStringAsTime(m_tmpExpirationDate));
                m_datePicker.onDatePicked = [outputDate = &m_tmpExpirationDate](DatePicker* self) {
                    *outputDate = SGCore::Utils::getTimeAsString("%Y-%m-%d", self->getTime());
                };
            }
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::Text("Production Date");

            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Text(m_tmpProductionDate.c_str());

            ImGui::TableNextColumn();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
            if(SGE::ImGuiUtils::ImageButton(m_dateIcon->getTextureNativeHandler(),
                                            { m_dateIcon->getWidth() + 3.0f, m_dateIcon->getHeight()+ 3.0f },
                                            { (float) m_dateIcon->getWidth(), (float) m_dateIcon->getHeight() }).m_isLMBClicked)
            {
                m_isDatePickerOpened = true;
                m_datePicker.setDate(SGCore::Utils::getStringAsTime(m_tmpProductionDate));
                m_datePicker.onDatePicked = [outputDate = &m_tmpProductionDate](DatePicker* self) {
                    *outputDate = SGCore::Utils::getTimeAsString("%Y-%m-%d", self->getTime());
                };
            }
        }

        ImGui::EndTable();
    }
}

void UpdateItemTypeInfoWindow::submit() noexcept
{
    if(m_record.m_name.empty())
    {
        m_errorMessage = "Field 'name' should not be empty!";
        return;
    }

    if(m_record.m_count < 0)
    {
        m_errorMessage = "'Count' can not be negative!";
        return;
    }

    if(m_tmpDateOfReceipt.empty())
    {
        m_errorMessage = "Please, choose date of receipt!";
        return;
    }

    if(m_tmpExpirationDate.empty())
    {
        m_errorMessage = "Please, choose expiration date!";
        return;
    }

    if(m_tmpProductionDate.empty())
    {
        m_errorMessage = "Please, choose production date!";
        return;
    }

    m_record.m_dateOfReceipt = m_tmpDateOfReceipt;
    m_record.m_expirationDate = m_tmpExpirationDate;
    m_record.m_productionDate = m_tmpProductionDate;

    switch(getTableUpdateType())
    {
        case TableUpdateType::ADD:
        {
            Client::addRecord(m_record);
            break;
        }
        case TableUpdateType::UPDATE:
        {
            Client::updateRecord(m_record.m_id, m_record);
            break;
        }
    }

    m_record = { };

    Main::getMainView()->getTablesView()->reloadTable(TableType::ITEM_TYPE_INFO);

    setActive(false);
}

void UpdateItemTypeInfoWindow::onActiveChangedListener() noexcept
{
    UpdateRecordWindow::onActiveChangedListener();
    m_errorMessage = "";

    if(isActive())
    {
        m_tmpDateOfReceipt = m_record.m_dateOfReceipt;
        m_tmpExpirationDate = m_record.m_expirationDate;
        m_tmpProductionDate = m_record.m_productionDate;
    }
}
