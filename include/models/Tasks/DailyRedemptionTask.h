//
// Created by y1726 on 24-7-21.
//

#ifndef DAILYREDEMPTIONTASK_H
#define DAILYREDEMPTIONTASK_H
#include "models/Tasks/BasicTask.h"

class DailyRedemptionTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    DailyRedemptionTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 银票礼盒
     * 1. 帮派捐献
     */
    std::vector<bool> record_event{true, true};
    // std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //DAILYREDEMPTIONTASK_H
