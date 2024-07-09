//
// Created by y1726 on 2024/6/28.
//

#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include <string>
#include <windows.h>
#include <vector>
#include "resources/images.h"


class ImageProcessor {
public:

    // 结构体用于存储匹配位置和得分
    struct Match {
        cv::Point location;  // 匹配位置
        float score;     // 匹配得分
    };


    struct MatchParams {
        cv::TemplateMatchModes modes = cv::TM_CCORR_NORMED; // 默认值
        bool convertToGray = true; // 默认值
        float similar{}; // 默认值
    };

    // 定义一个函数指针类型
    typedef std::vector<Match> (*FunctionPointer)(const cv::Mat& image, const cv::Mat &templ, ImageProcessor::MatchParams& match);

    //模板匹配
    static std::vector<Match> matchTemplate(const cv::Mat& image, const cv::Mat &templ, ImageProcessor::MatchParams& match, FunctionPointer funcPtr);

    //极大值抑制
    static void nonMaxSuppression(std::vector<Match> &matches, double distanceThreshold);

    //TM_SQDIFF_NORMED模板匹配
    static std::vector<Match> matchTemplate_TM_SQDIFF_NORMED(const cv::Mat& image, const cv::Mat &templ, ImageProcessor::MatchParams& match);

    //TM_CCORR_NORMED模板匹配
    static std::vector<Match> matchTemplate_TM_CCORR_NORMED(const cv::Mat& image, const cv::Mat &templ, ImageProcessor::MatchParams& match);

    //读取图片
    static cv::Mat imread(const std::wstring& filename);

    //图片类型转换
    static cv::Mat HBITMAPToMat(HBITMAP hBitmap);

private:



};



#endif //IMAGEPROCESS_H
