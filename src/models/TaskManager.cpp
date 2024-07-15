//
// Created by y1726 on 2024/6/26.
//


#include "models//TaskManager.h"

#include <codecvt>
#include <utils/signals.h>

#include "models/WindowManager.h"
#include "models/TaskSchedul.h"
#include "utils/Factory.h"
#include "utils/LoadJsonFile.h"
#include "utils/FunctionLibrary.h"

TaskManager::TaskManager(int id, HWND hwnd)
    : id(id), hwnd(hwnd), disrupted(false), unbind_event(true) {
}


void TaskManager::stop() {
    unbind_event = false;
    pause_event.unlock(); // 解除任何可能的暂停状态
}

void TaskManager::pause() {
    if (!pause_event.try_lock()) {
        std::cout << "Failed to pause, task is already paused or lock is held" << std::endl;
    }
}

void TaskManager::resume() {
    WindowManager::setWinodw(hwnd);
    pause_event.unlock();
}

void TaskManager::setState(const std::string &task) const {
    spdlog::info("状态设置窗口 {}: {}", id, task);
    emit Signals::instance()->Log(id, task);

}
void TaskManager::start(){
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    emit Signals::instance()->setPersion(id, hwnd);

    LoadJsonFile::instance().LoadFile(id);

    // 创建一个 vector 用于存储解码后的值
    std::vector<std::string> tasks;

    for (const auto& task : LoadJsonFile::instance().file_0.value("执行任务").toArray()) {
        tasks.push_back(task.toString().toStdString());
        std::cout << task.toString().toStdString() << std::endl;
    }

    TaskSchedul schedul(tasks);
    schedul.init();

    std::string task;
    while ((task = schedul.get_task()).empty() == false && unbind_event) {
        try {
            setState(task);
            auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event, disrupted);

            if (const int result = obj->implementation(); result == -1) {
            }
            obj.reset();
        } catch (const std::exception& e) {
            spdlog::error("Failed to thread: {}", e.what());
        }

    }



}



