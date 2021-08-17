#pragma once
#include <opencv2/core/types.hpp>
#include <functional>

struct ImageMatchResult;

class RRectUIElement
{
	int x = 0;
	int y = 0;
	int width = -1;
	int height = -1;
	
	std::function<cv::Point(const RRectUIElement &)> converter = nullptr;
	
public:
	double score = 0.0;
	void Init();
	RRectUIElement(int x, int y);
	RRectUIElement(ImageMatchResult& result);
	RRectUIElement(RRectUIElement& result);
	void SetConverter(std::function<cv::Point(const RRectUIElement&)> converter);
	void FromImageMatchResult(const ImageMatchResult& result);
	cv::Point GetClickPosition() const;

	RRectUIElement CreateByOffset(int offsetX,int offsetY);
};

