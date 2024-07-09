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
private slots:
    void StartTask();
    void Stop();

private:
    int getrowindex() const;

    std::map<int, std::shared_ptr<TaskManager>> instances; // 存储实例的映射
    std::map<int, std::jthread> threads; // 存储线程的映射
    std::mutex mtx; // 保护映射的互斥锁

};

#endif //RUNWINDOW_H
