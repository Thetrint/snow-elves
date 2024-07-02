//
// Created by y1726 on 2024/6/28.
//
#include "models/ImageProcess.h"
#include <unordered_map>
#include "resources/images.h"
using namespace std;



/**
 * 读取指定路径图片
 * @param templ_name 模板图片名称
 * @return
 */
cv::Mat ImageProcessor::imread(const std::wstring& templ_name) {

    std::vector<unsigned char> buffer(Images[templ_name], Images[templ_name] + Images_size[templ_name + L"_size"]);
    cv::Mat image = cv::imdecode(buffer, cv::IMREAD_COLOR);

    // std::string name = getPinyinString(filename);
    //
    // string filepath = "D:/Desktop/ChronoSnowDemo/cmake-build-release/" + name + ".bmp";
    //
    //
    // // 使用 cv::imread 读取图片
    // cv::Mat image = cv::imread(":/resources/images/image1.png", cv::IMREAD_COLOR);


    return image;
}




cv::Mat ImageProcessor::HBITMAPToMat(HBITMAP hBitmap) {
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // 确保位图信息正确设置
    BITMAPINFOHEADER bih;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = bitmap.bmWidth;
    bih.biHeight = -bitmap.bmHeight; // 设置为负值，确保从顶部到底部排列
    bih.biPlanes = 1;
    bih.biBitCount = bitmap.bmBitsPixel; // 假设为 32 位图像，如果不是，请根据实际情况修改
    bih.biCompression = BI_RGB;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    // 分配内存保存图像数据
    std::vector<uchar> buffer(bitmap.bmWidth * bitmap.bmHeight * 4); // 4 bytes per pixel (assuming 32-bit)

    // 获取设备上下文和图像数据
    HDC hdc = GetDC(NULL);
    int result = GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, buffer.data(), (BITMAPINFO *)&bih, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdc);

    if (result == 0) {
        // 处理错误的方式，比如输出错误信息
        DWORD err = GetLastError();
        // 可以输出错误代码以便调试
    }

    // 创建 OpenCV 的 Mat 对象
    cv::Mat mat(bitmap.bmHeight, bitmap.bmWidth, CV_8UC4, buffer.data());


    cv::Mat matRGB;
    cv::cvtColor(mat, matRGB, cv::COLOR_BGRA2BGR); // Convert from BGRA to BGR
    return matRGB.clone();   // 返回克隆的图像，确保 Mat 对象拥有独立的数据

}

vector<ImageProcessor::Match> ImageProcessor::matchTemplate_TM_SQDIFF_NORMED(const cv::Mat& image, const cv::Mat &templ) {
    // 创建结果矩阵，用于存储模板匹配的结果
    cv::Mat result;
    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    cv::matchTemplate(image, templ, result, cv::TM_SQDIFF_NORMED);

    // 归一化结果
    normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // 设置阈值，找到所有匹配得分小于等于阈值的位置
    vector<Match> matches;  // 存储所有满足条件的匹配位置和得分

    // 遍历结果矩阵，找到所有满足条件的匹配位置和得分
    for (int y = 0; y < result_rows; ++y) {
        for (int x = 0; x < result_cols; ++x) {
            float score = result.at<float>(y, x);
            if (constexpr double threshold = 0.1; score <= threshold) {
                Match match;
                match.location = cv::Point(x, y);
                match.score = score;
                matches.push_back(match);
            }
        }
    }

    // 打印匹配结果
    cout << "Found " << matches.size() << " matches:" << endl;
    for (size_t i = 0; i < matches.size(); ++i) {
        auto [location, score] = matches[i];
        cout << "Match " << i + 1 << ": Location (" << location.x << ", " << location.y << "), Score: " << score << endl;
    }

    // // 在大图像上绘制标记
    cv::Mat largeImageCopy;
    image.copyTo(largeImageCopy); // 复制一份大图像用于标记

    for (const auto [location, score] : matches) {
        rectangle(largeImageCopy, location, cv::Point(location.x + templ.cols, location.y + templ.rows), cv::Scalar(0, 255, 255), 2);
    }

    cv::imshow("Source Image", largeImageCopy);
    cv::waitKey(0);

    cv::imshow("Source Image", image);
    cv::waitKey(0);

    cv::imshow("Source Image", templ);
    cv::waitKey(0);
    return matches;
}

vector<ImageProcessor::Match> ImageProcessor::matchTemplate_TM_CCORR_NORMED(const cv::Mat& image, const cv::Mat &templ) {
    // 创建结果矩阵，用于存储模板匹配的结果
    cv::Mat result;
    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    cv::matchTemplate(image, templ, result, cv::TM_CCORR_NORMED);

    // 归一化结果
    normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // 设置阈值，找到所有匹配得分小于等于阈值的位置
    vector<Match> matches;  // 存储所有满足条件的匹配位置和得分

    // 遍历结果矩阵，找到所有满足条件的匹配位置和得分
    for (int y = 0; y < result_rows; ++y) {
        for (int x = 0; x < result_cols; ++x) {
            float score = result.at<float>(y, x);
            if (constexpr double threshold = 0.7; score >= threshold) {
                Match match;
                match.location = cv::Point(x, y);
                match.score = score;
                matches.push_back(match);
            }
        }
    }

    // 打印匹配结果
    cout << "Found " << matches.size() << " matches:" << endl;
    for (size_t i = 0; i < matches.size(); ++i) {
        auto [location, score] = matches[i];
        cout << "Match " << i + 1 << ": Location (" << location.x << ", " << location.y << "), Score: " << score << endl;
    }

    // // 在大图像上绘制标记
    cv::Mat largeImageCopy;
    image.copyTo(largeImageCopy); // 复制一份大图像用于标记

    for (const auto [location, score] : matches) {
        rectangle(largeImageCopy, location, cv::Point(location.x + templ.cols, location.y + templ.rows), cv::Scalar(0, 255, 255), 2);
    }

    cv::imshow("Source Image", largeImageCopy);
    cv::waitKey(0);

    cv::imshow("Source Image", image);
    cv::waitKey(0);

    cv::imshow("Source Image", templ);
    cv::waitKey(0);
    return matches;
}




vector<ImageProcessor::Match> ImageProcessor::matchTemplate(const cv::Mat& image, const cv::Mat &templ, FunctionPointer funcPtr) {


    vector<Match> matches = funcPtr(image, templ);

    return matches;
}