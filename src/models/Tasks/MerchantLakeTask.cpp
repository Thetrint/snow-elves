//
// Created by y1726 on 24-8-13.
//
#include "models/Tasks/MerchantLakeTask.h"

int MerchantLakeTask::implementation() {
    int target = 1;
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720 * 20)) {
            return 0;
        }

        if (config.value("江湖行商模式").toString() == "带队模式") {
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
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建");
                    }
                    ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮队伍下拉");
                    mouse_move({}, {258, 307}, {258, 607});
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍无目标");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍行当玩法");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍江湖行商");

                    ClickImageMatch(MatchParams{.similar = 0.65, .scope = {110, 510, 438, 632}}, nullptr, "按钮队伍自动匹配");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍确定");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮确定");
                    Log("队伍检测完成");
                    target = 3;
                    break;
                }
                case 3: {
                    OpenTeam();
                    // 离线检查
                    OfflineDetection();
                    // 检测队伍人数
                    if(CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍空位").size() <= 5 - 3) {
                        // 跟随召集
                        if(!FollowDetection()) {
                            continue;
                        }
                        // 返回主界面
                        BackInterface();
                        // 前往npc
                        Log("前往行商npc");
                        OpenKnapsack();
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品综合入口");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品活动");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮活动行当");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮活动江湖行商", "按钮活动江湖行商1");
                        ClickImageMatch(MatchParams{.similar = 0.65, .scope = {761, 525, 1335, 750}}, nullptr, "按钮江湖行商前往");
                        Arrive();
                        target = 4;

                    }else {
                        // 第一次喊话开启自动匹配
                        if(record_event[0]) {
                            record_event[0] = false;
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍自动匹配1");
                        }
                        // 江湖行商喊话
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
                    if(ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖行商参与行商").empty()) {
                        BackInterface();
                        target = 3;
                        continue;
                    }
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖行商确认发起");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮铜钱");
                    if(!ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志江湖行商等待确认").empty()) {
                        Defer(20, 1000);
                        Close(1);
                        target = 5;
                    }
                    break;
                }
                case 5: {

                    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[1]).count() > 360) {
                        ClickImageMatch(MatchParams{.similar = 0.75, .matchCount = 1, .scope = {41, 212, 110, 425}}, nullptr, "按钮大世界行商任务");
                        // 更新喊话时间
                        record_time[1] = std::chrono::steady_clock::now();
                    }
                    // 等待到达商人
                    if(!ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 1, .y = 85}, nullptr, "按钮江湖行商威逼交易").empty()) {
                        record_event[2] = false;
                        // 单次购买
                        if(record_num[0] % 2 == 0 && !record_event[1]) {
                            Log("购买商品");
                            Defer(2);
                            for(auto [x, y] : std::initializer_list<std::pair<int, int>>{{524, 258}, {285, 258}, {524, 178}, {285, 178}}) {
                                mouse_down_up({}, {x, y});
                                mouse_keep({}, {1037, 489}, 2000);
                                ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 1, .scope = {699, 548, 1099, 673}}, nullptr, "按钮江湖行商购买");
                            }
                            Defer(3);
                            Close(1);
                            ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
                            Log("前往文府别院");
                            ClickImageMatch({.similar = 0.65, .x = -105, .y = -30}, nullptr, "按钮江湖行商文府别院");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                            record_num[0]++;
                            // 出售标志
                            record_event[1] = true;
                        }else if(record_num[0] % 2 == 1 && !record_event[1]) {
                            Log("购买商品");
                            Defer(2);
                            for(auto [x, y] : std::initializer_list<std::pair<int, int>>{{524, 258}, {285, 258}, {524, 178}, {285, 178}}) {
                                mouse_down_up({}, {x, y});
                                mouse_keep({}, {1037, 489}, 2000);
                                ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 1, .scope = {699, 548, 1099, 673}}, nullptr, "按钮江湖行商购买");
                            }
                            Defer(3);
                            Close(1);
                            ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
                            Log("前往全氏山庄");
                            ClickImageMatch({.similar = 0.65, .x = -82, .y = 20}, nullptr, "按钮江湖行商全氏山庄");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                            record_num[0]++;
                            // 出售标志
                            record_event[1] = true;
                        }else {
                            Log("出售商品");
                            Defer(2);
                            ClickImageMatch(MatchParams{.similar = 0.65, .matchCount = 1, .clickCount = 5, .clickDelayNum = 1000, .scope = {699, 548, 1099, 673}}, nullptr, "按钮江湖行商出售");
                            Close(1);
                            // 出售完成
                            record_event[1] = false;
                            Defer(2);
                            // 检测是否完成
                            if (!ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖行商一键上缴").empty()) {
                                // 关闭奖励
                                CloseReward(3);
                                Log(std::format("江湖行商完成 {} 次", record_num[1]));
                                // 判断任务是否完成
                                if (++record_num[1] > config.value("江湖行商次数").toInt()) {
                                    MerchantLakeFinish[id] = true;
                                    target = 0;
                                    continue;
                                }
                                target = 3;
                                // 重置参数
                                record_num[2] = 0;
                                record_time[1] = std::chrono::steady_clock::time_point();
                                record_event[2] = true;
                                continue;
                            }
                            if(record_num[0] % 2 == 1) {
                                ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
                                ClickImageMatch({.similar = 0.65, .x = -105, .y = -30}, nullptr, "按钮江湖行商文府别院");
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                            }else if (record_num[0] % 2 == 0) {
                                ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
                                ClickImageMatch({.similar = 0.65, .x = -82, .y = 20}, nullptr, "按钮江湖行商全氏山庄");
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                            }
                        }
                    }else if(record_event[2]) {
                        // 判断行商开启是否失败
                        if(++record_num[2] > 5) {
                            // 开启失败
                            target = 3;
                            // 重置参数
                            record_num[2] = 0;
                            record_time[1] = std::chrono::steady_clock::time_point();
                            record_event[2] = true;
                        }
                    }
                    break;
                }
            }
        }else if (config.value("江湖行商模式").toString() == "固定队模式") {
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
                    if(MerchantLakeFinish[config.value("江湖行商队长编号").toInt() - 1]) {
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

void MerchantLakeTask::objective(const std::string ve) {

}

int MerchantLakeTask::determine() {

    return 307;
}

int MerchantLakeTask::detect() {

    return -5;

}