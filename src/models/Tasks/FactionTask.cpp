//
// Created by y1726 on 2024/6/30.
//
#include "models/Tasks/FactionTask.h"
#include <iostream>

void FactionTask::implementation() {
    if (!unbind_event.try_acquire()) {
        std::cout << "解码后的值: 2"  << std::endl;
    }

    // while (true) {
    //     // 休眠 2000 毫秒（即2秒）
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // }
}

void FactionTask::objective() {

}

int FactionTask::determine() {

    return 0;
}

int FactionTask::detect() {

    return 0;

}