//
// Created by y1726 on 2024/6/29.
//

#ifndef SCRIPTWINDOW_H
#define SCRIPTWINDOW_H
#include "Ui_ScriptWindow.h"

namespace Ui {
    class ScriptWindow;
}

class ScriptWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit ScriptWindow(QWidget *parent = nullptr);
    Ui::ScriptWindow ui{};
private:
    void AddTask() const;

};

#endif //SCRIPTWINDOW_H
