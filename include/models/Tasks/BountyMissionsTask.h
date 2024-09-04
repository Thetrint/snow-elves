//
// Created by y1726 on 24-7-17.
//

#ifndef BOUNTYMISSIONSTASK_H
#define BOUNTYMISSIONSTASK_H
#include "models/Tasks/BasicTask.h"

class BountyMissionsTask final : public BasicTask{


public:
    BountyMissionsTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    /**
     * 1. 记录判断是否在副本内次数
     * 2. 记录翻页次数
     * 3. 记录副本延迟退出
     * 4. 记录脱离卡死次数
     * 5. 带队模式-万劫召回次数记录
     */
    std::vector<int> record_num{0, 0, 0, 0, 0};
    /**
     * 1. 脱离卡死
     * 2. 喊话间隔
     */
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point()};
    /**
     * 1. 副本内任务激活标志
     * 2. 副本脱离卡死启动标志
     * 3. 第一次接悬赏标志
     */
    std::vector<bool> record_event{true, false, true};

};
#endif //BOUNTYMISSIONSTASK_H
