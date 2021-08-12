#pragma once
#include "CV/ImageMatcher.h"
#include "CV/line2Dup.h"

namespace line2Dup {
	class Detector;
}

class RShapeMatching
{
	static std::string prefix;

	static std::unordered_map<std::string, line2Dup::Detector> detectors;

	static void LoadDetector(std::string detectorName)
	{
		detectors[detectorName] = line2Dup::Detector(30, { 4, 8 });
		std::vector<std::string> ids;
		ids.push_back(detectorName);
		detectors[detectorName].readClasses(ids, prefix  +  "%s_templ.yaml");
	}

public:

	static line2Dup::Detector& GetDetector(std::string detectorName)
	{
		if(detectors.count(detectorName) == 0)
		{
			LoadDetector(detectorName);
		}
		return detectors[detectorName];
	}
	
	static ImageMatchResult Match(std::string objectName, const cv::Mat& img, float score, bool isDebug = true);

	static void Train(std::string objectName);
};

