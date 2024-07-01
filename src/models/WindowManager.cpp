//
// Created by y1726 on 2024/6/26.
//
#ifdef _WIN32
#include "models/WindowManager.h"

#include <fstream>

#include <iostream>
#include <windows.h>

#pragma comment(lib, "gdi32.lib")



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



#endif


