//
// Created by y1726 on 2024/6/30.
//
#include <iostream>
#include "models//Tasks/LessonTask.h"

#include <models/WindowManager.h>

int LessonTask::implementation() {

    std::vector<ImageProcessor::Match> matchs;
    // ClickImageMatch(ImageProcessor::MatchParams{.similar = 0.95}, matchs, L"活动");



    // ClickImageMatch(ImageProcessor::MatchParams{.similar = 0.95}, matchs, L"活动");


    objective("位置检测");
    while (unbind_event) {

        int sw = determine();

        switch (sw) {
            case 0:
                return 0;
            case -1:

                break;
            case 1:
                // ClickImageMatch(ImageProcessor::MatchParams{.similar = 0.98}, matchs, L"活动");
                break;
            default:
                break;;
        }

    }

    return 0;
    //
    // ClickImageMatch(TM_CCORR_NORMED, L"活动", L"包裹");
    // ClickImageMatch(TM_SQDIFF_NORMED, L"活动", L"包裹");


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
    return -6;
}

int LessonTask::detect() {
    if (std::vector<ImageProcessor::Match> matchs; CoortImageMatch(ImageProcessor::MatchParams{.similar = 0.75}, matchs, L"主界面") && !matchs.empty()) {
        return 1;
    }

    return -5;

}