//
// Created by y1726 on 24-7-16.
//
#include "models/Tasks/VientianeTask.h"

int VientianeTask::implementation() {
    spdlog::info("任务执行");
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
                if (OpenMap()) {
                    ClickImageMatch({.similar = 0.6}, nullptr, "按钮地图停止寻路");
                    ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
                    ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
                    input_text("307");

                    ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
                    input_text("360");

                    ClickImageMatch({.similar = 0.75}, nullptr, "按钮地图前往区域");
                    ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                    Arrive();
                    ClickImageMatch({.similar = 0.5}, nullptr, "按钮大世界拍照");

                }

                break;
            case 4:
                ClickImageMatch({.similar = 0.5, .matchCount = 1}, nullptr, "按钮场景环绕旋转");
                ClickImageMatch({.similar = 0.5, .matchCount = 1}, nullptr, "按钮场景确定");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮场景万象");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮场景拍照");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮江湖万象打卡");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮江湖万象上传打卡");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮确定");
                Defer(2);
                break;
            case 5:
                ClickImageMatch({.similar = 0.5, .x = -240, .y = -55}, nullptr, "按钮拍照打卡立刻拍照");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮拍照打卡立刻拍照");

                if(++record_num[0] >= 20) {
                    objective("任务退出");
                }
            default:
                break;;
        }

    }

    return 0;


}

void VientianeTask::objective(const std::string ve) {
    cause = ve;
}

int VientianeTask::determine() {
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
            case 3:
                return 5;
            default:
                return -1;
        }
    }


    return 307;
}

int VientianeTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面拍照打卡").empty()) {
        return 3;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面场景").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}
