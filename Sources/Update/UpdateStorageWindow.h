//
// Created by Ilya on 05.12.2024.
//

#ifndef PROJ3_UPDATESTORAGEWINDOW_H
#define PROJ3_UPDATESTORAGEWINDOW_H

#include "Models/Storage.h"
#include "UpdateRecordWindow.h"

struct UpdateStorageWindow : public UpdateRecordWindow<Storage>
{
    void renderBody() noexcept final;

private:
    void submit() noexcept final;
};

#endif //PROJ3_UPDATESTORAGEWINDOW_H
