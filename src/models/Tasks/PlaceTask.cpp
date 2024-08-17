//
// Created by y1726 on 24-7-15.
//
#include "models/Tasks/PlaceTask.h"

int PlaceTask::implementation() {
     int target = 3;
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720)) {
            return 0;
        }

        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch (target) {
            // 任务退出
            case 0: {
                return 0;
            }
            // 位置检测
            case 1: {
                LocationDetection();
                target = 2;
                break;
            }
            // 队伍检测
            case 2: {
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                target = 3;
                break;
            }
            case 3: {
                Defer(2);

                target = 0;
                break;
            }

        }

    }

    return 0;


}

void PlaceTask::objective(const std::string ve) {

}

int PlaceTask::determine() {

    return 307;
}

int PlaceTask::detect() {

    return -5;

}