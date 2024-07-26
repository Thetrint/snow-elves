//
// Created by y1726 on 24-7-13.
//
#include "models/Tasks/Submersion.h"
#include "models/ImageProcess.h"
#include <models/WindowManager.h>

int Submersion::implementation() {

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
                if (CoortImageMatch({.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch({.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch({.similar = 0.6}, nullptr, "按钮确定");
                }
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                objective("开始任务");
                break;
            case 3:
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮限时活动伏月节");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮限时活动潜神入忆");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮限时活动前往");
                objective("等待完成");
                break;
            case 4:

                if (!CoortImageMatch({.similar = 0.6}, nullptr, "标志限时活动记忆时间").empty()) {
                    last_matches = CoortImageMatch({.similar = 0.998, .scope = {336, 85, 960, 658}, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr,
                        "按钮限时活动图标");
                    if (!ClickImageMatch({.similar = 0.5, .matchCount = 9, .click = NoTap}, nullptr, "标志限时活动选择时间").empty()) {
                        matches = CoortImageMatch({.similar = 0.998, .scope = {336, 85, 960, 658}, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr,
                            "按钮限时活动图标");
                        ImageProcessor::removeMatches(matches, last_matches);
                        if (!matches.empty()) {
                            mouse_down_up({.clickCount = 4, .clickDelay = false}, matches.front().location);
                        }
                    }
                }




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

void Submersion::objective(const std::string ve) {
    cause = ve;
}

int Submersion::determine() {
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
            case 2:
                return 4;
            default:
                return -3;
        }
    }

    return -6;
}

int Submersion::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志限时活动潜神入忆").empty()) {
        return 2;
    }


    return -5;
}

