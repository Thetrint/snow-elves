//
// Created by y1726 on 24-7-17.
//
#include "models/Tasks/BountyMissionsTask.h"

#include <utils/LoadJsonFile.h>

int BountyMissionsTask::implementation() {
    std::vector<Match> matchs;
    objective("位置检测");
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务z
        }

        if (timer.read() >= std::chrono::seconds(7200)) {
            return 0;
        }

        if (LoadJsonFile::instance().jsonFiles[id].value("副本模式").toString() == "带队模式") {
            switch (determine()) {
                case 0:
                    return 0; // 任务正常退出
                case -1:
                    Close(1);
                    break;
                case 1:
                    LocationDetection();
                    objective("队伍检测");
                    break;
                case 2:
                    Log("队伍检测");
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
                    Close(1);
                    objective("开始任务");
                    break;
                case 3:
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        objective("队伍检测");
                    }
                    //离线检测
                    if(CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍空位").size() <= 10 - LoadJsonFile::instance().jsonFiles[id].value("副本人数").toInt()) {
                        if (++record_num[1] < 4) {
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍进入副本");
                            ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
                            record_event[0] = true;
                            record_num[3] = 0;
                            record_num[0] = 0;
                            objective("等待完成");
                        }else {
                            //退出队伍
                            LeaveTeam();
                            record_num[1] = 0;
                            objective("队伍检测");
                        }

                    }else {
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[2]).count() > 30) {
                            Close(2);
                            Shout(LoadJsonFile::instance().jsonFiles[id].value("副本喊话内容").toString().toStdString());
                            record_time[2] = std::chrono::steady_clock::now();
                        }
                    }
                    break;
                case 4:
                    if (record_num[2] == 0) {
                        Log("接取悬赏");
                        OpenKnapsack();
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动悬赏");
                        while (unbind_event) {
                            if (record_num[2] = static_cast<int>(CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮悬赏前往").size()); record_num[2] == 3) {
                                Close(3);
                                break;
                            }
                            if(!CoortImageMatch(MatchParams{.similar = 0.85, .clickDelay = false}, nullptr, "标志悬赏完成").empty()) {
                                if(record_num[2] == 0) {
                                    objective("任务退出");
                                }
                                Close(3);
                                break;
                            }
                            ClickImageMatch(MatchParams{.similar = 0.6, .clickDelay = false}, nullptr, "按钮悬赏刷新");
                            if(!ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 1, .y = 330, .clickDelay = false, .scope = {267 + 231 * record_num[2], 182, 1197, 558}}, nullptr, "标志悬赏江湖纪事").empty()) {
                                ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 15, .matchDelay = false}, nullptr, "按钮悬赏铜钱押金");
                            }

                        }
                    }
                    OpenTeam();
                    break;
                case 5:
                    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
                        if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本退出").empty()) {

                            if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志副本获得").empty()) {
                                mouse_down_up({}, {0, 0});
                            }
                            if (++record_num[0] >= 5) {
                                objective("开始任务");
                            }
                            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
                        }else {

                            if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志副本完成").empty()) {
                                if (++record_num[4] <= 3) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
                                    continue;
                                }
                                record_num[4] = 0;
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本退出");
                                if(!ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定").empty()) {
                                    Log("副本退出");
                                    record_num[2]--;
                                    objective("开始任务");
                                }
                            }
                            if (record_event[0]) {
                                Log("激活副本任务");
                                if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务").empty()) {
                                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮大世界任务栏");
                                }
                                ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮大世界任务");
                                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本任务");
                                record_time[0] = std::chrono::steady_clock::now();
                                record_event[0] = false;
                                record_num[1] = 0;
                                record_event[1] = true;

                            }



                            record_num[0] = 0;
                        }

                    }
                    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - record_time[0]).count() > 300 && record_event[1]) {
                        //脱离卡死
                        OffCard();
                        record_event[1] = false;
                        record_event[0] = true;
                        if(++record_num[3] >= 2) {
                            //离开队伍
                            LeaveTeam();
                            objective("队伍检测");
                        }
                        record_time[0] = std::chrono::steady_clock::now();
                    }
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面交易").empty()) {
                        Close(1);
                    }
                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情").empty()) {
                        Log("跳过剧情");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本跳过剧情");
                    }


                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
                case 6:
                    Close(1);
                    break;
                default:
                    break;;
            }
        }else if (LoadJsonFile::instance().jsonFiles[id].value("副本模式").toString() == "固定队模式") {
            switch (determine()) {
                case 0:
                    return 0; // 任务正常退出
                case -1:
                    Close(1);
                    break;
                case 1:
                    LocationDetection();
                    objective("队伍检测");
                    break;
                case 2:
                    OpenTeam();
                    if (!CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                        objective("任务退出");
                        continue;
                    }
                    Close(2);
                    objective("开始任务");
                    break;
                case 3:
                    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
                        if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本退出").empty()) {
                            if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志副本完成").empty()) {
                                Log("副本完成");
                                if(record_event[0]) {
                                    record_event[0] = false;
                                    record_num[2]--;
                                    continue;
                                }
                            }
                            while (unbind_event && record_num[2] == 0) {
                                if (CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "界面悬赏").empty()) {
                                    Log("接取悬赏");
                                    OpenKnapsack();
                                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品综合入口");
                                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮物品活动");
                                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮活动悬赏");
                                }else {
                                    if (record_num[2] = static_cast<int>(CoortImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮悬赏前往").size()); record_num[2] == 3) {
                                        Close(3);
                                        break;
                                    }
                                    if(!CoortImageMatch(MatchParams{.similar = 0.85, .clickDelay = false}, nullptr, "标志悬赏完成").empty()) {
                                        if(record_num[2] == 0) {
                                            objective("任务退出");
                                        }
                                        Close(3);
                                        break;
                                    }
                                    ClickImageMatch(MatchParams{.similar = 0.6, .clickDelay = false}, nullptr, "按钮悬赏刷新");
                                    if(!ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 1, .y = 330, .clickDelay = false, .scope = {267 + 231 * record_num[2], 182, 1197, 558}}, nullptr, "标志悬赏江湖纪事").empty()) {
                                        ClickImageMatch(MatchParams{.similar = 0.6, .matchCount = 15, .matchDelay = false}, nullptr, "按钮悬赏铜钱押金");
                                    }
                                }
                            }
                        }
                    }

                    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面日常").empty()) {
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮副本确认");
                        record_event[0] = true;
                    }

                    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志副本继续").empty()) {
                        mouse_down_up({}, {0, 0});
                        OpenTeam();
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍取消跟随");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮队伍跟随队长");
                        ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                        Close(2);
                    }

                    if (!CoortImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志副本获得").empty()) {
                        mouse_down_up({}, {0, 0});
                    }

                    break;
                default:
                    break;;
            }
        }
    }

    return 0;


}

void BountyMissionsTask::objective(const std::string ve) {
    cause = ve;
}

int BountyMissionsTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 15) {
            return -1;
        }
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

int BountyMissionsTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面队伍").empty()) {
        return 2;
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;
}


