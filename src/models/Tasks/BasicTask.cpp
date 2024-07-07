//
// Created by y1726 on 2024/6/30.
//
#include "models//Tasks/BasicTask.h"
#include "models/WindowManager.h"

void BasicTask::ImageMatch(const std::wstring& templ_name, std::vector<ImageProcessor::Match>& matches, FunctionType funcType) const {

    //读取模板图片
    cv::Mat templ = ImageProcessor::imread(templ_name);

    //截取窗口图片
    HBITMAP img = WindowManager::CaptureAnImage(hwnd);

    //模板匹配 数据转换类型
    cv::Mat image = ImageProcessor::HBITMAPToMat(img);

    std::vector<ImageProcessor::Match> matche;

    switch (funcType) {
        case TM_CCORR_NORMED:
            matche = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_CCORR_NORMED);
            break;
        case  TM_SQDIFF_NORMED:
            matche = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_SQDIFF_NORMED);
            break;
    }
    //模板匹配
    // std::vector<ImageProcessor::Match> matches = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_CCORR_NORMED);


    matches.insert(matches.end(), matche.begin(), matche.end());
}



