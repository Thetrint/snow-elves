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
        skillMap["技能1"] = config.value("技能1").toString().toStdString();
        skillMap["技能2"] = config.value("技能2").toString().toStdString();
        skillMap["技能3"] = config.value("技能3").toString().toStdString();
        skillMap["技能4"] = config.value("技能4").toString().toStdString();
        skillMap["技能5"] = config.value("技能5").toString().toStdString();
        skillMap["技能6"] = config.value("技能6").toString().toStdString();
        skillMap["技能7"] = config.value("技能7").toString().toStdString();
        skillMap["技能8"] = config.value("技能8").toString().toStdString();
        skillMap["普攻"] = config.value("普攻").toString().toStdString();
        skillMap["绝学"] = config.value("绝学").toString().toStdString();
        skillMap["关山"] = config.value("关山").toString().toStdString();
        skillMap["闪避"] = config.value("闪避").toString().toStdString();
        skillMap["自创1"] = config.value("自创1").toString().toStdString();
        skillMap["自创2"] = config.value("自创2").toString().toStdString();
        skillMap["自创3"] = config.value("自创3").toString().toStdString();
        skillMap["自创4"] = config.value("自创4").toString().toStdString();
        skillMap["前"] = config.value("前").toString().toStdString();
        skillMap["后"] = config.value("后").toString().toStdString();
        skillMap["左"] = config.value("左").toString().toStdString();
        skillMap["右"] = config.value("右").toString().toStdString();
        skillMap["背包"] = config.value("背包").toString().toStdString();
        skillMap["帮派"] = config.value("帮派").toString().toStdString();
        skillMap["队伍"] = config.value("队伍").toString().toStdString();
        skillMap["地图"] = config.value("地图").toString().toStdString();

        std::string text = config.value("自定义连招").toString().toStdString();
        const std::regex pattern("(按下|点击|抬起)#([^#]+)#(\\d+)");

        for (std::sregex_iterator it(text.begin(), text.end(), pattern), end_it; it != end_it; ++it) {
            const std::smatch& match = *it;
            Skill skill;
            skill.action = match.str(1);
            skill.skill = match.str(2);
            skill.time = std::stoi(match.str(3)); // 将字符串转换为整数
            // 将匹配结果存储到 skills 向量中
            skills.push_back(skill);
        }

        if(skills.empty()) {
            text = "点击#技能1#2000 点击#普攻#2000 点击#技能2#2000 点击#技能3#2000 点击#技能4#2000 点击#技能5#2000 点击#技能6#2000 点击#技能7#2000 点击#技能8#2000 点击#绝学#2000";

            for (std::sregex_iterator it(text.begin(), text.end(), pattern), end_it; it != end_it; ++it) {
                const std::smatch& match = *it;
                Skill skill;
                skill.action = match.str(1);
                skill.skill = match.str(2);
                skill.time = std::stoi(match.str(3)); // 将字符串转换为整数
                // 将匹配结果存储到 skills 向量中
                skills.push_back(skill);
            }
        }


    }

    struct CAUSE {
        std::string& cause;
        std::string ve;
    };

    struct Skill {
        std::string action;
        std::string skill;
        int time;
    };

    // 纯虚函数，要求子类必须实现
    virtual int implementation() = 0;

    virtual void objective(std::string ve) = 0;

    virtual int determine() = 0;

    virtual int detect() = 0;



protected:
    // 定义一个枚举类型
    Timer timer{unbind_event};

    std::map<std::string, std::string> skillMap;

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

    bool Close(const int &count);

    bool BackInterface();

    void OfflineDetection();

    bool FollowDetection();

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

    void key_down_up(const MatchParams &match, const std::string &key) const;

    void key_keep(const MatchParams &match, const std::string &key, const int dealy) const;

    void key_down(const MatchParams &match, const std::string &key) const;

    void key_up(const MatchParams &match, const std::string &key) const;

    void input_text(const std::string &text) const;

    void Log(const std::string &message) const;

    void AutoFight();

    void Fight();

    void FightStop();

    // 定义回调函数类型，包含一个字符串参数
    typedef std::function<void()> CallbackFunc;

    template <typename... Args>
    std::vector<Match> ClickImageMatch(MatchParams match, std::unique_ptr<CAUSE> cause, Args... templ_names);

    template <typename... Args>
    std::vector<Match> CoortImageMatch(MatchParams match, std::unique_ptr<CAUSE> cause, Args... templ_names);
private:
    std::jthread fight;
    std::vector<Skill> skills;

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

