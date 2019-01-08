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

double calculatePSNR(int*** Imagedata, int*** noiseFreeData, int height, int width, int BytesPerPixel)
{
	double MSE = 0;
	for (int i=0; i<BytesPerPixel; i++)
	{
		for (int h = 0; h <height; h++)
		{
			for (int w = 0; w < width; w++)
				MSE = MSE + pow((Imagedata[h][w][i]-noiseFreeData[h][w][i]),2);
		}
	}

	MSE = MSE/(height*width*3);
	double PSNR = 10*log10(pow(255,2)/MSE);

	return PSNR;
}

void constructFilter(double** filter, double a, int zeros)
{
	double pi = atan(1)*4;
	double weightSum = 0;
	for(int i = 0; i < zeros*2+1; i++)
	{
		for(int j = 0; j < zeros*2+1; j++)
		{
			double term1 = (1/(2*pi*pow(a,2)));
			double term2 = exp(-(pow(i-zeros,2)+pow(j-zeros,2))/(2*pow(a,2)));
			filter[i][j] = term1*term2;
			weightSum = weightSum + term1*term2;
		}
	}

	//normalize the new filter
	for(int i = 0; i < zeros*2+1; i++)
	{
		for(int j = 0; j < zeros*2+1; j++)
			filter[i][j] = filter[i][j]/weightSum;
	}
}

void calculateWeightMap(int*** Originaldata, int i, int j, double** weightMap, int height, int width, double** filter, double h, int search_size, int zeros, int channel)
{
	int rad = (search_size-1)/2;
	double weightSum = 0; 

	for(int y= i-rad; y<i+rad+1; y++)
	{
		for(int x=j-rad; x<j+rad+1; x++)
		{
			if((x<0 || x>=height)||(y<0 || y>=width))
				weightMap[y-(i-rad)][x-(j-rad)] = 0;
			else 
			{
				double blockSquaredSum = 0; 
				for(int p = -zeros; p < zeros + 1; p++)
				{
					for (int q = -zeros; q < zeros + 1; q++)
					{
						double temp = filter[zeros+p][zeros+q] * pow((Originaldata[y+p+zeros][x+q+zeros][channel]-Originaldata[i+p+zeros][j+q+zeros][channel]),2);
						blockSquaredSum = blockSquaredSum + temp;
					}
				}
				weightMap[y-(i-rad)][x-(j-rad)] = exp(-blockSquaredSum/pow(h, 2));
				weightSum = weightSum + weightMap[y-(i-rad)][x-(j-rad)];
			}
		}
	}

	for(int y=0; y < search_size; y++)
	{
		for(int x=0; x < search_size; x++)
			weightMap[y][x] = weightMap[y][x]/weightSum;
	}

}

//we choose the search area as the patch -6 - 6
void NLM(int*** Imagedata, int*** Originaldata, int height, int width, double** filter, double h, int search_size, int zeros, int channel)
{
	double** weightMap = new double*[search_size];
	for(int i = 0; i < search_size; i++)
		weightMap[i] = new double[search_size];
	int rad = (search_size-1)/2;


	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			//cout<<Originaldata[i][j][channel]<<endl;
			calculateWeightMap(Originaldata, i, j, weightMap, height, width, filter, h, search_size, zeros, channel);
			//cout<<"============Calculate Weight Map Finished============="<<endl;
			double sum = 0;
			for(int y=0; y<search_size; y++)
			{
				for(int x=0; x<search_size; x++)
				{
					if(weightMap[y][x]!=0)
						sum = sum + weightMap[y][x]*Originaldata[y-rad+i+zeros][x-rad+j+zeros][channel];
				}
			}
			//cout<<i<<" "<<j<<" "<<channel<<" "<<floor(sum)<<endl;
			Imagedata[i][j][channel] = floor(sum);
		}
	}
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width = 512;
	int height = 512;
	int filter_size = 3;
	int search_size = 13;
	double h = 40;
	double a = 1;

	// Check for proper syntax
	if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name noise_image.raw noise_free.raw filtered_image.raw [filter_size][search_size][filter_degree(h)][Gaussian_std(a)][BytesPerPixel = 1][Width = 512][Height = 512]" << endl;
		return 0;
	}

	filter_size = atoi(argv[4]);
	search_size = atoi(argv[5]);
	h = atoi(argv[6]);
	a = atoi(argv[7]);

	if (argc < 9){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[8]);
		// Check if size is specified
		if (argc >= 11){
			width = atoi(argv[9]);
			height = atoi(argv[10]);
		}
	}

	// Allocate image data array
    //store enhanced data
    int zeros = (filter_size-1)/2;
    int paddedWidth = width + zeros*2; 
    int paddedHeight = height + zeros*2;

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

	int*** noiseFreeData = new int**[height];
	for(int i = 0; i < height; i++)
		noiseFreeData[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			noiseFreeData[i][j] = new int[BytesPerPixel];
	}

	int*** Originaldata = new int**[paddedHeight];
	for(int i = 0; i < paddedHeight; i++)
		Originaldata[i] = new int*[paddedWidth];
	for(int i = 0; i < paddedHeight; i++)
	{
		for(int j = 0; j < paddedWidth; j++)
			Originaldata[i][j] = new int[BytesPerPixel];
	}

	unsigned char buf[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	//padded matrix -- noise data
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	for(int i = 0; i < paddedHeight; i++)
	{
		for(int j = 0; j < paddedWidth; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
			{
				if((i<zeros)|| (i>=paddedHeight-zeros)||(j<zeros)|| (j>=paddedWidth-zeros))
					Originaldata[i][j][z] = 0;
				else 
					Originaldata[i][j][z] = buf[i-zeros][j-zeros][z];
			}
		}
	}
	fclose(file);


	//noise free matrix
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				noiseFreeData[i][j][z] = buf[i][j][z];
		}
	}
	fclose(file);


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	//construct a new filter
	double** filter = new double*[zeros*2+1];
	for(int i = 0; i < zeros*2+1; i++)
		filter[i] = new double[zeros*2+1];
	constructFilter(filter, a, zeros);

    for(int i=0; i<BytesPerPixel; i++)
		NLM(Imagedata, Originaldata, height, width, filter, h, search_size, zeros, i);	

	double PSNR = calculatePSNR(Imagedata, noiseFreeData, height, width, BytesPerPixel);
	cout<<"The average PSNR is "<<PSNR<<endl;

	if (!(file=fopen(argv[3],"wb"))) 
	{
		cout << "Cannot open file: " << argv[3] << endl;
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
