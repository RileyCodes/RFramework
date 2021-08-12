#pragma once
#include <unordered_map>

#include "FileCapturer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

namespace cv {
	class Mat;
}

using cv::Mat;

class TemplateMgr
{
	struct Template
	{
		Mat templ;
		Mat mask;
	};
	std::unordered_map<std::string, Template> templates;

	const std::string extName = ".png";
	const std::string maskName = "_mask";
	const std::string folderName = "./templ/";

public:
	TemplateMgr() = default;
	void Add(std::string templateName)
	{
		FileCapturer templateCapturer(folderName + templateName + extName);
		FileCapturer templateMaskCapturer(folderName + templateName + maskName + extName);
		Template templ;
		templ.templ = templateCapturer.Capture();
		templ.mask = templateMaskCapturer.Capture();
		templates[templateName] = templ;
	}

	Template& operator [] (std::string templName) {
		if (templates.count(templName) == 0)
			Add(templName);
		
		return templates[templName];
	}
};

