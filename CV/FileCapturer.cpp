#include "FileCapturer.h"

#include "FileCapturerException.h"


Mat FileCapturer::Capture()
{
	auto img = cv::imread(filePath, cv::COLOR_RGB2BGR);
	if(img.empty())
	{
		throw FileCapturerException(filePath);
	}
	return img;
}
