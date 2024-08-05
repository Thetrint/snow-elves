//
// Created by y1726 on 2024/6/26.
//

#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include <qwindowdefs_win.h>
#include <semaphore>

#include "models//TaskManager.h"
#include "models/ImageProcess.h"
#include "models/Tasks/BasicTask.h"
#include "utils/TaskScheduler.h"

class TaskManager {

public:
    explicit TaskManager(int id, HWND hwnd, const QJsonDocument& configJson);

    ~TaskManager();

    void start();

    void stop();

    void pause();

    void resume();

    void setState(const std::string &task) const;

private:
    QJsonObject  config;
    int id;
    HWND hwnd;
    bool disrupted;
    std::mutex pause_event;
    bool unbind_event;
    bool LOCK;
    std::ifstream ifs;

    std::unique_ptr<TaskScheduler> scheduler;
    struct Task {
        std::string name;
        int priority;

        Task(const std::string &name, const int priority) : name(name), priority(priority) {}

        // 比较函数，放入结构体内部
        bool operator<(const Task& other) const {
            // 优先级高的任务优先
            return this->priority < other.priority;
        }

        // 自定义输出，便于测试
        friend std::ostream& operator<<(std::ostream& os, const Task& task) {
            os << "Task name: " << task.name << ", Priority: " << task.priority;
            return os;
        }

    };




    std::priority_queue<Task> taskQueue;

    void Init();

    std::string getTask();




};
#endif //TASKMANAGER_H




