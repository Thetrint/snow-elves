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
    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志地图当前坐标").empty()) {
        BackInterface();
        key_down_up({}, config.value("地图").toString().toStdString());
    }

    if (!CoortImageMatch(MatchParams{.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志地图当前坐标").empty()) {
        return true;
    }

    return false;

}

/**
 * 打开队伍函数
 * @return
 */
bool BasicTask::OpenTeam() {
    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        BackInterface();
        key_down_up({}, config.value("队伍").toString().toStdString());
    }
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        return true;
    }
    return false;

}

/**
 *  打开设置
 * @return
 */
bool BasicTask::OpenESC() {
    key_down_up({}, "ESC");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面设置").empty()) {
        return true;
    }

    return false;
}

/**
 * 打开背包
 * @return
 */
bool BasicTask::OpenKnapsack() {
    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面物品").empty()) {
        BackInterface();
        key_down_up({}, config.value("背包").toString().toStdString());
    }

    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面物品").empty()) {
        return true;
    }
    return false;
}

/**
 * 打开帮派
 * @return
 */
bool BasicTask::OpenFaction() {
    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面帮派").empty()) {
        BackInterface();
        key_down_up({}, config.value("帮派").toString().toStdString());
    }

    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面帮派").empty()) {
        return true;
    }

    return false;

}

/**
 * 打开好友
 * @return
 */
bool BasicTask::OpenBuddy() {
    key_down_up({}, "H");
    if (!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        return true;
    }

    return false;
}

/**
 * 基础功能 延迟指定倍率
 * @param count 延迟倍率
 */
void BasicTask::Defer(const int& count) const {
    for (int i = 0; i < count && unbind_event; i++) {
        key_down_up({.keyDelay = false}, "");
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }

}

/**
 * 基础功能 延迟指定倍率
 * @param count 延迟倍率
 * @param delay 延迟
 */
void BasicTask::Defer(const int& count, const int& delay) const {
    for (int i = 0; i < count && unbind_event; i++) {
        key_down_up({.keyDelay = false}, "");
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

}

bool BasicTask::CloseReward(const int &count) {
    for (int i = 0; i < count; ++i) {
        if (ClickImageMatch({.similar = 0.55, .matchCount = 2, .scope = {842, 153, 1178, 469}, .applyGaussianBlur = false}, nullptr, "按钮关闭").empty()) {
            break;
        }
    }
    return false;
}

bool BasicTask::Close(const MatchParams& match, const int &count) {
    for (int i = 0; i < count; ++i) {
        if (ClickImageMatch(match, nullptr, "按钮关闭").empty()) {
            break;
        }
    }
    Defer(1);
    return false;
}

/**
* 基础功能 关闭
*/

bool BasicTask::Close(const int &count) {
    for (int i = 0; i < count; ++i) {
        if (ClickImageMatch({.similar = 0.55, .matchCount = 1, .click = FORWARD}, nullptr, "按钮关闭", "按钮关闭1").empty()) {
            break;
        }
    }
    Defer(1);
    return false;
}

bool BasicTask::FlagImageMatchCount(const std::string& fileName, int& count) {
    if(CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, fileName).empty()) {
        if (++count >= 5) {
            return false;
        }
    }
    count = 0;
    return true;

}

bool BasicTask::RepeatVerification(const std::string& fileName, int& count) {
    if(!CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, fileName).empty()) {
        if (++count >= 5) {
            return true;
        }
        return false;
    }
    count = 0;
    return false;
}

/**
* 基础功能 返回主界面
*/

bool BasicTask::BackInterface() {
    Close(1);

    return false;
}

/**
 * 基础功能 离线队友检测
 */
bool BasicTask::OfflineDetection() {
    Log("离线队员检测");
    if (OpenTeam()) {
        if(CoortImageMatch({.similar = 0.65}, nullptr, "标志队伍离线").empty()) {
            return true;
        }
        while (unbind_event) {
            if(CoortImageMatch({.similar = 0.65}, nullptr, "标志队伍离线").empty()) {
                return false;
            }
            for(auto [location, score]: CoortImageMatch({.similar = 0.65}, nullptr, "标志队伍离线")) {
                mouse_down_up({}, {location.x, location.y});
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍请离队伍");
                break;
            }
        }

    }
    return false;
}

/**
* 基础功能 召回队友
*/

