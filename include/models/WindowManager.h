#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


#include <windows.h>

#include <QWidget>


class WindowManager
{
public:
    static HWND GetOwnedWindows(HWND owner);

    [[nodiscard]] static HWND getWindowHandle(std::wstring& wintitle);

    static void setWinodw(HWND const &hwnd);

    static HBITMAP CaptureAnImage(HWND hWnd);

    // Save the specified HBITMAP to a file with the given file path
    static void SaveBitmapToFile(HBITMAP &hBitmap, const wchar_t* filePath);

    // Capture an image from the specified window and save it to the given file path
    static int CaptureAndSaveImage(HWND hWnd, const std::string &filePath);

    static void GetFactor();

    static void MouseDownUp(const HWND &hwnd, const int x, const int y);

    static void MouseKeep(const HWND &hwnd, const int x, const int y, const int delay);

    static void MouseWHEEL(const HWND &hwnd, int x, int y, int delta);

    static void MouseMove(const HWND &hwnd, int x1, int y1, int x2, int y2);

    static int GetVkCode(const std::string &key);

    static void KeyDownUp(const HWND &hwnd, const std::string &key);

    static void KeyKeep(const HWND &hwnd, const std::string &key, int dealy);

    static void KeyDown(const HWND &hwnd, const std::string &key);

    static void KeyUp(const HWND &hwnd, const std::string &key);

    static void InputText(HWND hwnd, const std::string &text);

private:

    // 键值到虚拟键码的映射
    static std::map<std::string, int> VkCode;
    // 私有成员变量和函数
};

#endif // WINDOWMANAGER_H
