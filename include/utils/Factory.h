//
// Created by y1726 on 2024/7/1.
//

#ifndef FACTORY_H
#define FACTORY_H
#include <qwindowdefs_win.h>
#include <models/Tasks/BountyMissionsTask.h>
#include <models/Tasks/BreakBanTask.h>
#include <models/Tasks/ChivalryShoutTask.h>
#include <models/Tasks/DailyCopiesTask.h>
#include <models/Tasks/DoorBanTask.h>
#include <models/Tasks/FateSwordTask.h>
#include <models/Tasks/HeroListTask.h>
#include <models/Tasks/HouseClockTask.h>
#include <models/Tasks/RiverTask.h>
#include <models/Tasks/SectTask.h>
#include <models/Tasks/TeaStoryTask.h>
#include <models/Tasks/TheSwordTask.h>
#include <models/Tasks/TheSwordThreeTask.h>
#include <models/Tasks/VientianeTask.h>
#include "models/Tasks/DailyRedemptionTask.h"
#include "models//TaskManager.h"
#include "models/ImageProcess.h"
#include "models/Tasks/BasicTask.h"
#include "models/Tasks/LessonTask.h"
#include "models/Tasks/FactionTask.h"
#include "models/Tasks/Submersion.h"
#include "models/Tasks/SwitchRolesTask.h"
#include "models/Tasks/PlaceTask.h"


// 工厂类
class Factory {
public:
    using CreateFunc = std::function<std::unique_ptr<BasicTask>(int, HWND, std::mutex&, bool&, bool&, std::ifstream&)>;

    static Factory& instance() {
        static Factory factory;
        return factory;
    }

    void registerClass(const std::string& className, const CreateFunc& func) {
        registry_[className] = func;
    }

    std::unique_ptr<BasicTask> create(const std::string& className, int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) const {
        if (auto it = registry_.find(className); it != registry_.end()) {
            return it->second(id, hwnd, pause_event, unbind_event, disrupted, ifs);
        }
        return nullptr; // or throw an exception for unknown className
    }

    template <typename T>
    void autoRegister(const std::string& className) {
        registerClass(className, [](int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) {
            return std::make_unique<T>(id, hwnd, pause_event, unbind_event, disrupted, ifs);
        });
    }

private:
    Factory() {
        // 在构造函数中注册所有需要的类
        autoRegister<PlaceTask>("占位任务");
        autoRegister<SwitchRolesTask>("切换角色");
        autoRegister<LessonTask>("课业任务");
        autoRegister<FactionTask>("帮派任务");
        autoRegister<Submersion>("潜神入忆");
        autoRegister<TheSwordTask>("华山论剑");
        autoRegister<TheSwordThreeTask>("华山论剑3v3");
        autoRegister<VientianeTask>("万象刷赞");
        autoRegister<HeroListTask>("江湖英雄榜");
        autoRegister<DailyCopiesTask>("日常副本");
        autoRegister<BountyMissionsTask>("悬赏任务");
        autoRegister<TeaStoryTask>("茶馆说书");
        autoRegister<RiverTask>("山河器");
        autoRegister<DoorBanTask>("门客设宴");
        autoRegister<BreakBanTask>("破阵设宴");
        autoRegister<DailyRedemptionTask>("每日兑换");
        autoRegister<SectTask>("宗门任务");
        autoRegister<ChivalryShoutTask>("侠缘喊话");
        autoRegister<FateSwordTask>("生死剑冢");
        autoRegister<HouseClockTask>("宅邸打卡");

    }

    std::map<std::string, CreateFunc> registry_;
};

#endif // FACTORY_H
