//
// Created by y1726 on 2024/6/26.
//

#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include <qwindowdefs_win.h>
#include <semaphore>

#include "models//TaskManager.h"
#include "models/ImageProcess.h"
#include "models/Tasks/BasicTask.h"
#include "Tasks/LessonTask.h"
#include "Tasks/FactionTask.h"

class TaskManager {

public:
    explicit TaskManager(int id, HWND hwnd, const QJsonDocument& configJson);

    ~TaskManager();

    void start();

    void stop();

    void pause();

    void resume();

    void setState(const std::string &task) const;

private:
    QJsonObject  config;
    int id;
    HWND hwnd;
    bool disrupted;
    std::mutex pause_event;
    bool unbind_event;
    bool LOCK;
    std::ifstream ifs;

    struct Task {
        std::string name;
        int priority;

        Task(const std::string &name, const int priority) : name(name), priority(priority) {}

        // 比较函数，放入结构体内部
        bool operator<(const Task& other) const {
            // 优先级高的任务优先
            return this->priority < other.priority;
        }

        // 自定义输出，便于测试
        friend std::ostream& operator<<(std::ostream& os, const Task& task) {
            os << "Task name: " << task.name << ", Priority: " << task.priority;
            return os;
        }

    };

    enum class TaskMode {
        Interval, // 间隔执行
        Daily,    // 每日执行
        Weekly,   // 每周执行
        Monthly   // 每月执行
    };

    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        std::chrono::system_clock::time_point toTimePoint() const {
            using namespace std::chrono;
            std::tm timeInfo = {};

            // 获取当前时间
            const auto now = system_clock::now();
            const auto now_time_t = system_clock::to_time_t(now);
            const std::tm* current_time = std::localtime(&now_time_t);

            // 设置时间字段
            timeInfo.tm_year = (year > 0 ? year - 1900 : current_time->tm_year); // year 从1900年开始
            timeInfo.tm_mon = (month > 0 ? month - 1 : current_time->tm_mon);    // month 从0开始
            timeInfo.tm_mday = (day > 0 ? day : current_time->tm_mday);           // day
            timeInfo.tm_hour = (hour >= 0 ? hour : current_time->tm_hour);        // hour
            timeInfo.tm_min = (minute >= 0 ? minute : current_time->tm_min);      // minute
            timeInfo.tm_sec = (second >= 0 ? second : current_time->tm_sec);      // second

            // mktime 会处理闰年等情况
            const std::time_t time = std::mktime(&timeInfo);
            return system_clock::from_time_t(time);
        }
    };

    struct TimerTask {
        std::string name;  // 任务名称
        int priority;      // 任务优先级
        TaskMode mode;     // 执行模式
        DateTime firstExecutionTime; // 第一次执行时间
        std::chrono::system_clock::time_point lastExecutionTime; // 上次执行时间
        std::chrono::system_clock::time_point nextExecutionTime; // 下一次执行时间
        std::chrono::milliseconds interval; // 执行间隔
        std::chrono::system_clock::time_point startTime; // 任务开始时间
        std::chrono::system_clock::time_point endTime;   // 任务结束时间

        TimerTask(const std::string& name, const int priority,
                  const TaskMode mode, const std::chrono::milliseconds interval)
            : name(name), priority(priority), mode(mode), interval(interval) {
            lastExecutionTime = std::chrono::system_clock::now(); // 初始化上次执行时间为当前时间
            updateExecutionTimes();  // 初始化下一次执行时间
        }

        // 更新任务的执行时间
        void updateExecutionTimes() {
            using namespace std::chrono;
            const auto now = system_clock::now();
            lastExecutionTime = now;

            switch (mode) {
                case TaskMode::Interval:
                    nextExecutionTime = lastExecutionTime + interval;
                break;
                case TaskMode::Daily:
                    nextExecutionTime = time_point_cast<seconds>(now) + hours(24);
                break;
                case TaskMode::Weekly:
                    nextExecutionTime = time_point_cast<seconds>(now) + weeks(1);
                break;
                case TaskMode::Monthly:
                {
                    const auto currentTime = system_clock::to_time_t(now);
                    tm localTime;
                    localtime_s(&localTime, &currentTime);
                    localTime.tm_mon += 1; // 增加一个月
                    if (localTime.tm_mon > 11) { // 如果超过了12月
                        localTime.tm_mon = 0; // 重新从1月开始
                        localTime.tm_year += 1; // 增加一年
                    }
                    nextExecutionTime = system_clock::from_time_t(mktime(&localTime));
                }
                break;
            }
        }
    };

    std::priority_queue<Task> taskQueue;

    void Init();

    std::string getTask();




};
#endif //TASKMANAGER_H




