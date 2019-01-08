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

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	const int width = 512;
	const int height = 512;
	string method;

	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input.raw output.raw [method = F/JJN/S]"<<endl;		
		return 0; 
	}

	method = argv[3];

	double** Mdata = new double*[height];
	for (int i = 0; i < height; i++)
	{
		Mdata[i] = new double[width];
	}

	char *buf1;
	buf1 = new char[height* width];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf1, sizeof(char), height*width, file);
	fclose(file);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Mdata[i][j] = (unsigned char)buf1[i*width + j];
			Mdata[i][j] = Mdata[i][j]/255.0;
		}
	}


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	int pad; 
	double error;
	double** mat;

	if(method=="F")
	{
		pad = 1;
		double temp[3][3] = { {0, 0, 0},
							 {0, 0, 7.0/16.0},
							 {3.0/16.0, 5.0/16.0, 1.0/16.0} };
		
		mat = new double*[3];
		for (int i = 0; i < 3; i++)
			mat[i] = new double[3];

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				mat[i][j] = temp[i][j];
		}
	}
	else if(method=="JJN")
	{
		pad = 2;
		double temp[5][5] = { {0, 0, 0, 0, 0},
							  {0, 0, 0, 0, 0},
							  {0, 0, 0, 7.0/48.0, 5.0/48.0},
							  {3.0/48.0, 5.0/48.0, 7.0/48.0, 1.0/48.0, 3.0/48.0}, 
							  {5.0/48.0, 5.0/48.0, 3.0/48.0, 3.0/48.0, 1.0/48.0} };
		mat = new double*[5];
		for (int i = 0; i < 5; i++)
			mat[i] = new double[5];

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
				mat[i][j] = temp[i][j];
		}
	}
	else if(method=="S")
	{
		pad = 2;
		double temp[5][5] = { {0, 0, 0, 0, 0},
							  {0, 0, 0, 0, 0},
							  {0, 0, 0, 8.0/42.0, 4.0/42.0},
							  {2.0/42.0, 4.0/42.0, 8.0/42.0, 4.0/42.0, 2.0/42.0}, 
							  {1.0/42.0, 2.0/42.0, 4.0/42.0, 2.0/42.0, 1.0/42.0} };
		mat = new double*[5];
		for (int i = 0; i < 5; i++)
			mat[i] = new double[5];

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
				mat[i][j] = temp[i][j];
		}
	}
	else
	{
		cout<<"Invalid method. Please input F/JJN/S."<<endl;
		exit(0);
	}

	//tranformation
	for(int i=0; i<height; i++)
	{
		if(i%2==0) // --->
		{
			for(int j=0; j<width; j++)
			{
				if(Mdata[i][j]> 0.5)
				{
					
					error = Mdata[i][j] - 1.0;
					Mdata[i][j] = 1.0;
				}
				else
				{
					error = Mdata[i][j] - 0.0;
					Mdata[i][j] = 0.0;
				}

				//diffuse the error
				for(int p = 0; p < 2*pad+1; p++)
				{
					for(int q = 0; q < 2*pad+1; q++)
					{
						if(!(i+p-pad > height-1 || j+q-pad > width-1 || i+p-pad < 0 || j+q-pad <0))
							Mdata[i+p-pad][j+q-pad] += mat[p][q]*error;
					}
				}
			}
		}
		else  
		{
			for(int j= width-1; j>-1; j--)
			{
				if(Mdata[i][j] > 0.5)
				{
					error = Mdata[i][j] - 1.0;
					Mdata[i][j] = 1.0;
				}
				else
				{
					error = Mdata[i][j] - 0.0;
					Mdata[i][j] = 0.0;
				}

				//diffuse the error
				for(int p = 0; p < 2*pad+1; p++)
				{
					for(int q = 0; q < 2*pad+1; q++)
					{
						if(!(i+p-pad > height-1 || j+q-pad > width-1 || i+p-pad < 0 || j+q-pad <0))
							Mdata[i+p-pad][j+q-pad] += mat[p][2*pad-q]*error;
					}
				}
			}
		}
	}




	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Mdata[i][j] = Mdata[i][j]*255;
			buf1[i*width + j] = Mdata[i][j] < 128. ? 0 : 255;
		}
	}

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(buf1, sizeof(char), height*width, file);
	fclose(file);

	
	return 0;
}