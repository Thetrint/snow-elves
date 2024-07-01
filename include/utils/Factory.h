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
    using CreateFunc = std::function<std::unique_ptr<BasicTask>(int, HWND, std::counting_semaphore<1>&, std::counting_semaphore<1>&)>;

    static Factory& instance() {
        static Factory factory;
        return factory;
    }

    void registerClass(const std::wstring& className, const CreateFunc& func) {
        registry_[className] = func;
    }

    std::unique_ptr<BasicTask> create(const std::wstring& className, int id, HWND hwnd, std::counting_semaphore<1>& pause_event, std::counting_semaphore<1>& unbind_event) const {
        if (auto it = registry_.find(className); it != registry_.end()) {
            return it->second(id, hwnd, pause_event, unbind_event);
        }
        return nullptr; // or throw an exception for unknown className
    }

    template <typename T>
    void autoRegister(const std::wstring& className) {
        registerClass(className, [](int id, HWND hwnd, std::counting_semaphore<1>& pause_event, std::counting_semaphore<1>& unbind_event) {
            return std::make_unique<T>(id, hwnd, pause_event, unbind_event);
        });
    }

private:
    Factory() {
        // 在构造函数中注册所有需要的类
        autoRegister<LessonTask>(L"课业任务");
        autoRegister<FactionTask>(L"帮派任务");
    }

    std::map<std::wstring, CreateFunc> registry_;
};

#endif // FACTORY_H
