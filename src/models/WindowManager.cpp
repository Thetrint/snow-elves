//
// Created by y1726 on 2024/6/26.
//
#ifdef _WIN32
#include "models/WindowManager.h"

#include <fstream>

#include <iostream>
#include <windows.h>

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
    Sleep(3000);  // 单位是毫秒
    POINT pt;
    if (GetCursorPos(&pt))
    {
        wchar_t  title[256];
        HWND childHandle = WindowFromPoint(pt);
        HWND hwnd = GetAncestor(childHandle, GA_ROOT);
        GetWindowText(hwnd, title, sizeof(title));

        if(std::wstring(title) == L"一梦江湖") {
            return hwnd;
        }

        return nullptr;

    }
    else
    {
        std::cerr << "GetCursorPos failed." << std::endl;
        return nullptr;
    }
}

HBITMAP WindowManager::CaptureAnImage(HWND hWnd)
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

    // 获取屏幕和窗口的设备上下文句柄
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    // 创建与窗口DC兼容的内存DC
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    if (!hdcMemDC)
    {
        MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
        goto done;
    }

    // 获取客户区的大小
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    // 设置最佳拉伸模式
    SetStretchBltMode(hdcWindow, HALFTONE);

    // 进行位块传送（BitBlt），将窗口DC的内容传送到内存DC
    if (!BitBlt(hdcMemDC,
        0, 0,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
        hdcWindow,
        0, 0,
        SRCCOPY))
    {
        MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
        goto done;
    }

    // 创建与窗口DC兼容的位图
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

    if (!hbmScreen)
    {
        MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
        goto done;
    }

    // 选择兼容的位图到内存DC
    SelectObject(hdcMemDC, hbmScreen);

    // 再次进行位块传送（BitBlt），将窗口DC的内容传送到内存DC
    if (!BitBlt(hdcMemDC,
        0, 0,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
        hdcWindow,
        0, 0,
        SRCCOPY))
    {
        MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
        goto done;
    }

done:
    // 清理
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);

    return hbmScreen;
}

void WindowManager::SaveBitmapToFile(HBITMAP hBitmap, const wchar_t* filePath)
{
    BITMAP bmpScreen;
    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeofDIB = 0;
    HANDLE hFile = NULL;
    char* lpbitmap = NULL;
    HANDLE hDIB = NULL;
    DWORD dwBmpSize = 0;

    // 获取位图对象
    GetObject(hBitmap, sizeof(BITMAP), &bmpScreen);

    // 填充BITMAPINFOHEADER结构
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // 计算位图大小
    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // 分配内存
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char*)GlobalLock(hDIB);

    // 获取位图数据
    GetDIBits(GetDC(NULL), hBitmap, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // 创建文件
    hFile = CreateFile(filePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    // 计算文件大小
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // 填充BITMAPFILEHEADER结构
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42; // BM

    // 写入文件
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    // 释放内存
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    // 关闭文件句柄
    CloseHandle(hFile);
}

int WindowManager::CaptureAndSaveImage(HWND hWnd, const wchar_t* filePath)
{
    HBITMAP hBitmap = CaptureAnImage(hWnd);
    if (hBitmap)
    {
        SaveBitmapToFile(hBitmap, filePath);
        DeleteObject(hBitmap);
        return 0;
    }
    return -1;
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
    LPARAM lParam = MAKELPARAM(pt.x, pt.y);
    // 模拟鼠标按下
    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, lParam);
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



#endif


