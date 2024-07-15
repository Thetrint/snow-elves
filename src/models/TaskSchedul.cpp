//
// Created by y1726 on 2024/6/30.
//
#include "models/TaskSchedul.h"
#include "main.h"
#include <iostream>
#include <ostream>


std::string TaskSchedul::get_task() {
    std::unique_lock<std::mutex> lock(mtx);
    if (tasks.empty()) {
        return "";  // 如果 tasks 为空，返回空字符串
    }
    std::string taskname = tasks[0].taskname; // 获取并移除第一个任务名
    tasks.erase(tasks.begin()); // 移除任务列表中的第一个任务
    return taskname;
}

void TaskSchedul::init() {
    spdlog::info("任务调度器初始化");
    //获取锁
    std::unique_lock<std::mutex> lock(mtx);
    tasks.clear();

    for (const std::string&value : array) {
        // 将整数值转换为字符串，并创建Task结构体
        std::string taskname = value;
        tasks.emplace_back(taskname, 1);  // priority默认为1
    }
}

void TaskSchedul::task_schedul() {
    if (running_) {
        return;
    }
    struct TaskEntry {
        time_t time1;
        bool value;
    };

    TaskEntry entry = {0, true};

    running_ = true;
    thread_ = std::thread([this, &entry]() {
        while (running_) {
            //获取锁
            std::unique_lock<std::mutex> lock(mtx);
            //获取当前时间
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            if (now > entry.time1) {
                std::cout << "当前时间已经超过30分钟后的时间点。" << std::endl;
            }

            //休眠一秒
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

}