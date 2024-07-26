//
// Created by y1726 on 24-7-20.
//
#include "models/Tasks/DoorBanTask.h"

int DoorBanTask::implementation() {
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
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动帮派");

                if (ClickImageMatch(MatchParams{.similar = 0.6, .y = 45}, nullptr, "按钮活动门客设宴").empty()) {
                    objective("任务退出");
                    continue;
                }
                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮活动前往邀约");
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 1}, nullptr, "按钮门客设宴邀请赴宴");
                ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 1}, nullptr, "按钮门客设宴确认邀约");
                if (CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面门客设宴").empty()) {
                    objective("任务退出");
                }
                break;
            case 4:
                mouse_down_up({}, {633 + 172 * x, 282 + 182 * y});
                if(!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮门客设宴获取").empty()) {
                    for (int i = 1; i <= 6; i++) {
                        if(ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 2}, nullptr, "按钮门客设宴获取").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 2}, nullptr, "按钮门客设宴一键提交");
                            break;
                        }

                        if(i % 2 == 1) {
                            ClickImageMatch(MatchParams{.similar = 0.5, .y = -45}, nullptr, "按钮大世界帮派仓库");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮帮派仓库提交");
                            if(!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面帮派仓库").empty()) {
                                Close({.similar = 0.5}, 1);
                            }
                        }else {

                            if(!ClickImageMatch(MatchParams{.similar = 0.5, .y = -45}, nullptr, "按钮大世界摆摊购买").empty()) {
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮交易查看全服");

                                if(ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮交易购买").empty()) {
                                    Close({.similar = 0.5}, 1);
                                    continue;
                                }
                                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮确定");
                                continue;
                            }

                            if(!ClickImageMatch(MatchParams{.similar = 0.5, .y = -45}, nullptr, "按钮大世界商城购买").empty()) {
                                mouse_down_up({.clickCount = 1}, {988, 694});
                                Close({.similar = 0.5}, 1);
                            }
                        }

                    }
                }else {
                    ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 2}, nullptr, "按钮门客设宴一键提交");
                }

                if(++x == 5) {
                    x = 0;
                    if (++y == 2) {
                        ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 2}, nullptr, "按钮门客设宴开始设宴");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮确定");
                        Close({.similar = 0.5}, 4);
                        objective("任务退出");
                    }

                }


                break;
            default:
                break;;
        }

    }

    return 0;


}

void DoorBanTask::objective(const std::string ve) {
    cause = ve;
}

int DoorBanTask::determine() {
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
            case 2:
                return 4;
            default:
                return -1;
        }
    }


    return 307;
}

int DoorBanTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面门客设宴").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}