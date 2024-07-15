//
// Created by y1726 on 2024/6/26.
//


#include "models/ImageProcess.h"
#ifdef _WIN32
#include "models/WindowManager.h"
#include <fstream>
#include "main.h"
#include <thread>

#include <iostream>
#include <windows.h>
// #include <gdiplus.h>
// #pragma comment(lib, "gdiplus.lib")
//
// using namespace Gdiplus;
// #pragma comment(lib, "gdi32.lib")


std::map<std::string, int> WindowManager::VkCode = {
    {"back", 0x08},
    {"Tab", 0x09},
    {"return", 0x0D},
    {"shift", 0x10},
    {"control", 0x11},
    {"menu", 0x12},
    {"pause", 0x13},
    {"capital", 0x14},
    {"ESC", 0x1B},
    {"space", 0x20},
    {"end", 0x23},
    {"home", 0x24},
    {"left", 0x25},
    {"up", 0x26},
    {"right", 0x27},
    {"down", 0x28},
    {"print", 0x2A},
    {"snapshot", 0x2C},
    {"insert", 0x2D},
    {"delete", 0x2E},
    {"lwin", 0x5B},
    {"rwin", 0x5C},
    {"Num0", 0x60},
    {"Num1", 0x61},
    {"Num2", 0x62},
    {"Num3", 0x63},
    {"Num4", 0x64},
    {"Num5", 0x65},
    {"Num6", 0x66},
    {"Num7", 0x67},
    {"Num8", 0x68},
    {"Num9", 0x69},
    {"multiply", 0x6A},
    {"add", 0x6B},
    {"separator", 0x6C},
    {"subtract", 0x6D},
    {"decimal", 0x6E},
    {"divide", 0x6F},
    {"f1", 0x70},
    {"f2", 0x71},
    {"f3", 0x72},
    {"f4", 0x73},
    {"f5", 0x74},
    {"f6", 0x75},
    {"f7", 0x76},
    {"f8", 0x77},
    {"f9", 0x78},
    {"f10", 0x79},
    {"f11", 0x7A},
    {"f12", 0x7B},
    {"numlock", 0x90},
    {"scroll", 0x91},
    {"lshift", 0xA0},
    {"rshift", 0xA1},
    {"lcontrol", 0xA2},
    {"rcontrol", 0xA3},
    {"lmenu", 0xA4},
    {"rmenu", 0xA5},
    {"Enter", 0x0D}
};

HWND WindowManager::getWindowHandle()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    POINT pt;
    if (GetCursorPos(&pt))
    {
        wchar_t  title[256];
        HWND childHandle = WindowFromPoint(pt);
        HWND hwnd = GetAncestor(childHandle, GA_ROOT);
        GetWindowText(hwnd, title, sizeof(title));

        if(std::wstring(title) == L"一梦江湖") {
            std::cout << hwnd << std::endl;

            return hwnd;
        }

        return nullptr;

    }

    return nullptr;

}

