//
// Created by y1726 on 24-7-17.
//
#include "models/Tasks/TeaStoryTask.h"

int TeaStoryTask::implementation() {
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
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动江湖");
                if (ClickImageMatch(MatchParams{.similar = 0.5, .y = 45}, nullptr, "按钮活动茶馆说书").empty()) {
                    objective("任务退出");
                    continue;
                }
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮茶馆说书进入茶馆");
                objective("等待完成");
                break;
            case 4:
                if (!ClickImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, std::make_unique<CAUSE>(cause, "开始任务"), "界面茶馆").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮茶馆说书甲", "按钮茶馆说书乙", "按钮茶馆说书丙", "按钮茶馆说书丁");

                    if (!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮茶馆说书退出茶馆").empty()) {
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

void TeaStoryTask::objective(const std::string ve) {
    cause = ve;
}

int TeaStoryTask::determine() {
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
            case 1:
                return 4;
            default:
                return -1;
        }
    }


    return 307;
}

int TeaStoryTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}