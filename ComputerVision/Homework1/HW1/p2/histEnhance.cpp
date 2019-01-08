// Name: Chen Liang
// USC ID Number: 7314634396
// USC Email: lian455@usc.edu
// Submission Date: 02/05/2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <vector>

using namespace std;

//count histogram 
int findMax(int **Imagedata, int height, int width)
{
	int max = 0;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(Imagedata[y][x]>max && Imagedata[y][x]<=255) 
				max = Imagedata[y][x];
		}
	}

	return max; 
}

int findMin(int **Imagedata, int height, int width)
{
	int min = 255;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(Imagedata[y][x] < min)
				min = Imagedata[y][x];
		}
	}

	return min; 
}

void findGapValue(map<int, double> original,int& max, int& min)
{
	map<int, double>::iterator it;
	int temp;
	for(it=original.begin(); it!=original.end(); it++)
	{
		if(it->first - temp > 100)
		{
			max = it->first; 
			min = temp;
		}
		temp = it->first;
	}	
}

map<int, double> collectPixelNum(int** Imagedata, int height, int width)
{
	map<int, double> original;
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
		{
			if(original.find(Imagedata[y][x])==original.end())
				original[Imagedata[y][x]] = 1;
			else 
				original.find(Imagedata[y][x])->second ++;
		}
	}

	return original;
}


void linearStretching(int **Imagedata, int height, int width)
{
	int min = findMin(Imagedata, height, width); 
	int max = findMax(Imagedata, height, width);

	if(min==0 && max == 255)
	{
		map<int, double> original = collectPixelNum(Imagedata, height, width);
		findGapValue(original, max, min);

		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x <width; x++)
			{
				double temp;

				if(Imagedata[y][x]<=min)
					temp = (double(Imagedata[y][x]-0)/(min-0))*127.5;
				else
					temp = (double(Imagedata[y][x]-max)/(255-max))*125.0 + 127.5;

				Imagedata[y][x] = floor(temp);
			}
		}
	}
	else 
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x <width; x++)
			{
				double temp = (double(Imagedata[y][x]-min)/(max-min))*255.0;
				Imagedata[y][x] = floor(temp);
			}
		}
	}

	
}

void histogramEqualization(int** Imagedata, int height, int width)
{
	//count the total number of pixel associated with each pixel intensity value
	map<int, double> original = collectPixelNum(Imagedata, height, width);
	map<int, double>::iterator it;

	//mapping 
	map<int, double> equalized;
	double prob = 0 ; 
	double cumProb = 0;
	int result = 0;

	for(it=original.begin(); it!=original.end(); it++)
	{
		prob = (it->second)/double(width*height);
		cumProb = cumProb + prob;
		result = floor(cumProb*255);
		equalized.insert(make_pair(it->first, result));
	}

	//construct the Imagedata
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
			Imagedata[y][x] = equalized[Imagedata[y][x]];	
	}
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width = 400;
	int height = 366;
	string method = "A";

	
	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw result_image.raw method_name [Width = 400][Height = 366]" << endl;
		return 0;
	}

	method = argv[3];
	
	if (argc > 5){
		width = atoi(argv[4]);
		height = atoi(argv[5]);
	}

	// Allocate image data array
	int** Imagedata = new int*[height];
	for(int i = 0; i < height; i++)
		Imagedata[i] = new int[width];

	unsigned char buf[height][width];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	
	fread(buf, sizeof(unsigned char), height*width, file);
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			Imagedata[i][j] = buf[i][j];
	}

	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	if(method == "A")
	{
		linearStretching(Imagedata, height, width); 
	}
	else if(method == "B")
		histogramEqualization(Imagedata, height, width);
	else 
	{
		cout<< "Invalid method "<<argv[3]<<endl;
		exit(1);
	}
		
	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}


	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			buf[i][j] = Imagedata[i][j]; 
	}

	fwrite(buf, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}
