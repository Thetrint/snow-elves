//
// Created by y1726 on 24-8-4.
//
#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H
#include "main.h"

class TaskScheduler {

public:

    enum class TaskMode {
        Interval, // 间隔执行
        Daily,    // 每日执行
        Weekly,   // 每周执行
        Monthly,   // 每月执行
        Yearly   // 每年执行
    };

    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        // 获取当前时间
        static DateTime now() {
            using namespace std::chrono;
            const auto now = system_clock::now();
            const auto now_time_t = system_clock::to_time_t(now);
            const auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst] = *std::localtime(&now_time_t);

            return DateTime(
                tm_year + 1900,
                tm_mon + 1,
                tm_mday,
                tm_hour,
                tm_min,
                tm_sec
            );
        }

        // Builder 用于按需设置时间字段
        struct Builder {
            std::optional<int> year;
            std::optional<int> month;
            std::optional<int> day;
            std::optional<int> hour;
            std::optional<int> minute;
            std::optional<int> second;

            Builder& setYear(int y) { year = y; return *this; }
            Builder& setMonth(int m) { month = m; return *this; }
            Builder& setDay(int d) { day = d; return *this; }
            Builder& setHour(int h) { hour = h; return *this; }
            Builder& setMinute(int mi) { minute = mi; return *this; }
            Builder& setSecond(int s) { second = s; return *this; }

            DateTime build() const {
                DateTime now = DateTime::now();
                return DateTime(
                    year.value_or(now.year),
                    month.value_or(now.month),
                    day.value_or(now.day),
                    hour.value_or(now.hour),
                    minute.value_or(now.minute),
                    second.value_or(now.second)
                );
            }
        };

    private:
        // 私有构造函数，只能通过 Builder 构造
        DateTime(int y, int m, int d, int h, int mi, int s)
            : year(y), month(m), day(d), hour(h), minute(mi), second(s) {}

    public:
        std::chrono::system_clock::time_point to_time_point() const {
            std::tm timeinfo = {};
            timeinfo.tm_year = year - 1900;
            timeinfo.tm_mon = month - 1;
            timeinfo.tm_mday = day;
            timeinfo.tm_hour = hour;
            timeinfo.tm_min = minute;
            timeinfo.tm_sec = second;

            // Convert to time_t and then to system_clock::time_point
            const std::time_t time_t_val = std::mktime(&timeinfo);
            if (time_t_val == -1) {
                throw std::runtime_error("mktime failed");
            }
            return std::chrono::system_clock::from_time_t(time_t_val);
        }
    };

    struct TimerTask {
        std::string name;  // 任务名称
        TaskMode mode;     // 执行模式
        std::chrono::milliseconds interval; // 执行间隔
        DateTime executionTime; // 执行时间
        std::chrono::system_clock::time_point lastExecutionTime; // 上次执行时间
        std::chrono::system_clock::time_point nextExecutionTime; // 下次执行时间
        std::function<void()> taskFunction; // 任务函数

        TimerTask(const std::string& name, const TaskMode mode, const std::chrono::milliseconds interval, const DateTime& executionTime, const std::function<void()>& taskFunction)
            : name(name), mode(mode), interval(interval), executionTime(executionTime), taskFunction(taskFunction) {
            nextExecutionTime = executionTime.to_time_point(); // 初始化下次次执行时间为执行时间
            std::cout << executionTime.hour << std::endl;
            std::cout << executionTime.to_time_point() << std::endl;
            updateExecutionTimes();
            std::cout << executionTime.hour << std::endl;
            std::cout << executionTime.to_time_point() << std::endl;
        }

        // 判断是否执行任务
        bool shouldExecute() {
            using namespace std::chrono;
            if (const auto now = system_clock::now(); now >= nextExecutionTime) {
                updateExecutionTimes();  // 确保在应该执行任务时更新执行时间
                return true;
            }
            return false;
        }

        // 更新任务的执行时间
        void updateExecutionTimes() {
            using namespace std::chrono;
            const auto now = system_clock::now();
            lastExecutionTime = now;

            switch (mode) {
                case TaskMode::Interval:
                    std::cout << "Before update: " << nextExecutionTime << std::endl;
                    nextExecutionTime = now + interval;
                    std::cout << "After update: " << nextExecutionTime << std::endl;
                    break;
                case TaskMode::Daily:
                    nextExecutionTime = nextExecutionTime + hours(24);
                    break;
                case TaskMode::Weekly:
                    nextExecutionTime = nextExecutionTime + weeks(1);
                    break;
                case TaskMode::Monthly:
                {
                    const auto currentTime = system_clock::to_time_t(nextExecutionTime);
                    tm localTime;
                    localtime_s(&localTime, &currentTime);
                    localTime.tm_mon += 1; // 增加一个月
                    if (localTime.tm_mon > 11) { // 如果超过了12月
                        localTime.tm_mon = 0; // 重新从1月开始
                        localTime.tm_year += 1; // 增加一年
                    }
                    nextExecutionTime = system_clock::from_time_t(mktime(&localTime));
                    break;
                }
                case TaskMode::Yearly:
                {
                    const auto currentTime = system_clock::to_time_t(nextExecutionTime);
                    std::tm localTime;
                    localtime_s(&localTime, &currentTime);
                    localTime.tm_year += 1; // 增加一年
                    const std::time_t time = std::mktime(&localTime);
                    nextExecutionTime = system_clock::from_time_t(time);
                    break;
                }
            }
        }
    };

    void addTask(const TimerTask& timerTask);

    void start() {
        running = true;
        workerThread = std::thread(&TaskScheduler::run, this);
    }

    void stop() {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

private:
    std::vector<TimerTask> timerTaskList;
    std::atomic<bool> running;
    std::thread workerThread;

    void run() {
        while (running) {
            // 启动异步任务来检查条件
            std::vector<std::future<void>> futures;
            for (auto& task : timerTaskList) {
                futures.push_back(std::async(std::launch::async, [&task]() {
                    if (task.shouldExecute()) {
                        task.taskFunction();
                    }
                }));
            }

            // 等待所有异步任务完成
            // for (auto& future : futures) {
            //     future.get();
            // }

            // 等待一段时间再继续检查
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }



};
#endif //TASKSCHEDULER_H
