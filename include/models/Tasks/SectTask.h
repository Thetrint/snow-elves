//
// Created by y1726 on 24-7-25.
//

#ifndef SECTTASK_H
#define SECTTASK_H
#include "models/Tasks/BasicTask.h"

class SectTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    SectTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(
            id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event),
        unbind_event(unbind_event), ifs(ifs), index(0) {
    }

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    int index;
    std::map<int, std::string> team {
        {0, "标志天下宗师队伍1"},
        {1, "标志天下宗师队伍2"},
        {2, "标志天下宗师队伍3"}
    };
    std::map<int, std::string> mood {
            {0, "标志宗门生产心情1"},
            {1, "标志宗门生产心情2"},
            {2, "标志宗门生产心情3"}
    };
    /**
     *  1. 宗门试炼设置目标判断
     *  2. 宗门试炼五军
     *  3. 宗门试炼七星
     *  4. 宗门试炼八门
     */
    std::vector<bool> record_event{true, true, true, true};
    // std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //SECTTASK_H
