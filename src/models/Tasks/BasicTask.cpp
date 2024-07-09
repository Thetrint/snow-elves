//
// Created by y1726 on 2024/6/30.
//
#include "models//Tasks/BasicTask.h"
#include "models/WindowManager.h"

void BasicTask::ImageMatch(const std::wstring& templ_name, std::vector<ImageProcessor::Match>& matches, ImageProcessor::MatchParams& match) const {

    //读取模板图片
    cv::Mat templ = ImageProcessor::imread(templ_name);

    //截取窗口图片
    HBITMAP img = WindowManager::CaptureAnImage(hwnd);

    //模板匹配 数据转换类型
    cv::Mat image = ImageProcessor::HBITMAPToMat(img);

    //灰度处理
    if (match.convertToGray) {
        cv::cvtColor(templ, templ, cv::COLOR_BGR2GRAY);
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }

    cv::GaussianBlur(templ, templ, cv::Size(5, 5), 1.5);
    cv::Canny(templ, templ, 100, 200);

    cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
    cv::Canny(image, image, 100, 200);
    // if (match.canny) {
    //     cv::GaussianBlur(templ, templ, cv::Size(5, 5), 1.5);
    //     cv::Canny(templ, templ, 100, 200);
    // }

    std::vector<ImageProcessor::Match> matche;

    // ReSharper disable once CppIncompleteSwitchStatement
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (match.modes) {
        case cv::TM_CCORR_NORMED:
            matche = ImageProcessor::matchTemplate(image, templ, match, ImageProcessor::matchTemplate_TM_CCORR_NORMED);
            break;
        case  cv::TM_SQDIFF_NORMED:
            matche = ImageProcessor::matchTemplate(image, templ, match, ImageProcessor::matchTemplate_TM_SQDIFF_NORMED);
            break;

    }
    //模板匹配
    // std::vector<ImageProcessor::Match> matches = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_CCORR_NORMED);


    matches.insert(matches.end(), matche.begin(), matche.end());
}

void BasicTask::mouse_down_up(const cv::Point& location) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::MouseDownUp(hwnd, location.x, location.y);
    }

}

void BasicTask::key_down_up(const std::string& key) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyDownUp(hwnd, key);
    }

}


