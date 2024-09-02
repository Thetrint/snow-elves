//
// Created by y1726 on 24-7-21.
//
#include "models/Tasks/DailyRedemptionTask.h"


int DailyRedemptionTask::implementation() {
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
                if (CoortImageMatch({.similar = 0.65}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍退出");
                    ClickImageMatch({.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                target = 3;
                break;
            }
            case 3: {
                // 银票礼盒
                if (config.value("银票礼盒兑换").toBool() && record_event[0]) {
                    record_event[0] = false;
                    OpenKnapsack();
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品综合入口");
                    ClickImageMatch({.similar = 0.55}, nullptr, "按钮物品珍宝阁");
                    ClickImageMatch({.similar = 0.65, .y = -130}, nullptr, "按钮珍宝阁绑元商城");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮珍宝阁搜索");
                    ClickImageMatch({.similar = 0.55}, nullptr, "标志珍宝阁输入名称");
                    input_text("银票礼盒");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮珍宝阁搜索");
                    mouse_down_up({.clickCount = 30}, {988, 694});

                    CloseReward(12);
                    Close(2);
                    continue;
                }

                // 帮派捐献
                if ((config.value("帮派铜钱捐献").toBool() || config.value("帮派银两捐献").toBool()) && record_event[1]) {
                    record_event[1] = false;
                    OpenFaction();
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮帮派福利");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮帮派捐献");

                    if(config.value("帮派铜钱捐献").toBool()) {

                        ClickImageMatch({.similar = 0.65, .clickCount = 3, .scope = {132, 203, 448, 655}}, nullptr, "按钮捐献");
                        Defer(2);
                        if(!CoortImageMatch({.similar = 0.65}, nullptr, "标志帮派捐献不在提示").empty()) {
                            ClickImageMatch({.similar = 0.65}, nullptr, "标志帮派捐献不在提示");
                            ClickImageMatch({.similar = 0.5}, nullptr, "按钮帮派捐献确定");
                            ClickImageMatch({.similar = 0.65, .clickCount = 3, .scope = {132, 203, 448, 655}}, nullptr, "按钮捐献");
                        }


                    }

                    if(config.value("帮派银两捐献").toBool()) {

                        ClickImageMatch({.similar = 0.65, .clickCount = 3, .scope = {514, 212, 811, 646}}, nullptr, "按钮捐献");
                        Defer(2);
                        if(!CoortImageMatch({.similar = 0.65}, nullptr, "标志帮派捐献不在提示").empty()) {
                            ClickImageMatch({.similar = 0.65}, nullptr, "标志帮派捐献不在提示");
                            ClickImageMatch({.similar = 0.5}, nullptr, "按钮帮派捐献确定");
                            ClickImageMatch({.similar = 0.65, .clickCount = 3, .scope = {514, 212, 811, 646}}, nullptr, "按钮捐献");
                        }

                    }
                    CloseReward(12);
                    Close(2);
                    continue;
                }

                // 鸡蛋 卯眼
                if ((config.value("商会鸡蛋").toBool() || config.value("榫头卯眼").toBool()) && record_event[2]) {
                    record_event[2] = false;
                    OpenKnapsack();
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品综合入口");
                    ClickImageMatch({.similar = 0.55}, nullptr, "按钮物品珍宝阁");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮珍宝阁商会");
                    mouse_move({}, {192, 289}, {193, 489});
                    ClickImageMatch({.similar = 0.55}, nullptr, "按钮珍宝阁宝石");
                    ClickImageMatch({.similar = 0.55}, nullptr, "按钮珍宝阁上拉");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮珍宝阁江湖杂货");
                    mouse_move({}, {541, 651}, {541, 251});

                    if(config.value("商会鸡蛋").toBool()) {

                        if(!ClickImageMatch({.similar = 0.65}, nullptr, "按钮珍宝阁一筐鸡蛋").empty()) {
                            mouse_down_up({.clickCount = 5}, {965, 683});
                        }

                    }

                    if(config.value("榫头卯眼").toBool()) {

                        if(!ClickImageMatch({.similar = 0.55}, nullptr, "按钮珍宝阁榫头卯眼").empty()) {
                            mouse_down_up({.clickCount = 12}, {965, 683});
                        }

                    }
                    CloseReward(12);
                    Close(2);

                }

                if(config.value("锦芳绣残片").toBool() && record_event[3]) {
                    record_event[3] = false;
                    OpenKnapsack();
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品积分");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品兑换商店");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮兑换商店道具名称");
                    input_text("锦芳绣残片");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮兑换商店搜索");
                    if(!CoortImageMatch({.similar = 0.65}, nullptr, "标志兑换商店锦芳绣残片").empty()) {
                        mouse_down_up({.clickCount = 12}, {1150, 693});
                    }

                    Close(2);
                }

                target = 0;
                break;
            }

        }

    }

    return 0;


}

void DailyRedemptionTask::objective(const std::string ve) {

}

int DailyRedemptionTask::determine() {

    return 307;
}

int DailyRedemptionTask::detect() {

    return -5;

}