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
    : id(id), hwnd(hwnd), disrupted(false), unbind_event(true), LOCK(false) {
    LoadJsonFile::instance().LoadFile(id);
}

TaskManager::~TaskManager() {
    LoadJsonFile::instance().jsonFiles.erase(id);
}

void TaskManager::stop() {
    unbind_event = false;
    pause_event.unlock(); // 解除任何可能的暂停状态
}

void TaskManager::pause() {
    if(!LOCK) {
        std::cout << "锁定" << std::endl;
        pause_event.lock();
        LOCK = true;
    }

}

void TaskManager::resume() {
    WindowManager::setWinodw(hwnd);
    pause_event.unlock();
    LOCK = false;
}

void TaskManager::setState(const std::string &task) const {
    if (task != "占位任务") {
        emit Signals::instance()->Log(id, task);
    }


}
void TaskManager::start(){
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        emit Signals::instance()->setPersion(id, hwnd);


        // 创建一个 vector 用于存储解码后的值
        std::vector<std::string> tasks;

        for (const auto& task : LoadJsonFile::instance().jsonFiles[id].value("执行任务").toArray()) {
            tasks.push_back(task.toString().toStdString());
            std::cout << task.toString().toStdString() << std::endl;
        }

        const auto rol = Factory::instance().create("切换角色", id, hwnd, pause_event, unbind_event, disrupted);

        TaskSchedul schedul(tasks, rol, id);

        std::string task;
        while ((task = schedul.get_task()).empty() == false && unbind_event) {
            try {
                //更新状态
                setState(task);
                auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event, disrupted);
                if (const int result = obj->implementation(); result == -1) {
                }else {
                    if (task != "占位任务") {
                        emit Signals::instance()->Log(id, task + "结束");
                    }

                }

                obj.reset();
            } catch (const std::exception& e) {
                spdlog::error("Failed to thread: {}", e.what());
            }

        }

    } catch (const std::exception& e) {
        spdlog::error("Failed to thread: {}", e.what());
    }





}



