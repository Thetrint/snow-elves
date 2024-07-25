//
// Created by y1726 on 24-7-15.
//
#include "models/Tasks/SwitchRolesTask.h"

#include <utils/LoadJsonFile.h>


int SwitchRolesTask::implementation() {
    spdlog::info("任务执行");
    std::vector<Match> matchs;

    objective("位置检测");

    while (unbind_event) {

        switch (determine()) {
            case 0:
                return 0; // 任务正常退出
            case -1:
                Close({.similar = 0.5}, 1);
                break;
            case 1:
                spdlog::info("队伍检测");
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                spdlog::info("队伍检测");
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                objective("切换角色");
                break;
            case 3:
                if (LoadJsonFile::instance().jsonFiles[id].value("切角色1").toBool() && roles[1]) {
                    // switch_roles(1);
                    roles[1] = false;
                    return 1;
                }
                if (LoadJsonFile::instance().jsonFiles[id].value("切角色2").toBool() && roles[2]) {
                    // switch_roles(2);
                    roles[2] = false;
                    return 1;
                }
                if (LoadJsonFile::instance().jsonFiles[id].value("切角色3").toBool() && roles[3]) {
                    // switch_roles(3);
                    roles[3] = false;
                    return 1;
                }
                if (LoadJsonFile::instance().jsonFiles[id].value("切角色4").toBool() && roles[4]) {
                    // switch_roles(4);
                    roles[4] = false;
                    return 1;
                }
                if (LoadJsonFile::instance().jsonFiles[id].value("切角色5").toBool() && roles[5]) {
                    // switch_roles(5);
                    roles[5] = false;
                    return 1;
                }
                if (std::ranges::all_of(roles, [](const bool role) { return role; })) {
                    roles[0] = false;
                    return 1;
                }

                return 0;
                break;
            default:
                break;;
        }

    }

    return 0;

    // while (true) {
    //     // 休眠 2000 毫秒（即2秒）
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // }
}

void SwitchRolesTask::objective(const std::string ve) {
    cause = ve;
}

int SwitchRolesTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (detect_count++ >= 15) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
    detect_count = 0;

    if (cause == "任务退出") {
        switch (sw) {
            case 1:
                return 0;
            default:
                return -1;
        }
    }

    if (cause == "位置检测") {
        switch (sw) {
            case 1:
                return 1;
            default:
                return -1;
        }
    }

    if (cause == "队伍检测") {
        switch (sw) {
            case 1:
                return 2;
            default:
                return -1;
        }
    }

    if (cause == "切换角色") {
        switch (sw) {
            case 1:
                return 3;
            default:
                return -1;
        }
    }

    return 307;
}

int SwitchRolesTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }

    return -5;

}