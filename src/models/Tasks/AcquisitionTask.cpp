//
// Created by y1726 on 24-8-15.
//
#include "models/Tasks/AcquisitionTask.h"

int AcquisitionTask::implementation() {
    int target = 1;
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720 * config.value("采集次数").toInt())) {
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
                // LocationDetection();
                target = 2;
                break;
            }
            // 队伍检测
            case 2: {
                // OpenTeam();
                // if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍创建").empty()) {
                //     ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮队伍退出");
                //     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                // }
                // Close({.similar = 0.5}, 1);
                target = 3;
                break;
            }
            case 3: {
                std::string text;
                if(config.value("采集模式").toString() == "默认模式") {
                    text = SetText[config.value("采集物品").toString().toStdString()];
                }else if(config.value("采集模式").toString() == "自定义模式") {
                    text = config.value("采集自定义内容").toString().toStdString();
                }

                // 解析地图
                const std::regex regex("\\$(.*?)\\$");
                for (std::sregex_iterator it(text.begin(), text.end(), regex), end_it; it != end_it; ++it) {

                    if (it->str(1) == "江南") {
                        JNGo();
                    }
                    if (it->str(1) == "金陵") {
                        JLGo();
                    }
                    if (it->str(1) == "风雷岛") {
                        FLDGo();
                    }
                    if (it->str(1) == "中原") {
                        ZYGo();
                    }
                    if (it->str(1) == "塞北") {
                        SBGo();
                    }
                    break;
                }


                // 解析坐标
                const std::regex regex1("([\\d]+)#([\\d]+)");
                for (std::sregex_iterator it(text.begin(), text.end(), regex1), end_it; it != end_it; ++it) {
                    LocationVector.push_back({it->str(1), it->str(2)});
                }

                // 启动加速线程
                thread = std::jthread(&hasten, this);
                target = 4;
                break;
            }
            case 4: {
                if(!LocationVector.empty()) {
                    // 取出坐标
                    Location location = LocationVector[0];
                    // 删除坐标
                    LocationVector.erase(LocationVector.begin());

                    if(!LocationVector.empty()) {
                        // 添加坐标到末尾
                        LocationVector.push_back(location);
                    }

                    // 前往坐标
                    LocationGO(location.x, location.y);
                }
                target = 5;
                break;
            }
            case 5: {

                // 关闭奖励
                mouse_down_up({}, {0, 0});
                CloseReward(3);
                // 采集
                if(!ClickImageMatch(MatchParams{.similar = 0.60, .matchCount = 3, .clickDelay = false}, nullptr, "按钮生活采集挖矿", "按钮生活采集砍伐", "按钮生活采集采集").empty()) {
                    // 加速
                    record_event[0] = true;
                    Defer(2, 1300);
                    // 工具判断
                    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面交易", "按钮交易购买").empty() && config.value("采集工具购买").toBool()) {
                        record_event[0] = false;
                        if(!ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮交易购买").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
                            Close( 1);
                        }else {
                            target = 0;
                            continue;
                        }

                    }else if(!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易", "按钮交易购买").empty()){
                        target = 0;
                    }
                    for(int i = 0; i <= 4 && record_event[0]; i++) {
                        Defer(1, 1000);
                    }
                    // 结束加速
                    record_event[0] = false;
                    Log(std::format("采集 {} 次", record_num[1]));
                    // 次数判断
                    if (record_num[1]++ > config.value("采集次数").toInt()) {
                        target = 0;
                    }
                    Defer(2, 1300);
                    continue;

                }
                // 体力判断
                if (!CoortImageMatch(MatchParams{.similar = 0.91, .scope = {788, 292, 1132, 596}, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "标志生活采集体力耗尽").empty()
                    && config.value("采集自动吃鸡蛋").toBool()
                    && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 180) {
                    record_time[0] = std::chrono::steady_clock::now();
                    // 自动吃鸡蛋
                    OpenKnapsack();
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品搜索");
                    ClickImageMatch(MatchParams{.similar = 0.55}, nullptr, "按钮物品输入名称");
                    input_text("一筐鸡蛋");
                    ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品搜索");

                    if(CoortImageMatch(MatchParams{.similar = 0.98, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮物品一筐鸡蛋").empty()) {
                        target = 0;
                        continue;
                    }
                    for(int i = 1; i <= config.value("采集吃鸡蛋数量").toInt() && unbind_event; i++) {
                        ClickImageMatch(MatchParams{.similar = 0.98, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮物品一筐鸡蛋");
                        ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮物品使用");
                    }

                    BackInterface();

                    continue;
                }
                // 不吃鸡蛋 结束任务
                if(!CoortImageMatch(MatchParams{.similar = 0.91, .scope = {788, 292, 1132, 596}, .convertToGray = false, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "标志生活采集体力耗尽").empty()
                    && !config.value("采集自动吃鸡蛋").toBool()){
                    target = 0;
                    continue;
                }
                // 换线
                if(config.value("采集换线").toInt() !=1) {
                    Changeover(3, record_num[0]);
                    if (record_num[0]++ == config.value("采集换线").toInt()) {
                        record_num[0] = 1;
                        target = 4;
                    }
                }

                Defer(2);

                break;
            }

        }

    }

    return 0;


}

void AcquisitionTask::objective(const std::string ve) {

}

int AcquisitionTask::determine() {

    return 307;
}

int AcquisitionTask::detect() {

    return -5;

}

void AcquisitionTask::hasten() {

    while (unbind_event) {


        // 在此处添加你希望循环中执行的操作
        if (record_event[0] && !CoortImageMatch({.similar = 0.5, .scope = {569, 447, 740, 568}, .applyGaussianBlur = false}, nullptr, "标志生活采集采集加速").empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(930));
            mouse_down_up({.clickDelay = false}, {666, 474});
            record_event[0] = false;
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}