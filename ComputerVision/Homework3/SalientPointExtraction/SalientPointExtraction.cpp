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

int main(int argc, const char** argv)
{
	string filename;
	string method;
	if (argc < 3)
	{
		cout << "Insufficient Input Images. " << endl;
		cout << "program_name [method = SIFT|SURF] [filename]" << endl;
		exit(0);
	}
	else
	{
		method = argv[1];
		filename = argv[2];
	}
		

	Mat image = imread(filename + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image_rgb = imread(filename + ".jpg");
	if (method == "SIFT")
	{
		vector<KeyPoint> keypoints;
		Mat descriptors;

		// Create smart pointer for SIFT feature detector.
		Ptr<SiftFeatureDetector> featureDetector = SiftFeatureDetector::create("SIFT");
		featureDetector->detect(image, keypoints);

		//Create smart pointer for SIFT feature extractor.
		Ptr<SiftDescriptorExtractor> featureExtractor = SiftDescriptorExtractor::create("SIFT");
		featureExtractor->compute(image, keypoints, descriptors);

		// draw images 
		Mat outputImage;
		Scalar keypointColor = Scalar(255, 0, 0);
		drawKeypoints(image_rgb, keypoints, outputImage, keypointColor, DrawMatchesFlags::DEFAULT);

		namedWindow("Output");
		imshow("Output", outputImage);
		imwrite(filename + "_SIFT_keypoint.jpg", outputImage);
	}
	else if (method == "SURF")
	{
		vector<KeyPoint> keypoints;
		Mat descriptors;

		//Create SURF feature detector.
		SurfFeatureDetector featureDetector(1300);
		featureDetector.detect(image, keypoints);

		//Create SURF feature extractor.
		SurfDescriptorExtractor featureExtractor;
		featureExtractor.compute(image, keypoints, descriptors);

		// If you would like to draw the detected keypoint just to check
		Mat outputImage;
		Scalar keypointColor = Scalar(255, 0, 0);
		drawKeypoints(image_rgb, keypoints, outputImage, keypointColor, DrawMatchesFlags::DEFAULT);

		namedWindow("Output");
		imshow("Output", outputImage);
		imwrite(filename + "_SURF_keypoint.jpg", outputImage);
	}
	else
	{
		cout << "Not a valid method name. " << endl;
		exit(0);
	}
	

	return 0;
}

