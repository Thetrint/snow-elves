//
// Created by y1726 on 2024/6/30.
//

#ifndef LESSONTASK_H
#define LESSONTASK_H
#include "models/Tasks/BasicTask.h"

class LessonTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;

public:
    LessonTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event) : BasicTask(id, hwnd, pause_event, unbind_event), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::vector<std::chrono::steady_clock::time_point>
};

#endif //LESSONTASK_H
