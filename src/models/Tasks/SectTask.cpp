//
// Created by y1726 on 24-7-25.
//
#include "models/Tasks/SectTask.h"

int SectTask::implementation() {
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
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品天下宗师");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师回宗门");
                PassLevel();

                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品天下宗师");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师宗门玩法");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮宗门玩法以武会友");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮以武会友匹配");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志天下宗师队伍1");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师确认");
                if(ClickImageMatch(MatchParams{.similar = 0.5, .click = NoTap}, nullptr, "标志以武会友匹配消耗").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师确定");
                    ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 150}, nullptr, "按钮天下宗师返回");
                }else {
                    ClickImageMatch(MatchParams{.similar = 0.5, .scope = {318, 468, 611, 595}}, nullptr, "按钮取消");
                    Close({.similar = 0.5, .click = FORWARD}, 3);
                }

                if (config.value("宗门生产").toBool()) {
                    objective("宗门生产");
                    continue;
                }
                objective("宗门试炼");
                break;
            case 4:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品天下宗师");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师生产");
                ClickImageMatch(MatchParams{.similar = 0.5, .x = -60}, nullptr, "按钮宗门生产劳作中");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮宗门生产一键停工");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                ClickImageMatch(MatchParams{.similar = 0.6, .x = -60}, nullptr, "按钮宗门生产空闲中");
                mouse_move({.moveCount = 30}, {466, 582}, {466, 282});
                break;
            case 5:

                ClickImageMatch(MatchParams{.similar = 0.5, .click = LAST, .x = -90, .y = 85}, nullptr, mood[config.value("宗门生产心情等级").toInt()]);
                if(ClickImageMatch(MatchParams{.similar = 0.6,}, nullptr, "按钮宗门生产工作").empty()) {
                    mouse_move({}, {934, 461}, {934, 261});
                    if(ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮宗门生产工作").empty()) {
                        mouse_down_up({}, {0, 0});
                        if(config.value("宗门生产一键催命").toBool()) {
                            ClickImageMatch(MatchParams{.similar = 0.6, .x = -60}, nullptr, "按钮宗门生产劳作中");
                            ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮宗门生产一键催命");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                        }
                        Close({.similar = 0.5}, 3);
                        objective("宗门试炼");
                    }
                }
                break;
            case 6:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品天下宗师");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师宗门玩法");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮宗门玩法宗门试炼");
                break;
            case 7:

                if(record_event[0]) {
                    record_event[0] = false;
                    if(config.value("宗门试炼1").toString() == "五军之炼" and record_event[1]) {
                        record_event[1] = false;
                        ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮宗门试炼五军");
                        index = config.value("宗门试炼队伍1").toInt();
                        continue;
                    }

                    if(config.value("宗门试炼2").toString() == "七星之炼" and record_event[2]) {
                        record_event[2] = false;
                        ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮宗门试炼七星");
                        index = config.value("宗门试炼队伍2").toInt();
                        continue;
                    }

                    if(config.value("宗门试炼3").toString() == "八门之炼" and record_event[3]) {
                        record_event[3] = false;
                        ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮宗门试炼八门");
                        index = config.value("宗门试炼队伍3").toInt();
                        continue;
                    }
                    objective("任务退出");
                    continue;

                }


                if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮宗门试炼挑战").empty()) {
                    std::vector<Match> match = ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, team[index]);
                    if(match.empty()) {
                        record_event[0] = true;
                        continue;
                    }
                    if (!CoortImageMatch(MatchParams{.similar = 0.5, .scope = {match.front().location.x - 60, match.front().location.y + 110, match.front().location.x + 120, match.front().location.y + 220}}, nullptr, "按钮宗门试炼体力不足").empty()) {
                        record_event[0] = true;
                        Close({.similar = 0.5}, 1);
                        continue;
                    }
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师补充气血");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮气血补充快速分药");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮气血补充确定");
                    Close({.similar = 0.5, .scope = {1085, 0, 1242, 211}}, 1);
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮天下宗师确认");
                    ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 150}, nullptr, "按钮天下宗师返回");
                }



                break;
            default:
                break;;
        }

    }

    return 0;


}

void SectTask::objective(const std::string ve) {
    cause = ve;
}

int SectTask::determine() {
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

    if (cause == "宗门生产") {
        switch (sw) {
            case 1:
                return 4;
            case 3:
                return 5;
            default:
                return 307;
        }
    }

    if (cause == "宗门试炼") {
        switch (sw) {
            case 1:
                return 6;
            case 2:
                return 7;
            default:
                return 307;
        }
    }


    return 307;
}

int SectTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面宗门生产").empty()) {
        return 3;
    }

    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面宗门试炼").empty()) {
        return 2;
    }

    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}