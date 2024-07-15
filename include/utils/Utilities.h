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


#endif //UTILITIES_H
