//
// Created by y1726 on 24-7-14.
//
#include "utils/Utilities.h"


// Timer的析构函数
Timer::~Timer() {
    stop_flag = true;
    if (timer_thread.joinable()) {
        timer_thread.join();
    }
}

// 启动计时器
void Timer::start() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!is_running) {
        is_running = true;
        stop_flag = false;
        pause_flag = false;
        elapsed_seconds = std::chrono::seconds(0);
        timer_thread = std::jthread(&Timer::run, this);
    }else {
        resume();
    }
}

// 暂停计时器
void Timer::pause() {
    std::lock_guard<std::mutex> lock(mutex);
    if (is_running && !pause_flag) {
        pause_flag = true;
    }
}

// 恢复计时器
void Timer::resume() {
    if (is_running && pause_flag) {
        pause_flag = false;
    }
}

// 重置计时器
void Timer::reset() {
    std::lock_guard<std::mutex> lock(mutex);
    is_running = false;
    stop_flag = true;
    pause_flag = false;
    elapsed_seconds = std::chrono::seconds(0);
    if (timer_thread.joinable()) {
        timer_thread.join();
    }
}

// 读取计时器时间
std::chrono::seconds Timer::read() const {
    std::lock_guard<std::mutex> lock(mutex);
    return elapsed_seconds;
}

// 计时器线程运行函数
void Timer::run() {
    while (!stop_flag && unbind_event) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!pause_flag) {
            std::lock_guard<std::mutex> lock(mutex);
            ++elapsed_seconds;
        }
    }
}
