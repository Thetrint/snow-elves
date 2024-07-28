//
// Created by y1726 on 24-7-16.
//
#include "models//Tasks/HeroListTask.h"

#include <utils/LoadJsonFile.h>

int HeroListTask::implementation() {

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
                Close({.similar = 0.5}, 1);
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
                Close({.similar = 0.5}, 1);
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮活动纷争");

                if (ClickImageMatch(MatchParams{.similar = 0.6, .y = 45}, nullptr, "按钮活动江湖英雄榜").empty()) {
                    objective("任务退出");
                    continue;
                }
                Defer(4);
                break;
            case 4:
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "标志江湖英雄榜次数").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮江湖英雄榜晋级赛", "按钮江湖英雄榜匹配");
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮江湖英雄榜确定");
                    objective("等待战斗");
                    continue;
                }
                objective("任务退出");
                break;
            case 5:
                if (!ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 20, .click = NoTap}, std::make_unique<CAUSE>(cause, "开始任务"), "标志江湖英雄榜战斗时间", "标志江湖英雄榜准备时间").empty()) {
                    if (LoadJsonFile::instance().jsonFiles[id].value("江湖英雄榜秒退").toBool()) {
                        ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮江湖英雄榜退出");
                        ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮江湖英雄榜退出副本");
                        ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 400}, nullptr, "按钮论剑离开");
                    }else {
                        if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志江湖英雄榜准备时间").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮江湖英雄榜准备");
                        }
                        if (!ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 60, .click = NoTap}, nullptr, "标志江湖英雄榜战斗时间").empty()) {
                            key_keep("W", 4000);
                            AutoFight();
                            ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 400}, nullptr, "按钮论剑离开");
                            FightStop();
                        }
                    }
                    PassLevel();
                    Log(std::format("江湖英雄榜完成 {} 次", record_num[0]));
                    if (++record_num[0] >= config.value("江湖英雄榜次数").toInt() + 1) {
                        objective("任务退出");
                        continue;
                    }
                    objective("开始任务");
                }
                break;
            default:
                break;;
        }

    }

    return 0;


}

void HeroListTask::objective(const std::string ve) {
    cause = ve;
}

int HeroListTask::determine() {
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

    if (cause == "等待战斗") {
        switch (sw) {
            case 1:
                return 5;
            default:
                return -1;
        }
    }


    return 307;
}

int HeroListTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面江湖英雄榜").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}