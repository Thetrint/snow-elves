//
// Created by y1726 on 2024/6/28.
//

#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "main.h"

// 结构体用于存储匹配位置和得分
struct Match {
    cv::Point location;  // 匹配位置
    float score;     // 匹配得分
    bool operator==(const Match& other) const {
        return location == other.location; // 比较位置
    }
};


enum ClickType {
    NoTap,
    RANDOM,
    FIRST,
    LAST,
    FORWARD,
    BACKWARD
};

struct EdgeThreshold {
    int threshold1; // 边缘检测的第一个阈值
    int threshold2; // 边缘检测的第二个阈值
};

struct MatchScope {
    int x1; // 匹配左上角顶点 x坐标
    int y1; // 匹配左上角顶点 y坐标
    int x2;  // 匹配右下角顶点 x坐标
    int y2;  // 匹配右下角顶点 y坐标
};

struct MatchGauss {
    int width;
    int height;
    float sigmaX;
};

struct MatchParams {
    cv::TemplateMatchModes modes = cv::TM_CCORR_NORMED; // 默认值
    float similar{}; // 相似度
    int matchCount = 4;
    int moveCount = 2;
    ClickType click = RANDOM;
    int x = 0; //偏移坐标x
    int y = 0; //偏移坐标y
    int clickCount = 1;
    bool clickDelay = true;
    bool matchDelay = true;
    MatchScope scope = {0, 0, 1335, 750}; // 匹配范围
    MatchGauss gauss = {3, 3, 1.2}; //高斯模糊范围
    bool convertToGray = true; // 灰度处理开关
    bool applyGaussianBlur = true; // 控制高斯模糊的开关
    bool applyEdgeDetection  = true; // 控制边缘检测
    EdgeThreshold edgeThreshold = {100, 200}; // 编译检测范围
    bool Show = false;
};

class ImageProcessor {
public:



    // 定义一个函数指针类型
    typedef std::vector<Match> (*FunctionPointer)(const cv::Mat& image, const cv::Mat &templ, MatchParams& match);

    //模板匹配
    static std::vector<Match> matchTemplate(const cv::Mat& image, const cv::Mat &templ, MatchParams& match, FunctionPointer funcPtr);

    //极大值抑制
    static void nonMaxSuppression(std::vector<Match> &matches, double distanceThreshold);

    //TM_SQDIFF_NORMED模板匹配
    static std::vector<Match> matchTemplate_TM_SQDIFF_NORMED(const cv::Mat& image, const cv::Mat &templ, MatchParams& match);

    //TM_CCORR_NORMED模板匹配
    static std::vector<Match> matchTemplate_TM_CCORR_NORMED(const cv::Mat& image, const cv::Mat &templ, MatchParams& match);

    //读取图片
    static cv::Mat imread(const std::string &filename, const int& id, std::ifstream& ifs);

    //图片类型转换
    static cv::Mat HBITMAPToMat(const HBITMAP &hBitmap);

    static void removeMatches(std::vector<Match>& matches, const std::vector<Match>& last_matches) {
        std::vector<Match> result;
        for (const auto& loc1 : matches) {
            bool found = false;
            for (const auto&[location, score] : last_matches) {
                if (std::sqrt((loc1.location.x - location.x) * (loc1.location.x - location.x) + (loc1.location.y - location.y) * (loc1.location.y - location.y)) < 22 * FACTOR) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                result.push_back(loc1);
            }
        }
        matches = result;
    }

private:



};



#endif //IMAGEPROCESS_H
