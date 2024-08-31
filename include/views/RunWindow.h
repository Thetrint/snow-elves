//
// Created by y1726 on 2024/6/24.
//

#ifndef RUNWINDOW_H
#define RUNWINDOW_H
#include "Ui_RunWindow.h"
#include "models/TaskManager.h"
#include <thread>


class MainWindow;

namespace Ui {
    class RunWindow;
}

class RunWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit RunWindow(QWidget *parent = nullptr);

    Ui::RunWindow ui{};

    void setMainWindow(const MainWindow *mainWindow);

protected:

    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            qDebug() << "Mouse button pressed on" << this;
            return true; // Event handled
        }
        return QWidget::eventFilter(obj, event);
    }
private:
    const MainWindow* mainWindow;

    int getrowindex() const;

    bool detectWin(int &id, HWND &hwnd, std::wstring& wintitle);

    struct ManagerData {
        std::shared_ptr<TaskManager> instance;
        std::jthread thread;
        HWND hwnd;
        bool isActive;

        ManagerData()
       : instance(nullptr), thread(), hwnd(nullptr), isActive(true) {}

    };
    std::map<int, ManagerData> mapManagerData;

    // 创建一个存储 pair<id, handle> 的队列
    std::queue<std::pair<int, HWND>> idHWNDQueue;
    std::set<HWND> hwndSet;
    std::mutex mtxQueue; // 保护映射的互斥锁

    std::mutex mtx; // 保护映射的互斥锁


};

#endif //RUNWINDOW_H
