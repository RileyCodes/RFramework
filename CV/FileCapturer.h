#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#pragma once
#include "ImageCapturer.h"

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using cv::Mat;

class FileCapturer : ImageCapturer
{
public:
	Mat Capture() override;
	std::string filePath;

	FileCapturer(std::string filePath)
	{
		this->filePath = filePath;
	}
};

