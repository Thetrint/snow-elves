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
#include "models/Tasks/SwitchRolesTask.h"
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
    SwitchRolesTask rolesTask;

    std::unique_ptr<TaskScheduler> scheduler;

    std::unique_ptr<BasicTask> obj;

    struct Task {
        std::string name;
        int priority;
        long long timestamp; // 用于记录插入时间戳

        // 构造函数自动分配时间戳
        Task(const std::string &name, const int priority)
            : name(name), priority(priority), timestamp(currentTimestamp()) {}

        // 获取当前时间戳
        static long long currentTimestamp() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

        // 比较函数，放入结构体内部
        bool operator<(const Task& other) const {
            if (this->priority == other.priority) {
                return this->timestamp > other.timestamp; // 时间戳越小，优先级越高
            }
            return this->priority < other.priority; // 优先级越高，优先级越高
        }

        // 自定义输出，便于测试
        friend std::ostream& operator<<(std::ostream& os, const Task& task) {
            os << "Task name: " << task.name << ", Priority: " << task.priority << ", Timestamp: " << task.timestamp;
            return os;
        }
    };





    std::priority_queue<Task> taskQueue;

    void Init();

    std::string getTask();




};
#endif //TASKMANAGER_H




