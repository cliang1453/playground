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

void interpolate(int*** Pdata, int*** Tdata, Mat src, Mat dst)
{
	int i = floor(src.at<double>(1, 0));
	int j = floor(src.at<double>(0, 0));
	double a = src.at<double>(1, 0) - i;
	double b = src.at<double>(0, 0) - j;

	int y = dst.at<double>(1, 0);
	int x = dst.at<double>(0, 0);
	for (int k = 0; k < 3; k++)
	{
		Tdata[y][x][k] = (1 - a)*(1 - b)*Pdata[i][j][k] + a*(1 - b)*Pdata[i + 1][j][k]
			+ a*b*Pdata[i + 1][j + 1][k] + (1 - a)*b*Pdata[i][j + 1][k];
	}
	
}

void trans_T(int*** Pdata, int*** Tdata, Point2f T_src[], Point2f T_dst[], Mat inv_t1, Mat inv_t2, int width, int height)
{
	double theta0 = (double(T_dst[2].y) - double(T_dst[0].y)) / (double(T_dst[2].x) - double(T_dst[0].x));
	cout << theta0 << endl;
	double theta;

	for (int i = T_dst[0].y; i <= T_dst[2].y; i++)
	{
		for (int j = T_dst[0].x; j <= T_dst[1].x; j++)
		{
			Mat dst = (Mat_<double>(3, 1) << j, i, 1);
			Mat src = (Mat_<double>(3, 1) << 0, 0, 0);
			theta = (double(i) - double(T_src[0].y)) / (double(j)- double(T_src[0].x));
			if (theta <= theta0)
			{
				src = inv_t1*dst;
			}
			else
			{
				src = inv_t2*dst;
			}

			interpolate(Pdata, Tdata, src, dst);
		}
	}
}

void trans_H (int*** Pdata, int*** Hdata, Point2f H_src[], Point2f H_dst[], Mat h1, Mat h2, int width, int height)
{
	double theta0 = (double(H_src[2].y) - double(H_src[0].y)) / (double(H_src[2].x) - double(H_src[0].x));
	double theta;
	for (int i = 0; i < height / 2; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if ((Pdata[i][j][0] != 255 || Pdata[i][j][1] != 255 || Pdata[i][j][2] != 255))
			{
				Mat src = (Mat_<double>(3, 1) << j, i, 1);
				Mat dst = (Mat_<double>(3, 1) << 0, 0, 0);
				theta = (i - double(H_src[0].y)) / (j - double(H_src[0].x));
				if (theta <= theta0)
					dst = h1*src;
				else
					dst = h2*src; 
				int transfery = floor(dst.at<double>(1, 0));
				int transferx = floor(dst.at<double>(0, 0));

				Hdata[transfery][transferx][0] = Pdata[i][j][0];
				Hdata[transfery][transferx][1] = Pdata[i][j][1];
				Hdata[transfery][transferx][2] = Pdata[i][j][2];
			}
		}
	}
}

Mat findWarpingMatrix(Point2f a1, Point2f b1, Point2f a2, Point2f b2, Point2f a3, Point2f b3)
{
	Point2f srcTri[3];
	Point2f dstTri[3];

	srcTri[0] = a1;
	srcTri[1] = a2;
	srcTri[2] = a3;

	dstTri[0] = b1;
	dstTri[1] = b2;
	dstTri[2] = b3;

	/// Get the Affine Transform
	Mat warp = getAffineTransform(srcTri, dstTri);
	Mat adding = (Mat_<double>(1, 3) << 0, 0, 1);
	warp.push_back(adding);
	return warp;
}

void findHPieces(int*** data, const int width, const int height, Point2f Hpts[])
{
	bool flag = true;
	int a = 0;
	for (int i = 0; i < height/2; i++)
	{
		for (int j = 0; j < width / 2; j++)
		{
			if ((data[i][j][0] != 255 || data[i][j][1] != 255 || data[i][j][2] != 255) && flag)
			{
				Hpts[0] = Point2f(j+a, i+2*a);
				flag = false;
			}
			else if ((data[i][j][0] != 255 || data[i][j][1] != 255 || data[i][j][2] != 255) && !flag)
			{
				Hpts[2] = Point2f(j-a, i-2*a);
			}	
		}
	}

	flag = true;
	for (int j = 0; j < width/2; j++)
	{
		for (int i = 0; i < height / 2; i++)
		{
			if ((data[i][j][0] != 255 || data[i][j][1] != 255 || data[i][j][2] != 255) && flag)
			{
				Hpts[3] = Point2f(j+2*a, i-a);
				flag = false;
			}
			else if ((data[i][j][0] != 255 || data[i][j][1] != 255 || data[i][j][2] != 255) && !flag)
			{
				Hpts[1] = Point2f(j-2*a, i+a);
			}	
		}
	}
}

