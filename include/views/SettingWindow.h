//
// Created by y1726 on 24-9-3.
//

#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H
#include "Ui_SettingWindow.h"


namespace Ui {
    class SettingWindow;
}

class SettingWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    Ui::SettingWindow ui{};



};
#endif //SETTINGWINDOW_H
