// Name: Chen Liang
// USC ID Number: 7314634396
// USC Email: lian455@usc.edu
// Submission Date: 03/15/2017


#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
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


// Define file pointer and variables
FILE *file;
const int height = 128;
const int width = 128;
bool use_Mahalanobis = false;
bool use_Mahalabobis_test = true;
string readFlag;

//Read 12 training texture images
vector<double**> readTextureImages(string readFlag)
{
	vector<double**> textureBanks;
	int iter;
	if (readFlag == "training")
		iter = 13;
	else if (readFlag == "testing")
		iter = 7;
	for (int i = 1; i < iter; i++)
	{
		//Memory allocation
		double** data = new double*[height + 4];
		for (int i = 0; i < height + 4; i++)
			data[i] = new double[width + 4];

		//Read texture images
		unsigned char *buf;
		buf = new unsigned char[height * width];
		char fileIndex[10];
		if (readFlag == "training")
		{
			sprintf(fileIndex, "%d", i);
			if (!(file = fopen(("texture" + string(fileIndex) + ".raw").c_str(), "rb"))) {
				cout << "Cannot open file: " << ("texture" + string(fileIndex) + ".raw").c_str() << endl;
				exit(1);
			}
		}
		else if (readFlag == "testing")
		{
			string index;
			if (i == 1) index = "A";
			if (i == 2) index = "B";
			if (i == 3) index = "C";
			if (i == 4) index = "D";
			if (i == 5) index = "E";
			if (i == 6) index = "F";
			if (!(file = fopen(("texture" + index + ".raw").c_str(), "rb"))) {
				cout << "Cannot open file: " << ("texture" + index + ".raw").c_str() << endl;
				exit(1);
			}
		}
		
		fread(buf, sizeof(unsigned char), height * width, file);
		fclose(file);

		//Compute global mean 
		double sum = 0, average = 0, temp = 0;
		for (int i = 0; i < height*width; i++)
		{
			temp = buf[i];
			sum += temp;
		}
		average = sum / (height*width);

		//Padding
		for (int i = 0; i < height + 4; i++)
		{
			for (int j = 0; j < width + 4; j++)
				data[i][j] = 0;
		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				data[i + 2][j + 2] = buf[i*width + j];
				data[i + 2][j + 2] -= average;
				//data[i + 2][j + 2] = data[i + 2][j + 2] / 255;
			}	
		}
		
		textureBanks.push_back(data);
	}
	return textureBanks;
}

//Construct 25 law filters bank
vector<Mat> constructLawFilters(vector<Mat> oneDimKernels)
{
	vector<Mat> filterBanks;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Mat twoDimKernel = oneDimKernels[i].t() * oneDimKernels[j];
			filterBanks.push_back(twoDimKernel);
		}
	}

	return filterBanks;
}

//Find feature vectors
double findFeatures(Mat filter, double** texture)
{
	double totalEnergy = 0;
	for (int i = 2; i < height + 2; i++)
	{
		for (int j = 2; j < width + 2; j++)
		{
			double tempSum = 0;
			for (int p = -2; p < 3; p++)
			{
				for (int q = -2; q < 3; q++)
					tempSum += texture[i + p][j + q] * filter.at<double>(2 + p, 2 + q);
			}
			//Calculate the total energy
			//totalEnergy += pow(tempSum,2);
			totalEnergy += abs(tempSum);
		}
	}
	//Calculate the average energy and return
	return totalEnergy / (height*width);
}

