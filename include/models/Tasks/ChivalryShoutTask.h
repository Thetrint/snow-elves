//
// Created by y1726 on 24-7-26.
//

#ifndef CHIVALRYSHOUTTASK_H
#define CHIVALRYSHOUTTASK_H
#include "models/Tasks/BasicTask.h"

class ChivalryShoutTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    ChivalryShoutTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs)
    : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {
        // 在构造函数体内进行复杂的初始化
        QJsonArray  jsonArray  = config.value("侠缘喊话内容").toArray();
        content.reserve(jsonArray.size());

        for (const QJsonValue& item : jsonArray) {
            if (item.isString()) {
                content.push_back(item.toString().toStdString());
            } else {
                std::cerr << "Warning: JsonValue is not a string." << std::endl;
            }
        }
    }


    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::string text;
    std::vector<std::string> content;
    std::vector<int> record_num{1};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //CHIVALRYSHOUTTASK_H
