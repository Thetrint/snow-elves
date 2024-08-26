//
// Created by y1726 on 24-8-22.
//
#include "models/Tasks/SweepStallTask.h"

int SweepStallTask::implementation() {
    int target = 1;
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720 * 1000)) {
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
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮物品交易");
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮交易摆摊");

                // 启动刷新线程
                thread = std::jthread(&flushed, this);

                productSet.clear();  // 清空现有的元素

                if(config.value("商品1").toBool()) {
                    productSet.emplace(1);  // 插入 1 到 8
                }

                if(config.value("商品2").toBool()) {
                    productSet.emplace(2);  // 插入 1 到 8
                }
                if(config.value("商品3").toBool()) {
                    productSet.emplace(3);  // 插入 1 到 8
                }

                if(config.value("商品4").toBool()) {
                    productSet.emplace(4);  // 插入 1 到 8
                }

                if(config.value("商品5").toBool()) {
                    productSet.emplace(5);  // 插入 1 到 8
                }

                if(config.value("商品6").toBool()) {
                    productSet.emplace(6);  // 插入 1 到 8
                }

                if(config.value("商品7").toBool()) {
                    productSet.emplace(7);  // 插入 1 到 8
                }

                if(config.value("商品8").toBool()) {
                    productSet.emplace(8);  // 插入 1 到 8
                }


                record_event[0] = true;
                target = 4;
                break;
            }
            case 4: {
                // 检查 是否有商品
                if (!record_event[1]) {
                    continue;
                }
                record_event[1] = false;
                for(auto [location, score] :CoortImageMatch({.similar = 0.95, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "标志交易商品")) {
                    if(config.value("商品1").toBool()) {
                        if (547 - 8 <= location.x && location.x <= 547 + 8 && 218 - 8 <= location.y && location.y <= 218 +8) {
                            productSet.erase(1);
                            continue;
                        }
                    }

                    if(config.value("商品2").toBool()) {
                        if (960 - 8 <= location.x && location.x <= 960 + 8 && 218 - 8 <= location.y && location.y <= 218 +8) {
                            productSet.erase(2);
                            continue;
                        }
                    }

                    if(config.value("商品3").toBool()) {
                        if (547 - 8 <= location.x && location.x <= 547 + 8 && 326 - 8 <= location.y && location.y <= 326 +8) {
                            productSet.erase(3);
                            continue;
                        }
                    }

                    if(config.value("商品4").toBool()) {
                        if (960 - 8 <= location.x && location.x <= 960 + 8 && 326 - 8 <= location.y && location.y <= 326 +8) {
                            productSet.erase(4);
                            continue;
                        }
                    }

                    if(config.value("商品5").toBool()) {
                        if (547 - 8 <= location.x && location.x <= 547 + 8 && 434 - 8 <= location.y && location.y <= 434 +8) {
                            productSet.erase(5);
                            continue;
                        }
                    }

                    if(config.value("商品6").toBool()) {
                        if (960 - 8 <= location.x && location.x <= 960 + 8 && 434 - 8 <= location.y && location.y <= 434 +8) {
                            productSet.erase(6);
                            continue;
                        }
                    }

                    if(config.value("商品7").toBool()) {
                        if (547 - 8 <= location.x && location.x <= 547 + 8 && 542 - 8 <= location.y && location.y <= 542 +8) {
                            productSet.erase(7);
                            continue;
                        }
                    }

                    if(config.value("商品8").toBool()) {
                        if (960 - 8 <= location.x && location.x <= 960 + 8 && 542 - 8 <= location.y && location.y <= 542 +8) {
                            productSet.erase(8);
                            continue;
                        }
                    }
                }

                // 计算需要购买商品
                if(!productSet.empty()) {
                    record_event[0] = false;
                    for (auto key : productSet) {
                        productQueue.emplace(LocationMap[key], config.value(QString::fromStdString(std::format("优先级{}", key))).toInt());
                        if(config.value("快速购买").toBool()) {
                            break;
                        }
                    }

                }else {
                    record_event[0] = true;
                    continue;
                }


                // 购买商品
                if(!productQueue.empty()) {
                    auto [location, priority] = productQueue.top();
                    mouse_down_up({.clickDelay = false}, location);
                    // 延迟
                    std::this_thread::sleep_for(std::chrono::milliseconds(config.value("摆摊延迟1").toInt()));
                    while (unbind_event) {
                        // 点击
                        mouse_down_up({.clickDelay = false}, {547, 218});
                        std::this_thread::sleep_for(std::chrono::milliseconds(config.value("摆摊延迟2").toInt()));
                        if(!ClickImageMatch({.similar = 0.65, .matchCount = 5, .clickDelay = false, .matchDelay = false}, nullptr, "按钮交易购买").empty()) {
                            record_num[0] = 0;
                            ClickImageMatch({.similar = 0.6, .matchCount = 15, .clickDelay = false, .matchDelay = false}, nullptr, "按钮确定");
                            std::this_thread::sleep_for(std::chrono::milliseconds(config.value("摆摊延迟3").toInt()));
                        }else {
                            if(++record_num[0]> 2 && unbind_event) {
                                // 启动刷新
                                record_event[0] = true;

                                // 清空队列
                                std::priority_queue<Product, std::vector<Product>, Compare> empty;
                                std::swap(productQueue, empty);  // 快速清空队列

                                productSet.clear();  // 清空现有的元素

                                if(config.value("商品1").toBool()) {
                                    productSet.emplace(1);  // 插入 1 到 8
                                }

                                if(config.value("商品2").toBool()) {
                                    productSet.emplace(2);  // 插入 1 到 8
                                }
                                if(config.value("商品3").toBool()) {
                                    productSet.emplace(3);  // 插入 1 到 8
                                }

                                if(config.value("商品4").toBool()) {
                                    productSet.emplace(4);  // 插入 1 到 8
                                }

                                if(config.value("商品5").toBool()) {
                                    productSet.emplace(5);  // 插入 1 到 8
                                }

                                if(config.value("商品6").toBool()) {
                                    productSet.emplace(6);  // 插入 1 到 8
                                }

                                if(config.value("商品7").toBool()) {
                                    productSet.emplace(7);  // 插入 1 到 8
                                }

                                if(config.value("商品8").toBool()) {
                                    productSet.emplace(8);  // 插入 1 到 8
                                }

                                std::this_thread::sleep_for(std::chrono::milliseconds(1501));
                                break;
                            }
                        }

                    }

                }

            }

        }

    }

    return 0;


}

void SweepStallTask::objective(const std::string ve) {

}

int SweepStallTask::determine() {

    return 307;
}

int SweepStallTask::detect() {

    return -5;

}

void SweepStallTask::flushed() {

    while (unbind_event) {


        // 在此处添加你希望循环中执行的操作
        if (record_event[0]) {
            mouse_down_up({.clickDelay = false}, {175, 292});
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            record_event[1] = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(550));
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}