//Perform K-clustering using 2's split
vector<Mat> kmean(Mat features)
{
	//1. Choose 2 centroids
	Mat centroids = Mat(static_cast<int>(features.rows), 2, CV_64FC1);
	vector<Mat> clusters;
	Mat results = Mat::zeros(10, 2, CV_64FC1);
	vector<vector<double>> result;
	Vec4d mean, stddev;
	for (int i = 0; i < features.rows; i++)
	{
		meanStdDev(features.row(i), mean, stddev);
		if (use_Mahalanobis)
		{
			centroids.at<double>(i, 0) = mean[0] - stddev[0];
			centroids.at<double>(i, 1) = mean[0] + stddev[0];
		}
		else
		{
			centroids.at<double>(i, 0) = mean[0] - 1.2* stddev[0];
			centroids.at<double>(i, 1) = mean[0] + 0.8* stddev[0];
			cout << centroids.at<double>(i, 0) << " " << centroids.at<double>(i, 1) << endl;
		}
		cout << endl;
	}

	while (true)
	{
		//2. Use these 2 centroids to do clustering 
		vector<double> c1, c2;
		for (int i = 0; i < features.cols; i++)
		{
			if (use_Mahalanobis) //need modification
			{
				Mat sample1, sample2, cov1, cov2, invCov1, invCov2, mean1, mean2;
				hconcat(features.col(i), centroids.col(0), sample1);
				hconcat(features.col(i), centroids.col(1), sample2);
				calcCovarMatrix(sample1, cov1, mean1, CV_COVAR_COLS, CV_64F);
				calcCovarMatrix(sample2, cov2, mean2, CV_COVAR_COLS, CV_64F);
				invert(cov1, invCov1, DECOMP_SVD);
				invert(cov2, invCov2, DECOMP_SVD);
				if (Mahalanobis(features.col(i), centroids.col(0), invCov1) < Mahalanobis(features.col(i), centroids.col(1), invCov2))
					c1.push_back(i);
				else
					c2.push_back(i);
			}
			else
			{
				if (norm(features.col(i), centroids.col(0), NORM_L2) < norm(features.col(i), centroids.col(1), NORM_L2))
					c1.push_back(i);
				else
					c2.push_back(i);
			}
			
		}
		result.push_back(c1);
		result.push_back(c2);

		//3. Find 2 new centroids from the existing 2 clustering
		Mat temp_centroids = Mat(static_cast<int>(features.rows), 2, CV_64FC1);
		for (int k = 0; k < result.size(); k++)
		{
			Mat cluster = Mat(static_cast<int>(features.rows), static_cast<int>(result[k].size()), CV_64FC1);
			for (int j = 0; j < result[k].size(); j++)
				features.col(result[k][j]).copyTo(cluster.col(j));
			clusters.push_back(cluster);
			for (int i = 0; i < features.rows; i++)
			{
				meanStdDev(cluster.row(i), mean, stddev);
				temp_centroids.at<double>(i, k) = mean[0];
			}
		}

		if (countNonZero(temp_centroids == centroids) == 0) //Case I: converge, end the process
			break;
		else //Case II: not converge, continue the process
			temp_centroids.copyTo(centroids);

		clusters.clear();
		result.clear();
	}

	
	vector<Mat> returnValue; 
	//[0] = Mat of cluster 1
	//[1] = Mat of cluster 2
	//[2] = Mat of indices of texture image. 
	//		Each column represents texture image indices that are in the same cluster.
	//[3] = Mat of centroid, col1 centroid 1, col2 centroid 2

	for (int i = 0; i < clusters.size(); i++)
		returnValue.push_back(clusters[i]);
	
	for (int i = 0; i < result.size(); i++) //2
	{
		for (int j = 0; j < result[i].size(); j++) // no idea
			results.at<double>(j, i) = result[i][j] + 1;
	}
	returnValue.push_back(results);
	returnValue.push_back(centroids);
	return returnValue;
}

