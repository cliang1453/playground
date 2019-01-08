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
	int width = 600;
	int height = 398;
	
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw h_space.raw s_space.raw l_space.raw [BytesPerPixel = 1][Width = 600][Height = 398]" << endl;
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
	int** hSpace = new int*[height];
	for(int i = 0; i < height; i++)
	{
		hSpace[i] = new int[width];
	}

	int** sSpace = new int*[height];
	for(int i = 0; i < height; i++)
	{
		sSpace[i] = new int[width];
	}

	int** lSpace = new int*[height];
	for(int i = 0; i < height; i++)
	{
		lSpace[i] = new int[width];
	}

	unsigned char buf[height][width];
	
	
	// Write image data (filename specified by second argument) from image data matrix
	double R, G, B, C, M, m, H, S, L;
	for(int y = 0; y<height; y++)
	{
		for(int x = 0; x<width; x++)
		{
			R = Imagedata[y][x][0];
			G = Imagedata[y][x][1];
			B = Imagedata[y][x][2];
			M = max(max(R,G),B);
			m = min(min(R,G),B);
			C = M-m;

			if(C==0) H = 0;
			else if(M==R) 
			{
				if((G-B)/C<0) H = 60*(((G-B)/C)+6);
				else H = 60*((G-B)/C);
			}	
			else if(M==G) H = 60*((B-R)/C+2);
			else if(M==B) H = 60*((R-G)/C+4);

			L = (m+M)/(2*255); // L<=1

			if(C==0) S = 0;
			else 
			{
				if(L==0) S = 0;
				else if(L>0 && L<0.5) S = C/(2*L);
				else S = C/(2-2*L);
			}

			hSpace[y][x] = floor(H/360*255);
			sSpace[y][x] = floor(S);
			lSpace[y][x] = floor(L*255);
		}
	}


	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			buf[i][j] = hSpace[i][j];
	}

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(buf, sizeof(unsigned char), height*width, file);
	fclose(file);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			buf[i][j] = sSpace[i][j];
	}

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(buf, sizeof(unsigned char), height*width, file);
	fclose(file);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			buf[i][j] = lSpace[i][j];
	}

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(buf, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}
