#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using cv::Mat;

#pragma once
class ImageCapturer 
{
public:
	virtual Mat Capture() = 0;
};

