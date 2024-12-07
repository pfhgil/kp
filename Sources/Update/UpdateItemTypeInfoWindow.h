//
// Created by Ilya on 07.12.2024.
//

#ifndef PROJ3_UPDATEITEMTYPEINFOWINDOW_H
#define PROJ3_UPDATEITEMTYPEINFOWINDOW_H

#include "UpdateRecordWindow.h"
#include "Models/ItemTypeInfo.h"
#include "DatePicker.h"

struct UpdateItemTypeInfoWindow : public UpdateRecordWindow<ItemTypeInfo>
{
    UpdateItemTypeInfoWindow() noexcept;

    void renderBody() noexcept;

    void onActiveChangedListener() noexcept final;

private:
    void submit() noexcept final;

    SGCore::AssetRef<SGCore::ITexture2D> m_dateIcon;

    std::string m_tmpDateOfReceipt;
    std::string m_tmpExpirationDate;
    std::string m_tmpProductionDate;

    DatePicker m_datePicker;
    bool m_isDatePickerOpened = false;
};

#endif //PROJ3_UPDATEITEMTYPEINFOWINDOW_H
