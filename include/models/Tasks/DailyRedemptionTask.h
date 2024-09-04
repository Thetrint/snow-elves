//
// Created by y1726 on 24-7-21.
//

#ifndef DAILYREDEMPTIONTASK_H
#define DAILYREDEMPTIONTASK_H
#include "models/Tasks/BasicTask.h"

class DailyRedemptionTask final : public BasicTask{

public:
    DailyRedemptionTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs, const QJsonObject& config)
        : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs, config) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;


private:
    std::vector<std::string> Ingredients;
    /**
     * 0. 银票礼盒
     * 1. 帮派捐献
     * 2. 商会购买
     * 3. 锦芳绣残片
     * 4. 神厨食材
     * 5. 生活技能-莲子
     * 6. 生活技能-艾草
     * 7. 商票上缴
     */
    std::vector<bool> record_event{true, true, true, true, true, true, true, true};
    // std::vector<int> record_num{0};
    // std::vector<std::chrono::steady_clock::time_point> record_time{0};

};
#endif //DAILYREDEMPTIONTASK_H