void WindowManager::setWinodw(HWND const &hwnd) {
    RECT rect;
    BITMAP bmp;

    GetWindowRect(hwnd, &rect);

    constexpr int targetWidth = 1335;
    constexpr int targetHeight = 750;

    int width = targetWidth;
    int height = targetHeight;

    int widthStep = std::max(1, width / 2);  // 初始步长设为宽度的一半
    int heightStep = std::max(1, height / 2);  // 初始步长设为高度的一半

    for (int i = 1; i <= 300; i++) {
        try {
            MoveWindow(hwnd, rect.left, rect.top, width, height, TRUE);
        } catch (const std::exception& e) {
            std::cerr << "设置游戏窗口大小: " << e.what() << std::endl;
            continue;  // Skip the rest of the loop if MoveWindow fails
        }

        HBITMAP hBitmap = CaptureAnImage(hwnd);
        if (!hBitmap) {
            std::cerr << "Failed to capture image." << std::endl;
            continue;  // Skip the rest of the loop if CaptureAnImage fails
        }

        const cv::Mat mat = ImageProcessor::HBITMAPToMat(hBitmap);
        if (mat.empty()) {
            std::cerr << "Failed to convert HBITMAP to cv::Mat" << std::endl;
            DeleteObject(hBitmap);
            continue;  // Skip the rest of the loop if HBITMAPToMat fails
        }

        BITMAP bitmap;
        GetObject(hBitmap, sizeof(BITMAP), &bitmap);
        DeleteObject(hBitmap);

        std::cout << "Image Size: " << mat.cols << " x " << mat.rows << std::endl;
        std::cout << "Image: " << bitmap.bmWidth << " x " << bitmap.bmHeight << std::endl;

        if (targetWidth - 1 <= mat.cols && mat.cols <= targetWidth + 1 &&
            targetHeight - 1 <= mat.rows && mat.rows <= targetHeight + 1) {
            // Uncomment these lines if you need to do further processing
            // HBITMAP hBitmap1 = CaptureAnImage(hwnd);
            // const cv::Mat mat1 = ImageProcessor::HBITMAPToMat(hBitmap1);
            // cv::imshow("Source Image", mat1);
            // cv::waitKey(0);
            // cv::imwrite("2.bmp", mat);
            // SaveBitmapToFile(hBitmap1, L"1.bmp");
            // DeleteObject(hBitmap1);
            break;
            }

        // 调整宽度
        if (mat.cols > targetWidth) {
            width -= widthStep;
        } else if (mat.cols < targetWidth) {
            width += widthStep;
        }

        // 调整高度
        if (mat.rows > targetHeight) {
            height -= heightStep;
        } else if (mat.rows < targetHeight) {
            height += heightStep;
        }

        // 减小步长
        widthStep = std::max(1, widthStep / 2);
        heightStep = std::max(1, heightStep / 2);
    }
}

HBITMAP WindowManager::CaptureAnImage(HWND hWnd)
{
    HDC hdcScreen = nullptr;
    HDC hdcWindow = nullptr;
    HDC hdcMemDC = nullptr;
    HBITMAP hbmScreen = nullptr;

    // 获取屏幕和窗口的设备上下文句柄
    hdcScreen = GetDC(nullptr);
    hdcWindow = GetDC(hWnd);

    // 创建与窗口DC兼容的内存DC
    hdcMemDC = CreateCompatibleDC(hdcWindow);
    if (!hdcMemDC)
    {
        MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
    }

    // 获取客户区的大小
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    int width = static_cast<int>((rcClient.right - rcClient.left));
    int height = static_cast<int>((rcClient.bottom - rcClient.top));

    // 创建与窗口DC兼容的位图
    hbmScreen = CreateCompatibleBitmap(hdcWindow, width, height);
    if (!hbmScreen)
    {
        MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
    }

    // 选择兼容的位图到内存DC
    SelectObject(hdcMemDC, hbmScreen);

    // 设置最佳拉伸模式
    SetStretchBltMode(hdcMemDC, HALFTONE);

    // 使用 StretchBlt 将窗口DC的内容缩放到位图中
    // if (!StretchBlt(hdcMemDC, 0, 0, static_cast<int>(width * FACTOR), static_cast<int>(height * FACTOR), hdcWindow, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SRCCOPY))
    // {
    //     MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
    // }
    if (!StretchBlt(hdcMemDC, 0, 0, width, height, hdcWindow, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SRCCOPY))
    {
        MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
    }

    // 清理
    DeleteDC(hdcMemDC);
    ReleaseDC(nullptr, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);

    return hbmScreen;
}


