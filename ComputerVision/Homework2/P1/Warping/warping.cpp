// Name: Chen Liang
// USC ID Number: 7314634396
// USC Email: lian455@usc.edu
// Submission Date: 02/15/2017


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

using namespace std;
using namespace cv;

Mat findWarpingMatrix(Point2f a1, Point2f b1, Point2f a2, Point2f b2, Point2f c)
{
	Point2f srcTri[3];
	Point2f dstTri[3];

	srcTri[0] = a1;
	srcTri[1] = a2;
	srcTri[2] = c;

	dstTri[0] = b1;
	dstTri[1] = b2;
	dstTri[2] = c;

	/// Get the Affine Transform
	Mat warp = getAffineTransform(srcTri, dstTri);
	Mat adding = (Mat_<double>(1, 3) << 0, 0, 1);
	warp.push_back(adding);
	return warp;
}

void transformTriangle(int*** Imagedata, int*** Warpdata, int i, int j, Point2f c, Mat warp, const int width, const int height)
{
	int ox = c.x;
	int oy = c.y;

	if (i == 0 || i == 7)
	{
		for (int p = 0; p<ox; p++)
		{
			for (int q = 0; q<oy; q++)
			{
				if ((p>=q && i == 0) || (p<=q && i == 7))
				{
					Mat src = (Mat_<double>(3, 1) << p, q, 1);
					Mat dst = warp*src;
					int transfery = floor(dst.at<double>(1, 0));
					int transferx = floor(dst.at<double>(0, 0));
					Warpdata[transfery][transferx][j] = Imagedata[q][p][j];
				}
			}
		}
	}

	if (i == 1 || i == 2)
	{
		for (int p = ox; p<width; p++)
		{
			for (int q = 0; q<oy; q++)
			{
				if ((p - ox <= oy - q && i == 1) || (oy - q <= p - ox && i == 2))
				{
					Mat src = (Mat_<double>(3, 1) << p, q, 1);
					Mat dst = warp*src;
					int transfery = floor(dst.at<double>(1, 0));
					int transferx = floor(dst.at<double>(0, 0));
					Warpdata[transfery][transferx][j] = Imagedata[q][p][j];
				}
			}
		}
	}

	if (i == 3 || i == 4)
	{
		for (int p = ox; p<width; p++)
		{
			for (int q = oy; q<height; q++)
			{
				if ((p - ox >= q - oy && i == 3) || (q - oy >= p - ox && i == 4))
				{
					Mat src = (Mat_<double>(3, 1) << p, q, 1);
					Mat dst = warp*src;
					int transfery = floor(dst.at<double>(1, 0));
					int transferx = floor(dst.at<double>(0, 0));
					Warpdata[transfery][transferx][j] = Imagedata[q][p][j];
				}
			}
		}
	}

	if (i == 5 || i == 6)
	{
		for (int p = 0; p<ox; p++)
		{
			for (int q = oy; q<height; q++)
			{
				if ((p >= height - q  && i == 5) || (height - q >= p && i == 6))
				{
					Mat src = (Mat_<double>(3, 1) << p, q, 1);
					Mat dst = warp*src;
					int transfery = floor(dst.at<double>(1, 0));
					int transferx = floor(dst.at<double>(0, 0));
					Warpdata[transfery][transferx][j] = Imagedata[q][p][j];
				}
			}
		}
	}
}

void transformLines(int*** Imagedata, int*** Warpdata, const int width, const int height)
{
	for (int i = 0; i < height; i++)
	{
		float scale = 1.0 - abs(float(i) - height / 2.0) / float(height / 2.0);
		for (int j = 0; j < width / 2; j++)
		{
			int transfer_j = floor(float(width / 2) - (float(width / 2 - j)*scale));
			for(int k=0; k<3; k++)
				Warpdata[i][transfer_j][k] = Imagedata[i][j][k];
		}
	}

	for (int i = 0; i < height; i++)
	{
		float scale = 1.0 - abs(float(i) - height / 2.0) / float(height / 2.0);
		for (int j = width/2; j < width; j++)
		{
			int transfer_j = floor(float(width / 2) + (float(j - width / 2)*scale));
			for (int k = 0; k<3; k++)
				Warpdata[i][transfer_j][k] = Imagedata[i][j][k];
		}
	}
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	const int BytesPerPixel = 3;
	const int width = 500;
	const int height = 500;

	// Check for proper syntax
	if (argc < 4) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input.raw output.raw [method = A/B]" << endl;
		//A = Line Shrinking Method
		//B = Triangle Mapping Method
		return 0;
	}

	string method = argv[3];

	//Allocate image data array
	int*** Imagedata = new int**[height];
	for (int i = 0; i < height; i++)
	{
		Imagedata[i] = new int*[width];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			Imagedata[i][j] = new int[BytesPerPixel];
	}

	int*** Warpdata = new int**[height];
	for (int i = 0; i < height; i++)
	{
		Warpdata[i] = new int*[width];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			Warpdata[i][j] = new int[BytesPerPixel];
	}

	//Allocate buf
	unsigned char buf[height][width][BytesPerPixel];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	//store data in buf into Imagedata
	for(int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
			{
				Imagedata[i][j][z] = buf[i][j][z];
				Warpdata[i][j][z] = 0;
			}
		}
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	Point2f c(width / 2, height / 2);
	Point2f before[8];
	Point2f after[8];
	Mat warpMat = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);

	before[0] = Point2f(0, 0);
	before[1] = Point2f(width / 2, 0);
	before[2] = Point2f(width, 0);
	before[3] = Point2f(width, height / 2);
	before[4] = Point2f(width, height);
	before[5] = Point2f(width / 2, height);
	before[6] = Point2f(0, height);
	before[7] = Point2f(0, height / 2);

	after[0] = Point2f(width / 4, height / 4);
	after[1] = Point2f(width / 2, 0);
	after[2] = Point2f(width / 4 * 3, height / 4);
	after[3] = Point2f(width, height / 2);
	after[4] = Point2f(width / 4 * 3, height / 4 * 3);
	after[5] = Point2f(width / 2, height);
	after[6] = Point2f(width / 4, height / 4 * 3);
	after[7] = Point2f(0, height / 2);

	if (method == "A") // using line shrinking method
		transformLines(Imagedata, Warpdata, width, height);
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j<BytesPerPixel; j++)
			{
				if (i == 7)
				{
					warpMat = findWarpingMatrix(before[i], after[i], before[i - 7], after[i - 7], c);
					transformTriangle(Imagedata, Warpdata, i, j, c, warpMat, width, height);
				}
				else
				{
					warpMat = findWarpingMatrix(before[i], after[i], before[i + 1], after[i + 1], c);
					transformTriangle(Imagedata, Warpdata, i, j, c, warpMat, width, height);
				}
			}
		}
	}//using triangle method
	

	//rewirte Imagedata into buf
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
				buf[i][j][z] = Warpdata[i][j][z];
		}
	}

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	fwrite(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
