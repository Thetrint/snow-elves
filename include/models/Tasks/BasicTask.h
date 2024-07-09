//
// Created by y1726 on 2024/6/30.
//

#ifndef BASICTASK_H
#define BASICTASK_H
#include "models/ImageProcess.h"
#include <Windows.h>
#include <semaphore>
#include <httprequestid.h>


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


    void ImageMatch(const std::wstring& templ_name, std::vector<ImageProcessor::Match>& matches, ImageProcessor::MatchParams& match) const;

    void mouse_down_up(const cv::Point &location) const;

    void key_down_up(const std::string &key) const;


    template<class ... Args>
    void ClickImageMatch(ImageProcessor::MatchParams match, std::vector<ImageProcessor::Match>& matches, Args... templ_names);

    template<class ... Args>
    bool CoortImageMatch(ImageProcessor::MatchParams match, std::vector<ImageProcessor::Match>& matches, Args... templ_names);


};

template<typename... Args>
void BasicTask::ClickImageMatch(ImageProcessor::MatchParams match, std::vector<ImageProcessor::Match>& matches, Args... templ_names) {

    (ImageMatch(templ_names, matches, match), ...);


    for (const auto&[location, score] : matches) {
        std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;
        mouse_down_up(location);
    }

}

template<typename... Args>
bool BasicTask::CoortImageMatch(ImageProcessor::MatchParams match, std::vector<ImageProcessor::Match>& matches, Args... templ_names) {

    (ImageMatch(templ_names, matches, match), ...);

    for (const auto&[location, score] : matches) {
        std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;
    }

    return true;
}

#endif //BASICTASK_H