void WindowManager::SaveBitmapToFile(HBITMAP &hBitmap, const wchar_t* filePath)
{
    BITMAP bmpScreen;
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeofDIB = 0;
    HANDLE hFile = nullptr;
    char* lpbitmap = nullptr;
    HANDLE hDIB = nullptr;
    DWORD dwBmpSize = 0;

    // 获取位图对象
    if (!GetObject(hBitmap, sizeof(BITMAP), &bmpScreen))
    {
        // 错误处理
        return;
    }

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight; // 不要缩放
    bi.biPlanes = 1;
    bi.biBitCount = bmpScreen.bmBitsPixel; // 假设为32位图像，如果不是，请根据实际情况修改
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // 计算位图大小
    dwBmpSize = ((bi.biWidth * bi.biBitCount + 31) / 32) * 4 * abs(bi.biHeight);

    // 分配内存
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    if (hDIB == nullptr)
    {
        // 错误处理
        return;
    }

    lpbitmap = static_cast<char *>(GlobalLock(hDIB));
    if (lpbitmap == nullptr)
    {
        // 错误处理
        GlobalFree(hDIB);
        return;
    }

    // 获取位图数据
    HDC hdc = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);

    if (!GetDIBits(hdcMem, hBitmap, 0, bmpScreen.bmHeight, lpbitmap, reinterpret_cast<BITMAPINFO *>(&bi), DIB_RGB_COLORS))
    {
        // 错误处理
        GlobalUnlock(hDIB);
        GlobalFree(hDIB);
        ReleaseDC(nullptr, hdc);
        DeleteDC(hdcMem);
        return;
    }

    // 创建文件
    hFile = CreateFile(filePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    // 检查文件句柄是否有效
    if (hFile == INVALID_HANDLE_VALUE)
    {
        // 错误处理
        GlobalUnlock(hDIB);
        GlobalFree(hDIB);
        ReleaseDC(nullptr, hdc);
        DeleteDC(hdcMem);
        return;
    }

    // 计算文件大小
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // 填充BITMAPFILEHEADER结构
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42; // BM

    // 写入文件
    if (!WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, nullptr) ||
        !WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, nullptr) ||
        !WriteFile(hFile, lpbitmap, dwBmpSize, &dwBytesWritten, nullptr))
    {
        // 错误处理
        GlobalUnlock(hDIB);
        GlobalFree(hDIB);
        CloseHandle(hFile);
        ReleaseDC(nullptr, hdc);
        DeleteDC(hdcMem);
        return;
    }

    // 释放内存
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    CloseHandle(hFile);

    // 释放DC
    ReleaseDC(nullptr, hdc);
    DeleteDC(hdcMem);
}


int WindowManager::CaptureAndSaveImage(HWND hWnd, const std::string &filePath)
{
    if (HBITMAP hBitmap = CaptureAnImage(hWnd))
    {
        cv::Mat mat = ImageProcessor::HBITMAPToMat(hBitmap);
        std::cout << "Image Size: " << mat.cols << " x " << mat.rows << std::endl;

        cv::imshow("Source Image", mat);
        cv::waitKey(0);
        cv::imwrite("2.bmp", mat);
        SaveBitmapToFile(hBitmap, L"1.bmp");
        DeleteObject(hBitmap);
        return 0;
    }
    return -1;
}

void WindowManager::GetFactor() {
    // 获取当前桌面窗口的句柄
    HWND hWnd = GetForegroundWindow();
    if (!hWnd) {
        std::cerr << "Failed to get foreground window." << std::endl;
        return;
    }

    // 获取窗口所在的监视器句柄
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    if (!hMonitor) {
        std::cerr << "Failed to get monitor from window." << std::endl;
        return;
    }


    // 获取监视器信息
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);

    // 获取监视器逻辑尺寸
    int cxLogical = miex.rcMonitor.right - miex.rcMonitor.left;
    int cyLogical = miex.rcMonitor.bottom - miex.rcMonitor.top;

    // 获取监视器物理尺寸
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    if (!EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm)) {
        std::cerr << "Failed to get display settings." << std::endl;
        return;
    }
    DWORD cxPhysical = dm.dmPelsWidth;
    DWORD cyPhysical = dm.dmPelsHeight;


    // 计算缩放比例
    double scalingFactorX = static_cast<double>(cxPhysical) / static_cast<double>(cxLogical);
    double scalingFactorY = static_cast<double>(cyPhysical) / static_cast<double>(cyLogical);

    FACTOR = scalingFactorY;
    // 输出结果
    std::cout << scalingFactorX << std::endl;
    std::cout << scalingFactorY << std::endl;
    // return scalingFactorY;

}