bool BasicTask::FollowDetection() {
    Log("队员跟随检测");
    if (OpenTeam()) {
        if(ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍一键召回").empty()) {
            return true;
        }
        Log("等待队伍跟随");
        Defer(21, 1000);
        if(CoortImageMatch({.similar = 0.65}, nullptr, "标志队伍暂离").empty()) {
            return true;
        }
        while (unbind_event) {
            if(CoortImageMatch({.similar = 0.65}, nullptr, "标志队伍暂离").empty()) {
                return false;
            }
            for(auto [location, score]: CoortImageMatch({.similar = 0.65}, nullptr, "标志队伍暂离")) {
                mouse_down_up({}, {location.x, location.y});
                ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍请离队伍");
                break;
            }
        }

    }
    return false;
}

/**
* 基础功能 召回队友
*/

bool BasicTask::FollowDetectionNoWait() {
    Log("召回队员");
    if (OpenTeam()) {
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮队伍一键召回");
        Close(1);

    }

    return false;
}

/**
* 基础功能 切换互联
*/

bool BasicTask::SwitchInterconnection() {
    Log("切换互联分线");
    BackInterface();
    mouse_down_up({}, {1229, 5});
    if(ClickImageMatch({.similar = 0.65, .x = 24}, nullptr, "按钮大世界互联").empty()) {
        mouse_down_up({}, {0, 0});
    }
    Defer(2);
    return false;
}

/**
 * 基础功能 位置检查 重置位置
 */
void BasicTask::LocationDetection() {
    Log("位置检测");
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志地图金陵坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图金陵区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("571");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("484");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往江南
 */
void BasicTask::JNGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志地图江南坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往江南");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图江南区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("1095");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("1117");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往金陵
 */
void BasicTask::JLGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志地图金陵坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往金陵");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图金陵区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("571");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("484");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往风雷岛
 */
void BasicTask::FLDGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.75, .applyGaussianBlur = false}, nullptr, "标志地图风雷岛坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往风雷岛");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图风雷岛区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("970");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("542");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往中原
 */
void BasicTask::ZYGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.65, .applyGaussianBlur = false}, nullptr, "标志地图中原坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往中原");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图中原区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("1080");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("996");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往塞北
 */
void BasicTask::SBGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.75, .applyGaussianBlur = false}, nullptr, "标志地图塞北坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往塞北");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图塞北区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("1277");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("718");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往华山
 */
void BasicTask::HSGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.75, .applyGaussianBlur = false}, nullptr, "标志地图华山坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往华山");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图华山区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("344");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("206");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往少林
 */
void BasicTask::SLGo() {
    //打开地图
    if (OpenMap()) {
        if (!CoortImageMatch({.similar = 0.75, .applyGaussianBlur = false}, nullptr, "标志地图少林坐标").empty()) {
            ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
            return;
        }
        Log("前往少林");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮地图世界区域");
        ClickImageMatch({.similar = 0.65}, nullptr, "按钮地图少林区域");
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text("239");

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text("326");

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();

    }
}

/**
 * 基础功能 前往指定坐标
 * @param x 坐标 x
 * @param y 坐标 y
 */
void BasicTask::LocationGO(const std::string& x, const std::string& y) {
    //打开地图
    Log(std::format("前往坐标 {} {}", x, y));
    if (OpenMap()) {
        ClickImageMatch({.similar = 0.6, .matchCount = 1, .scope = {0, 614, 440, 750}}, nullptr, "按钮地图停止寻路");
        ClickImageMatch({.similar = 0.98, .matchCount = 1, .convertToGray = true, .applyGaussianBlur = false, .applyEdgeDetection = false}, nullptr, "按钮地图坐标展开");
        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图横坐标");
        input_text(x);

        ClickImageMatch({.similar = 0.55, .applyGaussianBlur = false}, nullptr, "按钮地图纵坐标");
        input_text(y);

        ClickImageMatch({.similar = 0.55, .scope = {337, 609, 463, 750}}, nullptr, "按钮地图前往区域");
        ClickImageMatch({.similar = 0.5}, nullptr, "按钮关闭");
        Arrive();
    }
}

