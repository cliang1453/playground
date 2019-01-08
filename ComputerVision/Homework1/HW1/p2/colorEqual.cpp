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

//count histogram 
int findMax(int ***Imagedata, int height, int width, int channel)
{
	int max = 0;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(Imagedata[y][x][channel]>max && Imagedata[y][x][channel]<=255) 
				max = Imagedata[y][x][channel];
		}
	}

	return max; 
}

int findMin(int ***Imagedata, int height, int width, int channel)
{
	int min = 255;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(Imagedata[y][x][channel] < min)
				min = Imagedata[y][x][channel];
		}
	}

	return min; 
}

void findGapValue(map<int, double> original,int& max, int& min)
{
	map<int, double>::iterator it;
	int temp;
	for(it=original.begin(); it!=original.end(); it++)
	{
		if(it->first - temp > 100)
		{
			max = it->first; 
			min = temp;
		}
		temp = it->first;
	}	
}

map<int, double> collectPixelNum(int*** Imagedata, int height, int width, int channel)
{
	map<int, double> original;
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
		{
			if(original.find(Imagedata[y][x][channel])==original.end())
				original[Imagedata[y][x][channel]] = 1;
			else 
				original.find(Imagedata[y][x][channel])->second ++;
		}
	}

	return original;
}


void linearStretching(int ***Imagedata, int height, int width, int channel)
{
	int min = findMin(Imagedata, height, width, channel); 
	int max = findMax(Imagedata, height, width, channel);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
		{
			double temp = (double(Imagedata[y][x][channel]-min)/(max-min))*255.0;
			Imagedata[y][x][channel] = floor(temp);
		}
	}	
}

void histogramEqualization(int*** Imagedata, int height, int width, int channel)
{
	//count the total number of pixel associated with each pixel intensity value
	map<int, double> original = collectPixelNum(Imagedata, height, width, channel);
	map<int, double>::iterator it;

	//mapping 
	map<int, double> equalized;
	double prob = 0 ; 
	double cumProb = 0;
	int result = 0;

	for(it=original.begin(); it!=original.end(); it++)
	{
		prob = (it->second)/double(width*height);
		cumProb = cumProb + prob;
		result = floor(cumProb*255);
		equalized.insert(make_pair(it->first, result));
	}
	
	//construct the Imagedata
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
			Imagedata[y][x][channel] = equalized[Imagedata[y][x][channel]];	
	}
}

void convertToRGB(int*** Imagedata, int height, int width, int BytesPerPixel)
{
	double S, L, C, X, M, R, G, B, H;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
		{
			H = double(Imagedata[y][x][0])/255.0*360.0;
			S = double(Imagedata[y][x][1])/255.0;
			L = double(Imagedata[y][x][2])/255.0;

			C = (1-fabs(2*L-1))*S; //0-1
			X = C*(1-fabs((H/60)-floor((H/60)/2)*2-1)); //X=0
			M = L - C/2; //0-1


			if(H<60){
				R = C; 
				G = X; 
				B = 0;
			}
			else if(H<120){//H=60
				R = X; 
				G = C; 
				B = 0; 
			}
			else if(H<180){
				R = 0; 
				G = C; 
				B = X; 
			}
			else if(H<240){
				R = 0; 
				G = X; 
				B = C; 
			}
			else if(H<300){
				R = X; 
				G = 0; 
				B = C; 
			}
			else {
				R = C; 
				G = 0; 
				B = X; 
			}

			if(R+M<0 || G+M<0 || B+M<0) M=0;

			Imagedata[y][x][0]= floor((R+M)*255);
			Imagedata[y][x][1]= floor((G+M)*255);
			Imagedata[y][x][2]= floor((B+M)*255);
		}
	}
}

void convertToHSL(int*** Imagedata, int height, int width, int BytesPerPixel)
{
	
	double R, G, B, C, M, m, H, S, L;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x <width; x++)
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

			Imagedata[y][x][0] = floor(H/360*255);
			Imagedata[y][x][1] = floor(S);
			Imagedata[y][x][2] = floor(L*255);
		}
	}
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width = 940;
	int height = 400;
	string method = "A";
	int preserve = 1;

	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw result_image.raw method_name [preserve = 1][BytesPerPixel = 1][Width = 940][Height = 400]" << endl;
		return 0;
	}

	method = argv[3];
	preserve = atoi(argv[4]);
	
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
    //store enhanced data
	int*** Imagedata = new int**[height];
	for(int i = 0; i < height; i++)
		Imagedata[i] = new int*[width];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
			Imagedata[i][j] = new int[BytesPerPixel];
	}

	unsigned char buf[height][width][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
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
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix

	if(method == "A")
	{
		for(int i=0; i<BytesPerPixel; i++)
			linearStretching(Imagedata, height, width, i); 
	}
	else if(method == "B")
	{
		if(preserve==1)
		{
			convertToHSL(Imagedata, height, width, BytesPerPixel);
			histogramEqualization(Imagedata, height, width, 2);	
			convertToRGB(Imagedata, height, width, BytesPerPixel);
		}
		else
		{
			for(int i=0; i<BytesPerPixel; i++)
				histogramEqualization(Imagedata, height, width, i);	
		}
	}
	else 
	{
		cout<< "Invalid method "<<argv[3]<<endl;
		exit(1);
	}


	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
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
