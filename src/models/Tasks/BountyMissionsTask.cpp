//
// Created by y1726 on 24-7-17.
//
#include "models/Tasks/BountyMissionsTask.h"

#include <utils/LoadJsonFile.h>


int BountyMissionsTask::implementation() {
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

        // 限制整体运行速度
        Defer(1);

        if (config.value("副本模式").toString() == "带队模式"){
            switch (target) {
                // 任务退出
                case 0: {
                    return 0;
                }
                // 位置检测
                case 1: {
                    LocationDetection();
                    break;
                }
                // 队伍检测
                case 2: {
                    OpenTeam();
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建");
                    }
                    ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮队伍下拉");
                    mouse_move({}, {258, 307}, {258, 607});
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍无目标");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍江湖纪事");
                    ClickImageMatch(MatchParams{.similar = 0.5, .scope = {110, 510, 438, 632}}, nullptr, "按钮队伍自动匹配");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍确定");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                    if(LoadJsonFile::instance().jsonFiles[id].value("副本人数").toInt() != 1) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍自动匹配1");
                    }
                    break;
                }
                case 3: {
                    // 悬赏检测
                    if(CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面悬赏").empty()) {
                        // 返回主界面
                        Close(1);
                        mouse_down_up({}, {0, 0});
                        OpenKnapsack();
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动悬赏");
                        Defer(2);
                    }else {
                        // 记录当前已接悬赏
                        const int num =  static_cast<int>(CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮悬赏前往").size());

                        if(!CoortImageMatch(MatchParams{.similar = 0.85, .clickDelay = false}, nullptr, "标志悬赏完成").empty()) {
                            if(record_event[2]) {
                                record_event[2] = false;
                                ClickImageMatch(MatchParams{.similar = 0.65, .x = -60, .clickCount = 51}, nullptr, "按钮悬赏上页");
                                continue;
                            }
                            Close(3);
                            if (num == 0 and std::ranges::all_of(BountyFinish, [](const bool event) { return event; })) {
                                target = 0;
                                continue;
                            }
                            break;
                        }

                        // 接取悬赏
                        ClickImageMatch(MatchParams{.similar = 0.6, .clickDelay = false}, nullptr, "按钮悬赏刷新");
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        if(!ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 1, .y = 330, .clickDelay = false, .scope = {267 + 231 * num, 182, 1335, 558}}, nullptr, "标志悬赏江湖纪事").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 15, .matchDelay = false}, nullptr, "按钮悬赏铜钱押金");
                            continue;
                        }
                        // record_num[0] = static_cast<int>(CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮悬赏前往").size());
                        ClickImageMatch(MatchParams{.similar = 0.65, .x = 60}, nullptr, "按钮悬赏下页");
                        if (++record_num[1] > 10) {
                            record_num[1] = 0;
                            ClickImageMatch(MatchParams{.similar = 0.5, .x = -60, .clickCount = 51}, nullptr, "按钮悬赏上页");
                        }

                    }
                    continue;
                }
                case 4: {
                    OpenTeam();
                    if(!std::ranges::all_of(BountyAccess, [](const bool event) { return event; })) {
                        // 等待成员接取悬赏
                        continue;
                    }
                    // 离线检查

                    // 检测队伍人数
                    if(CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍空位").size() <= 10 - config.value("副本人数").toInt()) {
                        // 跟随召集
                        // 进入副本
                        if(++record_num[0] > 3) {
                            // 重置进入副本次数
                            record_num[0] = 0;
                            LeaveTeam();
                            target = 2;
                            continue;
                        }
                        if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍进入副本").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
                            // 重置副本内任务激活标志
                            record_event[0] = true;
                            // 脱离卡死次数重置
                            record_num[3] = 0;
                            PassLevel();
                        }else {
                            // 没有进入副本按钮 队伍创建/目标设置失败重新创建/设置
                            target = 2;
                            continue;
                        }
                    }else {
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[1]).count() > 30) {
                            // 返回主界面
                            BackInterface();
                            Shout(config.value("副本喊话内容").toString().toStdString());
                            record_time[1] = std::chrono::steady_clock::now();
                            OpenTeam();
                        }
                        continue;
                    }
                    break;
                }
                case 5:{
                    // 副本内检测
                    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面大世界1", "界面大世界2", "按钮副本退出").size() == 3) {
                        // 重置副本内判断次数
                        record_num[0] = 0;

                        // 副本完成
                        if (!CoortImageMatch(MatchParams{.similar = 0.5, .scope = {1150, 175, 1335, 250}}, nullptr, "标志副本完成").empty()) {
                            // 副本延迟退出判断
                            if(++record_num[2] > 4) {
                                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮副本退出");
                                mouse_down_up({.clickCount = 2}, {0, 0});
                                if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
                                    target = 3;
                                    PassLevel();
                                }
                            }
                            Defer(1);
                            continue;
                        }

                        // 重置副本延迟退出次数
                        record_num[2] = 0;

                        // 副本激活
                        if (record_event[0]) {
                            if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务").empty()) {
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                            }
                            ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界任务");
                            if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本任务").empty()) {
                                record_event[0] = false;
                                record_event[1] = true;
                                // 副本脱离卡死时间重置
                                record_time[0] = std::chrono::steady_clock::now();
                            }
                        }
                        continue;
                    }
                    // 脱离卡死
                    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 420 && record_event[1]) {
                        record_event[1] = false;
                        //脱离卡死
                        OffCard();
                        // 副本激活
                        record_event[0] = true;
                        if(++record_num[3] > 2) {
                            //离开队伍
                            LeaveTeam();
                        }
                        record_time[0] = std::chrono::steady_clock::now();
                    }

                    // 交易界面
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易").empty()) {
                        Close({.similar = 0.5}, 1);
                    }

                    // 跳过剧情
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情");
                    }

                    if(++record_num[0] >= 15) {
                        target = 3;
                    }
                    Defer(2);
                    continue;
                }
                default:
                    target = -1;
                    break;
            }
        }else if (config.value("副本模式").toString() == "固定队模式") {
            switch (target) {
                // 任务退出
                case 0: {
                    return 0;
                }
                // 位置检测
                case 1: {
                    LocationDetection();
                    break;
                }
                // 队伍检测
                case 2: {
                    OpenTeam();
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        target = 0;
                        continue;
                    }
                    Close(1);
                    break;
                }
                case 3: {
                    // 同步标志让带队等待 检测悬赏
                    BountyAccess[id] = false;
                    // 同步标志通知带队完成
                    BountyFinish[id] = false;

                    // 悬赏检测
                    if(CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面悬赏").empty()) {
                        Close(1);
                        mouse_down_up({}, {0, 0});
                        OpenKnapsack();
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动悬赏");
                        Defer(2);
                    }else {
                        // 记录当前已接悬赏
                        const int num =  static_cast<int>(CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮悬赏前往").size());

                        if(!CoortImageMatch(MatchParams{.similar = 0.85, .clickDelay = false}, nullptr, "标志悬赏完成").empty()) {
                            if(record_event[2]) {
                                record_event[2] = false;
                                ClickImageMatch(MatchParams{.similar = 0.65, .x = -60, .clickCount = 11}, nullptr, "按钮悬赏上页");
                                continue;
                            }
                            BountyAccess[id] = true;
                            Close(3);
                            if (num == 0) {
                                target = 0;
                                BountyFinish[id] = true;
                                continue;
                            }
                            break;
                        }

                        // 接取悬赏
                        ClickImageMatch(MatchParams{.similar = 0.6, .clickDelay = false}, nullptr, "按钮悬赏刷新");
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        if(!ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 1, .y = 330, .clickDelay = false, .scope = {267 + 231 * num, 182, 1335, 558}}, nullptr, "标志悬赏江湖纪事").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 15, .matchDelay = false}, nullptr, "按钮悬赏铜钱押金");
                            continue;
                        }
                        // record_num[0] = static_cast<int>(CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮悬赏前往").size());
                        ClickImageMatch(MatchParams{.similar = 0.65, .x = 60}, nullptr, "按钮悬赏下页");
                        if (++record_num[1] > 10) {
                            record_num[1] = 0;
                            ClickImageMatch(MatchParams{.similar = 0.5, .x = -60, .clickCount = 11}, nullptr, "按钮悬赏上页");
                        }

                    }
                    continue;
                }
                case 4:{
                    // 副本内检测
                    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面大世界1", "界面大世界2", "按钮副本退出").size() == 3) {

                        // 万劫山庄跟随激活
                        if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志副本继续").empty()) {
                            mouse_down_up({}, {0, 0});
                            OpenTeam();
                            ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 2, .clickCount = 2}, nullptr, "按钮队伍取消跟随");
                            ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 2}, nullptr, "按钮队伍跟随队长");
                            ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 2}, nullptr, "按钮确定");
                            Close({.similar = 0.5}, 2);

                        }

                        // 副本完成
                        if (!CoortImageMatch(MatchParams{.similar = 0.5, .scope = {1150, 175, 1335, 250}}, nullptr, "标志副本完成").empty()) {
                            // 副本延迟退出判断
                            if(++record_num[2] > 2) {
                                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮副本退出");
                                mouse_down_up({.clickCount = 2}, {0, 0});
                                if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
                                    target = 3;
                                    PassLevel();
                                }
                            }
                            Defer(1);
                            continue;
                        }

                        // 重置副本延迟退出次数
                        record_num[2] = 0;
                    }

                    // 确认副本
                    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面日常").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
                    }

                    Defer(1);
                    continue;
                }

                default:
                    target = -1;
                    break;
            }
        }

        target++;


    }
    return 0;
}

void BountyMissionsTask::objective(const std::string ve) {
}

int BountyMissionsTask::determine() {
     return 307;
}

int BountyMissionsTask::detect() {

    return -5;
}


