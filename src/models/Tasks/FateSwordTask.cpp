//
// Created by y1726 on 24-7-27.
//
#include "models/Tasks/FateSwordTask.h"

int FateSwordTask::implementation() {
    std::vector<Match> matchs;
    objective("位置检测");
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(7200000)) {
            return 0;
        }

        switch (determine()) {
            case 0:
               Close({.similar = 0.5}, 3);;
                return 0; // 任务正常退出
            case -1:
                Close({.similar = 0.5}, 1);
                break;
            case 1:
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮物品综合入口");
                mouse_move({}, {1245, 671}, {1245, 271});
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮物品江湖奇趣");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮江湖奇趣生死剑冢");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮江湖奇趣匹配");
                ClickImageMatch({.similar = 0.5}, nullptr, "按钮奇趣关闭");
                // CoortImageMatch({.similar = 0.9967, .scope = {281, 147, 1056, 597}, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮限时活动图标", "按钮限时活动图标1");
                // input_text("你好sxy");
                // Shout("sxy");

                objective("等待进入");
                break;
            case 4:
                if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志生死剑冢匹配成功").empty()) {
                    record_event[0] = true;
                    Log("生死剑冢匹配成功");
                    objective("等待完成");
                }
                Defer(1);
                break;
            case 5:

                if (config.value("剑冢随机跳伞").toBool() and record_event[0]) {
                    record_event[0] = false;
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(10, 60);

                    int delay_seconds = dist(gen);
                    std::this_thread::sleep_for(std::chrono::seconds(delay_seconds));

                    // 跳伞

                }

                key_down_up("Q");


                if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮生死剑冢离开").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮生死剑冢离开");

                    Log(std::format("生死剑冢完成 {} 次", record_num[0]));
                    PassLevel();
                    if (++record_num[0] > config.value("剑冢次数").toInt()) {
                        objective("任务退出");
                        continue;
                    }

                    objective("开始任务");
                }
                break;
            default:
                break;;
        }

    }

    return 0;


}

void FateSwordTask::objective(const std::string ve) {
    cause = ve;
}

int FateSwordTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 10) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }else {
        detect_count = 0;
    }


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

    if (cause == "开始任务") {
        switch (sw) {
            case 1:
                return 3;
            default:
                return -1;
        }
    }

    if (cause == "等待进入") {
        switch (sw) {
            case 1:
                return 4;
            default:
                return 307;
        }
    }

    if (cause == "等待完成") {
        switch (sw) {
            default:
                return 5;
        }
    }


    return 307;
}

int FateSwordTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}