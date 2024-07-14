//
// Created by y1726 on 2024/6/30.
//
#include "models/Tasks/FactionTask.h"
#include <iostream>
#include <models/WindowManager.h>

int FactionTask::implementation() {
    // key_down_up("B");L
    // ClickImageMatch(ImageProcessor::MatchParams{.similar = 0.75}, L"世界区域");
    // 记录开始时间
    mouse_move({}, {0, 100}, {800, 100});

    return 0;
    // while (true) {
    //     // 休眠 2000 毫秒（即2秒）
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // }
}

void FactionTask::objective(std::string ve) {

}

int FactionTask::determine() {

    return 0;
}

int FactionTask::detect() {

    return 0;

}