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
const int height = 350;
const int width = 590;
bool use_Mahalanobis = false;
double color[10] = { 0, 0.4, 0.8, 0.2, 0.6, 0.9, 0.3, 0.7, 0.1, 0.5};

//Read 12 training texture images
double** readTextureImages()
{
	//Memory allocation
	double** data = new double*[height + 4];
	for (int i = 0; i < height + 4; i++)
		data[i] = new double[width + 4];

	//Read texture images
	unsigned char *buf;
	buf = new unsigned char[height * width * 3];
	if (!(file = fopen("kitten.raw", "rb"))) {
		cout << "Cannot open file: kitten.raw"<< endl;
		exit(1);
	}

	fread(buf, sizeof(unsigned char), height * width * 3, file);
	fclose(file);

	//Convert RGB to Gray Image
	double* buf_d;
	buf_d = new double [height * width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double temp1, temp2, temp3;
			temp1 = buf[i * width * 3 + j * 3];
			temp2 = buf[i * width * 3 + j * 3 + 1];
			temp3 = buf[i * width * 3 + j * 3 + 2];
			buf_d[i * width + j] = 0.2989 * temp1 + 0.5870 *  temp2 + 0.1140 * temp3;
		}
	}

	//Compute global mean 
	double sum = 0, average = 0;
	for (int i = 0; i < height*width; i++)
	{
		sum += buf_d[i];
		//cout << buf_d[i] <<" "<< sum << endl;
	}
		
	average = sum / double(height * width);
	cout << average << endl;
	

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
			data[i + 2][j + 2] = buf_d[i*width + j];
			data[i + 2][j + 2] -= average;
			//data[i + 2][j + 2] = data[i + 2][j + 2] / 255;
		}
	}
		
	return data;
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

//Find feature Mat
void findFeatures(Mat filter, double** texture, Mat& featureMat, int winSize)
{
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
			featureMat.at<double>(i - 2 + (winSize - 1) / 2, j - 2 + (winSize - 1) / 2) = tempSum;
			//cout << featureMat.at<double>(i - 2 + (winSize - 1) / 2, j - 2 + (winSize - 1) / 2) << endl;
		}
	}
}

//Compute average energy 
void computeEnergy(Mat& featureMat, int winSize)
{
	Mat newMat = Mat::zeros(static_cast<int>(height + winSize - 1), static_cast<int>(width + winSize - 1), CV_64FC1);
	int padSize = (winSize - 1) / 2;
	for (int i = padSize; i < height + padSize; i++)
	{
		for (int j = padSize; j < width + padSize; j++)
		{
			double tempSum = 0;
			for (int p = - padSize; p < padSize + 1; p++)
			{
				for (int q = - padSize; q < padSize + 1; q++)
				{
					tempSum += pow(featureMat.at<double>(i + p, j + q), 2) / pow(winSize,2);
					//cout << " tempSum = " <<tempSum << " " << featureMat.at<double>(i + p, j + q) << endl;
				}
			}
			//cout <<"===============================" << endl;
			newMat.at<double>(i, j) = tempSum;
			//cout << featureMat.at<double>(i, j) << endl;
		}
	}
	newMat.copyTo(featureMat);
}

//Use "largest distance" method to find the beginning centroids
void findCentroids(vector<int>& indexVector, Mat features, Mat& cur, Mat& next)
{
	int max = 0;
	int index; 
	//cout << cur.rows << " * " << cur.cols << endl;
	//cout << next.rows << " * " << next.cols << endl;
	vector<int>::iterator it;
	for (int i = 0; i < features.cols; i++)
	{
		it = find(indexVector.begin(), indexVector.end(), i);
		//cout << norm(features.col(i), cur, NORM_L2) << endl;
		if (norm(features.col(i), cur, NORM_L2) > max && it == indexVector.end())
		{
			max = norm(features.col(i), cur, NORM_L2);
			index = i;
			//cout << "we have i = " <<index << endl;
		}	
	}
	indexVector.push_back(index);
	features.col(index).copyTo(next);
}

