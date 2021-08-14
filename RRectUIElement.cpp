#include "pch.h"
#include "RRectUIElement.h"
double score;

RRectUIElement::RRectUIElement(ImageMatchResult& result)
{
	FromImageMatchResult(result);
}
void RRectUIElement::SetConverter(std::function<cv::Point()> converter)
{
	this->converter = converter;
}

void RRectUIElement::FromImageMatchResult(const ImageMatchResult& result)
{
	x = result.x;
	y = result.y;
	width = result.width;
	height = result.height;
	score = result.score;
}

cv::Point RRectUIElement::GetClickPosition()
{
	return converter();
}