void BasicTask::Changeover(const int& count,const int& line) {
    CloseReward(4);
    mouse_down_up({}, {1229, 5});

    for (int i = 1; i <= count; i++) {
        for(auto& [location, score] : CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "标志分线")) {

            switch (line) {
                case 1: {
                    if(!CoortImageMatch({.similar = 0.65, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "1线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 2: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "2线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 3: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "3线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 4: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "4线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 5: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "5线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 6: {
                    if(CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "5线").empty() &&
                        !CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "6线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 7: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "7线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 8: {
                    if(!CoortImageMatch({.similar = 0.75, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "8线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 9: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "9线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                case 10: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "1线").empty() &&
                        !CoortImageMatch({.similar = 0.65, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15},}, nullptr, "0线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                        }
                    break;
                }
                case 14: {
                    if(!CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15}}, nullptr, "1线").empty() &&
                        !CoortImageMatch({.similar = 0.70, .matchCount = 1, .scope = {location.x - 50, location.y - 15, location.x - 10, location.y + 15},}, nullptr, "4线").empty()) {
                        mouse_down_up({}, {location.x, location.y});
                        return ;
                    }
                    break;
                }
                default:
                    return ;
            }

        }
        mouse_move({}, {1055, 560}, {1055, 360});
    }

    mouse_down_up({}, {0, 0});

}
/**
 * 基础功能 双世界喊话
 */
void BasicTask::Shout(const std::string &text) {
    // 返回主界面
    BackInterface();
    mouse_down_up({}, {305, 600});

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

            if (count >= DELAY_MAGNIFICATION) {
                Log("寻路结束");
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
    if (CoortImageMatch(MatchParams{.similar = 0.65}, nullptr, "界面队伍").empty()) {
        OpenTeam();
    }
    ClickImageMatch(MatchParams{.similar = 0.6, .applyGaussianBlur = false}, nullptr, "按钮队伍退出");
    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮确定");
    Close({.similar = 0.5}, 2);

}

void BasicTask::OffCard() {
    Log("脱离卡死");
    key_down_up({}, "space");
    OpenESC();
    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮设置脱离卡死");
    ClickImageMatch(MatchParams{.similar = 0.6}, nullptr, "按钮设置确定");
    Close({.similar = 0.5}, 3);;

}

void BasicTask::PassLevel() const {
    Log("过图中");
    for(int i = 1; i <= DELAY_MAGNIFICATION; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}
void BasicTask::ImageMatch(const std::string& templ_name, std::vector<Match>& matches, MatchParams& match) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        spdlog::info("图片匹配 {}", templ_name);
        std::cout << templ_name << std::endl;
        emit Signals::instance()->View(templ_name);
        //读取模板图片
        cv::Mat templ = ImageProcessor::imread(templ_name, id, ifs);

        HBITMAP hbitmap = WindowManager::CaptureAnImage(hwnd);
        //模板匹配 数据转换类型
        const cv::Mat img = ImageProcessor::HBITMAPToMat(hbitmap);
        DeleteObject(hbitmap);

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

    }
}

void BasicTask::mouse_down_up(const MatchParams &match, const cv::Point& location) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        for (int i = 1; i <= match.clickCount; i++) {
            WindowManager::MouseDownUp(hwnd, location.x + match.x, location.y + match.y);
            std::this_thread::sleep_for(std::chrono::milliseconds(match.clickDelayNum));
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

void BasicTask::key_down_up(const MatchParams &match, const std::string& key) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyDownUp(hwnd, key);

        if(match.keyDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }

    }

}

void BasicTask::key_keep(const MatchParams &match, const std::string& key, const int dealy) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyKeep(hwnd, key, dealy);

        if(match.keyDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }
    }

}

void BasicTask::key_down(const MatchParams &match, const std::string& key) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyDown(hwnd, key);

        if(match.keyDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }
    }

}

void BasicTask::key_up(const MatchParams &match, const std::string& key) const {
    if (unbind_event) {
        std::lock_guard lock(pause_event);
        WindowManager::KeyUp(hwnd, key);

        if(match.keyDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }
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
    if(!fight_) {
        fight_ = true;

        fight = std::jthread([this]() {
            this->Fight();
        });
    }


}

void BasicTask::Fight() {
    while (unbind_event && fight_) {
        // 取出第一个元素
        Skill firstSkill = skills[0];

        // 将第一个元素添加到末尾
        skills.push_back(firstSkill);

        // 删除第一个元素
        skills.erase(skills.begin());


        // 根据不同的 action 执行不同的逻辑
        if (firstSkill.action == "按下") {
            std::cout << "Processing '按下' action for " << firstSkill.skill << std::endl;
            key_down({.keyDelay = false}, skillMap[firstSkill.skill]);
        } else if (firstSkill.action == "点击") {
            std::cout << "Processing '点击' action for " << firstSkill.skill << std::endl;
            key_down_up({.keyDelay = false}, skillMap[firstSkill.skill]);
        } else if (firstSkill.action == "抬起") {
            std::cout << "Processing '抬起' action for " << firstSkill.skill << std::endl;
            key_up({.keyDelay = false}, skillMap[firstSkill.skill]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(firstSkill.time));

    }
}

void BasicTask::FightStop() {
    fight_ = false;
}


void BasicTask::TimerPause() {
    timer.pause();
}

void BasicTask::TimerResume() {
    timer.resume();
}


