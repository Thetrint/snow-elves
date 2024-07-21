//
// Created by y1726 on 2024/6/30.
//
#include "models//Tasks/LessonTask.h"


int LessonTask::implementation() {
    spdlog::info("任务执行");
    std::vector<Match> matchs;

    objective("位置检测");

    timer.start();
    spdlog::info("计时器启动");
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720)) {
            return 0;
        }

        switch (determine()) {
            case 0:
                return 0; // 任务正常退出
            case -1:
                Close(1);
                break;
            case 1:
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                }
                Close(1);
                objective("开始任务");
                break;
            case 3:
                OpenKnapsack();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动江湖");

                if (ClickImageMatch(MatchParams{.similar = 0.5, .y = 45}, nullptr, "按钮活动吟风", "按钮活动含灵", "按钮活动寻道", "按钮活动归义", "按钮活动悟禅", "按钮活动止杀", "按钮活动漱尘", "按钮活动濯剑", "按钮活动观梦", "按钮活动锻心").empty()) {
                    objective("任务退出");
                    continue;
                }
                ClickImageMatch(MatchParams{.similar = 0.5, .scope = {170, 127, 404, 582}}, nullptr, "按钮前往");
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界课业", "按钮大世界悟禅");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮课业确定");
                for (int i = 0; i <= 8; i++) {
                    if (ClickImageMatch(MatchParams{.similar = 0.6, .click = FIRST}, nullptr, "按钮课业困难").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.6, .x = -55}, nullptr, "按钮课业刷新");
                        ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                    }else {
                        break;
                    }
                }
                Close(1);
                objective("等待完成");
                break;
            case 4:

                if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮大世界播放中").empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
                    continue;
                }

                if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮大世界自动").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮大世界自动");
                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
                    continue;
                }

                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 30) {
                    record_time[0] = std::chrono::steady_clock::now();
                    if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界江湖").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                    }
                    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界江湖");
                    ClickImageMatch(MatchParams{.similar = 0.9, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false},
                        nullptr, "按钮大世界吟风任务", "按钮大世界含灵任务", "按钮大世界寻道任务", "按钮大世界归义任务",
                        "按钮大世界悟禅任务", "按钮大世界止杀任务", "按钮大世界漱尘任务", "按钮大世界濯剑任务", "按钮大世界观梦任务", "按钮大世界锻心任务");
                }


                if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志课业答题").empty()) {
                    Close(1);
                }

                if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志论剑倒计时").empty()) {
                    matches = CoortImageMatch(MatchParams{.similar = 0.99, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "标志论剑排序");
                    // 创建一个 vector 来存储随机选择的两个元素
                    std::vector<Match> randomMatches(2);

                    // 创建一个随机数生成器
                    std::random_device rd;
                    std::mt19937 gen(rd());

                    // 从 matches 中随机选择两个元素
                    std::ranges::sample(matches, randomMatches.begin(), 2, gen);

                    mouse_move({}, randomMatches[0].location, randomMatches[1].location);

                }

                if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志课业杂货商人").empty()) {
                    mouse_down_up(MatchParams{}, cv::Point{1003, 617});
                }

                if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易", "按钮交易购买").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮交易购买");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮确定");
                    Close(1);
                }

                if (!ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 1, .matchDelay = false}, nullptr, "按钮大世界一键提交").empty()) {
                    if (!ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定").empty()) {
                        objective("任务退出");
                    }

                }

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

void LessonTask::objective(const std::string ve) {
    cause = ve;
}

int LessonTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 15) {
            return -1;
        }
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

    if (cause == "等待完成") {
        switch (sw) {
            default:
                return 4;
        }
    }

    return 307;
}

int LessonTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}