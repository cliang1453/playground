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


void mapText(int*** Tdata, int*** Fdata, Mat inv_H, int tw, int th, string arg2) 
{
	if (arg2 != "trojans.raw" && arg2 != "tartans.raw")
	{
		cout << "You entered image that is not in our options." << endl;
		return;
	}

	Mat src = (Mat_<float>(3, 1) << 0, 0, 1); 
	Mat dst = (Mat_<float>(3, 1) << 0, 0, 1);

	for (int i = 427; i < 613; i++)
	{
		for (int j = 318; j < 654; j++)
		{
			dst = (Mat_<float>(3, 1) << j, i, 1);
			src = inv_H * dst;
			float src_y = src.at<float>(1, 0)/ src.at<float>(2, 0);
			float src_x = src.at<float>(0, 0)/ src.at<float>(2, 0);
			
			if (src_y >= th || src_y < 0 || src_x >= tw || src_x < 0) 
				continue;
			else
			{
				int i = floor(src_y);
				int j = floor(src_x);
				double a = src_y - i;
				double b = src_x - j;
				int dst_y = dst.at<float>(1, 0);
				int dst_x = dst.at<float>(0, 0);
				
				if (((i + 1 < th) && (j + 1 < tw)) && ((i >= 0) && (j >= 0)))
				{
					float temp[3];
					for (int k = 0; k < 3; k++)
					{
						temp[k] = (1 - a)*(1 - b)*Tdata[i][j][k] + a*(1 - b)*Tdata[i + 1][j][k]
							+ a*b*Tdata[i + 1][j + 1][k] + (1 - a)*b*Tdata[i][j + 1][k];
					}

					if (arg2 == "tartans.raw")
					{
						if (!(temp[0] <= 15 && temp[1] <= 15 && temp[2] <= 15))
						{
							for (int k = 0; k < 3; k++)
								Fdata[dst_y][dst_x][k] = temp[k];
						}
					}
					else
					{
						if (!(temp[0] >=245 && temp[1] >=245 && temp[2] >=245))
						{
							for (int k = 0; k < 3; k++)
								Fdata[dst_y][dst_x][k] = temp[k];
						}
					}
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	
	FILE *file;
	const int BytesPerPixel = 3;
	const int fw = 972;
	const int fh = 648;
	int tw = 350;
	int th = 146;

	if (argc < 4) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name field.raw text.raw output.raw [text_width = 350] {text_height = 146]" << endl;
		return 0;
	}
	if (argc >= 6)
	{
		tw = atoi(argv[4]);
		th = atoi(argv[5]);
	}

	int*** Tdata = new int**[th];
	for (int i = 0; i < th; i++)
	{
		Tdata[i] = new int*[tw];
		for (int j = 0; j < tw; j++)
			Tdata[i][j] = new int[BytesPerPixel];
	}

	int*** Fdata = new int**[fh];
	for (int i = 0; i < fh; i++)
	{
		Fdata[i] = new int*[fw];
		for (int j = 0; j < fw; j++)
			Fdata[i][j] = new int[BytesPerPixel];
	}

	unsigned char *buf1;
	buf1 = new unsigned char[fh * fw * BytesPerPixel];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf1, sizeof(unsigned char), fh*fw*BytesPerPixel, file);
	fclose(file);
	for (int i = 0; i < fh; i++)
	{
		for (int j = 0; j < fw; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
			{
				Fdata[i][j][z] = buf1[i*fw*BytesPerPixel + j*BytesPerPixel + z];
			}
		}
	}

	string arg2(argv[2]);
	unsigned char *buf2;
	buf2 = new unsigned char[th * tw * BytesPerPixel];
	if (!(file = fopen(argv[2], "rb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fread(buf2, sizeof(unsigned char), th*tw*BytesPerPixel, file);
	fclose(file);
	for (int i = 0; i < th; i++)
	{
		for (int j = 0; j < tw; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
			{
				Tdata[i][j][z] = buf2[i*tw*BytesPerPixel + j*BytesPerPixel + z];
			}
		}
	}

	//calculate homography
	//the 8 selected points (4 start 4 ends)
	Point2f select1[4];
	select1[0] = Point2f(0, 0);
	select1[1] = Point2f(tw, 0);
	select1[2] = Point2f(tw, th);
	select1[3] = Point2f(0, th);

	Point2f select2[4];
	select2[0] = Point2f(318, 596);
	select2[1] = Point2f(540, 427);
	select2[2] = Point2f(653, 430);
	select2[3] = Point2f(529, 612);
	
	Point2f a1, a2, a3, a4, b1, b2, b3, b4;
	a1 = select1[0]; a2 = select1[1]; a3 = select1[2]; a4 = select1[3];
	b1 = select2[0]; b2 = select2[1]; b3 = select2[2]; b4 = select2[3];

	Mat P = (Mat_<float>(8, 8) <<
		a1.x, a1.y, 1, 0, 0, 0, -a1.x*b1.x, -a1.y*b1.x,
		0, 0, 0, a1.x, a1.y, 1, -a1.x*b1.y, -a1.y*b1.y,
		a2.x, a2.y, 1, 0, 0, 0, -a2.x*b2.x, -a2.y*b2.x,
		0, 0, 0, a2.x, a2.y, 1, -a2.x*b2.y, -a2.y*b2.y,
		a3.x, a3.y, 1, 0, 0, 0, -a3.x*b3.x, -a3.y*b3.x,
		0, 0, 0, a3.x, a3.y, 1, -a3.x*b3.y, -a3.y*b3.y,
		a4.x, a4.y, 1, 0, 0, 0, -a4.x*b4.x, -a4.y*b4.x,
		0, 0, 0, a4.x, a4.y, 1, -a4.x*b4.y, -a4.y*b4.y);
	
	Mat Pprime = (Mat_<float>(8, 1) <<
		b1.x, b1.y, b2.x, b2.y, b3.x, b3.y, b4.x, b4.y);
		
	//H' column vector
	Mat Hprime = P.inv()*Pprime;

	//rearrange the H' into 3*3 H Mat
	Mat H = (Mat_<float>(3, 3) <<
		Hprime.at<float>(0, 0), Hprime.at<float>(1, 0), Hprime.at<float>(2, 0),
		Hprime.at<float>(3, 0), Hprime.at<float>(4, 0), Hprime.at<float>(5, 0),
		Hprime.at<float>(6, 0), Hprime.at<float>(7, 0), 1 );

	Mat inv_H = H.inv();
	
	mapText(Tdata, Fdata, inv_H, tw, th, arg2);


	for (int i = 0; i < fh; i++)
	{
		for (int j = 0; j < fw; j++)
		{
			for (int z = 0; z < BytesPerPixel; z++)
				buf1[i*fw*BytesPerPixel + j*BytesPerPixel + z] = Fdata[i][j][z];
		}
	}
	if (!(file = fopen(argv[3], "wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(buf1, sizeof(unsigned char), fh*fw*BytesPerPixel, file);
	fclose(file);

	return 0;
}


