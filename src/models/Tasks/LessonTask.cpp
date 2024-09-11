//
// Created by y1726 on 2024/6/30.
//
#include "models//Tasks/LessonTask.h"

int LessonTask::implementation() {
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
                Log("检测队伍");
                OpenTeam();
                if (CoortImageMatch({.similar = 0.65}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍退出");
                    ClickImageMatch({.similar = 0.5}, nullptr, "按钮确定");
                }
                Close( 1);
                Log("队伍检测完成");
                target = 3;
                break;
            }
            case 3: {
                OpenKnapsack();
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品综合入口");
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品活动");
                ClickImageMatch({.similar = 0.55}, nullptr, "按钮活动江湖");

                if (ClickImageMatch({.similar = 0.5, .y = 45}, nullptr, "按钮活动吟风", "按钮活动含灵", "按钮活动寻道", "按钮活动归义", "按钮活动悟禅", "按钮活动止杀", "按钮活动漱尘", "按钮活动濯剑", "按钮活动观梦", "按钮活动锻心", "按钮活动问卜").empty()) {
                    target = 0;
                    continue;
                }

                ClickImageMatch({.similar = 0.65, .scope = {170, 127, 404, 582}}, nullptr, "按钮前往");
                Arrive();
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮大世界课业", "按钮大世界悟禅");
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮课业确定");

                target = 4;
                break;
            }
            case 4: {
                if(FlagImageMatchCount("按钮课业刷新", record_num[0])) {
                    if (ClickImageMatch({.similar = 0.65, .click = FIRST}, nullptr, "按钮课业困难").empty()) {
                        ClickImageMatch({.similar = 0.65, .x = -55}, nullptr, "按钮课业刷新");
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮确定");
                        continue;
                    }
                    target = 5;
                    Close(1);
                }else {
                    target = 3;
                }
                break;
            }
            case 5: {

                if (!CoortImageMatch({.similar = 0.65}, nullptr, "按钮大世界播放中").empty()) {
                    Defer(1);
                    continue;
                }

                if (!CoortImageMatch({.similar = 0.65}, nullptr, "按钮大世界自动").empty()) {
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮大世界自动");
                    Defer(1);
                    continue;
                }

                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 60) {
                    record_time[0] = std::chrono::steady_clock::now();
                    // if (CoortImageMatch({.similar = 0.5, .matchCount = 1}, nullptr, "按钮大世界江湖").empty()) {
                    //     ClickImageMatch({.similar = 0.5, .matchCount = 1}, nullptr, "按钮大世界任务栏");
                    // }
                    ClickImageMatch({.similar = 0.6, .matchCount = 1}, nullptr, "按钮大世界江湖");
                    ClickImageMatch(MatchParams{.similar = 0.55, .matchCount = 1, .scope = {41, 212, 110, 425}}, nullptr, "按钮大世界吟风任务", "按钮大世界含灵任务", "按钮大世界寻道任务", "按钮大世界归义任务",
                        "按钮大世界悟禅任务", "按钮大世界止杀任务", "按钮大世界漱尘任务", "按钮大世界濯剑任务", "按钮大世界观梦任务",
                        "按钮大世界锻心任务", "按钮大世界问卜任务");

                }

                if (!CoortImageMatch({.similar = 0.5}, nullptr, "标志课业答题").empty()) {
                    Close({.similar = 0.5}, 1);
                }

                if (!CoortImageMatch({.similar = 0.5}, nullptr, "标志论剑倒计时").empty()) {

                    // 创建一个 vector 来存储随机选择的两个元素
                    std::vector<Match> randomMatches(2);

                    // 创建一个随机数生成器
                    std::random_device rd;
                    std::mt19937 gen(rd());

                    // 从 matches 中随机选择两个元素
                    std::ranges::sample(CoortImageMatch({.similar = 0.99, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "标志论剑排序"), randomMatches.begin(), 2, gen);

                    mouse_move({}, randomMatches[0].location, randomMatches[1].location);

                }

                if (!CoortImageMatch({.similar = 0.5}, nullptr, "标志课业杂货商人").empty()) {
                    mouse_down_up({}, {1003, 617});
                }

                if (!CoortImageMatch({.similar = 0.5}, nullptr, "标志课业挑水").empty()) {
                    mouse_down_up({}, {1008, 398});
                }

                if(!CoortImageMatch({.similar = 0.5, .y = -45}, nullptr, "按钮大世界商城购买").empty()) {
                    ClickImageMatch({.similar = 0.5, .y = -45}, nullptr, "按钮大世界商城购买");
                    mouse_down_up({.clickCount = 14}, {988, 694});
                    Close({.similar = 0.5}, 1);
                }

                if (!CoortImageMatch({.similar = 0.5}, nullptr, "界面交易", "按钮交易购买").empty()) {
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮交易购买");
                    ClickImageMatch({.similar = 0.6}, nullptr, "按钮确定");
                    record_time[0] = std::chrono::steady_clock::time_point();
                    Close(1);
                }

                if (!ClickImageMatch({.similar = 0.6, .matchCount = 1, .matchDelay = false}, nullptr, "按钮大世界一键提交").empty()) {
                    if (!ClickImageMatch({.similar = 0.6}, nullptr, "按钮确定").empty()) {
                        target = 0;
                    }

                }

                break;
            }

        }

    }

    return 0;


}

void LessonTask::objective(const std::string ve) {

}

int LessonTask::determine() {

    return 307;
}

int LessonTask::detect() {

    return -5;

}