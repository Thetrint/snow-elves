//
// Created by y1726 on 24-7-26.
//
#include "models/Tasks/ChivalryShoutTask.h"

int ChivalryShoutTask::implementation() {
    std::vector<Match> matchs;
    objective("位置检测");
    timer.start();
    while (unbind_event) {

        if (disrupted) {
            timer.pause();
            return -1; //任务调度中止任务
        }

        if (timer.read() >= std::chrono::seconds(720)) {
            return 0;
        }

        switch (determine()) {
            case 0:
               Close({.similar = 0.5}, 3);;
                return 0; // 任务正常退出
            case -1:
                Close({.similar = 0.5}, 1);
                break;
            case 1:
                LocationDetection();
                objective("队伍检测");
                break;
            case 2:
                OpenTeam();
                if (CoortImageMatch(MatchParams{.similar = 0.75}, nullptr, "按钮队伍创建").empty()) {
                    ClickImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
                    ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮确定");
                }
                Close({.similar = 0.5}, 1);
                objective("开始任务");
                break;
            case 3:
                OpenBuddy();
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮好友联系人");
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志好友输入编号昵称");
                input_text(config.value("侠缘喊话昵称编号").toString().toStdString());
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮好友搜索");
                mouse_down_up({}, {290, 333});
                break;
            case 4:
                text = content.front();
                content.erase(content.begin());
                if (text.empty()) {
                    continue;
                }
                content.push_back(text);
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "标志好友输入文字");
                input_text(text);
                ClickImageMatch(MatchParams{.similar = 0.5}, nullptr, "按钮好友发送");
                Log(std::format("侠缘喊话 {} 次", record_num[0]));
                if(++record_num[0] > config.value("侠缘喊话次数").toInt()) {
                    objective("任务退出");
                }
                break;
            default:
                break;
        }

    }

    return 0;


}

void ChivalryShoutTask::objective(const std::string ve) {
    cause = ve;
}

int ChivalryShoutTask::determine() {
    const int sw = detect();
    if (sw == -5) {
        if (++detect_count >= 10) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }else {
        detect_count = 0;
    }


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
                return 3;
            case 2:
                return 4;
            default:
                return -1;
        }
    }



    return 307;
}

int ChivalryShoutTask::detect() {
    if (!CoortImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "界面好友").empty()) {
        return 2;
    }

    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "界面大世界1", "界面大世界2").empty()) {
        return 1;
    }


    return -5;

}