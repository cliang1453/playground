// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

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
	int width = 512;
	int height = 384;
	
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw c_space.raw m_space.raw y_space.raw [BytesPerPixel = 1][Width = 512][Height = 384]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 6){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[5]);
		// Check if size is specified
		if (argc >= 8){
			width = atoi(argv[6]);
			height = atoi(argv[7]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[height][width][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	unsigned char cSpace[height][width];
	for(int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			cSpace[y][x] = 255 - Imagedata[y][x][0];	
	}

	unsigned char mSpace[height][width];
	for(int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			mSpace[y][x] = 255 - Imagedata[y][x][1];	
	}

	unsigned char ySpace[height][width];
	for(int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			ySpace[y][x] = 255 - Imagedata[y][x][2];
	}
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(cSpace, sizeof(unsigned char), height*width, file);
	fclose(file);

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(mSpace, sizeof(unsigned char), height*width, file);
	fclose(file);

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(ySpace, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
