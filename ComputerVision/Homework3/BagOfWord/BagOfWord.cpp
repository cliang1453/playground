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

	if (argc < 5)
	{
		cout << "Insufficient Input Images. " << endl;
		cout << "program_name [training_image1] [training_image2] [training_image3] [training_image4]" << endl;
		exit(0);
	}
	

	vector<KeyPoint> keypoints;
	Mat descriptors;
	SiftFeatureDetector featureDetector;
	SiftDescriptorExtractor featureExtractor;
	Mat featuresUnclustered;

	for (int i = 0; i < 3; i++)
	{
		filename = argv[i + 1];
		Mat image = imread(filename + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
		featureDetector.detect(image, keypoints);
		//compute the descriptors for each keypoint
		featureExtractor.compute(image, keypoints, descriptors);
		//put the all feature descriptors in a single Mat object 
		featuresUnclustered.push_back(descriptors);
	}

	//Construct BOWKMeansTrainer
	//the number of bags
	int dictionarySize = 8;
	//define Term Criteria
	TermCriteria tc(CV_TERMCRIT_ITER, 100, 0.001);
	//retries number
	int retries = 1;
	//necessary flags
	int flags = KMEANS_PP_CENTERS;
	//Create the BoW (or BoF) trainer
	BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);
	//cluster the feature vectors
	Mat dictionary = bowTrainer.cluster(featuresUnclustered);
	//store the vocabulary
	FileStorage fs("dictionary.yml", FileStorage::WRITE);
	fs << "vocabulary" << dictionary;
	fs.release();

	cout << "CHECK POINT 1 FINISHED -- TRAINING STAGE FINISHED" << endl;
	///TESTING

	//create a nearest neighbor matcher
	Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
	//create Sift feature point extracter
	Ptr<FeatureDetector> detector(new SiftFeatureDetector());
	//create Sift descriptor extractor
	Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
	//create BoF (or BoW) descriptor extractor
	BOWImgDescriptorExtractor bowDE(extractor, matcher);
	//Set the dictionary with the vocabulary we created in the first step
	bowDE.setVocabulary(dictionary);

	char * imageTag = new char[10];

	vector<Mat> images;
	vector<Mat> bowDescriptors;
	vector<vector<KeyPoint>> keypoint_testings;
	Mat image, bowDescriptor;
	vector<KeyPoint> keypoint_testing; 
	for (int i = 0; i < 4; i++)
	{
		images.push_back(image);
		bowDescriptors.push_back(bowDescriptor);
		keypoint_testings.push_back(keypoint_testing);
	}

	for (int i = 0; i < 4; i++)
	{
		cout << "here" << endl;
		//read image
		filename = argv[i + 1];
		images[i] = imread(filename + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cout << "here1" << endl;
		//Detect SIFT keypoints (or feature points)
		detector->detect(images[i], keypoint_testings[i]);
		cout << "here2" << endl;
		bowDE.compute(images[i], keypoint_testings[i], bowDescriptors[i]);
		cout << "here3" << endl;
		cout << bowDescriptors[i].rows << " * " << bowDescriptors[i].cols << endl;
		cout << "here4" << endl;
	
		for (int j = 0; j < 8; j++)
		{
			cout<< bowDescriptors[i].at<int>(0, j)<<" ";
		}
		cout << endl;
	}

	int max = 0, index; 
	for (int i = 0; i < 3; i++)
	{
		if (norm(bowDescriptors[3], bowDescriptors[i], NORM_L2, noArray()) > max)
		{
			max = norm(bowDescriptors[3], bowDescriptors[i], NORM_L2, noArray());
			index = i;
		}	
	}

	cout << "convertible_2 should be matched to " << argv[index + 1] <<" , which has the most closest histgram distribution. " << endl;

	//open the file to write the resultant descriptor
	//FileStorage fs1("descriptor.yml", FileStorage::WRITE);
	//sprintf(imageTag, filename.c_str());
	////write the new BoF descriptor to the file
	//fs1 << imageTag << bowDescriptors[i];
	////release the file storage
	//fs1.release();

	cout << "CHECK POINT 2 FINISHED -- BOW DESCRIPTOR COMPUTING FINISHED" << endl;


	waitKey(0);
	
	return 0;
}

