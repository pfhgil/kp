//
// Created by Ilya on 27.07.2024.
//
#include "IStyle.h"

void SGE::IStyle::apply()
{

}

void SGE::IStyle::fillInitialColors() noexcept
{
    for(std::int64_t i = 0; i < m_initialColors.size(); ++i)
    {
        m_initialColors[i] = ImGui::GetStyle().Colors[i];
    }
}

