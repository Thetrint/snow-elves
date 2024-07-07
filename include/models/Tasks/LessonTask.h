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
    std::counting_semaphore<1>& pause_event;
    std::counting_semaphore<1>& unbind_event;

public:
    LessonTask(const int id, const HWND hwnd, std::counting_semaphore<1>& pause_event, std::counting_semaphore<1>& unbind_event) : BasicTask(id, hwnd, pause_event, unbind_event), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}

    // 实现基类的纯虚函数
    void implementation() override;

    void objective() override;

    int determine() override;

    int detect() override;


private:

};

#endif //LESSONTASK_H
