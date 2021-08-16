#include "pch.h"
#include "RShapeMaching.h"


#include "CV/ImageMatcher.h"
#include "CV/line2Dup.h"

using namespace cv;
using namespace std;

// NMS, got from cv::dnn so we don't need opencv contrib
// just collapse it

std::string RFramework::RShapeMatching::prefix = "./templ/";
std::unordered_map<std::string, line2Dup::Detector> RFramework::RShapeMatching::detectors;

namespace  cv_dnn {
	namespace
	{
		template <typename T>
		static inline bool SortScorePairDescend(const std::pair<float, T>& pair1,
			const std::pair<float, T>& pair2)
		{
			return pair1.first > pair2.first;
		}

	} // namespace

	inline void GetMaxScoreIndex(const std::vector<float>& scores, const float threshold, const int top_k,
		std::vector<std::pair<float, int> >& score_index_vec)
	{
		for (size_t i = 0; i < scores.size(); ++i)
		{
			if (scores[i] > threshold)
			{
				score_index_vec.push_back(std::make_pair(scores[i], i));
			}
		}
		std::stable_sort(score_index_vec.begin(), score_index_vec.end(),
			SortScorePairDescend<int>);
		if (top_k > 0 && top_k < (int)score_index_vec.size())
		{
			score_index_vec.resize(top_k);
		}
	}

	template <typename BoxType>
	inline void NMSFast_(const std::vector<BoxType>& bboxes,
		const std::vector<float>& scores, const float score_threshold,
		const float nms_threshold, const float eta, const int top_k,
		std::vector<int>& indices, float (*computeOverlap)(const BoxType&, const BoxType&))
	{
		CV_Assert(bboxes.size() == scores.size());
		std::vector<std::pair<float, int> > score_index_vec;
		GetMaxScoreIndex(scores, score_threshold, top_k, score_index_vec);

		// Do nms.
		float adaptive_threshold = nms_threshold;
		indices.clear();
		for (size_t i = 0; i < score_index_vec.size(); ++i) {
			const int idx = score_index_vec[i].second;
			bool keep = true;
			for (int k = 0; k < (int)indices.size() && keep; ++k) {
				const int kept_idx = indices[k];
				float overlap = computeOverlap(bboxes[idx], bboxes[kept_idx]);
				keep = overlap <= adaptive_threshold;
			}
			if (keep)
				indices.push_back(idx);
			if (keep && eta < 1 && adaptive_threshold > 0.5) {
				adaptive_threshold *= eta;
			}
		}
	}


	// copied from opencv 3.4, not exist in 3.0
	template<typename _Tp> static inline
		double jaccardDistance__(const Rect_<_Tp>& a, const Rect_<_Tp>& b) {
		_Tp Aa = a.area();
		_Tp Ab = b.area();

		if ((Aa + Ab) <= std::numeric_limits<_Tp>::epsilon()) {
			// jaccard_index = 1 -> distance = 0
			return 0.0;
		}

		double Aab = (a & b).area();
		// distance = 1 - jaccard_index
		return 1.0 - Aab / (Aa + Ab - Aab);
	}

	template <typename T>
	static inline float rectOverlap(const T& a, const T& b)
	{
		return 1.f - static_cast<float>(jaccardDistance__(a, b));
	}

	void NMSBoxes(const std::vector<Rect>& bboxes, const std::vector<float>& scores,
		const float score_threshold, const float nms_threshold,
		std::vector<int>& indices, const float eta = 1, const int top_k = 0)
	{
		NMSFast_(bboxes, scores, score_threshold, nms_threshold, eta, top_k, indices, rectOverlap);
	}

}


namespace RFramework
{

	ImageMatchResult RShapeMatching::Match(string objectName, const Mat& img, float score, bool isDebug)
	{

		std::vector<std::string> ids;
		ids.push_back(objectName);
		//ids.push_back("test");

		auto detector = GetDetector(objectName);


		// cvtColor(test_img, test_img, CV_BGR2GRAY);

		int stride = 16;
		int n = img.rows / stride;
		int m = img.cols / stride;
		Rect roi(0, 0, stride * m, stride * n);

		Mat img2 = img(roi).clone();

		line2Dup::Timer timer;
		auto matches = detector.match(img2, 80, ids);
		ImageMatchResult imageMatchResult;
		if (matches.size() > 0 && matches[0].similarity > score)
		{
			imageMatchResult.x = matches[0].x;
			imageMatchResult.y = matches[0].y;

			
			imageMatchResult.score = matches[0].similarity;

			auto templ = detector.getTemplates(objectName,matches[0].template_id);
			
			imageMatchResult.height = templ[0].height;
			imageMatchResult.width = templ[0].width;
		}
		
		if (!isDebug)
			return imageMatchResult;
		timer.out("Total time");

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

			auto templ = detector.getTemplates(objectName,
				match.template_id);

			box.width = templ[0].width;
			box.height = templ[0].height;
			boxes.push_back(box);
			scores.push_back(match.similarity);
		}
		cv_dnn::NMSBoxes(boxes, scores, 0, 0.5f, idxs);

		for (auto idx : idxs) {
			auto match = matches[idx];
			auto templ = detector.getTemplates(objectName,
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
				cv::circle(img, { feat.x + match.x, feat.y + match.y }, 2, randColor, -1);
			}

			cv::putText(img, to_string(int(round(match.similarity))),
				Point(match.x + r - 10, match.y - 3), FONT_HERSHEY_PLAIN, 2, randColor);
			cv::rectangle(img, { match.x, match.y }, { x, y }, randColor, 2);


			std::cout << "\nmatch.template_id: " << match.template_id << std::endl;
			std::cout << "match.similarity: " << match.similarity << std::endl;
		}
		imshow("img", img);
		
		waitKey(0);
		std::cout << "test end" << std::endl << std::endl;
		
		return imageMatchResult;
	}
	
	void RShapeMatching::Train(std::string objectName,std::vector<float> angleRange)
	{
		line2Dup::Detector detector(30, { 4,4 });
		Mat img = imread(prefix + "train/" + objectName + ".png");
		assert(!img.empty() && "check your img path");
		Mat mask = Mat(img.size(), CV_8UC1, { 255 });

		shape_based_matching::shapeInfo_producer shapes(img, mask);
		shapes.angle_range = angleRange;
		shapes.angle_step = 0.1;
		shapes.produce_infos();
		std::vector<shape_based_matching::shapeInfo_producer::Info> infos_have_templ;
		string class_id = objectName;
		for (auto& info : shapes.infos) {
			imshow("train", shapes.src_of(info));
			waitKey(1);

			std::cout << "\ninfo.angle: " << info.angle << std::endl;
			int templ_id = detector.addTemplate(shapes.src_of(info), class_id, shapes.mask_of(info));
			std::cout << "templ_id: " << templ_id << std::endl;
			if (templ_id != -1) {
				infos_have_templ.push_back(info);
			}
		}
		detector.writeClasses(prefix + "%s_templ.yaml");
		//shapes.save_infos(infos_have_templ, prefix + "test_info.yaml");
		std::cout << "train end" << std::endl << std::endl;
	}

}

