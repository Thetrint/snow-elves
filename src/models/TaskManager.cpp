//
// Created by y1726 on 2024/6/26.
//


#include "models//TaskManager.h"

#include <codecvt>
#include "models/WindowManager.h"
#include "models/TaskSchedul.h"
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



    // WindowManager::CaptureAndSaveImage(hwnd, L"1.bmp");

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

    }



    // // 使用两个迭代器初始化 std::vector<unsigned char>
    // std::vector<unsigned char> buffer(SnowElves_1, SnowElves_1 + SnowElves_1_size);
    //
    // // 调试信息，打印数据的前几个字节
    // std::cout << "Data: ";
    // for (size_t i = 0; i < std::min(SnowElves_1_size, static_cast<size_t>(10)); ++i) {
    //     std::cout << std::hex << static_cast<int>(SnowElves_1[i]) << " ";
    // }
    // std::cout << std::dec << std::endl;
    //
    // // 尝试解码图像
    // cv::Mat image = cv::imdecode(cv::Mat(buffer), cv::IMREAD_COLOR);
    //
    // cv::imshow("Source Image", image);
    // cv::waitKey(0);
    // std::vector<ImageProcessor::Match> matches = ImageMatch(FunctionType::TM_SQDIFF_NORMED);



    // WindowManager::MouseUp(hwnd, 1, 1);
    // const auto* filePath = L"1.bmp";

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



