//
// Created by y1726 on 24-7-15.
//
#include "models/Tasks/PlaceTask.h"

int PlaceTask::implementation() {
    spdlog::info("任务执行");
    std::vector<Match> matchs;

    objective("开始任务");
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
                Close();
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
                Close();
                objective("开始任务");
                break;
            case 3:
                // mouse_move({}, {100, 300}, {900, 300});
                objective("任务退出");
                break;

            default:
                break;;
        }

    }

    return 0;


}

void PlaceTask::objective(const std::string ve) {
    cause = ve;
}

int PlaceTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (detect_count++ >= 15) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        return 307;
    }
    detect_count = 0;

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

int PlaceTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界").empty()) {
        return 1;
    }


    return -5;

}