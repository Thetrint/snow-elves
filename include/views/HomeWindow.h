//
// Created by y1726 on 2024/6/19.
//

#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H
#include "Ui_HomeWindow.h"

namespace Ui {
    class HomeWindow;
}

class HomeWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);

    Ui::HomeWindow ui{};
private:

};

#endif //HOMEWINDOW_H
