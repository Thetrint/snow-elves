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
    bool& disrupted;

public:
    FactionTask(int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), disrupted(disrupted) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string) override;

    int determine() override;

    int detect() override;

private:
    std::string cause;
    std::vector<std::chrono::steady_clock::time_point> record_time{1};
};

#endif //FACTIONTASK_H
