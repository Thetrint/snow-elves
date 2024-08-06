//
// Created by y1726 on 2024/6/26.
//


#include "models//TaskManager.h"

#include <utils/signals.h>

#include "models/WindowManager.h"
#include "models/TaskSchedul.h"
#include "utils/Factory.h"
#include "utils/LoadJsonFile.h"
#include "utils/FunctionLibrary.h"


TaskManager::TaskManager(const int id, HWND hwnd, const QJsonDocument& configJson)
    : id(id), hwnd(hwnd), disrupted(false), unbind_event(true), LOCK(false), rolesTask(id, hwnd, pause_event, unbind_event, disrupted, ifs) {
    this->config = configJson.object();

    LoadJsonFile::instance().LoadFile(id);

    scheduler = std::make_unique<TaskScheduler>();

    scheduler->addTask(TaskScheduler::TimerTask(
        "宅邸农场任务",
        TaskScheduler::TaskMode::Interval,
        std::chrono::milliseconds(5000),
        TaskScheduler::DateTime::Builder().build(),
        []() {
            std::cout << 1 << std::endl;
        }
    ));

    // scheduler->start();
}

TaskManager::~TaskManager() {
    LoadJsonFile::instance().jsonFiles.erase(id);
}

/**
 * 任务队列初始化
 */
void TaskManager::Init() {
    for (QJsonValue task : config.value("执行任务").toArray()) {
        taskQueue.emplace(task.toString().toStdString(), 1);
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

/**
 * 获取下一个任务名称
 * @return 返回优先级别任务
 */
std::string TaskManager::getTask() {
    if (taskQueue.empty()) {
        if (LoadJsonFile::instance().jsonFiles[id].value("切角色1").toBool() && rolesTask.roles[1]) {
            // switch_rol.roles(1);
            rolesTask.roles[1] = false;
            Init();
            return "占位任务";
        }
        if (LoadJsonFile::instance().jsonFiles[id].value("切角色2").toBool() && rolesTask.roles[2]) {
            // switch_rol.roles(2);
            rolesTask.roles[2] = false;
            Init();
            return "占位任务";
        }
        if (LoadJsonFile::instance().jsonFiles[id].value("切角色3").toBool() && rolesTask.roles[3]) {
            // switch_rol.roles(3);
            rolesTask.roles[3] = false;
            Init();
            return "占位任务";
        }
        if (LoadJsonFile::instance().jsonFiles[id].value("切角色4").toBool() && rolesTask.roles[4]) {
            // switch_rol.roles(4);
            rolesTask.roles[4] = false;
            Init();
            return "占位任务";
        }
        if (LoadJsonFile::instance().jsonFiles[id].value("切角色5").toBool() && rolesTask.roles[5]) {
            // switch_rol.roles(5);
            rolesTask.roles[5] = false;
            Init();
            return "占位任务";
        }
        if (std::ranges::all_of(rolesTask.roles, [](const bool role) { return role; })) {
            rolesTask.roles[0] = false;
            Init();
            return "占位任务";
        }

        return "占位任务";  // 如果 tasks 为空，返回空字符串
    }

    Task highestPriorityTask = taskQueue.top();

    // 删除优先级最高的任务
    taskQueue.pop();
    return highestPriorityTask.name;
}

void TaskManager::stop() {
    unbind_event = false;
    scheduler->stop();
    if(LOCK) {
        pause_event.unlock(); // 解除任何可能的暂停状态
        LOCK = false;
    }
}

void TaskManager::pause() {
    if(!LOCK) {
        std::cout << "锁定" << std::endl;
        pause_event.lock();
        LOCK = true;
    }

}

void TaskManager::resume() {
    pause_event.unlock();
    LOCK = false;
}

void TaskManager::setState(const std::string &task) const {

    emit Signals::instance()->Log(id, task);



}
void TaskManager::start(){
    try {
        // 生成目标文件路径
        const std::string source_file = "resources/images.dat";
        const std::string destination_file = "resources/images_" + std::to_string(id) + ".dat";
        // 检查并删除目标文件（如果存在）
        if (std::filesystem::exists(destination_file)) {
            std::filesystem::remove(destination_file);
        }
        // 复制文件，并使用 overwrite_existing 选项自动覆盖已存在的文件
        std::filesystem::copy(source_file, destination_file, std::filesystem::copy_options::overwrite_existing);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        emit Signals::instance()->setPersion(id, hwnd);


        std::string task;
        while ((task = getTask()).empty() == false && unbind_event){
            try {
                if (task != "占位任务") {
                    //更新状态
                    setState(task);
                    auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event, disrupted, ifs);
                    if (const int result = obj->implementation(); result == -1) {

                    }
                    emit Signals::instance()->Log(id, task + "结束");
                    obj.reset();
                }else {
                    auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event, disrupted, ifs);
                    if (const int result = obj->implementation(); result == -1) {
                    }
                    obj.reset();
                }

            } catch (const std::exception& e) {
                spdlog::error("Failed to thread: {}", e.what());
            }
        }

        // // 创建一个 vector 用于存储解码后的值
        // std::vector<std::string> tasks;
        //
        // for (const auto& task : LoadJsonFile::instance().jsonFiles[id].value("执行任务").toArray()) {
        //     tasks.push_back(task.toString().toStdString());
        //     std::cout << task.toString().toStdString() << std::endl;
        // }
        //
        // const auto rol = Factory::instance().create("切换角色", id, hwnd, pause_event, unbind_event, disrupted, ifs);
        //
        // TaskSchedul schedul(tasks, rol, id);

        // std::string task;
        // while ((task = schedul.get_task()).empty() == false && unbind_event) {
        //     try {
        //         if (task != "占位任务") {
        //             //更新状态
        //             setState(task);
        //             auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event, disrupted, ifs);
        //             if (const int result = obj->implementation(); result == -1) {
        //
        //             }
        //
        //             emit Signals::instance()->Log(id, task + "结束");
        //
        //             obj.reset();
        //         }else {
        //
        //             auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event, disrupted, ifs);
        //
        //             if (const int result = obj->implementation(); result == -1) {
        //
        //             }
        //             obj.reset();
        //         }
        //
        //     } catch (const std::exception& e) {
        //         spdlog::error("Failed to thread: {}", e.what());
        //     }
        //
        // }

    } catch (const std::exception& e) {
        spdlog::error("Failed to thread: {}", e.what());
    }





}



