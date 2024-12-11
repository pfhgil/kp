//
// Created by Ilya on 11.12.2024.
//

#ifndef PROJ3_UPDATEPROVIDERWINDOW_H
#define PROJ3_UPDATEPROVIDERWINDOW_H

#include "Models/Provider.h"
#include "UpdateRecordWindow.h"

class UpdateProviderWindow : public UpdateRecordWindow<Provider>
{
    void renderBody() noexcept final;

private:
    void submit() noexcept final;
};


#endif //PROJ3_UPDATEPROVIDERWINDOW_H
