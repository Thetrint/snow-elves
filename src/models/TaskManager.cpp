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


TaskManager::TaskManager(int id, HWND hwnd, const QJsonDocument& configJson)
    :id(id), hwnd(hwnd), disrupted(false), unbind_event(true), LOCK(false) {
    this->config = configJson.object();

    LoadJsonFile::instance().LoadFile(id);
    Init();

    std::cout << getTask() << std::endl;
    std::cout << getTask() << std::endl;

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
    }
    taskQueue.emplace("12", 2);
    taskQueue.emplace("11", 1);
}

/**
 * 获取下一个任务名称
 * @return 返回优先级别任务
 */
std::string TaskManager::getTask() {
    if (!taskQueue.empty()) {
        Task highestPriorityTask = taskQueue.top();

        // 删除优先级最高的任务
        taskQueue.pop();
        return highestPriorityTask.name;
    }

    return "";
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


        // 创建一个 vector 用于存储解码后的值
        std::vector<std::string> tasks;

        for (const auto& task : LoadJsonFile::instance().jsonFiles[id].value("执行任务").toArray()) {
            tasks.push_back(task.toString().toStdString());
            std::cout << task.toString().toStdString() << std::endl;
        }

        const auto rol = Factory::instance().create("切换角色", id, hwnd, pause_event, unbind_event, disrupted, ifs);

        TaskSchedul schedul(tasks, rol, id);

        std::string task;
        while ((task = schedul.get_task()).empty() == false && unbind_event) {
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

    } catch (const std::exception& e) {
        spdlog::error("Failed to thread: {}", e.what());
    }





}



