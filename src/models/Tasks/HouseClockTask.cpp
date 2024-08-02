//
// Created by y1726 on 24-8-2.
//
#include "models/Tasks/HouseClockTask.h"

int HouseClockTask::implementation() {
    std::vector<Match> matchs;
    objective("位置检测");
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
                Close({.similar = 0.5}, 3);
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮退出");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮前往金陵");
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
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品宅邸");

                if (!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮宅邸不再显示").empty()) {
                    Close({.similar = 0.5}, 1);
                }

                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮宅邸街坊");

                for (auto&[location, score] : CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志宅邸江南坊")) {
                    mouse_down_up({}, location);

                    for (auto&[location, score] : CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志宅邸房子")) {
                        mouse_down_up({}, location);
                        if (CoortImageMatch(MatchParams{.similar = 0.5, .matchCount = 1}, nullptr, "按钮宅邸发送消息").empty()) {
                            mouse_down_up({}, {0, 0});
                            continue;
                        }
                        if (ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 1}, nullptr, "按钮宅邸拜访宅邸").empty()) {
                            mouse_down_up({}, {0, 0});
                            continue;
                        }
                        PassLevel();
                        Defer(25, 1000);
                        CloseReward(3);
                        if(++record_num[0] >= 8) {
                            objective("任务退出");
                            break;
                        }
                    }

                    Close({.similar = 0.5}, 1);
                    if(record_num[0] >= 5) {
                        Close({.similar = 0.5}, 2);
                        break;
                    }

                }

                if(record_num[0] >= 5) {
                    continue;
                }
                mouse_move({}, {1000, 625}, {1008, 325});

                break;

            default:
                break;;
        }

    }

    return 0;


}

void HouseClockTask::objective(const std::string ve) {
    cause = ve;
}

int HouseClockTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 10) {
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

int HouseClockTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}