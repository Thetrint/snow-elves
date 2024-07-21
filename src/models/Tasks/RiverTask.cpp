//
// Created by y1726 on 24-7-17.
//
#include "models/Tasks/RiverTask.h"

int RiverTask::implementation() {
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
                Close(3);
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
                    ClickImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close(1);
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品山河器");
                break;
            case 4:
                if (ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮山河器探索").empty()) {
                    if(ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮山河器免费搜索").empty()) {
                        // 退出
                        Close(3);
                        objective("任务退出");
                        continue;
                    }
                    Log("搜索山河器");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮山河器日晷");
                    continue;
                }
                Arrive();
                Log("拾取山河器");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮山河器拾取");
                mouse_down_up({}, {0, 0});
                mouse_down_up({}, {0, 0});
                break;
            default:
                break;;
        }

    }

    return 0;


}

void RiverTask::objective(const std::string ve) {
    cause = ve;
}

int RiverTask::determine() {
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
            case 2:
                return 4;
            default:
                return -1;
        }
    }


    return 307;
}

int RiverTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面山河器").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}