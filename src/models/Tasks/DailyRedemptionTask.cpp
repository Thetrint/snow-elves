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
                    ClickImageMatch({.similar = 0.45, .applyGaussianBlur = false}, nullptr, "按钮珍宝阁上拉");
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

                if(config.value("神厨食材购买").toBool() && record_event[4]) {
                    record_event[4] = false;

                    std::string text = config.value("神厨食材购买清单").toString().toStdString();
                    const std::regex pattern("([^#]+)#");

                    for (std::sregex_iterator it(text.begin(), text.end(), pattern), end_it; it != end_it; ++it) {
                        Ingredients.push_back(it->str(1));
                    }

                    if (OpenMap()) {
                        Log("前往食材供应商");
                        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图中原区域");
                        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
                        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
                        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
                        input_text("1272");

                        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
                        input_text("1726");

                        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
                        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                        Arrive();
                    }

                    ClickImageMatch({.similar = 0.56, .matchCount = 2, .clickDelay = false, .scope = {786, 283, 1033, 534}, .applyGaussianBlur = false}, nullptr, "按钮大世界对话");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮神厨食材购买");


                    for (auto& ingredient : Ingredients) {

                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮杂货商人输入名称");
                        input_text(ingredient);
                        ClickImageMatch({.similar = 0.60}, nullptr, "按钮杂货商人搜索");
                        mouse_down_up({.clickCount = 52}, {1021, 613});
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮杂货商人搜索返回");

                    }

                    Close(2);
                }

                if(config.value("生活技能-艾草").toBool() && record_event[5]) {
                    record_event[5] = false;

                    if (OpenMap()) {
                        Log("前往食材供应商");
                        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
                        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
                        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
                        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
                        input_text("674");

                        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
                        input_text("1721");

                        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
                        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                        Arrive();
                    }

                    ClickImageMatch({.similar = 0.56, .matchCount = 2, .clickDelay = false, .scope = {786, 283, 1033, 534}, .applyGaussianBlur = false}, nullptr, "按钮大世界对话");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮商人新鲜蔬菜");


                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮杂货商人输入名称");
                    input_text("艾草");
                    ClickImageMatch({.similar = 0.60}, nullptr, "按钮杂货商人搜索");
                    mouse_down_up({.clickCount = 52}, {1021, 613});
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮杂货商人搜索返回");

                    Close(2);
                }

                if(config.value("生活技能-莲子").toBool() && record_event[6]) {
                    record_event[6] = false;

                    if (OpenMap()) {
                        Log("前往食材供应商");
                        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
                        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
                        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
                        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
                        input_text("669");

                        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
                        input_text("1700");

                        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
                        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
                        Arrive();
                    }

                    ClickImageMatch({.similar = 0.56, .matchCount = 2, .clickDelay = false, .scope = {786, 283, 1033, 534}, .applyGaussianBlur = false}, nullptr, "按钮大世界对话");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮商人小蟹小虾");

                    mouse_down_up({.clickCount = 52}, {1021, 613});


                    Close(2);
                }

                // 银票礼盒
                if (config.value("商票上缴").toBool() && record_event[7]) {
                    record_event[7] = false;
                    OpenFaction();
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮帮派福利");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮帮派上缴商票");
                    Arrive();
                    ClickImageMatch({.similar = 0.55}, nullptr, "按钮行商商票上缴");
                    ClickImageMatch({.similar = 0.65, .click = FORWARD}, nullptr, "标志行商高级商票");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮行商上缴");

                    CloseReward(12);
                    Close(2);
                    continue;
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