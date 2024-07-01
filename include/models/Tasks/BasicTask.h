//
// Created by y1726 on 2024/6/30.
//

#ifndef BASICTASK_H
#define BASICTASK_H
#include <Windows.h>
#include <semaphore>

class BasicTask {

protected:
    int id; // 任务ID
    HWND hwnd; // 窗口句柄，假设 HWND 是窗口句柄的类型
    std::counting_semaphore<1>& pause_event;
    std::counting_semaphore<1>& unbind_event;

public:
    virtual ~BasicTask() = default;

    // ReSharper disable once CppParameterMayBeConst
    BasicTask(int id, HWND hwnd, std::counting_semaphore<1>& pause_event, std::counting_semaphore<1>& unbind_event) : id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}


    // 纯虚函数，要求子类必须实现
    virtual void implementation() = 0;

    virtual void objective() = 0;

    virtual int determine() = 0;

    virtual int detect() = 0;
};

#endif //BASICTASK_H
