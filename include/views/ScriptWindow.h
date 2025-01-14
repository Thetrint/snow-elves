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
        {"全局设置", 0},
        {"华山论剑", 1},
        {"华山论剑3v3", 1},
        {"江湖英雄榜", 1},
        {"江湖行商", 1},
        {"聚义平冤", 1},
        {"日常副本", 2},
        {"悬赏副本", 2},
        {"每日兑换", 3},
        {"宗门任务", 4},
        {"侠缘喊话", 5},
        {"生死剑冢", 6},
        {"采集任务", 7},
        {"扫摆摊", 8}
    };

};

#endif //SCRIPTWINDOW_H
