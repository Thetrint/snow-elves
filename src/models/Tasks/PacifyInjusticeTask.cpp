//
// Created by y1726 on 24-8-14.
//
#include "models/Tasks/PacifyInjusticeTask.h"

int PacifyInjusticeTask::implementation() {
    int target = 1;
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720 * 10)) {
            return 0;
        }
        if (config.value("江湖行商模式").toString() == "带队模式") {
            // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
            switch (target) {
                // 任务退出
                case 0: {
                    // 重置全局资源
                    PacifyFinish[id] = false;
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
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建");
                    }
                    ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮队伍下拉");
                    mouse_move({}, {258, 307}, {258, 607});
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍无目标");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍行当玩法");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍聚义平冤");

                    ClickImageMatch(MatchParams{.similar = 0.65, .scope = {110, 510, 438, 632}}, nullptr, "按钮队伍自动匹配");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍确定");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮确定");
                    Log("队伍检测完成");

                    // 开启互联分线
                    SwitchInterconnection();

                    target = 3;
                    break;
                }
                // 检测队伍人数 开启任务
                case 3: {
                    OpenTeam();
                    // 检测队伍人数
                    if(CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍空位").size() <= 10 - 3) {
                        // 离线检测
                        if(!OfflineDetection()) {
                            continue;
                        }

                        // 跟随召集
                        if(!FollowDetection()) {
                            continue;
                        }
                        // 返回主界面
                        BackInterface();
                        // 前往npc
                        Log("前往聚义npc");
                        OpenKnapsack();
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品综合入口");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品活动");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮活动行当");
                        // 任务已被完成
                        if (ClickImageMatch(MatchParams{.similar = 0.6, .y = 45}, nullptr, "按钮活动聚义平冤").empty()) {
                            target = 0;
                            continue;
                        }
                        Arrive();
                        target = 4;
                    }else {
                        // 第一次喊话开启自动匹配
                        if(record_event[0]) {
                            record_event[0] = false;
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍自动匹配1");
                        }
                        // 聚义平冤喊话
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 30) {
                            // 返回主界面
                            BackInterface();
                            Shout(config.value("江湖行商喊话内容").toString().toStdString());
                            // 更新喊话时间
                            record_time[0] = std::chrono::steady_clock::now();
                        }
                    }
                    break;
                }
                case 4: {
                    if(ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮聚义平冤聚义平冤").empty()) {
                        BackInterface();
                        target = 3;
                        continue;
                    }
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮聚义平冤确定");
                    target = 5;
                    break;
                }
                case 5: {
                    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面大世界1", "界面大世界2", "按钮副本退出").size() == 3) {
                        // 重置副本内次数
                        record_num[0] = 0;
                        if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志聚义平冤破门中").empty()) {
                            Defer(30, 1000);
                            PassLevel();
                            // 通知队员任务完成
                            PacifyFinish[id] = true;
                            CloseReward(3);
                            target = 0;
                        }
                        continue;
                    }

                    // 判断副本内次数
                    if(++record_num[0] >= 25) {
                        target = 3;
                    }
                    Defer(3, 2000);

                    break;
                }

            }
        }else if(config.value("江湖行商模式").toString() == "固定队模式") {
            // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
            switch (target) {
                // 任务退出
                case 0: {
                    return 0;
                }
                // 位置检测
                case 1: {
                    // LocationDetection();
                    target = 2;
                    break;
                }
                // 队伍检测
                case 2: {
                    Log("检测队伍");
                    OpenTeam();
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        Log("没有队伍 任务结束");
                        target = 0;
                        continue;
                    }
                    Log("队伍检测完成");
                    Close(1);
                    target = 3;
                    break;
                }
                case 3: {
                    // 等待队长发布完成信号
                    if(PacifyFinish[config.value("江湖行商队长编号").toInt() - 1]) {
                        CloseReward(5);
                        target = 0;
                        continue;
                    }
                    Defer(2);
                    break;
                }


            }
        }

    }

    return 0;


}

void PacifyInjusticeTask::objective(const std::string ve) {

}

int PacifyInjusticeTask::determine() {

    return 307;
}

int PacifyInjusticeTask::detect() {

    return -5;

}