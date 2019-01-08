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
#include <algorithm> 

using namespace std;

void zeroPadding(int*** Imagedata, int*** Originaldata, int paddedHeight, int paddedWidth, int BytesPerPixel, int zeros)
{
	for(int i = 0; i < paddedHeight; i++)
	{
		for(int j = 0; j < paddedWidth; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
			{
				if((i<zeros)|| (i>=paddedHeight-zeros)||(j<zeros)|| (j>=paddedWidth-zeros))
					Originaldata[i][j][z] = 0;
				else 
					Originaldata[i][j][z] = Imagedata[i-zeros][j-zeros][z];
			}
		}
	}
}

map<int, double> collectPixelNum(int** diffData, int height, int width)
{
	map<int, double> original;
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
		{
			if(original.find(diffData[y][x])==original.end())
				original[diffData[y][x]] = 1;
			else 
				original.find(diffData[y][x])->second ++;
		}
	}

	return original;
}

void histogramAnalysis(int*** Imagedata, int*** noiseFreeData, int height, int width, int channel)
{
	int** diffData = new int*[height];
	for(int i = 0; i < height; i++)
		diffData[i] = new int[width];
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			diffData[i][j] = Imagedata[i][j][channel] - noiseFreeData[i][j][channel];
	}

	//count the total number of pixel associated with each pixel intensity value
	map<int, double> original = collectPixelNum(diffData, height, width);
	// map<int, double>::iterator it;
	// for(it=original.begin(); it!=original.end(); it++)
	// 	cout<<it->first<<"=="<<it->second<<endl;
}

double calculatePSNR(int*** Imagedata, int*** noiseFreeData, int height, int width, int BytesPerPixel)
{
	cout<<"PSNR"<<endl;
	double MSE = 0;
	for (int i=0; i<BytesPerPixel; i++)
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
				MSE = MSE + pow((Imagedata[h][w][i]-noiseFreeData[h][w][i]),2);
		}
	}

	MSE = MSE/(height*width);
	double PSNR = 10*log10(pow(255,2)/MSE);

	return PSNR;
}

void medianFilter(int*** Imagedata, int*** Originaldata, int paddedHeight, int paddedWidth, int BytesPerPixel, int zeros)
{
	zeroPadding(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
	
	vector<int> temp;
	for (int i=0; i<BytesPerPixel; i++)
	{
		for (int h = zeros; h < paddedHeight-zeros; h++)
		{
			for (int w = zeros; w < paddedWidth-zeros; w++)
			{
				for(int h1 = -zeros; h1 < zeros + 1; h1++)
				{
					for(int w1 = -zeros; w1 < zeros + 1; w1++)
						temp.push_back(Originaldata[h+h1][w+w1][i]);
				}
				sort (temp.begin(), temp.end()); 
				// for(int a=0; a<9; a++)
				// 	cout<<temp[a]<<endl;
				// cout<<"======================="<<endl;
				Imagedata[h-zeros][w-zeros][i] = temp[(pow((zeros*2+1),2)-1)/2];
				//cout<<Imagedata[h+1][w+1][i]<<" at position "<<h+1<<", "<<w+1<<", "<<i<<endl;
				temp.clear();
			}
		}
	}
}

void guassianFilter(int*** Imagedata, int*** Originaldata, int paddedHeight, int paddedWidth, int BytesPerPixel, int zeros, int sigma)
{
	zeroPadding(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);

	//construct a new filter
	double** filter = new double*[zeros*2+1];
	for(int i = 0; i < zeros*2+1; i++)
		filter[i] = new double[zeros*2+1];

	double pi = atan(1)*4;
	double weightSum = 0;
	for(int i = 0; i < zeros*2+1; i++)
	{
		for(int j = 0; j < zeros*2+1; j++)
		{
			double term1 = (1/(2*pi*pow(sigma,2)));
			double term2 = exp(-(pow(i-zeros,2)+pow(j-zeros,2))/(2*pow(sigma,2)));
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

	for (int i=0; i<BytesPerPixel; i++)
	{
		for (int h = zeros; h < paddedHeight-zeros; h++)
		{
			for (int w = zeros; w < paddedWidth-zeros; w++)
			{
				double sum = 0;
				for(int h1 = -zeros; h1 < zeros+1; h1++)
				{
					for(int w1 = -zeros; w1 < zeros+1; w1++)
						sum = sum + filter[zeros+h1][zeros+w1]*Originaldata[h+h1][w+w1][i];
				}
				Imagedata[h-zeros][w-zeros][i] = floor(sum);
			}
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
	double sigma = 1;
	string method = "A";

	// Check for proper syntax
	if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name noise_image.raw noise_free.raw filtered_image.raw method [filter_size][sigma][BytesPerPixel = 1][Width = 940][Height = 400]" << endl;
		return 0;
	}

	method = argv[4];
	filter_size = atoi(argv[5]);
	sigma = atoi(argv[6]);

	if (argc < 8){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[7]);
		// Check if size is specified
		if (argc >= 9){
			width = atoi(argv[8]);
			height = atoi(argv[9]);
		}
	}

	// Allocate image data array
    //store enhanced data
    int zeros = (filter_size-1)/2;
    int paddedWidth = width + zeros*2; 
    int paddedHeight = height + zeros*2;
	

	//memory allocation
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
		noiseFreeData[i] = new int*[widt];
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


	//read the noise image
	unsigned char buf[height][width][BytesPerPixel];
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

	//read the noise free image 
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
	for(int i=0; i<BytesPerPixel; i++)
		histogramAnalysis(Imagedata, noiseFreeData, height, width, i);
		
    if(method == "A")
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma); 
	else if(method == "B")
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
	else if(method == "C")
	{
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
	}
	else if(method == "D")
	{
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
	}
	else if(method == "E")
	{
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
	}
	else if(method == "F")
	{
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
	}
	else if(method == "G")
	{
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
		medianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros);
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
		guassianFilter(Imagedata, Originaldata, paddedHeight, paddedWidth, BytesPerPixel, zeros, sigma);
	}
	else if(method == "N")
		cout<<"No filter!"<<endl;
	else
	{
		cout<< "Invalid method "<<argv[3]<<endl;
		exit(1);
	}

	double PSNR = calculatePSNR(Imagedata, noiseFreeData, height, width, BytesPerPixel);
	cout<<"The average PSNR is "<<PSNR<<endl;


	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				buf[i][j][z] = Imagedata[i][j][z];
		} 
	}

	if (!(file=fopen(argv[3],"wb"))) 
	{
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}