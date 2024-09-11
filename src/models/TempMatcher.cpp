//
// Created by y1726 on 24-9-10.
//
#include "models/TempMatcher.h"

#include <models/ReadTemp.h>
#include <models/ScreenShotImage.h>


void TempMatcher::matchTemplate(const double& threshold) {
	cv::Mat result;

	if (!mask_.empty()) {
		// 使用掩码进行模板匹配
		cv::matchTemplate(image_, template_, result, cv::TM_CCORR_NORMED, mask_);
	} else {
		// 不使用掩码进行模板匹配
		cv::matchTemplate(image_, template_, result, cv::TM_CCORR_NORMED);
	}


	for (int y = 0; y < result.rows; ++y) {
		for (int x = 0; x < result.cols; ++x) {
			if (const float score = result.at<float>(y, x); score >= threshold && !std::isinf(score)) {
				matchLocations.push_back({{x + template_.cols / 2 + x1, y + template_.rows /2 + y1}, score});
			}
		}
	}

}

void TempMatcher::readTemplate(const std::string &templName) {
	template_ = ReadTemp(templName).getTemp();
}

void TempMatcher::screenShotImage(const HWND& hwnd) {
	image_ = ScreenShotImage(hwnd).getImage();
	imageCopy_ = image_;
}

void TempMatcher::showMatches() {

	cv::Mat ImageCopy = imageCopy_.clone(); // 使用 clone() 避免重复拷贝

	for (const auto& [location, score] : matchLocations) {
		cv::rectangle(ImageCopy,
					  cv::Point(location.x - template_.cols / 2, location.y - template_.rows / 2),
					  cv::Point(location.x + template_.cols / 2, location.y + template_.rows / 2),
					  cv::Scalar(255, 255, 255), 2);
	}
	cv::imshow("Source Image", ImageCopy);
	cv::waitKey(0);

}

void TempMatcher::splitImage(const int& x1, const int& y1, const int& x2, const int& y2) {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	const cv::Rect roi(x1, y1, x2 - x1, y2 - y1);
	image_ = image_(roi);
}


void TempMatcher::createMaskIfCornersMatch() {
	const auto cornerColor = template_.at<cv::Vec3b>(0, 0);
	const auto topRight = template_.at<cv::Vec3b>(0, template_.cols - 1);
	const auto bottomLeft = template_.at<cv::Vec3b>(template_.rows - 1, 0);
	const auto bottomRight = template_.at<cv::Vec3b>(template_.rows - 1, template_.cols - 1);

	if (cornerColor == topRight && cornerColor == bottomLeft && cornerColor == bottomRight) {
		cv::inRange(template_, cornerColor, cornerColor, mask_);
	}

}

void TempMatcher::convertToGrayScale() {
	cv::cvtColor(template_, template_, cv::COLOR_BGR2GRAY);
	cv::cvtColor(image_, image_, cv::COLOR_BGR2GRAY);
}

void TempMatcher::applyGaussianBlur(const int kernelWidth, const int kernelHeight, const double sigmaX) {
	cv::GaussianBlur(template_, template_, cv::Size(kernelWidth, kernelHeight), sigmaX);
	cv::GaussianBlur(image_, image_, cv::Size(kernelWidth, kernelHeight), sigmaX);
}

void TempMatcher::detectEdges(const double lowThreshold, const double highThreshold) {
	cv::Canny(template_, template_, lowThreshold, highThreshold);
	cv::Canny(image_, image_, lowThreshold, highThreshold);

}

std::vector<TempMatcher::Match> TempMatcher::getMatchLocations() const {
	return matchLocations;
}

