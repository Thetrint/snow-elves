//
// Created by y1726 on 2024/6/30.
//

#ifndef TASKSCHEDUL_H
#define TASKSCHEDUL_H
#include <string>
#include <utility>
#include <vector>

class TaskSchedul {

public:
    explicit TaskSchedul(const std::vector<std::wstring>& array);

    std::wstring get_task();

private:
    // 定义一个结构体，每个结构体包含两个整数
    struct Task {
        std::wstring taskname;
        int priority;

        // explicit Task(std::wstring  task, int priority = 1) : task(std::move(task)), priority(priority) {}
    };

    std::vector<Task> tasks;

};
#endif //TASKSCHEDUL_H
