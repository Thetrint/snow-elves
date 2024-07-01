//
// Created by y1726 on 2024/6/30.
//
#include "models/TaskSchedul.h"

#include <iostream>
#include <ostream>

TaskSchedul::TaskSchedul(const std::vector<std::wstring> &array) {
    for (const auto& elem : array) {
        std::wcout << elem << std::endl;
    }

    for (const std::wstring&value : array) {
        std::wcout << L"Task Name: " << value << L", Priority: " << 1 << std::endl;
        // 将整数值转换为字符串，并创建Task结构体
        std::wstring taskname = value;
        tasks.emplace_back(taskname, 1);  // priority默认为1
    }

    for (const auto&[taskname, priority] : tasks) {
        std::wcout << L"Task Name: " << taskname << L", Priority: " << priority << std::endl;
    }
}

std::wstring TaskSchedul::get_task() {

    if (tasks.empty()) {
        return L"";  // 如果 tasks 为空，返回空字符串
    } else {
        std::wstring taskname = tasks[0].taskname; // 获取并移除第一个任务名
        tasks.erase(tasks.begin()); // 移除任务列表中的第一个任务
        return taskname;
    }

}

