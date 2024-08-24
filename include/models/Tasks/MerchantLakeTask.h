//
// Created by y1726 on 24-8-13.
//

#ifndef MERCHANTLAKE_H
#define MERCHANTLAKE_H
#include "models/Tasks/BasicTask.h"

class MerchantLakeTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    MerchantLakeTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 确定当前是购买/出售下一个商人地点
     * 1. 记录当前行商次数
     * 2. 记录判断是否开启行商次数
     */
    std::vector<int> record_num{0, 1, 0};
    /**
     * 0. 江湖行商世界喊人间隔时间
     * 1. 定时激活任务
     */
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point(), std::chrono::steady_clock::time_point()};
    /**
     * 0. 江湖行商第一次喊话 开启自动匹配标志
     * 1. 江湖行商购买完标志下次出售
     * 2. 判断行商是否被开启
     */
    std::vector<bool> record_event{true, false, true};


};
#endif //MERCHANTLAKE_H
