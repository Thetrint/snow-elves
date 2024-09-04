//
// Created by y1726 on 24-7-16.
//

#ifndef VIENTIANETASK_H
#define VIENTIANETASK_H
#include "models/Tasks/BasicTask.h"

class VientianeTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;
    const QJsonObject& config;

public:
    VientianeTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
    : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs), config(config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 记录万象刷赞次数
     */
    std::vector<int> record_num{0};
    //
    // std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point()};
    //
    // std::vector<bool> record_event{true};

};
#endif //VIENTIANETASK_H