//Perform K-clustering 
Mat kmean(Mat features, int cluster_num)
{
	vector<vector<int>> indicesCluster;
	for (int j = 0; j < cluster_num; j++)
	{
		vector<int> temp;
		indicesCluster.push_back(temp);
	}
	
	Mat result_Mat = Mat(static_cast<int>(height), static_cast<int>(width), CV_64FC1);
	
	//1. Choose begining centroids using largest distance method
	Mat centroids = Mat(static_cast<int>(features.rows), cluster_num, CV_64FC1);
	Vec4d mean, stddev;
	vector<int> index;
	for (int i = 0; i < features.rows; i++)
	{
		meanStdDev(features.row(i), mean, stddev);
		centroids.at<double>(i, 0) = mean[0];
		cout << centroids.at<double>(i, 0) << endl;
	}

	cout << "CHECK POINT 4.0 FINISHED" << endl;

	for (int i = 0; i < cluster_num - 1; i++) // change here
	{
		findCentroids(index, features, centroids.col(i), centroids.col(i + 1));
		for (int j = 0; j < centroids.rows; j++)
		{
			cout << centroids.at<double>(j, i+1) << endl;
		}
		cout << "Turn " << i+1 << endl;
	}
		
	cout << "CHECK POINT 4.1 FINISHED" << endl;

	while (true)
	{
		cout << "while loop" << endl;
		//2. Use the chosen centroids to do clustering 
		for (int i = 0; i < features.cols; i++)
		{
			Mat dist = Mat(1, static_cast<int>(centroids.cols), CV_64FC1);
			Mat sortedIndex;
			for (int j = 0; j < cluster_num; j++)
				dist.at<double>(0, j) = norm(features.col(i), centroids.col(j), NORM_L2);
			sortIdx(dist, sortedIndex, CV_SORT_ASCENDING);
			indicesCluster[sortedIndex.at<int>(0, 0)].push_back(i);
		}

		cout << "CHECK POINT 4.2 FINISHED" << endl; 

		//3. Find new centroids from the existing clusters
		Mat temp_centroids = Mat(static_cast<int>(features.rows), cluster_num, CV_64FC1);
		for (int k = 0; k < cluster_num; k++)
		{
			Mat cluster = Mat(static_cast<int>(features.rows), static_cast<int>(indicesCluster[k].size()), CV_64FC1);
			for (int j = 0; j < indicesCluster[k].size(); j++)
				features.col(indicesCluster[k][j]).copyTo(cluster.col(j));
			//clusters.push_back(cluster);
			for (int i = 0; i < features.rows; i++)
			{
				meanStdDev(cluster.row(i), mean, stddev);
				temp_centroids.at<double>(i, k) = mean[0];
			}
		}

		cout << "CHECK POINT 4.3 FINISHED" << endl;
		Mat dst;
		bitwise_xor(temp_centroids, centroids, dst);
		if (countNonZero(dst) > 0)
		{
			cout << countNonZero(dst) << endl;
			temp_centroids.copyTo(centroids);
		}
		else
		{
			int count = 0;
			for (int k = 0; k < cluster_num; k++)
			{
				for (int j = 0; j < indicesCluster[k].size(); j++)
				{			
					count++;
					cout << " cluster num = " << k <<" the corresponding color is " << color[k]<< endl;
					cout << count <<" " << indicesCluster[k][j] << endl;
					result_Mat.at<double>(int(indicesCluster[k][j] / width), indicesCluster[k][j] % width) = color[k] * 255;
				}
			}
			cout << "converge, break" << endl;
			break;
		}
			

		cout << "CHECK POINT 4.4 FINISHED" << endl;

		indicesCluster.clear();
		for (int j = 0; j < cluster_num; j++)
		{
			vector<int> temp;
			indicesCluster.push_back(temp);
		}
	}

	return result_Mat;
}

