//
// Created by y1726 on 24-8-5.
//
#include "models/Tasks/HexagramDayTask.h"


int HexagramDayTask::implementation() {
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

        switch (target) {
            // 任务退出
            case 0: {
                return 0;
            }
            // 位置检测
            case 1: {
                LocationDetection();
                break;
            }
            // 队伍检测
            case 2: {
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                break;
            }
            case 3: {
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动游历");

                if (ClickImageMatch(MatchParams{.similar = 0.5, .y = 45}, nullptr, "按钮活动每日一卦").empty()) {
                    target = 0;
                    continue;
                }
                Arrive();
                break;
            }
            case 4: {
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮每日一卦算命占卜");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮每日一卦确定");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮每日一卦听天由命");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮每日一卦占卜");
                if(ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮每日一卦接受卦象").empty()) {
                    //返回主界面
                    target = 1;
                    continue;
                }
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                Close(2);
                break;
            }
            default:
                target = -1;
                break;
        }

        target++;


    }
    return 0;
}

// int HexagramDayTask::implementation() {
//     std::vector<Match> matchs;
//     objective("开始任务");
//     timer.start();
//     while (unbind_event) {
//
//         if (disrupted) {
//             timer.pause();
//             return -1; //任务调度中止任务
//         }
//
//         if (timer.read() >= std::chrono::seconds(720)) {
//             return 0;
//         }
//
//         switch (determine()) {
//             case 0:
//                Close({.similar = 0.5}, 3);;
//                 return 0; // 任务正常退出
//             case -1:
//                 Close({.similar = 0.5}, 1);
//                 break;
//             case 1:
//                 LocationDetection();
//                 objective("队伍检测");
//                 break;
//             case 2:
//                 OpenTeam();
//                 if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
//                     ClickImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
//                     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
//                 }
//                 Close({.similar = 0.5}, 1);
//                 objective("开始任务");
//                 break;
//             case 3:
//                 // CoortImageMatch({.similar = 0.9967, .scope = {281, 147, 1056, 597}, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮限时活动图标", "按钮限时活动图标1");
//                 // input_text("你好sxy");
//                 // Shout("sxy");
//                 objective("任务退出");
//                 return 0;
//                 break;
//
//             default:
//                 break;;
//         }
//
//     }
//
//     return 0;
//
//
// }

void HexagramDayTask::objective(const std::string ve) {
    cause = ve;
}

int HexagramDayTask::determine() {
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


    return 307;
}

int HexagramDayTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}