#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


#include <windows.h>

#include <QWidget>


class WindowManager
{
public:
    [[nodiscard]] static HWND getWindowHandle();

    static void setWinodw(HWND const &hwnd);

    static HBITMAP CaptureAnImage(HWND hWnd);

    // Save the specified HBITMAP to a file with the given file path
    static void SaveBitmapToFile(HBITMAP &hBitmap, const wchar_t* filePath);

    // Capture an image from the specified window and save it to the given file path
    static int CaptureAndSaveImage(HWND hWnd, const std::string &filePath);

    static void MouseDownUp(HWND hwnd, int x, int y);

    static int GetVkCode(const std::string &key);

    static void KeyDownUp(HWND hwnd, const std::string& key);

    static void InputText(HWND hwnd, const std::string &text);

private:
    // 键值到虚拟键码的映射
    static std::map<std::string, int> VkCode;
    // 私有成员变量和函数
};

#endif // WINDOWMANAGER_H
