//
// Created by y1726 on 24-7-17.
//

#ifndef TEASTORYTASK_H
#define TEASTORYTASK_H
#include "models/Tasks/BasicTask.h"

class TeaStoryTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    TeaStoryTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    /**
     * 0. 记录界面匹配失败次数
     * 1. 记录完成延迟退出
     */
    std::vector<int> record_num{0, 0};
    //
    // std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point()};
    //
    // std::vector<bool> record_event{true};

};
#endif //TEASTORYTASK_H
