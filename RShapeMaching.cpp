#include "pch.h"
#include "RShapeMaching.h"

#include "CV/line2Dup.h"

using namespace cv;
using namespace std;
void RShapeMatching::Run()
{
	line2Dup::Detector detector(30, { 4, 8 });

	std::vector<std::string> ids;
	ids.push_back("test");
	detector.readClasses(ids, prefix + "case2/%s_templ.yaml");

	Mat test_img = imread(prefix + "case2/test.png");
	assert(!test_img.empty() && "check your img path");

	// cvtColor(test_img, test_img, CV_BGR2GRAY);

	int stride = 16;
	int n = test_img.rows / stride;
	int m = test_img.cols / stride;
	Rect roi(0, 0, stride * m, stride * n);

	test_img = test_img(roi).clone();


	auto matches = detector.match(test_img, 90, ids);

	std::cout << "matches.size(): " << matches.size() << std::endl;
	size_t top5 = 500;
	if (top5 > matches.size()) top5 = matches.size();

	vector<Rect> boxes;
	vector<float> scores;
	vector<int> idxs;
	for (auto match : matches) {
		Rect box;
		box.x = match.x;
		box.y = match.y;

		auto templ = detector.getTemplates("test",
			match.template_id);

		box.width = templ[0].width;
		box.height = templ[0].height;
		boxes.push_back(box);
		scores.push_back(match.similarity);
	}
	cv_dnn::NMSBoxes(boxes, scores, 0, 0.5f, idxs);

	for (auto idx : idxs) {
		auto match = matches[idx];
		auto templ = detector.getTemplates("test",
			match.template_id);

		int x = templ[0].width + match.x;
		int y = templ[0].height + match.y;
		int r = templ[0].width / 2;
		cv::Vec3b randColor;
		randColor[0] = rand() % 155 + 100;
		randColor[1] = rand() % 155 + 100;
		randColor[2] = rand() % 155 + 100;

		for (int i = 0; i < templ[0].features.size(); i++) {
			auto feat = templ[0].features[i];
			cv::circle(test_img, { feat.x + match.x, feat.y + match.y }, 2, randColor, -1);
		}

		cv::putText(test_img, to_string(int(round(match.similarity))),
			Point(match.x + r - 10, match.y - 3), FONT_HERSHEY_PLAIN, 2, randColor);
		cv::rectangle(test_img, { match.x, match.y }, { x, y }, randColor, 2);

		std::cout << "\nmatch.template_id: " << match.template_id << std::endl;
		std::cout << "match.similarity: " << match.similarity << std::endl;

		imshow("img", test_img);
		waitKey(0);

		std::cout << "test end" << std::endl << std::endl;
	}
}
