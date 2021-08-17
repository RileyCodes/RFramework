#include "pch.h"
#include "RRectUIElement.h"
#include "CV/SharedTypes.h"


void RRectUIElement::Init()
{
	if (converter == nullptr)
	{
		converter = [](const RRectUIElement& ui)
		{
			return cv::Point(ui.x + (ui.width / 2), ui.y + (ui.height / 2));
		};
	}
}

RRectUIElement::RRectUIElement(int x, int y)
{
	this->x = x;
	this->y = y;
	Init();
}

RRectUIElement::RRectUIElement(ImageMatchResult& result)
{
	FromImageMatchResult(result);
	Init();
}

RRectUIElement::RRectUIElement(RRectUIElement& result)
{
	Init();
	this->x = result.x;
	this->y = result.y;
	this->score = result.score;
	this->width = result.width;
	this->height = result.height;
}

void RRectUIElement::SetConverter(std::function<cv::Point(const RRectUIElement&)> converter)
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

cv::Point RRectUIElement::GetClickPosition() const
{
	return converter(*this);
}

RRectUIElement RRectUIElement::CreateByOffset(int offsetX, int offsetY)
{
	RRectUIElement newUI(*this);
	newUI.x += offsetX;
	newUI.y += offsetY;
	return newUI;
}
