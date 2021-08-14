#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#pragma once

#include "ImageCapturer.h"
#include "FileCapturer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "../RRectUIElement.h"

#include "SharedTypes.h"
using cv::Mat;




class ImageMatcher
{
public:
	static bool MatchSift(FileCapturer& temp, FileCapturer&match, ImageMatchResult& result);

	
	static bool MatchTemplateEx(Mat& img, Mat& templateImg, Mat& mask, ImageMatchResult& findResult,double score = 0.9,
		int match_method = cv::TM_SQDIFF, bool use_mask = true, bool isDebug = false)
	{


		
		cvtColor(img, img, CV_RGB2GRAY);
		//cvtColor(templateImg, templateImg, CV_RGB2GRAY);
		//cvtColor(mask, mask, CV_RGB2GRAY);

		imshow("1", img);
		imshow("2", templateImg);
		imshow("3", mask);
		cv::waitKey();
		
		Mat result;
		Mat img_display;
		img.copyTo(img_display);
		int result_cols = img.cols - templateImg.cols + 1;
		int result_rows = img.rows - templateImg.rows + 1;
		result.create(result_rows, result_cols, CV_32FC1);
		bool method_accepts_mask = (cv::TM_SQDIFF == match_method || match_method == cv::TM_CCORR_NORMED);
		if (use_mask && method_accepts_mask)
		{
			matchTemplate(img, templateImg, result, match_method, mask);
		}
		else
		{
			matchTemplate(img, templateImg, result, match_method);
		}
		normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, Mat());
		double minVal; double maxVal;
		cv::Point minLoc;
		cv::Point maxLoc;
		cv::Point matchLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED)
		{
			matchLoc = minLoc;
		}
		else
		{
			matchLoc = maxLoc;
		}

		if (maxVal >= score)
		{
			findResult.x = matchLoc.x;
			findResult.y = matchLoc.y;
			findResult.score = maxVal;
		}
		else
			return false;


		if(isDebug)
		{
			rectangle(img_display, matchLoc, cv::Point(matchLoc.x + templateImg.cols, matchLoc.y + templateImg.rows), cv::Scalar::all(150), 2, 8, 0);
			rectangle(result, matchLoc, cv::Point(matchLoc.x + templateImg.cols, matchLoc.y + templateImg.rows), cv::Scalar::all(150), 2, 8, 0);
			
			imshow("image_window", img_display);
			imshow("result_window", result);
			cv::waitKey();

		}

		return true;
	}
};

