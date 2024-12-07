//
// Created by Ilya on 07.12.2024.
//

#ifndef PROJ3_DATEPICKER_H
#define PROJ3_DATEPICKER_H

#include <cstdint>

struct DatePicker
{
    void draw() noexcept;

    [[nodiscard]] bool isOpened() const noexcept;
    void setOpened(bool isOpened) noexcept;

private:
    std::int64_t m_day = -1;
    std::int64_t m_month = -1;
    std::int64_t m_year = -1;
};

#endif //PROJ3_DATEPICKER_H
