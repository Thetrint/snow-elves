//
// Created by y1726 on 24-7-17.
//

#ifndef BOUNTYMISSIONSTASK_H
#define BOUNTYMISSIONSTASK_H
#include "models/Tasks/BasicTask.h"

class BountyMissionsTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    BountyMissionsTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    /**
     * 0. 判断副本开启成功最大次数
     * 1. 副本开启次数
     * 2. 身上悬赏数量
     * 3. 脱离卡死最大次数
     * 4. 副本完成判断最大次数
     */
    std::vector<int> record_num{0, 0, 0, 0, 0};
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point()};
    std::vector<bool> record_event{true, false};

};
#endif //BOUNTYMISSIONSTASK_H