/**
 * MouseDown()-对指定句柄窗口发送鼠标单击指令
 * @param hwnd 窗口句柄
 * @param x 坐标
 * @param y 坐标
 */
void WindowManager::MouseDownUp(HWND hwnd, int x, int y) {
    // 将屏幕坐标转换为窗口客户区坐标
    POINT pt = { x, y };
    // ScreenToClient(hwnd, &pt);

    // 模拟鼠标指针，传送到指定坐标
    LPARAM const lParam = MAKELPARAM(pt.x, pt.y);
    // 模拟鼠标按下
    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, lParam);
}


void WindowManager::MouseMove(HWND hwnd, const int x1, const int y1, const int x2, const int y2) {
    // 将屏幕坐标转换为窗口客户区坐标
    POINT startPt = { x1, y1 };
    POINT endPt = { x2, y2 };

    ScreenToClient(hwnd, &startPt);
    ScreenToClient(hwnd, &endPt);

    // 生成随机控制点
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);
    const POINT controlPt = { (startPt.x + endPt.x) / 2 + dis(gen), (startPt.y + endPt.y) / 2 + dis(gen) };

    // 贝塞尔曲线函数
    auto bezierCurve = [](const double t, const double p0, const double p1, const double p2) {
        return (1 - t) * (1 - t) * p0 + 2 * (1 - t) * t * p1 + t * t * p2;
    };

    // 计算移动距离
    const double distance = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    // 动态生成步长
    const int steps = static_cast<int>(distance / 20);

    // 模拟鼠标按下
    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(startPt.x, startPt.y));

    for (int i = 0; i <= steps; i++) {
        const double t = static_cast<double>(i) / steps;
        const int x = static_cast<int>(bezierCurve(t, startPt.x, controlPt.x, endPt.x));
        const int y = static_cast<int>(bezierCurve(t, startPt.y, controlPt.y, endPt.y));
        const LPARAM lParam = MAKELPARAM(x, y);

        // 模拟鼠标移动
        PostMessage(hwnd, WM_MOUSEMOVE, MK_LBUTTON, lParam);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // 模拟鼠标松开
    PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(endPt.x, endPt.y));
}

int WindowManager::GetVkCode(const std::string &key) {
    if (key.length() == 1) {
        return VkKeyScanA(key[0]) & 0xFF;
    } else {
        const auto it = VkCode.find(key);
        if (it != VkCode.end()) {
            return it->second;
        } else {
            return 0; // 无效键值
        }
    }
}

void WindowManager::KeyDownUp(HWND hwnd, const std::string &key) {
    const int vk_code = GetVkCode(key);
    if (vk_code == 0) {
        std::cerr << "Invalid key: " << key << std::endl;
        return; // 无效键值
    }

    const UINT scan_code = MapVirtualKey(vk_code, MAPVK_VK_TO_VSC);
    const WPARAM wparam = vk_code;
    const LPARAM lparam = (scan_code << 16) | 1;

    PostMessage(hwnd, WM_KEYDOWN, wparam, lparam);
    PostMessageW(hwnd, WM_KEYUP, wparam, lparam);
}


void WindowManager::InputText(HWND hwnd, const std::string& text) {
    for (const char c : text) {
        SendMessage(hwnd, WM_CHAR, static_cast<WPARAM>(c), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 模拟键入延迟
    }
}


#endif


