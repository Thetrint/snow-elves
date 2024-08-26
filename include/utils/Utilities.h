//
// Created by y1726 on 24-7-14.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include "main.h"
#include "utils/Signals.h"

class NativeEventFilter final : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT

public:
    explicit NativeEventFilter(QObject *parent = nullptr) : QObject(parent){}

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
        if (eventType == "windows_generic_MSG") {
            if (const MSG *msg = static_cast<MSG *>(message); msg->message == WM_HOTKEY) {
                std::cout << "成功了吧~~" << std::endl;
                std::cout << msg->wParam << std::endl;
                if (msg->wParam == 1) {
                    emit Signals::instance()->Start();
                }

                return true;
            }
        }
        return false;
    }

};


class Timer {
public:
    explicit Timer(const bool& unbind_event) : unbind_event(unbind_event), elapsed_seconds{0}, is_running{false}, stop_flag{false}, pause_flag{false} {};
    ~Timer();

    void start();
    void pause();
    void resume();
    void reset();
    std::chrono::seconds read() const;

private:
    void run();
    bool unbind_event;
    std::chrono::seconds elapsed_seconds;
    std::atomic<bool> is_running;
    std::atomic<bool> stop_flag;
    std::atomic<bool> pause_flag;
    mutable std::mutex mutex;
    std::jthread timer_thread;
};

class Logger {
public:
    static void init() {
        spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] [thread %t] %v");
        spdlog::set_level(spdlog::level::info);

        // 创建循环文件日志记录器
        // 参数依次为：日志记录器名称、文件路径、单个文件最大大小（字节）、保留的最大文件数
        constexpr auto max_size = 5 * 1024 * 1024; // 5 MB
        constexpr auto max_files = 1; // 保留一个备份

        const auto logger = spdlog::rotating_logger_mt("global_logger", "logs/log.txt", max_size, max_files);

        spdlog::set_default_logger(logger);
    }
};

class SingleInstanceGuard {
public:
    explicit SingleInstanceGuard(const QString &key) : sharedMemory(key) {
        if (sharedMemory.attach()) {
            // 如果能附加到共享内存，说明已经有一个实例在运行
            spdlog::warn("Another instance is already running.");
            isRunning = true;
        } else {
            if (!sharedMemory.create(10240)) { // 创建共享内存
                spdlog::error("Failed to create shared memory: {}", sharedMemory.errorString().toStdString());
            }
        }
    }

    ~SingleInstanceGuard() {
        if (!isRunning) {
            sharedMemory.detach(); // 分离共享内存
        }
    }

    bool isAnotherInstanceRunning() const {
        return isRunning;
    }

private:
    QSharedMemory sharedMemory;
    bool isRunning = false;
};


#endif //UTILITIES_H
