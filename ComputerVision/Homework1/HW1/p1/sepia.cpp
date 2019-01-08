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
	int width = 1914;
	int height = 808;
	
	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw intermeidate_image.raw result_image.raw [BytesPerPixel = 1][Width = 512][Height = 384]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 5){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[4]);
		// Check if size is specified
		if (argc >= 7){
			width = atoi(argv[5]);
			height = atoi(argv[6]);
		}
	}

	int*** Imagedata = new int**[height];
	for(int i = 0; i < height; i++)
		Imagedata[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			Imagedata[i][j] = new int[BytesPerPixel];
	}

	int** interData = new int*[height];
	for(int i = 0; i < height; i++)
		interData[i] = new int[width];
	
	// Allocate image data array
	unsigned char buf3[height][width][BytesPerPixel];
	unsigned char buf2[height][width];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(buf3, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				Imagedata[i][j][z] = buf3[i][j][z];
		}
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	for(int y = 0; y<height; y++)
	{
		for(int x = 0; x<width; x++)
		{
			interData[y][x] = floor(0.21*double(Imagedata[y][x][0]) + 0.72*double(Imagedata[y][x][1]) + 0.07*double(Imagedata[y][x][2]));
			if(interData[y][x]>255) interData[y][x]=255;
		}
	}

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			buf2[i][j] = interData[i][j];
	}

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(buf2, sizeof(unsigned char), height*width, file);
	fclose(file);


	for(int y = 0; y<height; y++)
	{
		for(int x = 0; x<width; x++)
		{
			Imagedata[y][x][0] = floor((0.393 + 0.769 + 0.189)*double(interData[y][x]));
			Imagedata[y][x][1] = floor((0.349 + 0.686 + 0.168)*double(interData[y][x]));
			Imagedata[y][x][2] = floor((0.272 + 0.534 + 0.131)*double(interData[y][x]));

			if(Imagedata[y][x][0]>255) Imagedata[y][x][0]=255; 
			if(Imagedata[y][x][1]>255) Imagedata[y][x][1]=255; 
			if(Imagedata[y][x][2]>255) Imagedata[y][x][2]=255; 
		}	     
	}

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int z = 0; z < BytesPerPixel; z++)
				buf3[i][j][z] = Imagedata[i][j][z];
		}
	}

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}

	fwrite(buf3, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
