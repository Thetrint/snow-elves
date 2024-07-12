//
// Created by y1726 on 2024/6/30.
//
#include <iostream>
#include "models//Tasks/LessonTask.h"

#include <models/WindowManager.h>

int LessonTask::implementation() {

    std::vector<Match> matchs;

    objective("位置检测");

    while (unbind_event) {
        switch (int sw = determine()) {
            case 0:
                return 0;
            case -1:
                break;
            case 1:
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.6}, "按钮确定");
                }
                ClickImageMatch({.similar = 0.5}, "按钮关闭");
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.6}, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.6}, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.6}, "按钮活动江湖");
                ClickImageMatch(MatchParams{.similar = 0.6}, "按钮活动濯剑");
                if (ClickImageMatch(MatchParams{.similar = 0.6, .y = 45}, "按钮活动濯剑").empty()) {
                    objective("任务退出");
                    continue;
                }
                ClickImageMatch(MatchParams{.similar = 0.6, .scope = {170, 127, 404, 582}}, "按钮前往");
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.6}, "按钮大世界课业");
                ClickImageMatch(MatchParams{.similar = 0.5}, "按钮课业确定");
                for (int i = 0; i <= 8; i++) {
                    if (ClickImageMatch(MatchParams{.similar = 0.6}, "按钮课业困难").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.6}, "按钮课业刷新");
                        ClickImageMatch(MatchParams{.similar = 0.5}, "按钮确定");
                    }else {
                        break;
                    }
                }
                Close({.similar = 0.75}, "按钮关闭");
                objective("等待完成");
                break;
            case 4:
                break;
            default:
                break;;
        }

    }

    return 0;


    // while (true) {
    //     // 休眠 2000 毫秒（即2秒）
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // }
}

void LessonTask::objective(const std::string ve) {
    cause = ve;
}

int LessonTask::determine() {
    const int sw = detect();

    if (cause == "任务退出") {
        switch (sw) {
            case 1:
                return 0;
            default:
                return -3;
        }
    }

    if (cause == "位置检测") {
        switch (sw) {
            case 1:
                return 1;
            default:
                return -3;
        }
    }

    if (cause == "队伍检测") {
        switch (sw) {
            case 1:
                return 2;
            default:
                return -3;
        }
    }

    if (cause == "开始任务") {
        switch (sw) {
            case 1:
                return 3;
            default:
                return -3;
        }
    }

    if (cause == "等待完成") {
        switch (sw) {
            case 1:
                return 4;
            default:
                return -3;
        }
    }

    return -6;
}

int LessonTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.75, .applyGaussianBlur = false}, "界面大世界").empty()) {
        return 1;
    }

    return -5;

}