//
// Created by y1726 on 24-7-16.
//

#ifndef DAILYCOPIESTASK_H
#define DAILYCOPIESTASK_H
#include "models/Tasks/BasicTask.h"

class DailyCopiesTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    DailyCopiesTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 带队模式-进入副本次数记录
     * 1. 带队模式-判断副本内次数
     * 2. 带队模式-副本内脱离卡死次数记录
     * 3. 带队模式-副本退出延迟判断记录
     * 4. 带队模式-万劫召回次数记录
     */
    std::vector<int> record_num{0, 0, 0, 0, 0};
    /**
     * 0. 带队模式-世界喊话时间记录
     * 1. 带队模式-副本内脱离卡死时间记录
     */
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point()};
    /**
     * 0. 带队模式-任务激活标志
     * 1. 带队模式-脱离卡死启动标志
     */
    std::vector<bool> record_event{true, false};

};
#endif //DAILYCOPIESTASK_H
