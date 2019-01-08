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
	int Size = 300;
	
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw 200*200_resized_image.raw 400*400_resized_image.raw 600*600_resized_image.raw [BytesPerPixel = 1] [Size = 300]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 6){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[5]);
		// Check if size is specified
		if (argc >= 7){
			Size = atoi(argv[6]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	unsigned char Outputdata200[200][200][BytesPerPixel];
	double scale = 200/double(Size);
	for(int y = 0; y < 200; y++)
	{
		for (int x = 0; x < 200; x++)
		{
			int i = floor(y/scale);
			int j = floor(x/scale);
			double a = y/scale - i;
			double b = x/scale - j;

			if(((i+1<Size) && (j+1<Size)) &&((i>=0) && (j>=0)))
			{
				for(int z = 0; z<BytesPerPixel; z++)
					Outputdata200[y][x][z] = (1-a)*(1-b)*Imagedata[i][j][z] + a*(1-b)*Imagedata[i+1][j][z] + a*b*Imagedata[i+1][j+1][z] + (1-a)*b*Imagedata[i][j+1][z];
			}
		}
	}


	unsigned char Outputdata400[400][400][BytesPerPixel];
	scale = 400/double(Size);
	for(int y = 0; y < 400; y++)
	{
		for (int x = 0; x < 400; x++)
		{
			int i = floor(y/scale);
			int j = floor(x/scale);
			double a = y/scale - i;
			double b = x/scale - j;

			if(((i+1<Size) && (j+1<Size)) &&((i>=0) && (j>=0)))
			{
				for(int z = 0; z<BytesPerPixel; z++)
					Outputdata400[y][x][z] = (1-a)*(1-b)*Imagedata[i][j][z] + a*(1-b)*Imagedata[i+1][j][z] + a*b*Imagedata[i+1][j+1][z] + (1-a)*b*Imagedata[i][j+1][z];
			}
		}
	}


	unsigned char Outputdata600[600][600][BytesPerPixel];
	scale = 600/double(Size);
	for(int y = 0; y < 600; y++)
	{
		for (int x = 0; x < 600; x++)
		{
			int i = floor(y/scale);
			int j = floor(x/scale);
			double a = y/scale - i;
			double b = x/scale - j;

			if(((i+1<Size) && (j+1<Size)) &&((i>=0) && (j>=0)))
			{
				for(int z = 0; z<BytesPerPixel; z++)
					Outputdata600[y][x][z] = (1-a)*(1-b)*Imagedata[i][j][z] + a*(1-b)*Imagedata[i+1][j][z] + a*b*Imagedata[i+1][j+1][z] + (1-a)*b*Imagedata[i][j+1][z];
			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Outputdata200, sizeof(unsigned char), 200*200*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Outputdata400, sizeof(unsigned char), 400*400*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Outputdata600, sizeof(unsigned char), 600*600*BytesPerPixel, file);
	fclose(file);

	return 0;
}
