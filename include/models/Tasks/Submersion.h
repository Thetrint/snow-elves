//
// Created by y1726 on 24-7-13.
//

#ifndef SUBMERSION_H
#define SUBMERSION_H
#include "models/Tasks/BasicTask.h"


class Submersion final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    bool& disrupted;
    std::ifstream& ifs;
    const QJsonObject& config;
public:
    Submersion(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
    : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config)
    , id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), disrupted(disrupted), ifs(ifs), config(config){}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::string cause;
    std::vector<std::chrono::steady_clock::time_point> record_time{1};
    std::vector<Match> matches;
    std::vector<Match> last_matches;
    std::vector<Match> result;

};

#endif //SUBMERSION_H
