//
// Created by y1726 on 2024/6/30.
//
#include <iostream>
#include "models//Tasks/LessonTask.h"

#include <models/WindowManager.h>

int LessonTask::implementation() {

    std::vector<Match> matchs;

    objective("等待完成");

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
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                }
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮活动江湖");

                if (ClickImageMatch(MatchParams{.similar = 0.6, .y = 45}, nullptr, "按钮活动吟风", "按钮活动含灵", "按钮活动寻道", "按钮活动归义", "按钮活动悟禅", "按钮活动止杀", "按钮活动漱尘", "按钮活动濯剑", "按钮活动观梦", "按钮活动锻心").empty()) {
                    objective("任务退出");
                    continue;
                }
                ClickImageMatch(MatchParams{.similar = 0.6, .scope = {170, 127, 404, 582}}, nullptr, "按钮前往");
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界课业");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮课业确定");
                for (int i = 0; i <= 8; i++) {
                    if (ClickImageMatch(MatchParams{.similar = 0.6, .click = FIRST}, nullptr, "按钮课业困难").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.6, .x = -55}, nullptr, "按钮课业刷新");
                        ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                    }else {
                        break;
                    }
                }
                Close();
                objective("等待完成");
                break;
            case 4:

                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 30) {
                    record_time[0] = std::chrono::steady_clock::now();
                    if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界江湖").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                    }
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界江湖");
                    ClickImageMatch(MatchParams{.similar = 0.85, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮大世界吟风任务", "按钮大世界濯剑任务", "按钮大世界锻心任务");
                }


                if (!ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界一键提交").empty()) {
                    if (!ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定").empty()) {
                        objective("任务退出");
                    }

                }
                break;
            case 5:
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮交易购买");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                Close();
                break;
            case 6:
                Close();
                break;
            case 7:
                mouse_down_up(MatchParams{}, cv::Point{1003, 617});
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
            case 2:
                return 5;
            case 3:
                return 6;
            case 4:
                return 7;
            default:
                return -3;
        }
    }

    return -6;
}

int LessonTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.75, .applyGaussianBlur = false}, nullptr, "界面大世界").empty()) {
        return 1;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易", "按钮交易购买").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志课业答题").empty()) {
        return 3;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志课业杂货商人").empty()) {
        return 4;
    }

    return -5;

}