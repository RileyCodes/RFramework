#include "ImageMatcher.h"


using namespace std;
using namespace cv;
bool ImageMatcher::MatchSift(FileCapturer& temp, FileCapturer& match, ImageMatchResult& result)
{
    Mat image1 = temp.Capture();
    Mat image2 = match.Capture();

    imshow("p2", image1);
    imshow("p1", image2);
    waitKey();

    //灰度图转换  
    //Mat image1, image2;
    //cvtColor(image01, image1, CV_BGR2GRAY);
    //cvtColor(image02, image2, CV_BGR2GRAY);

 
    //提取特征点    
    Ptr<SiftFeatureDetector> siftDetector = SiftFeatureDetector::create();
    

    vector<KeyPoint> keyPoint1, keyPoint2;
    siftDetector->detect(image1, keyPoint1);
    siftDetector->detect(image2, keyPoint2);

    //特征点描述，为下边的特征点配做准备

    Ptr<SiftFeatureDetector> SiftDescriptor = SiftDescriptorExtractor::create();
    Mat imageDesc1, imageDesc2;
    SiftDescriptor->compute(image1, keyPoint1, imageDesc1);
    SiftDescriptor->compute(image2, keyPoint2, imageDesc2);

    FlannBasedMatcher matcher;
    vector<vector<DMatch> > matchePoints;
    vector<DMatch> GoodMatchePoints;

    vector<Mat> train_desc(1, imageDesc1);
    matcher.add(train_desc);
    matcher.train();

    matcher.knnMatch(imageDesc2, matchePoints, 2);
    cout << "total match points: " << matchePoints.size() << endl;

    // Lowe's algorithm,获取优秀匹配点
    for (int i = 0; i < matchePoints.size(); i++)
    {
        if (matchePoints[i][0].distance < 0.6 * matchePoints[i][1].distance)
        {
            GoodMatchePoints.push_back(matchePoints[i][0]);
        }
    }

    Mat first_match;
    drawMatches(image2, keyPoint2, image1, keyPoint1, GoodMatchePoints, first_match);

    Size size(1024, 768);
    Mat imSmall;
    resize(first_match, imSmall, size);
	
    imshow("first_match ", imSmall);

    waitKey();
    return true;
}


