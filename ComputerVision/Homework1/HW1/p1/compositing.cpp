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
	int dogSize = 300;
	int beachWidth = 1914; 
	int beachHeight = 808;
	int threshould = 30;
	
	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image1.raw input_image2.raw output_image.raw [threshould = 30] [BytesPerPixel = 1]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 5){
		threshould = 30;
	} 
	else {
		if(argc < 6)
		{
			threshould = atoi(argv[4]);
			BytesPerPixel = 1;
		}
		else
		{
			threshould = atoi(argv[4]);
			BytesPerPixel = atoi(argv[5]);
		}	
		
	}
	
	// Allocate image data array
	unsigned char Dogdata[dogSize][dogSize][BytesPerPixel];
	unsigned char Beachdata[beachHeight][beachWidth][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Dogdata, sizeof(unsigned char), dogSize*dogSize*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Beachdata, sizeof(unsigned char), beachHeight*beachWidth*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	for(int y = 0; y < beachHeight; y++)
	{
		for(int x = 0; x < beachWidth; x++)
		{
			if((y>=400 && y<700) && (x>=1100 && x<1400))
			{
				if(!(Dogdata[y-400][x-1100][0]< threshould && Dogdata[y-400][x-1100][1]< threshould && Dogdata[y-400][x-1100][2] > 255-threshould))
				{
					for(int z=0; z<BytesPerPixel; z++)
						Beachdata[y][x][z] = Dogdata[y-400][x-1100][z];
				}		
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file = fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Beachdata, sizeof(unsigned char), beachHeight*beachWidth*BytesPerPixel, file);
	fclose(file);

	return 0;
}
