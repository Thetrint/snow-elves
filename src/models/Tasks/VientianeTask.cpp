//
// Created by y1726 on 24-7-16.
//
#include "models/Tasks/VientianeTask.h"

int VientianeTask::implementation() {
    int target = 1;
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
                Close(1);
                target = 3;
                break;
            }
            case 3: {
                if (OpenMap()) {
                    ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
                    ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
                    ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
                    input_text("307");

                    ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
                    input_text("360");

                    ClickImageMatch({.similar = 0.65, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
                    ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                    Arrive();

                }
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮大世界拍照");
                Defer(2);
                ClickImageMatch({.similar = 0.65, .matchCount = 1}, nullptr, "按钮场景环绕旋转");
                ClickImageMatch({.similar = 0.65, .matchCount = 1}, nullptr, "按钮场景确定");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮场景万象");
                target = 4;
                break;
            }
            case 4: {
                if(CoortImageMatch({.similar = 0.65}, nullptr, "界面场景").empty()) {
                    target = 3;
                    continue;
                }
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮场景拍照");
                Defer(1);
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮江湖万象打卡");
                Defer(2);
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮江湖万象上传打卡");
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮确定");
                Defer(4);
                ClickImageMatch({.similar = 0.65, .x = -240, .y = -55}, nullptr, "按钮拍照打卡立刻拍照");
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮拍照打卡立刻拍照");
                if(++record_num[0] >= 20) {
                    target = 0;
                    Close(3);
                }
                break;
            }

        }

    }

    return 0;


}

void VientianeTask::objective(const std::string ve) {

}

int VientianeTask::determine() {

    return 307;
}

int VientianeTask::detect() {

    return -5;

}