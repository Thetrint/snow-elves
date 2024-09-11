//
// Created by y1726 on 24-9-10.
//
#include "models/ScreenShotImage.h"

#include <models/ImageProcess.h>
#include <models/WindowManager.h>

ScreenShotImage::ScreenShotImage(const HWND& hwnd) {
	hbitmap = WindowManager::CaptureAnImage(hwnd);
	//模板匹配 数据转换类型
	image = ImageProcessor::HBITMAPToMat(hbitmap);
}

ScreenShotImage::~ScreenShotImage() {
	DeleteObject(hbitmap);
}

cv::Mat ScreenShotImage::getImage() const {
	return image;
}



