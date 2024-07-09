//
// Created by y1726 on 2024/6/30.
//

#ifndef FACTIONTASK_H
#define FACTIONTASK_H
#include "models/Tasks/BasicTask.h"

class FactionTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;

public:
    FactionTask(int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event) : BasicTask(id, hwnd, pause_event, unbind_event), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string) override;

    int determine() override;

    int detect() override;

private:

};

#endif //FACTIONTASK_H
