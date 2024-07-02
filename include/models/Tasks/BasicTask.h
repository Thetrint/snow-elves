//
// Created by y1726 on 2024/6/30.
//

#ifndef BASICTASK_H
#define BASICTASK_H
#include "models/ImageProcess.h"
#include <Windows.h>
#include <semaphore>


class BasicTask {

protected:
    int id; // 任务ID
    HWND hwnd; // 窗口句柄，假设 HWND 是窗口句柄的类型
    std::counting_semaphore<1>& pause_event;
    std::counting_semaphore<1>& unbind_event;

public:
    virtual ~BasicTask() = default;



    // ReSharper disable once CppParameterMayBeConst
    BasicTask(int id, HWND hwnd, std::counting_semaphore<1>& pause_event, std::counting_semaphore<1>& unbind_event) : id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event) {}


    // 纯虚函数，要求子类必须实现
    virtual void implementation() = 0;

    virtual void objective() = 0;

    virtual int determine() = 0;

    virtual int detect() = 0;



protected:
    // 定义一个枚举类型
    enum FunctionType {
        TM_CCORR_NORMED,
        TM_SQDIFF_NORMED,

    };

    void ImageMatch(const std::wstring& templ_name, std::vector<ImageProcessor::Match>& matches, FunctionType funcType) const;

    template<class ... Args>
    void ClickImageMatch(FunctionType funcType, Args... templ_names) const;


};

template<typename... Args>
void BasicTask::ClickImageMatch(FunctionType funcType, Args... templ_names) const {

    std::vector<ImageProcessor::Match> matches;
    (ImageMatch(templ_names, matches, funcType), ...);

    for (const auto&[location, score] : matches) {
        std::cout << "Location: (" << location.x << ", " << location.y << "), Score: " << score << std::endl;
    }

}

#endif //BASICTASK_H
