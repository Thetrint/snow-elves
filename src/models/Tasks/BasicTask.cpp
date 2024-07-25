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

/**
 * 打开队伍函数
 * @return
 */
bool BasicTask::OpenTeam() {
    key_down_up("T");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        return true;
    }

    return false;

}

bool BasicTask::OpenESC() {
    key_down_up("ESC");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面设置").empty()) {
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

bool BasicTask::OpenFaction() {
    key_down_up("O");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        return true;
    }

    return false;

}

bool BasicTask::Defer(const int& count) {
    for (int i = 0; i <= count; i++) {
        key_down_up("Tab");
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
    return false;
}

bool BasicTask::Close(const MatchParams& match, const int &count) {
    for (int i = 0; i < count; ++i) {
        if (ClickImageMatch(match, nullptr, "按钮关闭").empty()) {
            break;
        }
    }
    return false;
}

/**
 * 基础功能 位置检查 重置位置
 */
void BasicTask::LocationDetection() {
    Log("位置检测");
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.8}, nullptr, "标志地图金陵坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图金陵区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("571");

        ClickImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("484");

        ClickImageMatch({.similar = 0.75}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 双世界喊话
 */
void BasicTask::Shout(const std::string &text) {
    mouse_down_up({}, {15, 600});

    ClickImageMatch({.similar = 0.65, .scope = {0, 0, 140, 695}}, nullptr, "按钮大世界世界");
    ClickImageMatch({.similar = 0.5}, nullptr, "标志大世界输入文字");
    input_text(text);
    ClickImageMatch({.similar = 0.5}, nullptr, "按钮大世界发送");

    ClickImageMatch({.similar = 0.65, .scope = {0, 0, 140, 695}}, nullptr, "按钮大世界互联世界");
    ClickImageMatch({.similar = 0.5}, nullptr, "标志大世界输入文字");
    input_text(text);
    ClickImageMatch({.similar = 0.5}, nullptr, "按钮大世界发送");
    ClickImageMatch({.similar = 0.5}, nullptr, "按钮大世界聊天退出");
}

/**
 * 基础功能 等待寻路结束
 */
void BasicTask::Arrive() {
    Log("等待到达");
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

/**
 * 基础功能 离开队伍
 */
void BasicTask::LeaveTeam() {
    Log("离开队伍");
    OpenTeam();
    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
    Close({.similar = 0.5}, 2);

}

void BasicTask::OffCard() {
    Log("脱离卡死");
    OpenESC();
    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮设置脱离卡死");
    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮设置确定");
   Close({.similar = 0.5}, 3);;

}

void BasicTask::PassLevel() const {
    Log("过图中");
    for(int i = 1; i <=5; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}
void BasicTask::ImageMatch(const std::string& templ_name, std::vector<Match>& matches, MatchParams& match) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        spdlog::info("图片匹配 {}", templ_name);
        std::cout << templ_name << std::endl;
        //读取模板图片
        cv::Mat templ = ImageProcessor::imread(templ_name, id, ifs);

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
            cv::GaussianBlur(templ, templ, cv::Size(match.gauss.width, match.gauss.height),  match.gauss.sigmaX);
            cv::GaussianBlur(image, image, cv::Size(match.gauss.width, match.gauss.height),  match.gauss.sigmaX);
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
}

void BasicTask::mouse_down_up(const MatchParams &match, const cv::Point& location) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        for (int i = 1; i <= match.clickCount; i++) {
            WindowManager::MouseDownUp(hwnd, location.x + match.x, location.y + match.y);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        if (match.clickDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }

    }

}

void BasicTask::mouse_wheel(const MatchParams &match, const cv::Point& location, const int delta) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        for (int i = 0; i < match.matchCount; i++) {
            WindowManager::MouseWHEEL(hwnd, location.x + match.x, location.y + match.y, delta);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}

void BasicTask::mouse_keep(const MatchParams &match, const cv::Point& location, const int delay) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::MouseKeep(hwnd, location.x, location.y, delay);

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }

}
void BasicTask::mouse_move(const MatchParams &match, const cv::Point &start, const cv::Point &end) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);

        for (int i = 1; i <= match.moveCount; i++) {
            WindowManager::MouseMove(hwnd, start.x, start.y, end.x, end.y);
        }


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

void BasicTask::key_keep(const std::string& key, const int dealy) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyKeep(hwnd, key, dealy);

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

void BasicTask::AutoFight() {
    fight_ = true;
    std::string skillSequence = "1-2000; 2-2000; 3-2000; 4-2000; 5-2000; 6-2000; 7-2000; 8-2000";
    std::vector<Skill> skills;
    std::stringstream ss(skillSequence);

    std::string item;

    while (std::getline(ss, item, ';')) {
        std::stringstream itemStream(item);
        std::string idStr, intervalStr;
        std::getline(itemStream, idStr, '-');
        std::getline(itemStream, intervalStr);
        skills.push_back({std::stoi(idStr), std::stoi(intervalStr)});
    }

    fight = std::jthread(&BasicTask::Fight, this, skills);

}

void BasicTask::Fight(const std::vector<Skill> &skills) {
    while (unbind_event && fight_) {
        for (const auto&[id, interval] : skills) {
            if (!fight_ || !unbind_event) break;
            key_down_up(skillMap[id]);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }
}

void BasicTask::FightStop() {
    fight_ = false;
}

