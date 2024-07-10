//
// Created by y1726 on 2024/6/26.
//
#include <models/WindowManager.h>
#include "models/ImageProcess.h"


int main() {

    WindowManager::setWinodw(WindowManager::getWindowHandle());
    WindowManager::CaptureAndSaveImage(WindowManager::getWindowHandle(), "1.bmp");

    return 0;
}