//
// Created by y1726 on 24-7-16.
//
#include "models/Tasks/DailyCopiesTask.h"

#include <utils/LoadJsonFile.h>



int DailyCopiesTask::implementation() {
    int target = 1;
    timer.start();
    // 重置同步机制
    DailCopiesFinish[id] = false;

    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720)) {
            return 0;
        }
        if (config.value("副本模式").toString() == "带队模式") {
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
                    if (!CoortImageMatch({.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        ClickImageMatch({.similar = 0.75}, nullptr, "按钮队伍创建");
                    }
                    ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮队伍下拉");
                    mouse_move({}, {258, 307}, {258, 607});
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍无目标");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍江湖纪事");
                    ClickImageMatch({.similar = 0.65, .scope = {110, 510, 438, 632}}, nullptr, "按钮队伍自动匹配");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍确定");
                    ClickImageMatch({.similar = 0.65}, nullptr, "按钮确定");
                    if(LoadJsonFile::instance().jsonFiles[id].value("副本人数").toInt() != 1) {
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍自动匹配1");
                    }
                    Log("队伍检测完成");
                    target = 3;
                    break;
                }
                // 队伍检测
                case 3: {
                    OpenTeam();
                    // 离线检测
                    OfflineDetection();
                    // 检测队伍人数
                    if(CoortImageMatch({.similar = 0.5}, nullptr, "按钮队伍空位").size() <= 10 - config.value("副本人数").toInt()) {
                        // 判断进入副本最大次数
                        if(++record_num[0] > 3) {
                            // 重置进入副本次数
                            record_num[0] = 0;
                            // 离开队伍
                            LeaveTeam();
                            target = 2;
                            continue;
                        }
                        // 进入副本
                        if(!ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍进入副本").empty()) {
                            ClickImageMatch({.similar = 0.65}, nullptr, "按钮副本确认");
                            // 重置副本内任务激活标志
                            record_event[0] = true;
                            // 重置副本内脱离卡死次数
                            record_num[2] = 0;
                            PassLevel();
                            target = 4;
                            continue;
                        }
                        // 没有进入副本按钮 队伍创建/目标设置失败重新创建/设置
                        target = 2;
                        continue;
                    }
                    // 世界喊话
                    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 30) {
                        Shout(config.value("副本喊话内容").toString().toStdString());
                        record_time[0] = std::chrono::steady_clock::now();
                    }
                    break;
                }
                case 4: {
                    // 判断是否在副本内
                    if (CoortImageMatch({.similar = 0.65}, nullptr, "界面大世界1", "界面大世界2", "按钮副本退出").size() == 3) {
                        // 重置进入副本次数
                        record_num[1] = 0;
                        // 副本中重置进本次数
                        record_num[0] = 0;

                        // 万劫召回
                        if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志悬赏开始探查").empty()) {
                            if(++record_num[4] > 5) {
                                FollowDetectionNoWait();
                            }
                            Defer(1);
                            continue;
                        }
                        record_num[4] = 0;


                        // 检测副本完成
                        if (!CoortImageMatch(MatchParams{.similar = 0.5, .scope = {1150, 175, 1335, 250}}, nullptr, "标志副本完成").empty()) {
                            // 副本延迟退出判断
                            if(++record_num[3] > 4) {
                                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮副本退出");
                                mouse_down_up({.clickCount = 2}, {0, 0});
                                if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
                                    // 通知队员完成
                                    DailCopiesFinish[id] = true;
                                    PassLevel();
                                    target = 0;
                                }
                            }
                            Defer(1);
                            continue;
                        }

                        // 重置副本延迟退出次数
                        record_num[3] = 0;

                        // 副本激活
                        if (record_event[0]) {
                            // 队员召回
                            FollowDetectionNoWait();
                            if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务").empty()) {
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                            }
                            ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界任务");
                            if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本任务").empty()) {
                                // 关闭副本内激活
                                record_event[0] = false;
                                // 启动副本内脱离卡死
                                record_event[1] = true;
                                // 重置脱离卡死时间
                                record_time[1] = std::chrono::steady_clock::now();
                            }
                        }
                        continue;

                    }

                    // 脱离卡死or重开
                    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[1]).count() > 420 && record_event[1]) {
                        // 关闭脱离卡死
                        record_event[1] = false;
                        //脱离卡死
                        OffCard();
                        // 副本激活
                        record_event[0] = true;
                        if(++record_num[2] > 2) {
                            //离开队伍
                            LeaveTeam();
                        }
                    }

                    // 奖励结算界面处理
                    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志副本获得").empty()) {
                        mouse_down_up({}, {0, 0});
                    }


                    // 交易界面
                    if (!CoortImageMatch({.similar = 0.65}, nullptr, "界面交易").empty()) {
                        Close(1);
                    }

                    if (!CoortImageMatch({.similar = 0.65}, nullptr, "界面队伍").empty()) {
                        Close(1);
                    }

                    // 跳过剧情
                    if (!CoortImageMatch({.similar = 0.65}, nullptr, "按钮副本跳过剧情").empty()) {
                        ClickImageMatch({.similar = 0.65}, nullptr, "按钮副本跳过剧情");
                    }

                    // 副本内判断次数上限
                    if(++record_num[1] >= 15) {
                        target = 3;
                    }

                    Defer(1);
                }

            }
        }else if (config.value("副本模式").toString() == "固定队模式") {
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
                        target = 0;
                        continue;
                    }
                    Log("队伍检测完成");
                    target = 3;
                    Close(1);
                    break;
                }
                case 3: {
                    // 等待队长发布完成信号
                    if(DailCopiesFinish[config.value("日常悬赏队长编号").toInt() - 1]) {
                        PassLevel();
                        target = 0;
                        continue;
                    }

                    // 确认副本
                    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面日常").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
                    }

                    Defer(1);
                }
            }
        }

    }

    return 0;


}


void DailyCopiesTask::objective(const std::string ve) {

}

int DailyCopiesTask::determine() {

    return 307;
}

int DailyCopiesTask::detect() {

    return -5;

}