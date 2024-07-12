//
// Created by y1726 on 2024/7/1.
//

#ifndef FACTORY_H
#define FACTORY_H
#include <qwindowdefs_win.h>
#include "models//TaskManager.h"
#include "models/ImageProcess.h"
#include "models/Tasks/BasicTask.h"
#include "models/Tasks/LessonTask.h"
#include "models/Tasks/FactionTask.h"

// 工厂类
class Factory {
public:
    using CreateFunc = std::function<std::unique_ptr<BasicTask>(int, HWND, std::mutex&, bool&)>;

    static Factory& instance() {
        static Factory factory;
        return factory;
    }

    void registerClass(const std::string& className, const CreateFunc& func) {
        registry_[className] = func;
    }

    std::unique_ptr<BasicTask> create(const std::string& className, int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event) const {
        if (auto it = registry_.find(className); it != registry_.end()) {
            return it->second(id, hwnd, pause_event, unbind_event);
        }
        return nullptr; // or throw an exception for unknown className
    }

    template <typename T>
    void autoRegister(const std::string& className) {
        registerClass(className, [](int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event) {
            return std::make_unique<T>(id, hwnd, pause_event, unbind_event);
        });
    }

private:
    Factory() {
        // 在构造函数中注册所有需要的类
        autoRegister<LessonTask>("课业任务");
        autoRegister<FactionTask>("帮派任务");
    }

    std::map<std::string, CreateFunc> registry_;
};

#endif // FACTORY_H
