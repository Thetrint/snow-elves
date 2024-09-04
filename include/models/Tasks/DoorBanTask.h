//
// Created by y1726 on 24-7-20.
//

#ifndef DOORBANTASK_H
#define DOORBANTASK_H
#include "models/Tasks/BasicTask.h"

class DoorBanTask final : public BasicTask{


public:
    DoorBanTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    int x = 0;
    int y = 0;
    // std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //DOORBANTASK_H
