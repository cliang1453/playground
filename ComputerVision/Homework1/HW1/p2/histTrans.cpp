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

using namespace std;
double calculate(int index, double sigma, double mu)
{
	double pi = atan(1)*4;
	double constant  = 1/sqrt(2*pi*pow(sigma,2));
	double expTerm = exp(-pow(index-mu,2)/(2*pow(sigma,2)));
	return (constant*expTerm);
}

int findCorrespondingIntensity(double cumProb, double normalizedCum, double sigma, double mu)
{
	int intensity;
	double term;
	double sum = 0;  
	for(int i=0; i<256; i++)
	{
		term = calculate(i, sigma, mu);
		sum = sum + term;
		if( sum/normalizedCum > cumProb ) 
		{
			intensity = i;
			break;
		}
	}
	return intensity;
}

map<int, double> collectPixelNum(int*** Imagedata, int height, int width, int channel)
{
	map<int, double> original;
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
		{
			if(original.find(Imagedata[y][x][channel])==original.end())
				original[Imagedata[y][x][channel]] = 1;
			else 
				original.find(Imagedata[y][x][channel])->second ++;
		}
	}
	return original;
}

void histogramEqualization(int*** Imagedata, int height, int width, int channel)
{
	//count the total number of pixel associated with each pixel intensity value
	map<int, double> original = collectPixelNum(Imagedata, height, width, channel);
	map<int, double>::iterator it;

	//mapping 
	map<int, double> equalized;
	double prob = 0 ; 
	double cumProb = 0;
	int corresponding = 0;
	int result = 0;

	//deal with mapping target
	double normalizedCum = 0;
	double term;
	for(int i=0; i<256; i++)
	{
		term = calculate(i, 20.0, 70.0);
		normalizedCum = normalizedCum + term;
	}
	
	for(it=original.begin(); it!=original.end(); it++)
	{
		prob = (it->second)/double(width*height);
		cumProb = cumProb + prob;
		corresponding = findCorrespondingIntensity(cumProb, normalizedCum, 20.0, 70.0);
		equalized.insert(make_pair(it->first, corresponding));
	}

	//construct the Imagedata
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
			Imagedata[y][x][channel] = equalized[Imagedata[y][x][channel]];	
	}
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width = 960;
	int height = 600;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw result_image.raw [BytesPerPixel = 1][Width = 940][Height = 400]" << endl;
		return 0;
	}

	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 6){
			width = atoi(argv[4]);
			height = atoi(argv[5]);
		}
	}

	// Allocate image data array
    //store enhanced data
	int*** Imagedata = new int**[height];
	for(int i = 0; i < height; i++)
		Imagedata[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			Imagedata[i][j] = new int[BytesPerPixel];
	}
	
	unsigned char buf[height][width][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				Imagedata[i][j][z] = buf[i][j][z];
		}
	}
	fclose(file);
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
    for(int i=0; i<BytesPerPixel; i++)
		histogramEqualization(Imagedata, height, width, i);	

	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				buf[i][j][z] = Imagedata[i][j][z];
		} 
	}

	fwrite(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}