Mat classificationTestImages(Mat testVectors_Mat, Mat kMean_Centroids)
{
	Mat test_Results = Mat::zeros(6, static_cast<int>(kMean_Centroids.cols), CV_64FC1); //6*4
	for (int i = 0; i < testVectors_Mat.cols; i++) //6
	{
		Mat dist = Mat(1, static_cast<int>(kMean_Centroids.cols), CV_64FC1); //1*4
		Mat sortedIndex;
		for (int j = 0; j < kMean_Centroids.cols; j++)//4
		{
			if (use_Mahalabobis_test)
			{
				Mat sample, cov, invCov, mean;
				hconcat(testVectors_Mat.col(i), kMean_Centroids.col(j), sample);
				calcCovarMatrix(sample, cov, mean, CV_COVAR_NORMAL | CV_COVAR_COLS, CV_64F);
				invert(cov, invCov, DECOMP_SVD);
				dist.at<double>(0, j) = Mahalanobis(testVectors_Mat.col(i), kMean_Centroids.col(j), invCov);
			}
			else
				dist.at<double>(0, j) = norm(testVectors_Mat.col(i), kMean_Centroids.col(j), NORM_L2);
		}
		sortIdx(dist, sortedIndex, CV_SORT_ASCENDING);
		for (int k = 0; k < test_Results.rows; k++)
		{
			if (test_Results.at<double>(k, (sortedIndex.at<int>(0, 0))) == 0)
			{
				test_Results.at<double>(k, (sortedIndex.at<int>(0, 0))) = i + 1;
				break;
			}
		}
	}

	return test_Results; 
}