void writeSegmentedImage(Mat kMean_Results)
{
	unsigned char *buf;
	buf = new unsigned char[height * width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			buf[i*width + j] = kMean_Results.at<double>(i,j);
	}

	if (!(file = fopen("Segmented_result.raw", "wb"))) {
		cout << "Cannot open file: Segmented_result.raw" << endl;
		exit(1);
	}

	fwrite(buf, sizeof(unsigned char), height * width, file);
	fclose(file);
}

int main(int argc, char *argv[])
{
	for (int k = 0; k < 10; k++)
		cout << color[k] << endl;

	// Check for proper syntax
	if (argc < 2) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name [windowsize = 11/13/15] [cluster_number = 5/6/7/8/9/10]" << endl;
		return 0;
	}
	int winSize = atoi(argv[1]);
	int cluster_num = atoi(argv[2]);

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
	cout << "CHECK POINT 1 FINISHED" << endl;

	//===================================================================
	//Read texture image
	double** textureImage = readTextureImages();

	//===================================================================
	//Law feature extraction & Energy computation
	vector<Mat> featureVectors;
	for (int i = 0; i < filterBanks.size(); i++)
	{
		//cout << winSize << endl;
		Mat featureVector = Mat::zeros(static_cast<int>(height + winSize - 1), static_cast<int>(width + winSize - 1), CV_64FC1);
		findFeatures(filterBanks[i], textureImage, featureVector, winSize);

		/*for (int k = (winSize - 1) / 2; k < height + (winSize - 1) / 2; k++)
		{
			for (int j = (winSize - 1) / 2; j < width + (winSize - 1) / 2; j++)
			{
				if (featureVector.at<double>(k, j) > 100)
				{
					cout << featureVector.at<double>(k, j) << endl;
				}
			}
		}*/

		computeEnergy(featureVector, winSize);

		/*for (int k = (winSize - 1) / 2; k < height + (winSize - 1) / 2; k++)
		{
			for (int j = (winSize - 1) / 2; j < width + (winSize - 1) / 2; j++)
			{
				cout << featureVector.at<double>(k, j) << endl;
			}
		}*/


		featureVectors.push_back(featureVector);
	}

	
	cout << "CHECK POINT 3 FINISHED" << endl;

	//=====================================================================
	//Energy normalization by using L5*L5
	Mat featureVectors_Mat = Mat(static_cast<int>(filterBanks.size()), static_cast<int>(width*height), CV_64FC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < filterBanks.size(); k++)
			{
				featureVectors_Mat.at<double>(k, i*width + j) =
					featureVectors[k].at<double>(i + (winSize - 1) / 2, j + (winSize - 1) / 2) / featureVectors[0].at<double>(i + (winSize - 1) / 2, j + (winSize - 1) / 2);
				//cout << featureVectors_Mat.at<double>(k, i*width + j) << endl;
			}
				
		}
	}
	cout << "CHECK POINT 4 FINISHED" << endl;

	//========================================================================
	//PCA 
	Mat featureReduced_Mat = Mat(3, static_cast<int>(featureVectors.size()), CV_64FC1); //3*12, column major
	PCA pca_analysis(featureVectors_Mat, Mat(), CV_PCA_DATA_AS_COL, 3);
	pca_analysis.project(featureVectors_Mat, featureReduced_Mat);

	//=======================================================================
	//K-Mean Clustering
	//cout << featureVectors_Mat.rows << " "<< featureVectors_Mat.cols << " "<< 128*128<< endl;
	Mat kMean_Results = kmean(featureReduced_Mat, cluster_num);

	/*for (int k = 0; k < height; k++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << kMean_Results.at<double>(k, j) << endl;
		}
	}*/


	cout << "CHECK POINT 5 FINISHED" << endl;

	//========================================================================
	//Write image
	writeSegmentedImage(kMean_Results);
	cout << "CHECK POINT 6 FINISHED" << endl;

	return 0;
}