void findTPieces(int*** data, const int width, const int height, Point2f Tpts[])
{
	bool flag = true;
	for (int j = height / 2; j < height; j++)
	{
		for (int i = width/2; i < width; i++)
		{
			if ((data[j][i][0] != 255 || data[j][i][1] != 255 || data[j][i][2] != 255) && flag)
			{
				Tpts[2] = Point2f(i, j);
				flag = false;
			}
			else if ((data[j][i][0] != 255 || data[j][i][1] != 255 || data[j][i][2] != 255) && !flag)
				Tpts[0] = Point2f(i, j);
		}
	}

	flag = true;
	for (int i = width/2; i < width; i++)
	{
		for (int j = height / 2; j < height; j++)
		{
			if ((data[j][i][0] != 255 || data[j][i][1] != 255 || data[j][i][2] != 255) && flag)
			{
				Tpts[1] = Point2f(i, j);
				flag = false;
			}
			else if ((data[j][i][0] != 255 || data[j][i][1] != 255 || data[j][i][2] != 255) && !flag)
				Tpts[3] = Point2f(i, j);
		}
	}

	//due to false artifacts
	Tpts[0].x = Tpts[0].x - 3;
	Tpts[2].x = Tpts[2].x + 10;
	Tpts[3].y= Tpts[3].y + 1;
}

void findWhiteSquare(int*** data, const int width, const int height, Point2f pts[])
{
	bool flag = true;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (data[i][j][0] == 255 && data[i][j][1] == 255 && data[i][j][2] == 255 && flag)
			{
				pts[0] = Point2f(j, i);
				flag = false;
			}
			else if (data[i][j][0] == 255 && data[i][j][1] == 255 && data[i][j][2] == 255 && !flag)
				pts[2] = Point2f(j, i);
		}
	}
	pts[1] = Point2f(pts[2].x, pts[0].y);
	pts[3] = Point2f(pts[0].x, pts[2].y);
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	const int BytesPerPixel = 3;
	const int pw = 500;
	const int ph = 500;
	const int width = 512;
	const int height = 512;

	// Check for proper syntax
	if (argc < 6) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_people1.raw input_people2.raw input_piece.raw Trump.raw Hillary.raw" << endl;
		return 0;
	}

	//Allocate image data array
	int*** Tdata = new int**[height];
	for (int i = 0; i < height; i++)
	{
		Tdata[i] = new int*[width];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			Tdata[i][j] = new int[BytesPerPixel];
	}

	int*** Hdata = new int**[height];
	for (int i = 0; i < height; i++)
	{
		Hdata[i] = new int*[width];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			Hdata[i][j] = new int[BytesPerPixel];
	}

	int*** Pdata = new int**[ph];
	for (int i = 0; i < ph; i++)
	{
		Pdata[i] = new int*[pw];
	}
	for (int i = 0; i < ph; i++)
	{
		for (int j = 0; j < pw; j++)
			Pdata[i][j] = new int[BytesPerPixel];
	}

	//Allocate buf
	unsigned char *buf1;
	buf1 = new unsigned char[ph * pw * BytesPerPixel];
	if (!(file = fopen(argv[3], "rb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fread(buf1, sizeof(unsigned char), ph*pw*BytesPerPixel, file);
	fclose(file);
	for (int i = 0; i < ph; i++)
	{
		for (int j = 0; j < pw; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
			{
				Pdata[i][j][z] = buf1[i*pw*BytesPerPixel + j*BytesPerPixel + z];
			}
		}
	}

	unsigned char *buf;
	buf = new unsigned char[height * width * BytesPerPixel];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
	std::string arg1(argv[1]);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
			{
				if (arg1 == "Trump.raw")
					Tdata[i][j][z] = buf[i*width*BytesPerPixel + j*BytesPerPixel + z];
				else if (arg1 == "Hillary.raw")
					Hdata[i][j][z] = buf[i*width*BytesPerPixel + j*BytesPerPixel + z];
			}
		}
	}

	if (!(file = fopen(argv[2], "rb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
	std::string arg2(argv[2]);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
			{
				if (arg2 == "Trump.raw")
					Tdata[i][j][z] = buf[i*width*BytesPerPixel + j*BytesPerPixel + z];
				else if (arg2 == "Hillary.raw")
					Hdata[i][j][z] = buf[i*width*BytesPerPixel + j*BytesPerPixel + z];
			}
		}
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	Point2f T_dst[4];
	Point2f H_dst[4];
	Point2f T_src[4];
	Point2f H_src[4];
	findWhiteSquare(Tdata, width, height, T_dst);
	findWhiteSquare(Hdata, width, height, H_dst);
	
	findTPieces(Pdata, pw, ph, T_src);
	findHPieces(Pdata, pw, ph, H_src);

	//For Hilary piece, downscaling, no need for interpolation 
	Mat h1 = findWarpingMatrix(H_src[0], H_dst[0], H_src[1], H_dst[1], H_src[2], H_dst[2]);
	Mat h2 = findWarpingMatrix(H_src[0], H_dst[0], H_src[3], H_dst[3], H_src[2], H_dst[2]);
	trans_H(Pdata, Hdata, H_src, H_dst, h1, h2, pw, ph);
	
	//Trump piece, upscaling, need for interpolation
	Mat t1 = findWarpingMatrix(T_dst[0], T_src[0], T_dst[1], T_src[1], T_dst[2], T_src[2]);
	Mat t2 = findWarpingMatrix(T_dst[0], T_src[0], T_dst[3], T_src[3], T_dst[2], T_src[2]);
	trans_T(Pdata, Tdata, T_src, T_dst, t1, t2, width, height);

	//rewirte Imagedata into buf
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
				buf[i*width*BytesPerPixel + j*BytesPerPixel + z] = Tdata[i][j][z];
		}
	}
	if (!(file = fopen(argv[4], "wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
	

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
				buf[i*width*BytesPerPixel + j*BytesPerPixel + z] = Hdata[i][j][z];
		}
	}
	if (!(file = fopen(argv[5], "wb"))) {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