int main(int argc, char *argv[])
{
	// Check for proper syntax
	if (argc < 2) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name [method = S/US]" << endl;
		//S = Supervised Learning 
		//US = Unsupervised Learning
		return 0;
	}
	string method = argv[1];


	Mat L5 = (Mat_<double>(1, 5) << 1, 4, 6, 4, 1);
	Mat E5 = (Mat_<double>(1, 5) << -1, -2, 0, 2, 1);
	Mat S5 = (Mat_<double>(1, 5) << -1, 0, 2, 0, -1);
	Mat W5 = (Mat_<double>(1, 5) << -1, 2, 0, -2, 1);
	Mat R5 = (Mat_<double>(1, 5) << 1, -4, 6, -4, 1);
	vector<Mat> oneDimKernels;
	oneDimKernels.push_back(L5);
	oneDimKernels.push_back(E5);
	oneDimKernels.push_back(S5);
	oneDimKernels.push_back(W5);
	oneDimKernels.push_back(R5);

	//==================================================================
	//Construct filter banks
	vector<Mat> filterBanks = constructLawFilters(oneDimKernels);

	//===================================================================
	//Read texture images
	readFlag = "training";
	vector<double**> textureBanks = readTextureImages(readFlag);
	//Read testing images
	readFlag = "testing";
	vector<double**> testBanks = readTextureImages(readFlag);

	//====================================================================
	//Extract 12 feature vectors from texture images. Each feature vector has the dimension of 25*1. 
	vector<vector<double>> featureVectors; //25*12
	for (int numText = 0; numText < textureBanks.size(); numText++) //12
	{
		vector<double> featureVector; //25*1
		for (int numFil = 0; numFil < filterBanks.size(); numFil++) //25
		{
			double feature = findFeatures(filterBanks[numFil], textureBanks[numText]);
			featureVector.push_back(feature);
		}
		featureVectors.push_back(featureVector);
	}

	Mat featureVectors_Mat = Mat(static_cast<int>(featureVectors[0].size()), static_cast<int>(featureVectors.size()), CV_64FC1); //25*12, column major
	for (int i = 0; i < featureVectors.size(); i++) //12
	{
		for (int j = 0; j < featureVectors[0].size(); j++) //25
			featureVectors_Mat.at<double>(j, i) = featureVectors[i][j];
	}

	//====================================================================
	//Extract 6 feature vectors from test images. Each feature vector has the dimension of 25*1. 
	vector<vector<double>> testVectors; //25*12
	for (int numText = 0; numText < testBanks.size(); numText++) //12
	{
		vector<double> testVector; //25*1
		for (int numFil = 0; numFil < filterBanks.size(); numFil++) //25
		{
			double feature = findFeatures(filterBanks[numFil], testBanks[numText]);
			testVector.push_back(feature);
		}
		testVectors.push_back(testVector);
	}

	Mat testVectors_Mat = Mat(static_cast<int>(testVectors[0].size()), static_cast<int>(testVectors.size()), CV_64FC1); //25*6, column major
	for (int i = 0; i < testVectors.size(); i++) //12
	{
		for (int j = 0; j < testVectors[0].size(); j++) //25
			testVectors_Mat.at<double>(j, i) = testVectors[i][j];	
	}

	//==============================================================================
	//Perform PCA for training samples
	Mat featureReduced_Mat = Mat(3, static_cast<int>(featureVectors.size()), CV_64FC1); //3*12, column major
	PCA pca_analysis(featureVectors_Mat, Mat(), CV_PCA_DATA_AS_COL, 3);
	pca_analysis.project(featureVectors_Mat, featureReduced_Mat);

	for (int i = 0; i < featureReduced_Mat.rows; i++)
	{
		for (int j = 0; j < featureReduced_Mat.cols; j++)
		{
			cout << featureReduced_Mat.at<double>(i, j) << " ";
		}
		cout << endl;
	}

	//==============================================================================
	//Perform PCA for testing samples
	Mat testReduced_Mat = Mat(3, static_cast<int>(testVectors.size()), CV_64FC1); //3*12, column major
	PCA pca_analysis2(testVectors_Mat, Mat(), CV_PCA_DATA_AS_COL, 3);
	pca_analysis2.project(testVectors_Mat, testReduced_Mat);

	


	//Classification
	if (method == "US") //Unsupervised classification
	{
		//==============================================================================
		//Perform K-clustering
		//25-D result
		vector<Mat> kMean_25_Half = kmean(featureVectors_Mat);
		vector<Mat> kMean_25_Quater1 = kmean(kMean_25_Half[0]);
		for (int i = 0; i < kMean_25_Quater1[2].rows; i++)
		{
			for (int j = 0; j < kMean_25_Quater1[2].cols; j++)
			{
				if (kMean_25_Quater1[2].at<double>(i, j) != 0)
				{
					int temp = kMean_25_Quater1[2].at<double>(i, j);
					kMean_25_Quater1[2].at<double>(i, j) = kMean_25_Half[2].at<double>(temp - 1, 0);
				}
			}
		}
		vector<Mat> kMean_25_Quater2 = kmean(kMean_25_Half[1]);
		for (int i = 0; i < kMean_25_Quater2[2].rows; i++)
		{
			for (int j = 0; j < kMean_25_Quater2[2].cols; j++)
			{
				if (kMean_25_Quater2[2].at<double>(i, j) != 0)
				{
					int temp = kMean_25_Quater2[2].at<double>(i, j);
					kMean_25_Quater2[2].at<double>(i, j) = kMean_25_Half[2].at<double>(temp - 1, 1);
				}
			}
		}
		Mat kMean_25_Results, kMean_25_Centroids;
		hconcat(kMean_25_Quater1[2], kMean_25_Quater2[2], kMean_25_Results);
		hconcat(kMean_25_Quater1[3], kMean_25_Quater2[3], kMean_25_Centroids);

		//3-D result
		vector<Mat> kMean_3_Half = kmean(featureReduced_Mat);
		vector<Mat> kMean_3_Quater1 = kmean(kMean_3_Half[0]);
		for (int i = 0; i < kMean_3_Quater1[2].rows; i++)
		{
			for (int j = 0; j < kMean_3_Quater1[2].cols; j++)
			{
				if (kMean_3_Quater1[2].at<double>(i, j) != 0)
				{
					int temp = kMean_3_Quater1[2].at<double>(i, j);
					kMean_3_Quater1[2].at<double>(i, j) = kMean_3_Half[2].at<double>(temp - 1, 0);
				}
			}
		}
		vector<Mat> kMean_3_Quater2 = kmean(kMean_3_Half[1]);
		for (int i = 0; i < kMean_3_Quater2[2].rows; i++)
		{
			for (int j = 0; j < kMean_3_Quater2[2].cols; j++)
			{
				if (kMean_3_Quater2[2].at<double>(i, j) != 0)
				{
					int temp = kMean_3_Quater2[2].at<double>(i, j);
					kMean_3_Quater2[2].at<double>(i, j) = kMean_3_Half[2].at<double>(temp - 1, 1);
				}
			}
		}
		Mat kMean_3_Results, kMean_3_Centroids;
		hconcat(kMean_3_Quater1[2], kMean_3_Quater2[2], kMean_3_Results);
		hconcat(kMean_3_Quater1[3], kMean_3_Quater2[3], kMean_3_Centroids);

		//==============================================================================
		//Output classification Results
		cout << "The Training Result of 25-D Feature Vector = " << endl;
		for (int i = 0; i < kMean_25_Results.rows; i++)
		{
			for (int j = 0; j < kMean_25_Results.cols; j++)
				cout << kMean_25_Results.at<double>(i, j)<<" ";
			cout << endl;
		}
		cout << "The Training Result of 3-D Feature Vector = " << endl;
		for (int i = 0; i < kMean_3_Results.rows; i++)
		{
			for (int j = 0; j < kMean_3_Results.cols; j++)
				cout << kMean_3_Results.at<double>(i, j)<<" ";
			cout << endl;
		}

		//=============================================================================
		//Perform classification on the test images using the classification results of 25-D or 3-D feature vector
		Mat test_25_Results = classificationTestImages(testVectors_Mat, kMean_25_Centroids);
		Mat test_3_Results = classificationTestImages(testReduced_Mat, kMean_3_Centroids);

		//==============================================================================
		//Output classification Results
		cout << " 1=A; 2=B; C=3; D=4; E=5; F=6 " << endl;
		cout << "The Unsupervised Testing Result of 25-D Feature Vector = " << endl;
		for (int i = 0; i < test_25_Results.rows; i++)
		{
			for (int j = 0; j < test_25_Results.cols; j++)
				cout << test_25_Results.at<double>(i, j) << " ";
			cout << endl;
		}
		cout << "The Unsupervised Testing Result of 3-D Feature Vector = " << endl;
		for (int i = 0; i < test_3_Results.rows; i++)
		{
			for (int j = 0; j < test_3_Results.cols; j++)
				cout << test_3_Results.at<double>(i, j) << " ";
			cout << endl;
		}

		
	}
	else if (method == "S")
	{
		//===========================================================================
		//Create 4 centroids for 25-D
		vector<Mat> super_centroids_vector;
		super_centroids_vector.push_back((featureVectors_Mat.col(3) + featureVectors_Mat.col(4) + featureVectors_Mat.col(5)) / 3); //first group: 4 5 6
		super_centroids_vector.push_back((featureVectors_Mat.col(9) + featureVectors_Mat.col(10) + featureVectors_Mat.col(11)) / 3);//second group: 10 11 12
		super_centroids_vector.push_back((featureVectors_Mat.col(0) + featureVectors_Mat.col(1) + featureVectors_Mat.col(2)) / 3);//third group: 1 2 3
		super_centroids_vector.push_back((featureVectors_Mat.col(6) + featureVectors_Mat.col(7) + featureVectors_Mat.col(8)) / 3);//fourth group: 7 8 9

		Mat super_centroids;
		cv::hconcat(super_centroids_vector, super_centroids);

		//=============================================================================
		//Perform classification on the test images using the classification results of 25-D feature vector
		Mat test_super_Results = classificationTestImages(testVectors_Mat, super_centroids);

		//==============================================================================
		//Output classification Results
		cout << " 1=A; 2=B; C=3; D=4; E=5; F=6 " << endl;
		cout << "The Supervised Testing Result of 25-D Feature Vector = " << endl;
		for (int i = 0; i < test_super_Results.rows; i++)
		{
			for (int j = 0; j < test_super_Results.cols; j++)
				cout << test_super_Results.at<double>(i, j) << " ";
			cout << endl;
		}
	}

	return 0;
}
