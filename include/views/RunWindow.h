//
// Created by y1726 on 2024/6/24.
//

#ifndef RUNWINDOW_H
#define RUNWINDOW_H
#include "Ui_RunWindow.h"
#include "models/TaskManager.h"
#include <thread>


namespace Ui {
    class RunWindow;
}

class RunWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit RunWindow(QWidget *parent = nullptr);
    Ui::RunWindow ui{};
    // ~MainWindow();
    //
protected:

    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            qDebug() << "Mouse button pressed on" << this;
            return true; // Event handled
        }
        return QWidget::eventFilter(obj, event);
    }
private:
    // 结构体保存窗口属性
    struct WindowAttributes {
        RECT geometry;
        std::wstring title;
        LONG style;
    };

    int getrowindex() const;

    bool detectWin(int &id, HWND &hwnd, std::wstring& wintitle);

    struct ManagerData {
        std::shared_ptr<TaskManager> instance;
        std::jthread thread;
        HWND windowHwnd;

    };
    std::list<HWND> winHwnd;
    std::map<int, ManagerData> managerDictionary;
    std::mutex mtx; // 保护映射的互斥锁

};

#endif //RUNWINDOW_H
