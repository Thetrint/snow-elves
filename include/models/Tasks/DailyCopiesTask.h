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
    std::string cause;
    /**
     * 1. 记录开启副本次数
     * 2. 记录判断是否在副本内次数
     * 3. 记录副本延迟退出次数
     * 4. 记录副本脱离卡死次数
     */
    std::vector<int> record_num{0, 0, 0, 0};
    /**
     * 1. 副本脱离卡死时间
     * 2. 世界喊话时间
     */
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point()};
    /**
     * 1. 副本内任务激活标志
     * 2. 副本脱离卡死启动标志
     */
    std::vector<bool> record_event{true, false};

};
#endif //DAILYCOPIESTASK_H
