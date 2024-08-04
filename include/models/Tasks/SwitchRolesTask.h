//
// Created by y1726 on 24-7-15.
//

#ifndef SWITCHROLESTASK_H
#define SWITCHROLESTASK_H
#include "models/Tasks/BasicTask.h"

class SwitchRolesTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    SwitchRolesTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;

    std::vector<bool> roles = {true, true, true, true, true, true};
private:

    std::string cause;


};

#endif //SWITCHROLESTASK_H