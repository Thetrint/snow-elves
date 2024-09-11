//
// Created by y1726 on 24-9-10.
//

#ifndef READTEMP_H
#define READTEMP_H
#include "main.h"

class ReadTemp {

public:
	explicit ReadTemp(const std::string& tempName);

	cv::Mat getTemp() const;

private:
	cv::Mat tempImage;


};
#endif //READTEMP_H
