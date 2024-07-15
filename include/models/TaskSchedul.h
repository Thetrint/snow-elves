//
// Created by y1726 on 2024/6/30.
//

#ifndef TASKSCHEDUL_H
#define TASKSCHEDUL_H
#include "main.h"
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "models/Tasks/BasicTask.h"


class TaskSchedul {

public:
    explicit TaskSchedul(const std::vector<std::string> &array, const std::unique_ptr<BasicTask> &rol) : rol(rol), array(array){
        spdlog::info("创建任务调度器");
    };
    void init();

    void task_schedul();

    std::string get_task();

private:
    // 定义一个结构体，每个结构体包含两个整数
    struct Task {
        std::string taskname;
        int priority;

        // explicit Task(std::wstring  task, int priority = 1) : task(std::move(task)), priority(priority) {}
    };

    std::mutex mtx;
    bool running_ = true;
    std::thread thread_;
    const std::unique_ptr<BasicTask>& rol;
    std::vector<std::string> array;
    std::vector<Task> tasks;

};
#endif //TASKSCHEDUL_H
