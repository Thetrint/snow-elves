//
// Created by y1726 on 24-7-21.
//
#include "models/Tasks/DailyRedemptionTask.h"

#include <utils/LoadJsonFile.h>

int DailyRedemptionTask::implementation() {
    std::vector<Match> matchs;
    objective("开始任务");
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720)) {
            return 0;
        }

        switch (determine()) {
            case 0:
               Close({.similar = 0.5}, 3);;
                return 0; // 任务正常退出
            case -1:
                Close({.similar = 0.5}, 1);
                break;
            case 1:
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                objective("开始任务");
                break;
            case 3:
                if (config.value("银票礼盒兑换").toBool() && record_event[0]) {
                    record_event[0] = false;
                    OpenKnapsack();
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品珍宝阁");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮珍宝阁商城");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮珍宝阁搜索");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志珍宝阁输入名称");
                    input_text("银票礼盒");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮珍宝阁搜索");
                    if(!ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志珍宝阁银票礼盒").empty()) {
                        mouse_down_up({.clickCount = 30}, {988, 694});
                    }
                    Close({.similar = 0.5}, 4);
                    continue;
                }

                if (config.value("帮派铜钱捐献").toBool() && config.value("帮派银两捐献").toBool() && record_event[1]) {
                    record_event[1] = false;
                    OpenFaction();
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮帮派福利");
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮帮派捐献");

                    if(config.value("帮派铜钱捐献").toBool()) {
                        for(int i = 1; i <= 3; i++) {
                            // ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮捐献");
                        }
                    }

                    if(config.value("帮派银两捐献").toBool()) {
                        for(int i = 1; i <= 3; i++) {
                            // ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮捐献");
                        }
                    }
                    Close({.similar = 0.5}, 4);
                }


                objective("任务退出");
                return 0;
                break;

            default:
                break;;
        }

    }

    return 0;


}

void DailyRedemptionTask::objective(const std::string ve) {
    cause = ve;
}

int DailyRedemptionTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 15) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }else {
        detect_count = 0;
    }


    if (cause == "任务退出") {
        switch (sw) {
            case 1:
                return 0;
            default:
                return -1;
        }
    }

    if (cause == "位置检测") {
        switch (sw) {
            case 1:
                return 1;
            default:
                return -1;
        }
    }

    if (cause == "队伍检测") {
        switch (sw) {
            case 1:
                return 2;
            default:
                return -1;
        }
    }

    if (cause == "开始任务") {
        switch (sw) {
            case 1:
                return 3;
            default:
                return -1;
        }
    }


    return 307;
}

int DailyRedemptionTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}