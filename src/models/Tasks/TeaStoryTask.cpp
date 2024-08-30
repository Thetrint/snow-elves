//
// Created by y1726 on 24-7-17.
//
#include "models/Tasks/TeaStoryTask.h"


int TeaStoryTask::implementation() {
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
                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品综合入口");
                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品活动");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动江湖");
                if (ClickImageMatch(MatchParams{.similar = 0.55, .y = 45}, nullptr, "按钮活动茶馆说书").empty()) {
                    target = 0;
                    continue;
                }
                Arrive();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮茶馆说书进入茶馆");
                PassLevel();
                target = 4;
                break;
            }
            case 4: {
                if(FlagImageMatchCount("界面茶馆", record_num[0])) {

                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮茶馆说书甲", "按钮茶馆说书乙", "按钮茶馆说书丙", "按钮茶馆说书丁");


                    if(RepeatVerification("按钮茶馆说书退出茶馆", record_num[1])) {
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮茶馆说书退出茶馆");
                        CloseReward(3);
                        target = 0;
                    }

                    Defer(1);
                }else {
                    target = 3;
                }
            }

        }

    }

    return 0;


}

void TeaStoryTask::objective(const std::string ve) {

}

int TeaStoryTask::determine() {

    return 307;
}

int TeaStoryTask::detect() {

    return -5;

}