//
// Created by y1726 on 2024/6/30.
//
#include "main.h"
#include "models//Tasks/BasicTask.h"
#include <utils/signals.h>
#include "models/WindowManager.h"

/**
 * 打开背包函数
 * @return
 */
bool BasicTask::OpenMap() {
    key_down_up("M");
    if (!CoortImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "标志地图当前坐标").empty()) {

        return true;
    }

    return false;

}

bool BasicTask::OpenTeam() {
    key_down_up("T");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        return true;
    }

    return false;

}

bool BasicTask::OpenKnapsack() {
    key_down_up("B");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        return true;
    }

    return false;

}

bool BasicTask::Close() {
    for (int i = 0; i < 3; ++i) {
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
    }
    return false;
}

/**
 * 基础功能 位置检查 重置位置
 */
void BasicTask::LocationDetection() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.75}, nullptr, "标志地图金陵坐标").empty()) {
            ClickImageMatch({.similar = 0.75}, nullptr, "按钮关闭");
            return;
        }
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图金陵区域");
        ClickImageMatch({.similar = 0.6}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("571");

        ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("484");

        ClickImageMatch({.similar = 0.75}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.75}, nullptr, "按钮关闭");
        Arrive();

    }


}

/**
 * 基础功能 等等寻路结束
 */
void BasicTask::Arrive() {
    int count = 0;
    while (unbind_event) {
        if (CoortImageMatch(MatchParams{.similar = 0.5, .applyGaussianBlur = false}, nullptr, "标志大世界自动寻路中").empty()) {
            count++;

            if (count >= 15) {
                return;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));

        }else {
            count = 0;
        }
    }
}


void BasicTask::ImageMatch(const std::string& templ_name, std::vector<Match>& matches, MatchParams& match) const {
    spdlog::info("图片匹配 {}", templ_name);
    std::cout << templ_name << std::endl;
    //读取模板图片
    cv::Mat templ = ImageProcessor::imread(templ_name);

    HBITMAP hbitmap = WindowManager::CaptureAnImage(hwnd);
    //模板匹配 数据转换类型
    const cv::Mat img = ImageProcessor::HBITMAPToMat(hbitmap);

    const cv::Rect roi(match.scope.x1, match.scope.y1, match.scope.x2 - match.scope.x1, match.scope.y2 - match.scope.y1);
    cv::Mat image = img(roi);

    //灰度处理
    if (match.convertToGray) {
        cv::cvtColor(templ, templ, cv::COLOR_BGR2GRAY);
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }

    //高斯模糊
    if (match.applyGaussianBlur) {
        cv::GaussianBlur(templ, templ, cv::Size(3, 3), 1.2);
        cv::GaussianBlur(image, image, cv::Size(3, 3), 1.2);
    }

    //边缘检测
    if (match.applyEdgeDetection) {
        cv::Canny(templ, templ, match.edgeThreshold.threshold1, match.edgeThreshold.threshold2);
        cv::Canny(image, image, match.edgeThreshold.threshold1, match.edgeThreshold.threshold2);
    }


    std::vector<Match> matche;

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
    DeleteObject(hbitmap);
}

void BasicTask::mouse_down_up(const MatchParams &match, const cv::Point& location) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::MouseDownUp(hwnd, location.x + match.x, location.y + match.y);

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }

}

void BasicTask::mouse_move(const MatchParams &match, const cv::Point &start, const cv::Point &end) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::MouseMove(hwnd, start.x, start.y, end.x, end.y);

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}

void BasicTask::key_down_up(const std::string& key) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyDownUp(hwnd, key);

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }

}

void BasicTask::input_text(const std::string &text) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::InputText(hwnd, text);

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}

void BasicTask::Log(const std::string &message) const {
    emit Signals::instance()->Log(id, message);

}

