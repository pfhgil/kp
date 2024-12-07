//
// Created by Ilya on 07.12.2024.
//

#include "DatePicker.h"
#include <imgui.h>
#include <SGCore/Utils/Utils.h>

void DatePicker::draw() noexcept
{
    const std::string chosenYearStr = std::to_string(m_chosenYear);
    const std::string chosenMonthStr = std::to_string(m_chosenMonth);
    const std::string chosenDayStr = std::to_string(m_chosenDay);

    if(ImGui::BeginPopup(m_name.c_str()))
    {
        if(ImGui::BeginCombo("Year", chosenYearStr.c_str()))
        {
            for(std::int64_t curYear = 1800; curYear <= 2200; ++curYear)
            {
                const std::string curYearStr = std::to_string(curYear);

                if (ImGui::Selectable(curYearStr.c_str(),
                                      m_chosenYear == curYear))
                {
                    m_chosenYear = curYear;
                    updateTime();
                }

                if (m_chosenYear == curYear)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        if(ImGui::BeginCombo("Month", s_months[m_chosenMonth - 1].c_str()))
        {
            for(std::int8_t curMonth = 1; curMonth <= 12; ++curMonth)
            {
                const std::string curMonthStr = std::to_string(curMonth);

                if (ImGui::Selectable(s_months[curMonth - 1].c_str(),
                                      m_chosenMonth == curMonth))
                {
                    m_chosenMonth = curMonth;
                    updateTime();
                }

                if (m_chosenMonth == curMonth)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        if(ImGui::BeginCombo("Day", chosenDayStr.c_str()))
        {
            const auto lastDay = std::uint32_t { SGCore::Utils::getLastDayOfTimePoint(m_time) };
            // const auto lastDay = std::uint32_t { (std::chrono::last / SGCore::Utils::getMonthFromTimePoint(m_time) / SGCore::Utils::getYearFromTimePoint(m_time)).day() };

            /*std::cout << SGCore::Utils::getMonthFromTimePoint(m_time) << std::endl;
            std::cout << m_time << std::endl;*/

            for(std::int8_t curDay = 1; curDay <= lastDay; ++curDay)
            {
                const std::string curDayStr = std::to_string(curDay);

                if (ImGui::Selectable(std::to_string(curDay).c_str(),
                                      m_chosenDay == curDay))
                {
                    m_chosenDay = curDay;
                    updateTime();
                }

                if (m_chosenDay == curDay)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        if(ImGui::Button("OK"))
        {
            onDatePicked(this);
        }

        ImGui::EndPopup();
    }
}

void DatePicker::setOpened(bool isOpened) noexcept
{
    if(isOpened)
    {
        ImGui::OpenPopup(m_name.c_str());
    }
}

void DatePicker::updateTime() noexcept
{
    std::chrono::year_month_day ymd { std::chrono::year { m_chosenYear } / m_chosenMonth / m_chosenDay};

    auto dateDays = std::chrono::sys_days { ymd };

    m_time = std::chrono::time_point_cast<std::chrono::seconds>(dateDays);
}

const std::chrono::sys_time<std::chrono::seconds>& DatePicker::getTime() const noexcept
{
    return m_time;
}

void DatePicker::setDate(const std::chrono::sys_time<std::chrono::seconds>& timePoint) noexcept
{
    m_chosenYear = SGCore::Utils::getYearFromTimePoint(timePoint).operator int();
    m_chosenMonth = SGCore::Utils::getMonthFromTimePoint(timePoint).operator unsigned int();
    m_chosenDay = SGCore::Utils::getDayFromTimePoint(timePoint).operator unsigned int();

    m_time = timePoint;
}
