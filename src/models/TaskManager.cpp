//
// Created by y1726 on 2024/6/26.
//


#include "models//TaskManager.h"

#include <codecvt>

#include "models/WindowManager.h"
#include "models/ImageProcess.h"
#include "models/TaskSchedul.h"
#include <opencv2/opencv.hpp>
#include "utils/Factory.h"
#include "utils/LoadJsonFile.h"
#include "utils/FunctionLibrary.h"


TaskManager::TaskManager(int id, HWND hwnd)
    : id(id), hwnd(hwnd), pause_event(1), unbind_event(1) {
}


void TaskManager::stop() {
    unbind_event.acquire();
    pause_event.release(); // 解除任何可能的暂停状态
}

void TaskManager::pause() {
    pause_event.acquire();
}

void TaskManager::resume() {
    pause_event.release();
}

void TaskManager::start(){

    LoadJsonFile::instance().LoadFile(id);

    // 创建一个 vector 用于存储解码后的值
    std::vector<std::wstring> tasks;

    // 遍历数组元素并解码 Unicode 转义序列
    for (const auto& value : LoadJsonFile::instance().file_0["执行任务"]) {
        // std::wstring decodedValue = FunctionLibrary::decodeUnicode(value.asString());
        // std::cout << "解码后的值: " << value.asString() << std::endl;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        tasks.push_back(converter.from_bytes(value.asString()));
    }


    TaskSchedul schedul(tasks);
    unbind_event.acquire();
    std::wstring task;
    while ((task = schedul.get_task()).empty() == false) {
        auto obj = Factory::instance().create(task, id, hwnd, pause_event, unbind_event);
        obj->implementation();
        obj.reset();
        unbind_event.release();
    }
    std::cout << "解码后的值: 3"  << std::endl;


    // if (auto it =); it != taskMap.end()) {
    //     BasicTask* task = it->second;
    //     task->implementation(); // 调用基类指针指向对象的 implementation() 函数
    // }

    // std::vector<ImageProcessor::Match> matches = ImageMatch(FunctionType::TM_SQDIFF_NORMED);



    // WindowManager::MouseUp(hwnd, 1, 1);
    // const auto* filePath = L"1.bmp";
    // WindowManager::CaptureAndSaveImage(hwnd, filePath);
    // if (hBitmap != NULL) {
    //     WindowManager::SaveBitmapToFile(hBitmap, L"captured_image.jpg", L"image/jpeg");
    //     DeleteObject(hBitmap); // 释放截取的位图资源
    // }


    // try {
    //     std::cout << "Created instance 2 ID " << id << std::endl;
    // } catch (...) {
    //     std::cerr << "Unknown exception in TaskManager with ID " << i << std::endl;
    // }


}

std::vector<ImageProcessor::Match> TaskManager::ImageMatch(FunctionType funcType) const {

    //读取模板图片
    cv::Mat templ = ImageProcessor::imread(L"活动");

    //截取窗口图片
    HBITMAP img = WindowManager::CaptureAnImage(hwnd);

    //模板匹配 数据转换类型
    cv::Mat image = ImageProcessor::HBITMAPToMat(img);

    std::vector<ImageProcessor::Match> matches;

    switch (funcType) {
        case TM_CCORR_NORMED:
            matches = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_CCORR_NORMED);
            break;
        case  TM_SQDIFF_NORMED:
            matches = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_SQDIFF_NORMED);
            break;
    }
    //模板匹配
    // std::vector<ImageProcessor::Match> matches = ImageProcessor::matchTemplate(image, templ, ImageProcessor::matchTemplate_TM_CCORR_NORMED);


    return matches;
}



