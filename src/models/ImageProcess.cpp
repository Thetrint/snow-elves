//
// Created by y1726 on 2024/6/28.
//
#include "models/ImageProcess.h"
using namespace std;

/**
 * 读取指定路径图片
 * @param filename 模板图片名称
 * @param id 窗口id
 * @param ifs 文件流
 * @return
 */
cv::Mat ImageProcessor::imread(const std::string &filename, const int &id, std::ifstream& ifs) {
    if (!Images.contains(filename)) {
        std::cerr << "Image not found: " << filename << std::endl;
        return {};
    }

    const unsigned char* img_data = Images[filename].data;
    const size_t img_size = Images[filename].size;
    // 检查线程局部的文件流对象是否已经打开
    if (!ifs.is_open()) {
        ifs.open("resources/images_" + std::to_string(id) + ".dat", std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open images.dat file" << std::endl;
            return {};
        }
    }
    // 定位到图像数据的偏移量位置
    ifs.seekg(reinterpret_cast<std::streamoff>(img_data));

    // 读取图像数据到缓冲区
    std::vector<unsigned char> buffer(img_size);
    ifs.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(img_size));


    cv::Mat image = cv::imdecode(buffer, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Failed to decode image: " << filename << std::endl;
    }

    return image;
}


cv::Mat ImageProcessor::HBITMAPToMat(const HBITMAP &hBitmap) {
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // 确保位图信息正确设置
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bitmap.bmWidth;
    bi.biHeight = -bitmap.bmHeight; // 设置为负值，确保从顶部到底部排列
    bi.biPlanes = 1;
    bi.biBitCount = bitmap.bmBitsPixel; // 假设为 32 位图像，如果不是，请根据实际情况修改
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    // bih.biSize = sizeof(BITMAPINFOHEADER);
    // bih.biWidth = bitmap.bmWidth;
    // bih.biHeight = -bitmap.bmHeight; // 设置为负值，确保从顶部到底部排列
    // bih.biPlanes = 1;
    // bih.biBitCount = bitmap.bmBitsPixel; // 假设为 32 位图像，如果不是，请根据实际情况修改
    // bih.biCompression = BI_RGB;
    // bih.biSizeImage = 0;
    // bih.biXPelsPerMeter = 0;
    // bih.biYPelsPerMeter = 0;
    // bih.biClrUsed = 0;
    // bih.biClrImportant = 0;

    // 分配内存保存图像数据

    std::vector<uchar> buffer(bitmap.bmWidth * bitmap.bmHeight * 4); // 4 bytes per pixel (assuming 32-bit)

    // 获取设备上下文和图像数据
    HDC hdc = GetDC(nullptr);
    int result = GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, buffer.data(), (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    ReleaseDC(nullptr, hdc);


    // 创建 OpenCV 的 Mat 对象
    cv::Mat mat(bitmap.bmHeight, bitmap.bmWidth, CV_8UC4, buffer.data());


    cv::Mat matRGB;
    cv::cvtColor(mat, matRGB, cv::COLOR_BGRA2BGR); // Convert from BGRA to BGR

    // const cv::Rect roi(0, 0, 1334, 750);
    // cv::Mat persionImage = matRGB(roi);
    return matRGB.clone();   // 返回克隆的图像，确保 Mat 对象拥有独立的数据


}

vector<Match> ImageProcessor::matchTemplate_TM_SQDIFF_NORMED(const cv::Mat& image, const cv::Mat &templ, MatchParams& match) {
    // 创建结果矩阵，用于存储模板匹配的结果
    cv::Mat result;
    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    cv::matchTemplate(image, templ, result, cv::TM_SQDIFF_NORMED);

    // 归一化结果
    // normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // 设置阈值，找到所有匹配得分小于等于阈值的位置
    vector<Match> matches;  // 存储所有满足条件的匹配位置和得分

    // 遍历结果矩阵，找到所有满足条件的匹配位置和得分
    for (int y = 0; y < result_rows; ++y) {
        for (int x = 0; x < result_cols; ++x) {
            // ReSharper disable once CppLocalVariableMayBeConst
            float score = result.at<float>(y, x);
            // std::cout << x << "," << y << std::endl;
            // ReSharper disable once CppLocalVariableMayBeConst
            if (float threshold = match.similar; score <= threshold) {
                Match match1;
                match1.location = cv::Point(x, y);
                match1.score = score;
                matches.push_back(match1);
            }
        }
    }


    if (match.Show) {
        // 在大图像上绘制标记
        cv::Mat largeImageCopy;
        image.copyTo(largeImageCopy); // 复制一份大图像用于标记

        for (const auto [location, score] : matches) {
            rectangle(largeImageCopy, location, cv::Point(location.x + templ.cols, location.y + templ.rows), cv::Scalar(255, 255, 255), 2);
        }
        cv::imshow("Source Image", largeImageCopy);
        cv::waitKey(0);
    }




    // 打印匹配结果
    // cout << "Found " << matches.size() << " matches:" << endl;
    // for (size_t i = 0; i < matches.size(); ++i) {
    //     auto [location, score] = matches[i];
    //     cout << "Match " << i + 1 << ": Location (" << location.x << ", " << location.y << "), Score: " << score << endl;
    // }


    //
    // cv::imshow("Source Image", largeImageCopy);
    // cv::waitKey(0);
    //
    // cv::imshow("Source Image", image);
    // cv::waitKey(0);
    //
    // cv::imshow("Source Image", templ);
    // cv::waitKey(0);
    return matches;
}

vector<Match> ImageProcessor::matchTemplate_TM_CCORR_NORMED(const cv::Mat& image, const cv::Mat &templ, MatchParams& match) {
    // 创建结果矩阵，用于存储模板匹配的结果
    cv::Mat result;
    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    cv::matchTemplate(image, templ, result, cv::TM_CCORR_NORMED);

    // 归一化结果
    // normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // 设置阈值，找到所有匹配得分小于等于阈值的位置
    vector<Match> matches;  // 存储所有满足条件的匹配位置和得分


    // 遍历结果矩阵，找到所有满足条件的匹配位置和得分
    for (int y = 0; y < result_rows; ++y) {
        for (int x = 0; x < result_cols; ++x) {
            // ReSharper disable once CppLocalVariableMayBeConst
            float score = result.at<float>(y, x);
            // std::cout << x << "," << y << std::endl;
            // ReSharper disable once CppLocalVariableMayBeConst
            if (float threshold = match.similar; score >= threshold) {
                Match match1;
                match1.location = cv::Point(x, y);
                match1.score = score;
                matches.push_back(match1);
            }
        }
    }


    if (match.Show) {
        // 在大图像上绘制标记
        cv::Mat largeImageCopy;
        image.copyTo(largeImageCopy); // 复制一份大图像用于标记

        for (const auto [location, score] : matches) {
            rectangle(largeImageCopy, location, cv::Point(location.x + templ.cols, location.y + templ.rows), cv::Scalar(255, 255, 255), 2);
        }
        cv::imshow("Source Image", templ);
        cv::imshow("Source ", largeImageCopy);
        cv::waitKey(0);
    }

    //计算中心坐标


    // // 打印匹配结果
    // cout << "Found " << matches.size() << " matches:" << endl;
    // for (size_t i = 0; i < matches.size(); ++i) {
    //     auto [location, score] = matches[i];
    //     cout << "Match " << i + 1 << ": Location (" << location.x << ", " << location.y << "), Score: " << score << endl;
    // }


    //
    // cv::imshow("Source Image", largeImageCopy);
    // cv::waitKey(0);
    //
    // cv::imshow("Source Image", image);
    // cv::waitKey(0);
    //
    // cv::imshow("Source Image", templ);
    // cv::waitKey(0);
    return matches;
}




vector<Match> ImageProcessor::matchTemplate(const cv::Mat& image, const cv::Mat &templ, MatchParams& match, const FunctionPointer funcPtr) {


    vector<Match> matches = funcPtr(image, templ, match);

    for (auto& [location, score] : matches) {
        // 计算中心点
        const cv::Point center( match.scope.x1 + location.x + templ.cols / 2, match.scope.y1 + location.y + templ.rows / 2);
        // std::cout << "Location: " << location << ", Center: " << center << ", Score: " << score << std::endl;
        location = center;

    }



    return matches;
}

// 自定义的非最大值抑制函数
void ImageProcessor::nonMaxSuppression(std::vector<Match>& matches, const double distanceThreshold) {
    if (matches.empty()) {
        return;
    }

    // 按得分从高到低排序
    ranges::sort(matches, [](const Match& a, const Match& b) {
        return a.score > b.score;
    });

    std::vector<Match> suppressedMatches;

    for (const auto& match : matches) {
        bool keep = true;
        for (const auto&[location, score] : suppressedMatches) {
            if (cv::norm(match.location - location) < distanceThreshold) {
                keep = false;
                break;
            }
        }
        if (keep) {
            suppressedMatches.push_back(match);
        }
    }

    // // 缩放位置
    // for (auto&[location, score] : suppressedMatches) {
    //     location.x = static_cast<int>(std::round(location.x * FACTOR));
    //     location.y = static_cast<int>(std::round(location.y * FACTOR));
    // }

    matches = std::move(suppressedMatches);
}