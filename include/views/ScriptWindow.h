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

    std::map<std::string, int> TaskMap{
        {"华山论剑", 1},
        {"日常副本", 2},
        {"每日兑换", 3},
        {"宗门任务", 4},
        {"侠缘喊话", 5}
    };

};

#endif //SCRIPTWINDOW_H
