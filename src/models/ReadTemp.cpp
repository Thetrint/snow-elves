//
// Created by y1726 on 24-9-10.
//
#include "models/ReadTemp.h"


ReadTemp::ReadTemp(const std::string& tempName) {
	// 打开文件（每个线程独立的文件流）
	std::ifstream ifs("resources/images.dat", std::ios::binary);
	if (!ifs) {
		std::cerr << "Failed to open images.dat file" << std::endl;
		return;
	}

	// 获取图像的偏移量和大小
	const unsigned char* img_data = Images[tempName].data;
	const size_t img_size = Images[tempName].size;

	// 定位到图像数据的偏移量位置
	ifs.seekg(reinterpret_cast<std::streamoff>(img_data));
	if (!ifs.good()) {
		std::cerr << "Failed to seek to the image data position for: " << tempName << std::endl;
		return;
	}

	// 读取图像数据到缓冲区
	std::vector<unsigned char> buffer(img_size);
	ifs.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(img_size));
	if (!ifs) {
		std::cerr << "Failed to read image data for: " << tempName << std::endl;
		return;
	}

	// 使用 OpenCV 将字节数据解码为图像
	tempImage = cv::imdecode(buffer, cv::IMREAD_COLOR);
	if (tempImage.empty()) {
		std::cerr << "Failed to decode image: " << tempName << std::endl;
	}
}

cv::Mat ReadTemp::getTemp() const {
	return tempImage;
}
