#pragma once
//#include <opencv2/core/types.hpp>
//
//#include "CV/ImageMatcher.h"
//
//
//class RRectUIElement
//{
//	int x = 0;
//	int y = 0;
//	int width = -1;
//	int height = -1;
//	std::function<cv::Point()> converter = [this]()
//	{
//		return cv::Point(x + (width / 2), y + (height /2));
//	};
//
//public:
//
//	void SetConverter(std::function<cv::Point()> converter)
//	{
//		this->converter = converter;
//	}
//	
//	void FromImageMatchResult(const ImageMatchResult& result)
//	{
//		x = result.x;
//		y = result.y;
//		width = result.width;
//		height = result.height;
//	}
//	
//	cv::Point GetClickPosition()
//	{
//		return converter();
//	}
//};
//
