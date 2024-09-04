//
// Created by y1726 on 24-7-15.
//

#ifndef THESWORDTASK_H
#define THESWORDTASK_H
#include "models/Tasks/BasicTask.h"

class TheSwordTask final : public BasicTask{

public:
    TheSwordTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::vector<int> record_num{1, 0};
    /**
     * 1. 战斗标志
     */
    std::vector<bool> record_event{true};
    // std::vector<std::chrono::steady_clock::time_point> record_time{1};

};
#endif //THESWORDTASK_H
