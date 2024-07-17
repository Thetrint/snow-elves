//
// Created by y1726 on 24-7-16.
//
#include "models/Tasks/TheSwordThreeTask.h"

#include <utils/LoadJsonFile.h>

int TheSwordThreeTask::implementation() {
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
                return 0; // 任务正常退出
            case -1:
                Close(1);
                break;
            case 1:
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                }
                Close(1);
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮活动纷争");

                if (ClickImageMatch(MatchParams{.similar = 0.6, .x = 60, .y = 45}, nullptr, "按钮活动华山论剑").empty()) {
                    objective("任务退出");
                    continue;
                }
                objective("等待战斗");
                break;
            case 4:
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮论剑取消匹配").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮论剑匹配");
                }
                ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 1}, nullptr, "按钮论剑确认");
                break;
            case 5:
                if (!ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 20, .click = NoTap}, std::make_unique<CAUSE>(cause, "开始任务"), "标志论剑战斗时间", "标志论剑准备时间").empty()) {

                if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志论剑准备时间").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮论剑准备");
                }
                if (!ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 60, .click = NoTap}, nullptr, "标志论剑战斗时间").empty()) {
                    key_keep("W", 3000);
                    AutoFight();
                    ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 400}, nullptr, "按钮论剑离开");
                    FightStop();
                }

                    if (++record_num[0] >= LoadJsonFile::instance().jsonFiles[id].value("华山论剑次数").toInt()) {
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

void TheSwordThreeTask::objective(const std::string ve) {
    cause = ve;
}

int TheSwordThreeTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 15) {
            return -1;
        }
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

    if (cause == "等待战斗") {
        switch (sw) {
            case 1:
                return 5;
            case 2:
                return 4;
            default:
                return -1;
        }
    }



    return 307;
}

int TheSwordThreeTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面论剑").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}