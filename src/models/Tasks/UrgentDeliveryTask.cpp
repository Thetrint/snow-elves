//
// Created by y1726 on 24-8-13.
//
#include "models/Tasks/UrgentDeliveryTask.h"


int UrgentDeliveryTask::implementation() {
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
                    ClickImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                target = 3;
                break;
            }
            case 3: {
                // 打开帮派
                OpenFaction();
                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮帮派势力");
                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮帮派江湖急送");

                // 判断是否完成
                if(!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志江湖急送订单上限").empty()) {
                    Close(2);
                    target = 0;
                    continue;
                }
                // 抢单
                ClickImageMatch(MatchParams{.similar = 0.65, .scope = {200, 500, 490, 750}}, nullptr, "按钮帮派抢单");
                Close(2);
                target = 4;

                break;
            }
            case 4: {

                ClickImageMatch(MatchParams{.similar = 0.55, .matchCount = 1, .scope = {41, 212, 110, 425}}, nullptr, "按钮大世界外卖任务");

                // 送达
                if(!ClickImageMatch(MatchParams{.similar = 0.55, .matchCount = 1, .scope = {786, 283, 1033, 534}}, nullptr, "按钮江湖急送菜品送达").empty()) {
                    Defer(7, 1000);
                    Close(1);
                    target = 3;
                    continue;
                }

                // 购买
                if(!CoortImageMatch(MatchParams{.similar = 0.65, .x = 100}, nullptr, "按钮江湖急送前往购买").empty()) {
                    if (record_event[0]) {
                        ClickImageMatch(MatchParams{.similar = 0.65, .x = 100}, nullptr, "按钮江湖急送前往购买");
                        ClickImageMatch(MatchParams{.similar = 0.65, .y = -65}, nullptr, "按钮江湖急送神厨商会");
                        Defer(2);
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送菜品标签");
                        auto action = CoortImageMatch(MatchParams{.similar = 0.65, .y = -65}, nullptr, "标志江湖急送标签");
                        action.push_back({{0, 0}, {0.8}});
                        mouse_down_up({}, {0, 0});
                        for(auto [location, score] : action) {
                            if(ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送符合").empty()) {
                                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送菜品标签");
                                mouse_down_up({}, {location.x, location.y});
                                mouse_down_up({}, {0, 0});
                                break;
                            }
                            // 购买
                            ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送购买");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                            break;

                        }
                        Close(1);
                        record_event[0] = false;
                    }else {
                        record_event[0] = true;
                        ClickImageMatch(MatchParams{.similar = 0.55}, nullptr, "按钮江湖急送放弃订单");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                        target = 3;
                    }

                }
                // 领取食盆
                if(!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送领取食盆").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送领取食盆");
                    Arrive();
                    ClickImageMatch(MatchParams{.similar = 0.65, .scope = {786, 283, 1033, 534}}, nullptr, "按钮江湖急送菜品打包");
                    ClickImageMatch(MatchParams{.similar = 0.55}, nullptr, "按钮江湖急送添加");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送选择");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮江湖急送选择菜品");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");

                }

                break;
            }
        }



    }
    return 0;
}


void UrgentDeliveryTask::objective(const std::string ve) {
}

int UrgentDeliveryTask::determine() {
    return 0;
}

int UrgentDeliveryTask::detect() {
    return 0;
}
