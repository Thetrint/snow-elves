//
// Created by y1726 on 24-7-16.
//

#ifndef THESWORDTHREETASK_H
#define THESWORDTHREETASK_H
#include "models/Tasks/BasicTask.h"

class TheSwordThreeTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;

public:
    TheSwordThreeTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{1};

};
#endif //THESWORDTHREETASK_H
