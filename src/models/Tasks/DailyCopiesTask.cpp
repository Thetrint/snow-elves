//
// Created by y1726 on 24-7-16.
//
#include "models/Tasks/DailyCopiesTask.h"

#include <utils/LoadJsonFile.h>



int DailyCopiesTask::implementation() {
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
                    OpenTeam();
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
                            record_num[3] = 0;
                            PassLevel();
                        }else {
                            // 没有进入副本按钮 队伍创建/目标设置失败重新创建/设置
                            target = 2;
                            continue;
                        }
                    }else {
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[1]).count() > 30) {
                            Close(1);
                            Shout(config.value("副本喊话内容").toString().toStdString());
                            record_time[1] = std::chrono::steady_clock::now();
                            OpenTeam();
                        }
                        continue;
                    }
                    break;
                }
                case 4: {
                    // 副本内检测
                    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面大世界1", "界面大世界2", "按钮副本退出").size() == 3) {
                        // 重置副本内判断次数
                        record_num[1] = 0;

                        // 副本完成
                        if (!CoortImageMatch(MatchParams{.similar = 0.5, .scope = {1150, 175, 1335, 250}}, nullptr, "标志副本完成").empty()) {
                            // 副本延迟退出判断
                            if(++record_num[2] > 3) {
                                ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮副本退出");
                                mouse_down_up({.clickCount = 2}, {0, 0});
                                if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
                                    target = 0;
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
                                record_time[0] = std::chrono::steady_clock::now();
                            }
                        }
                        continue;
                    }

                    // 奖励结算界面处理
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志副本获得").empty()) {
                        mouse_down_up({}, {0, 0});
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
                    }

                    // 交易界面
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易").empty()) {
                        Close({.similar = 0.5}, 1);
                    }

                    // 跳过剧情
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情");
                    }

                    if(++record_num[1] >= 15) {
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

                        // 奖励结算界面处理
                        if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志副本获得").empty()) {
                            mouse_down_up({}, {0, 0});
                        }

                        // 副本完成
                        if (!CoortImageMatch(MatchParams{.similar = 0.5, .scope = {1150, 175, 1335, 250}}, nullptr, "标志副本完成").empty()) {
                            ClickImageMatch(MatchParams{.similar = 0.65}, nullptr, "按钮副本退出");
                            mouse_down_up({.clickCount = 2}, {0, 0});
                            if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
                                target = 0;
                                continue;
                            }
                        }
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
//
// int DailyCopiesTask::implementation() {
//     std::vector<Match> matchs;
//     objective("位置检测");
//     timer.start();
//     while (unbind_event) {
//
//         if (disrupted) {
//             timer.pause();
//             return -1; //任务调度中止任务z
//         }
//
//         if (timer.read() >= std::chrono::seconds(720)) {
//             return 0;
//         }
//
//         if (LoadJsonFile::instance().jsonFiles[id].value("副本模式").toString() == "带队模式") {
//             switch (determine()) {
//                 case 0:
//                     return 0; // 任务正常退出
//                 case -1:
//                     Close({.similar = 0.5}, 1);
//                     break;
//                 case 1:
//                     LocationDetection();
//                     objective("队伍检测");
//                     break;
//                 case 2:
//                     OpenTeam();
//                     if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
//                         ClickImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建");
//                     }
//                     ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮队伍下拉");
//                     mouse_move({}, {258, 307}, {258, 607});
//                     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍无目标");
//                     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍江湖纪事");
//                     ClickImageMatch(MatchParams{.similar = 0.5, .scope = {110, 510, 438, 632}}, nullptr, "按钮队伍自动匹配");
//                     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍确定");
//                     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
//                     if(LoadJsonFile::instance().jsonFiles[id].value("副本人数").toInt() != 1) {
//                         ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍自动匹配1");
//
//                     }
//                     objective("开始任务");
//                     break;
//                 case 3:
//                     if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
//                         objective("队伍检测");
//                     }
//                     //离线检测
//                     if(CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍空位").size() <= 10 - LoadJsonFile::instance().jsonFiles[id].value("副本人数").toInt()) {
//                         if (++record_num[1] < 4) {
//                             ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍进入副本");
//                             ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
//                             if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
//                                 Close({.similar = 0.5}, 1);
//                             }
//                             record_event[0] = true;
//                             record_num[2] = 0;
//                             record_num[0] = 0;
//                             objective("等待完成");
//                         }else {
//                             //退出队伍
//                             LeaveTeam();
//                             record_num[1] = 0;
//                             objective("队伍检测");
//                         }
//
//                     }else {
//                         if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[1]).count() > 300) {
//                             Close({.similar = 0.5}, 2);
//                             Shout(LoadJsonFile::instance().jsonFiles[id].value("副本喊话内容").toString().toStdString());
//                             record_time[1] = std::chrono::steady_clock::now();
//                         }
//                     }
//                     break;
//                 case 4:
//                     OpenTeam();
//                     break;
//                 case 5:
//                     if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
//                         if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本退出").empty()) {
//                             if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志副本获得").empty()) {
//                                 mouse_down_up({}, {0, 0});
//                             }
//                             if (++record_num[0] >= 5) {
//                                 record_num[0] = 0;
//                                 objective("开始任务");
//                             }
//                             std::this_thread::sleep_for(std::chrono::milliseconds(2800));
//                         }else {
//
//                             if (!CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "标志副本完成").empty()) {
//                                 ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本退出");
//                                 mouse_down_up({}, {0, 0});
//                                 mouse_down_up({}, {0, 0});
//                                 if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
//                                     objective("任务退出");
//                                 }
//
//
//                             }
//                             if (record_event[0]) {
//                                 if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务").empty()) {
//                                     ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
//                                 }
//                                 ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界任务");
//                                 ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本任务");
//                                 record_time[0] = std::chrono::steady_clock::now();
//                                 record_event[0] = false;
//                                 record_num[1] = 0;
//                                 record_event[1] = true;
//
//                             }
//
//                             record_num[0] = 0;
//                         }
//
//                     }
//                     if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 300 && record_event[1]) {
//                         //脱离卡死
//                         OffCard();
//                         record_event[1] = false;
//                         record_event[0] = true;
//                         if(++record_num[2] >= 2) {
//                             //离开队伍
//                             LeaveTeam();
//                             objective("队伍检测");
//                         }
//                         record_time[0] = std::chrono::steady_clock::now();
//                     }
//                     if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易").empty()) {
//                         Close({.similar = 0.5}, 1);
//                     }
//                     if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情").empty()) {
//                         ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情");
//                     }
//
//
//                     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//                     break;
//                 case 6:
//                     Close({.similar = 0.5}, 1);
//                     break;
//                 default:
//                     break;;
//             }
//         }else if (LoadJsonFile::instance().jsonFiles[id].value("副本模式").toString() == "固定队模式") {
//             switch (determine()) {
//                 case 0:
//                     return 0; // 任务正常退出
//                 case -1:
//                     Close({.similar = 0.5}, 1);
//                     break;
//                 case 1:
//                     LocationDetection();
//                     objective("队伍检测");
//                     break;
//                 case 2:
//                     OpenTeam();
//                     if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
//                         objective("任务退出");
//                         continue;
//                     }
//                     Close({.similar = 0.5}, 2);
//                     objective("开始任务");
//                     break;
//                 case 3:
//                     if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面日常").empty()) {
//                         ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
//                     }
//
//                     if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志副本继续").empty()) {
//                         mouse_down_up({}, {0, 0});
//                         OpenTeam();
//                         ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 2, .clickCount = 2}, nullptr, "按钮队伍取消跟随");
//                         ClickImageMatch(MatchParams{.similar = 0.5, .matchCount = 2}, nullptr, "按钮队伍跟随队长");
//                         ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 2}, nullptr, "按钮确定");
//                         Close({.similar = 0.5}, 2);
//
//                     }
//
//                     if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志副本获得").empty()) {
//                         mouse_down_up({}, {0, 0});
//                     }
//
//                     if (!CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "标志副本完成").empty()) {
//                         objective("任务退出");
//
//                     }
//                     break;
//                 default:
//                     break;;
//             }
//         }
//     }
//
//     return 0;
//
//
// }

