//
// Created by y1726 on 2024/6/30.
//
#include "models/Tasks/FactionTask.h"
#include <iostream>

int FactionTask::implementation() {
    // key_down_up("B");L
    // ClickImageMatch(ImageProcessor::MatchParams{.similar = 0.75}, L"世界区域");
    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();

    // 执行函数
    CoortImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false, .edgeThreshold = {100, 200}}, nullptr, "自动寻路中");

    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算持续时间
    std::chrono::duration<double> duration = end - start;

    // 打印执行时间
    std::cout << "CoortImageMatch 执行时间: " << duration.count() << " 秒" << std::endl;

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