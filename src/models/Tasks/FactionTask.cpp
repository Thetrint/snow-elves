//
// Created by y1726 on 2024/6/30.
//
#include "models/Tasks/FactionTask.h"
#include <iostream>
#include <models/WindowManager.h>


int FactionTask::implementation() {
    std::vector<Match> matchs;
    objective("位置检测");
    timer.start();
    spdlog::info("计时器启动");
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
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮活动帮派");

                if (ClickImageMatch(MatchParams{.similar = 0.6, .y = 45}, nullptr, "按钮活动帮派任务").empty()) {
                    objective("任务退出");
                    continue;
                }
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮帮派任务帮派任务");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮课业确定");

                objective("等待完成");
                break;
            case 4:
                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 30) {
                    record_time[0] = std::chrono::steady_clock::now();
                    if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界江湖").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                    }
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界江湖");
                    ClickImageMatch(MatchParams{.similar = 0.85, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮大世界帮派任务");
                }

                if (!CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界帮派仓库", "按钮大世界摆摊购买").empty()) {
                    record_time[0] = std::chrono::steady_clock::time_point{};
                    if (!ClickImageMatch(MatchParams{.similar = 0.6, .y = -65}, nullptr, "按钮大世界帮派仓库").empty()) {
                        if (!ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮帮派仓库提交").empty()) {
                            Arrive();
                        }
                        Close(2);
                    }
                    ClickImageMatch(MatchParams{.similar = 0.85, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮大世界帮派任务");
                    if (!ClickImageMatch(MatchParams{.similar = 0.6, .y = -65}, nullptr, "按钮大世界摆摊购买").empty()) {
                        if (ClickImageMatch(MatchParams{.similar = 0.6, .scope = {259, 410, 1076, 602}}, nullptr, "按钮交易购买").empty()) {
                            Close(3);
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮任务任务");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮任务江湖");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮任务回帮派复命");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮任务重新接取");
                            ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                            Close(3);
                        }
                        ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                        Close(1);
                    }


                }

                if (!ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界一键提交").empty()) {
                    if (!ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "标志帮派任务已经完成").empty()) {
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

void FactionTask::objective(const std::string ve) {
    cause = ve;
}

int FactionTask::determine() {
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

    if (cause == "等待完成") {
        switch (sw) {
            default:
                return 4;
        }
    }

    return 307;
}

int FactionTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }

    return -5;

}