void DailyCopiesTask::objective(const std::string ve) {
    cause = ve;
}

int DailyCopiesTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 10) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }else {
        detect_count = 0;
    }

    if (LoadJsonFile::instance().jsonFiles[id].value("副本模式").toString() == "带队模式") {
        if (cause == "任务退出") {
            switch (sw) {
                case 1:
                    return 0;
                default:
                    return -1;
            }
        }

        if (cause == "位置检测") {
            switch (sw) {
                case 1:
                    return 1;
                default:
                    return -1;
            }
        }

        if (cause == "队伍检测") {
            switch (sw) {
                case 1:
                    return 2;
                default:
                    return -1;
            }
        }

        if (cause == "开始任务") {
            switch (sw) {
                case 1:
                    return 4;
                case 2:
                    return 3;
                default:
                    return -1;
            }
        }

        if (cause == "等待完成") {
            switch (sw) {
                case 2:
                    return 6;
                default:
                    return 5;
            }
        }
    }else if(LoadJsonFile::instance().jsonFiles[id].value("副本模式").toString() == "固定队模式") {
        if (cause == "任务退出") {
            switch (sw) {
                case 1:
                    return 0;
                default:
                    return -1;
            }
        }

        if (cause == "位置检测") {
            switch (sw) {
                case 1:
                    return 1;
                default:
                    return -1;
            }
        }

        if (cause == "队伍检测") {
            switch (sw) {
                case 1:
                    return 2;
                default:
                    return -1;
            }
        }

        if (cause == "开始任务") {
            switch (sw) {
                default:
                    return 3;
            }
        }


    }

    return 307;


}

int DailyCopiesTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面队伍").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;
}
