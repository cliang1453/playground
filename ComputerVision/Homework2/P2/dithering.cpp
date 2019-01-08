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

void transformToThreshould(void* MatrixPtr, int N)
{
	double* M = (double*)MatrixPtr;
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			M[i*N + j] = (M[i*N + j]+0.5)/double(pow(N,2))*255.0;
			cout<<M[i*N + j]<<endl;
		}
	}
}

void dithering(void* MatrixPtr, int** Mdata, const int width, const int height, int N, int level)
{
	double* M = (double*)MatrixPtr;


	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int i_m = i%N; 
			int j_m = j%N; 
			//cout<<M[i_m * N + j_m]<<endl;
			if(level==2)
			{
				if(Mdata[i][j] > M[i_m * N + j_m])
					Mdata[i][j] = 255;
				else 
					Mdata[i][j] = 0;
			}
			else if(level==4)
			{
				if(Mdata[i][j] > M[i_m * N + j_m])
				{
					if(Mdata[i][j] > 255.0 - 0.5*M[i_m * N + j_m])
						Mdata[i][j] = 255;
					else
						Mdata[i][j] = 170;
				}
				else 
				{
					if(Mdata[i][j] < 0.5*M[i_m * N + j_m])
						Mdata[i][j] = 0;
					else
						Mdata[i][j] = 85;
				}
			}
			else 
			{
				cout<<"Please choose dithering levels of 2/4."<<endl;
				exit(0);
			}
			
		}
	}
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	const int width = 512;
	const int height = 512;
	string method;
	int matrixSize; 
	int colorLevel;

	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input.raw output.raw [method = I/A] [matrixSize = 2/4/8] [colorLevel = 2/4]"<<endl;		return 0; 
	}

	method = argv[3];
	matrixSize = atoi(argv[4]);
	colorLevel = atoi(argv[5]);

	int** Mdata = new int*[height];
	for (int i = 0; i < height; i++)
	{
		Mdata[i] = new int[width];
	}

	unsigned char *buf1;
	buf1 = new unsigned char[height* width];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf1, sizeof(unsigned char), height*width, file);
	fclose(file);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			Mdata[i][j] = buf1[i*width + j];
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	double I2[2][2] = { {0,2}, {3,1} };

	double I4[4][4];
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if (i<2 && j<2)
				I4[i][j] = 4*I2[i][j];
			else if(i<2 && j>=2)
				I4[i][j] = 4*I2[i][j-2]+2;
			else if(i>=2 && j<2)
				I4[i][j] = 4*I2[i-2][j]+3;
			else if(i>=2 && j>=2)
				I4[i][j] = 4*I2[i-2][j-2]+1;
		}
	}

	double I8[8][8];
	for(int i=0; i<8; i++)
	{
		for(int j=0; j<8; j++)
		{
			if (i<4 && j<4)
				I8[i][j] = 4*I4[i][j];
			else if(i<4 && j>=4)
				I8[i][j] = 4*I4[i][j-4]+2;
			else if(i>=4 && j<4)
				I8[i][j] = 4*I4[i-4][j]+3;
			else if(i>=4 && j>=4)
				I8[i][j] = 4*I4[i-4][j-4]+1;
		}
	}

	double A[4][4] = { {14, 10, 11, 15},
				{9, 3, 0, 4},
				{8, 2, 1, 5},
				{13, 7, 6, 12}};

	//question a
	if(method=="I")
	{
		if(matrixSize==2)
		{
			transformToThreshould(I2, matrixSize);
			dithering(I2, Mdata, width, height, matrixSize, colorLevel);
		}
		else if(matrixSize==4)
		{
			transformToThreshould(I4, matrixSize);
			dithering(I4, Mdata, width, height, matrixSize, colorLevel);
		}
		else if(matrixSize==8)
		{
			transformToThreshould(I8, matrixSize);
			dithering(I8, Mdata, width, height, matrixSize, colorLevel);
		}
		else 
		{
			cout<<"Please input 2/4/8 for dithering matrix size."<<endl;
			exit(0);
		}
	}
	else if(method=="A")
	{
		if(matrixSize==4)
		{
			transformToThreshould(A, matrixSize);
			dithering(A, Mdata, width, height, matrixSize, colorLevel);
		}
		else 
		{
			cout<<"Please input 4 for dithering matrix size."<<endl;
			exit(0);
		}
	}
	
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			buf1[i*width + j] = Mdata[i][j];
	}

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(buf1, sizeof(unsigned char), height*width, file);
	fclose(file);

	
	return 0;
}