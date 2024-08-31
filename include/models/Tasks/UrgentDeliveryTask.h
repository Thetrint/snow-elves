//
// Created by y1726 on 24-8-13.
//

#ifndef URGENTDELIVERYTASK_H
#define URGENTDELIVERYTASK_H
#include "models/Tasks/BasicTask.h"
#include "main.h"
class UrgentDeliveryTask final : public BasicTask{

public:
    UrgentDeliveryTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 购买成功或者尝试购买一次 放弃任务
     */
    std::vector<bool> record_event{true};
    // std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //URGENTDELIVERYTASK_H
