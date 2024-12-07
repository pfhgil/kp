//
// Created by Ilya on 07.12.2024.
//

#ifndef PROJ3_DATEPICKER_H
#define PROJ3_DATEPICKER_H

#include <cstdint>
#include <string>
#include <chrono>
#include <functional>

struct DatePicker
{
    std::function<void(DatePicker* self)> onDatePicked;

    std::string m_name = "Date Picker";

    void draw() noexcept;

    void setOpened(bool isOpened) noexcept;

    [[nodiscard]] const std::chrono::sys_time<std::chrono::seconds>& getTime() const noexcept;

    void setDate(const std::chrono::sys_time<std::chrono::seconds>& timePoint) noexcept;

private:
    void updateTime() noexcept;

    std::int8_t m_chosenMonth = 1;
    std::int8_t m_chosenDay = 1;
    std::int32_t m_chosenYear = 2020;

    static inline const std::string s_months[] = { "January",
                                                   "February",
                                                   "March",
                                                   "April",
                                                   "May",
                                                   "June",
                                                   "July",
                                                   "August",
                                                   "September",
                                                   "October",
                                                   "November",
                                                   "December" };

    std::chrono::sys_time<std::chrono::seconds> m_time;
};

#endif //PROJ3_DATEPICKER_H
