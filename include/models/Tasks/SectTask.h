//
// Created by y1726 on 24-7-25.
//

#ifndef SECTTASK_H
#define SECTTASK_H
#include "models/Tasks/BasicTask.h"

class SectTask final : public BasicTask{


public:
    SectTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config), index(0) {
    }

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::vector<Match> matche;
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

    std::map<std::string, std::string> moods {
                {"五军之炼", "按钮宗门试炼五军"},
                {"七星之炼", "按钮宗门试炼七星"},
                {"八门之炼", "按钮宗门试炼八门"}
    };
    /**
     *  1. 宗门试炼设置目标判断
     *  2. 宗门试炼五军
     *  3. 宗门试炼七星
     *  4. 宗门试炼八门
     *  5. 宗门试炼快速战斗
     *  6. 门人选择滑动
     *
     */
    std::vector<bool> record_event{true, true, true, true, true, true};
    std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //SECTTASK_H
