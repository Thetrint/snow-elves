//
// Created by y1726 on 2024/6/26.
//
#include <models/WindowManager.h>
#include "models/ImageProcess.h"


int main() {

    WindowManager::CaptureAndSaveImage(WindowManager::getWindowHandle(), L"1.bmp");

    return 0;
}