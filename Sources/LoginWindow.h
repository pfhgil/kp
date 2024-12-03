//
// Created by Ilya on 03.12.2024.
//

#ifndef PROJ3_LOGINWINDOW_H
#define PROJ3_LOGINWINDOW_H

#include "SGE/Window.h"

struct LoginWindow : public SGE::Window
{
    LoginWindow() noexcept;

    void renderBody() noexcept final;
    void footerRender() noexcept final;

    void onActiveChangedListener() noexcept final;

private:
    void submit() noexcept;
    static void quit() noexcept;

    std::string m_currentLogin;
    std::string m_currentPassword;

    std::string m_errorMessage;
};

#endif //PROJ3_LOGINWINDOW_H
