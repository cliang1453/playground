// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <utility>

using namespace std;
const int deltaj[4] = {1, -1, 0, 0};
const int deltai[4] = {0, 0, 1, -1};

void bfsBlack(int** data, int** mask, int height, int width, queue<pair<int, int> > &queue)
{
	while(!queue.empty())
	{
		pair<int, int> temp = queue.front();
		queue.pop();
		int i = temp.first;
		int j = temp.second; 

		for(int a=0; a<4; a++)
		{
				if(i + deltai[a] < height && j + deltaj[a] < width && i + deltai[a] >= 0 && j + deltaj[a] >= 0)
				{
					if(mask[i + deltai[a]][j + deltaj[a]] == 0 && data[i + deltai[a]][j + deltaj[a]] == 0)
					{
						mask[i + deltai[a]][j + deltaj[a]] = 1;
						queue.push(make_pair(i + deltai[a], j + deltaj[a]));
					}
				}  	
		}
	}
}

void labelBlack(int** data, int** mask, int height, int width)
{
	queue<pair<int, int> > queue;
	queue.push(make_pair(0, 0));
	mask[0][0] =1;
	bfsBlack(data, mask, height, width, queue);
}

void bfsWhite(int** mask, int& i, int& j, int& area, int& xmin, int& xmax, 
	int& ymin, int& ymax, queue<pair<int, int> > &queue, int height, int width)
{
	
	while(!queue.empty())
	{
		pair<int, int> temp = queue.front();
		queue.pop();
		area++;
		int i = temp.first;
		int j = temp.second; 

		for(int a=0; a<4; a++)
		{
				if(i + deltai[a] < height && j + deltaj[a] < width && i + deltai[a] >= 0 && j + deltaj[a] >= 0)
				{
					if(mask[i + deltai[a]][j + deltaj[a]] == 0)
					{
						mask[i + deltai[a]][j + deltaj[a]] = 1;
						xmin = min(xmin, j + deltaj[a]); 
						xmax = max(xmax, j + deltaj[a]);
						ymin = min(ymin, i + deltai[a]); 
						ymax = max(ymax, i + deltai[a]);
						queue.push(make_pair(i + deltai[a], j + deltaj[a]));
					}
				}  	
		}
	}
}

void labelWhite(int** mask, int height, int width, int& numSquare, int& numCircle)
{
	numSquare = 0; 
	numCircle = 0; 
	int xmin, xmax, ymin, ymax; 
	queue<pair<int, int> > queue;
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			if(mask[i][j] == 0)
			{
				int area = 0;
				xmin = j; xmax = j; ymin = i; ymax = i;
				queue.push(make_pair(i, j));
				mask[i][j] =1;
				bfsWhite(mask, i, j, area, xmin, xmax, ymin, ymax, queue, height, width);

				//check sqaure or triangle
				int width = xmax - xmin + 1; 
				int height = ymax - ymin + 1;
				double squareArea = width * height;
				if(area < squareArea + 5 && area > squareArea - 5)
					numSquare++;
				else
					numCircle++;
			}	
		}
	}
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	const int width = 480;
	const int height = 480;
	string method;
	int numSquare; 
	int numCircle;

	// Check for proper syntax
	if (argc < 2){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input.raw"<<endl;		
		return 0; 
	}

	int** data = new int*[height];
	for (int i = 0; i < height; i++)
	{
		data[i] = new int[width];
	}

	int** mask = new int*[height];
	for (int i = 0; i < height; i++)
	{
		mask[i] = new int[width];
	}

	unsigned char *buf1;
	buf1 = new unsigned char[height* width];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf1, sizeof(unsigned char), height*width, file);
	fclose(file);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			data[i][j] = buf1[i*width + j]/255;
			mask[i][j] = 0;
		}	
	}
	//Implemetation 
	//label the background black area to be 1 and white area to be 0
	labelBlack(data, mask, height, width);

	//go through the mask, everytime encountered a pixel that is labeled as 0, 
	//go through each pixel that labeled as 0 and update the value of xmin, xmax, ymin, ymax
	//check the ratio of the area and the radius
	labelWhite(mask, height, width, numSquare, numCircle);
	cout<<"The number of Sqaure is "<<numSquare<<endl;
	cout<<"The number of Circle is "<<numCircle<<endl;
	cout<<"The total number of object is "<<numSquare + numCircle<<endl;

	return 0; 
}