//
// Created by y1726 on 2024/6/30.
//

#ifndef BASICTASK_H
#define BASICTASK_H

#include <utils/LoadJsonFile.h>
#include "main.h"
#include "models/ImageProcess.h"
#include "utils/Utilities.h"



class BasicTask {

protected:
    int id; // 任务ID
    HWND hwnd; // 窗口句柄，假设 HWND 是窗口句柄的类型
    std::mutex& pause_event;
    bool& unbind_event;
    bool& disrupted;
    std::ifstream& ifs;
    QJsonObject config = LoadJsonFile::instance().jsonFiles[id];

public:
    virtual ~BasicTask() = default;

    // ReSharper disable once CppParameterMayBeConst
    BasicTask(int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : id(id),
        hwnd(hwnd),  pause_event(pause_event), unbind_event(unbind_event), disrupted(disrupted), ifs(ifs), detect_count(0),
        fight_(false) {
        skillMap[1] = "1";
        skillMap[2] = "2";
        skillMap[3] = "E";
        skillMap[4] = "F";
        skillMap[5] = "Z";
        skillMap[6] = "X";
        skillMap[7] = "C";
        skillMap[8] = "V";
    }

    struct CAUSE {
        std::string& cause;
        std::string ve;
    };

    struct Skill {
        int id;
        int interval;
    };

    // 纯虚函数，要求子类必须实现
    virtual int implementation() = 0;

    virtual void objective(std::string ve) = 0;

    virtual int determine() = 0;

    virtual int detect() = 0;



protected:
    // 定义一个枚举类型
    Timer timer{unbind_event};

    std::map<int, std::string> skillMap;

    int detect_count;

    bool fight_;

    bool OpenMap();

    bool OpenTeam();

    bool OpenESC();

    bool OpenKnapsack();

    bool OpenFaction();

    bool OpenBuddy();

    void Defer(const int &count) const;

    void Defer(const int &count, const int &delay) const;

    bool CloseReward(const int &count);

    bool Close(const MatchParams& match, const int &count);

    void LocationDetection();

    void Shout(const std::string &text);

    void Arrive();

    void LeaveTeam();

    void OffCard();

    void PassLevel() const;

    void ImageMatch(const std::string &templ_name, std::vector<Match> &matches, MatchParams &match) const;

    void mouse_down_up(const MatchParams &match, const cv::Point &location) const;

    void mouse_wheel(const MatchParams &match, const cv::Point &location, int delta) const;

    void mouse_keep(const MatchParams &match, const cv::Point &location, int delay) const;

    void mouse_move(const MatchParams &match, const cv::Point &start, const cv::Point &end) const;

    void key_down_up(const std::string &key) const;

    void key_keep(const std::string &key, int dealy) const;

    void input_text(const std::string &text) const;

    void Log(const std::string &message) const;

    void AutoFight();

    void Fight(const std::vector<Skill> &skills);

    void FightStop();

    // 定义回调函数类型，包含一个字符串参数
    typedef std::function<void()> CallbackFunc;

    template <typename... Args>
    std::vector<Match> ClickImageMatch(MatchParams match, std::unique_ptr<CAUSE> cause, Args... templ_names);

    template <typename... Args>
    std::vector<Match> CoortImageMatch(MatchParams match, std::unique_ptr<CAUSE> cause, Args... templ_names);
private:
    std::jthread fight;

};


template <typename... Args>
std::vector<Match> BasicTask::ClickImageMatch(MatchParams match, std::unique_ptr<CAUSE> cause, Args... templ_names) {
    std::vector<Match> matches;
    

    //定义循环控制匹配失败最大匹配次数
    for(int i = 1; i <= match.matchCount && unbind_event; i++) {

        (ImageMatch(templ_names, matches, match), ...);
        ImageProcessor::nonMaxSuppression(matches, 22 * FACTOR);
        for (const auto&[location, score] : matches) {
            std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;

        }

        // 初始化随机数种子
        std::uniform_int_distribution<std::vector<Match>::size_type> dis(0, matches.size() - 1);

        if (!matches.empty()) {
            std::random_device rd;
            //不为空直接返回
            switch (match.click) {
                case NoTap:
                    break;
                case RANDOM:
                    mouse_down_up(match, matches[dis(rd)].location);
                    break;
                case FIRST:
                    // 取出第一个元素
                    mouse_down_up(match,  matches.front().location);
                    break;
                case LAST:
                    mouse_down_up(match, matches.back().location);
                    break;
                case FORWARD:
                    for (const auto& [location, score] : matches) {
                        mouse_down_up(match, location);
                    }
                    break;
                case BACKWARD:
                    for (auto &[location, score] : std::ranges::reverse_view(matches)) {
                        mouse_down_up(match, location);
                    }
                    break;
                default:
                    break;;
            }

            return matches;
        }


        if (match.matchDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }

    }

    if (cause) {
        cause->cause = cause->ve;
    }
    return matches;


}

template <typename... Args>
std::vector<Match> BasicTask::CoortImageMatch(MatchParams match, std::unique_ptr<CAUSE> cause, Args... templ_names) {
    std::vector<Match> matches;

    if(unbind_event) {

        (ImageMatch(templ_names, matches, match), ...);


        ImageProcessor::nonMaxSuppression(matches, 22 * FACTOR);

        for (const auto&[location, score] : matches) {
            std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;

        }


    }

    if (cause) {
        cause->cause = cause->ve;
    }

    return matches;
}

#endif //BASICTASK_H

