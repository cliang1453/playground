// Name: Chen Liang
// USC ID Number: 7314634396
// USC Email: lian455@usc.edu
// Submission Date: 02/05/2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int height = 900; 
	int width = 890;
	
	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name top_layer.raw bottom_layere.raw output_image.raw [BytesPerPixel = 1][width = 890][height = 900]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 5){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[4]);
		if(argc <= 7)
		{
			width = atoi(argv[5]); 
			height = atoi(argv[6]);
		}
	}

	//Dynamic Allocation 
	int*** top = new int**[height];
	for(int i = 0; i < height; i++)
		top[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			top[i][j] = new int[BytesPerPixel];
	}

	int*** bottom = new int**[height];
	for(int i = 0; i < height; i++)
		bottom[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			bottom[i][j] = new int[BytesPerPixel];
	}

	int*** result = new int**[height];
	for(int i = 0; i < height; i++)
		result[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			result[i][j] = new int[BytesPerPixel];
	}
	
	// Allocate image data array
	unsigned char buf[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				top[i][j][z] = buf[i][j][z];
		}
	}

	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				bottom[i][j][z] = buf[i][j][z];
		}
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				result[y][x][z] = floor((1 - (1-double(bottom[y][x][z])/255.0)*(1-double(top[y][x][z])/255.0))*255);
		}
	}

	// Write image data (filename specified by second argument) from image data matrix
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				buf[i][j][z] = result[i][j][z];
		}
	}

	if (!(file = fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}

	fwrite(buf, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
