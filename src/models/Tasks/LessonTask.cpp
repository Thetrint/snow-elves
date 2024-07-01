//
// Created by y1726 on 2024/6/30.
//
#include <iostream>
#include "models//Tasks/LessonTask.h"

void LessonTask::implementation() {
    if (!unbind_event.try_acquire()) {
        std::cout << "解码后的值: 1"  << std::endl;
    }

    // while (true) {
    //     // 休眠 2000 毫秒（即2秒）
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // }
}

void LessonTask::objective() {

}

int LessonTask::determine() {

    return 0;
}

int LessonTask::detect() {

    return 0;

}