// Name: Chen Liang
// USC ID Number: 7314634396
// USC Email: lian455@usc.edu
// Submission Date: 03/15/2017


#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>
#include <utility>
#include <string>

using namespace std;
using namespace cv;

bool findOut(Point2f a, Point2f b, int threshold, Mat H)
{
	Mat source = (Mat_<float>(3, 1) << a.x, a.y, 1);
	Mat temp = H * source;
	float xPrime = temp.at<float>(0, 0);
	float yPrime = temp.at<float>(1, 0);
	float w = temp.at<float>(2, 0);
	float calc_x = xPrime / w;
	float calc_y = yPrime / w;
	float diff = sqrt(pow((calc_x - b.x), 2) + pow((calc_y - b.y), 2));
	cout << "diff = " << diff << endl;
	if (diff > threshold)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void main(int argc, const char** argv)
{
	srand(time(NULL));
	string filename1;
	string filename2;
	string method;
	if (argc < 4)
	{
		cout << "Insufficient Input Images. " << endl;
		cout << "program_name [method = SIFT|SURF] [filename_1] [filename_2]" << endl;
		exit(0);
	}
	else
	{
		method = argv[1];
		filename1 = argv[2];
		filename2 = argv[3];
	}
	
	Mat image1_rgb = imread(filename1 + ".jpg");
	Mat image2_rgb = imread(filename2 + ".jpg");
	Mat image1 = imread(filename1 + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image2 = imread(filename2 + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;

	if (method == "SIFT")
	{
		// Create smart pointer for SIFT feature detector.
		SiftFeatureDetector featureDetector1, featureDetector2;
		featureDetector1.detect(image1, keypoints1);
		featureDetector2.detect(image2, keypoints2);

		//Create smart pointer for SIFT feature extractor.
		SiftDescriptorExtractor featureExtractor1, featureExtractor2;
		featureExtractor1.compute(image1, keypoints1, descriptors1);
		featureExtractor2.compute(image2, keypoints2, descriptors2);
	}
	else if (method == "SURF")
	{
		// Create smart pointer for SIFT feature detector.
		SurfFeatureDetector featureDetector1(1300) , featureDetector2(1300);
		featureDetector1.detect(image1, keypoints1);
		featureDetector2.detect(image2, keypoints2);

		//Create smart pointer for SIFT feature extractor.
		SurfDescriptorExtractor featureExtractor1, featureExtractor2;
		featureExtractor1.compute(image1, keypoints1, descriptors1);
		featureExtractor2.compute(image2, keypoints2, descriptors2);
	}
	else
	{
		cout << "Not a valid method name. " << endl;
		exit(0);
	}

	

	//Perform k nearest match to calculate all match lines
	FlannBasedMatcher match;
	//BFMatcher match(NORM_L2, false);
	vector<vector<DMatch>> matchLines;
	vector<DMatch> goodLines;
	goodLines.reserve(matchLines.size());
	match.knnMatch(descriptors1, descriptors2, matchLines, 2);

	//Apply the ratio test to choose good match from all the matches
	for (int i = 0; i < matchLines.size(); i++)
	{
		if (matchLines[i].size() < 2)
			continue;

		if (matchLines[i][0].distance < 0.75 * matchLines[i][1].distance)
		{
			goodLines.push_back(matchLines[i][0]);
		}
	}

	Mat matchMat;
	drawMatches(image1_rgb, keypoints1, image2_rgb, keypoints2, goodLines, matchMat, CV_RGB(255, 0, 0));
	imshow("matching", matchMat);
	imwrite(method + "_" + filename1 + "_" + filename2 + ".jpg", matchMat);
	
	waitKey(0);

}

