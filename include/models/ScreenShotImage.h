//
// Created by y1726 on 24-9-10.
//

#ifndef SCREENSHOTIMAGE_H
#define SCREENSHOTIMAGE_H
#include "main.h"

class ScreenShotImage {


public:
	explicit ScreenShotImage(const HWND& hwnd);

	~ScreenShotImage();

	cv::Mat getImage() const;

private:
	cv::Mat image;
	HBITMAP hbitmap;


};
#endif //SCREENSHOTIMAGE_H
