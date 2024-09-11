//
// Created by y1726 on 24-9-10.
//

#ifndef TEMPMATCHER_H
#define TEMPMATCHER_H
#include "main.h"

class TempMatcher {

public:
	TempMatcher() {

	}

	struct Match {
		cv::Point location;
		float score;
	};

	/**
	 * 模板匹配
	 * @param threshold 相似度阈值
	 */
	void matchTemplate(const double& threshold);

	/**
	 * 读取模板图片
	 * @param templName 模板名称
	 */
	void readTemplate(const std::string& templName);

	/**
	 * 接取窗口图片
	 * @param hwnd 窗口句柄
	 */
	void screenShotImage(const HWND& hwnd);

	/**
	 * 展示匹配结果
	 */
	void showMatches();

	/**
	 * 拆分图片
	 * @param x1 坐标
	 * @param y1 坐标
	 * @param x2 坐标
	 * @param y2 坐标
	 */
	void splitImage(const int& x1, const int& y1, const int& x2, const int& y2);

	/**
	 * 检查模板四个角的颜色是否相同，并生成掩码
	 */
	void createMaskIfCornersMatch();

	/**
	 * 应用灰度处理到模板和源图像
	 */
	void convertToGrayScale();

	/**
	 * 应用高斯模糊到模板和源图像。
	 *
	 * @param kernelWidth 模糊核的宽度，必须为正奇数。
	 * @param kernelHeight 模糊核的高度，必须为正奇数。
	 * @param sigmaX X 方向的标准差，决定模糊的强度。
	 */
	void applyGaussianBlur(int kernelWidth, int kernelHeight, double sigmaX);

	/**
	 * 使用 Canny 边缘检测算法检测图像中的边缘。
	 *
	 * @param lowThreshold 用于边缘检测的低阈值，较小的值将检测到较多的边缘。
	 * @param highThreshold 用于边缘检测的高阈值，较大的值将检测到较少的边缘。
	 */
	void detectEdges(double lowThreshold, double highThreshold);


	/**
	 * 获取匹配结果
	 * @return 匹配结果
	 */
	std::vector<Match> getMatchLocations() const;

private:
	int x1;
	int y1;
	int x2;
	int y2;
	cv::Mat image_;      // 原图
	cv::Mat imageCopy_;      // 原图
	cv::Mat template_;   // 模板图
	cv::Mat mask_ ;   // 掩码图
	std::vector<Match> matchLocations; // 匹配结果

};
#endif //TEMPMATCHER_H
