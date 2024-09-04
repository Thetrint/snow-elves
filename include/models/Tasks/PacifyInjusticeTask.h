//
// Created by y1726 on 24-8-14.
//

#ifndef PACIFYINJUSTICETASK_H
#define PACIFYINJUSTICETASK_H
#include "models/Tasks/BasicTask.h"

class PacifyInjusticeTask final : public BasicTask{

public:
    PacifyInjusticeTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 记录判断是否在副本内次数
     */
    std::vector<int> record_num{0};
    /**
     * 0. 喊话间隔
     */
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point()};
    /**
     * 0. 第一次喊话开启自动匹配
     */
    std::vector<bool> record_event{true};

};
#endif //PACIFYINJUSTICETASK_H
