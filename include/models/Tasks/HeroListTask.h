//
// Created by y1726 on 24-7-16.
//

#ifndef HEROLISTTASK_H
#define HEROLISTTASK_H
#include "models/Tasks/BasicTask.h"

class HeroListTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    HeroListTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::vector<int> record_num{1};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //HEROLISTTASK_H
