//
// Created by y1726 on 2024/6/30.
//

#ifndef BASICTASK_H
#define BASICTASK_H
#include "main.h"
#include "models/ImageProcess.h"




class BasicTask {

protected:
    int id; // 任务ID
    HWND hwnd; // 窗口句柄，假设 HWND 是窗口句柄的类型
    std::mutex& pause_event;
    bool& unbind_event;

public:
    virtual ~BasicTask() = default;

    // ReSharper disable once CppParameterMayBeConst
    BasicTask(int id, HWND hwnd, std::mutex& pause_event, bool& unbind_event) : id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}


    // 纯虚函数，要求子类必须实现
    virtual int implementation() = 0;

    virtual void objective(std::string ve) = 0;

    virtual int determine() = 0;

    virtual int detect() = 0;



protected:
    // 定义一个枚举类型


    bool OpenMap();

    bool OpenTeam();

    void LocationDetection();

    void Arrive();

    void ImageMatch(const std::string &templ_name, std::vector<Match> &matches, MatchParams &match) const;

    void mouse_down_up(const cv::Point &location) const;

    void key_down_up(const std::string &key) const;

    void input_text(const std::string &text) const;


    template<class ... Args>
    std::vector<Match> ClickImageMatch(MatchParams match, Args... templ_names);

    template<class ... Args>
    std::vector<Match> CoortImageMatch(MatchParams match, Args... templ_names);


};

template<typename... Args>
std::vector<Match> BasicTask::ClickImageMatch(MatchParams match, Args... templ_names) {
    std::vector<Match> matches;
    //定义循环控制匹配失败最大匹配次数
    for(int i = 1; i <= match.matchCount && unbind_event; i++) {
        ((std::cout << std::forward<Args>(templ_names) << std::endl), ...);
        (ImageMatch(templ_names, matches, match), ...);

        for (const auto&[location, score] : matches) {
            std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;

        }

        if (!matches.empty()) {
            //不为空直接返回
            switch (match.click) {
                case RANDOM: {
                    // 初始化随机数种子
                    std::srand(static_cast<unsigned int>(std::time(nullptr)));

                    // 随机取出一个元素
                    const auto& [location, score] = matches[std::rand() % matches.size()];
                    mouse_down_up(location);
                    break;
                }
                case FORWARD:
                    for (const auto& [location, score] : matches) {
                        mouse_down_up(location);
                    }
                break;
                case BACKWARD:
                    for (auto it = matches.rbegin(); it != matches.rend(); ++it) {
                        mouse_down_up(it->location);
                    }
                break;
            }

            return matches;
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));

    }

    return matches;


}

template<typename... Args>
std::vector<Match> BasicTask::CoortImageMatch(MatchParams match, Args... templ_names) {
    std::vector<Match> matches;

    if(unbind_event) {
        ((std::cout << std::forward<Args>(templ_names) << std::endl), ...);
        (ImageMatch(templ_names, matches, match), ...);

        //如果不为空打印一下匹配信息
        if (!matches.empty()) {
            for (const auto&[location, score] : matches) {
                std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;
            }
        }

    }


    return matches;
}

#endif //BASICTASK_H

