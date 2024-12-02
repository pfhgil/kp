//
// Created by stuka on 01.12.2024.
//

#ifndef PROJ3_MAIN_H
#define PROJ3_MAIN_H

#include "MainView.h"

struct Main
{
    static void update(const double& dt, const double& fixedDt) noexcept;
    static void fixedUpdate(const double& dt, const double& fixedDt) noexcept;

    static void init() noexcept;

    static SGCore::Ref<MainView> getMainView() noexcept;

private:
    static inline SGCore::Ref<MainView> s_mainView;
};

#endif //PROJ3_MAIN_H
