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

bool firstStage(int** data, int** mask, vector<int*> filter, int width, int height, int count, map<int, int>* countingMap)
{
	int oddSizeCheck[3][3] = {{0,0,0},{0,1,0},{0,0,0}};

	bool hit;
	bool notConverge = 0;
	int n = filter.size();
	for(int i=1; i<height+1; i++)
	{
		for(int j=1; j<width+1; j++)
		{
			if(data[i][j]==1) // hit a 1
			{
				bool final = 1;
				for(int p=-1; p<2; p++)
				{
					for(int q=-1; q<2; q++)
					{
						if(data[i+p][j+q] != oddSizeCheck[1+p][1+q])
							final = 0; 	
					}
				}
				if(final==1)
				{
					if(countingMap->find(i*width+j) == countingMap->end()
						&& countingMap->find((i-1)*width+j) == countingMap->end()
						&& countingMap->find(i*width+(j-1)) == countingMap->end())
					{
						countingMap->insert(make_pair(i*width+j,count+1));
					}
				}

				//check if the mask has a hit on
				for(int k=0; k<n; k++)
				{
					hit = 1; 
					for(int p=-1; p<2; p++)
					{
						for(int q=-1; q<2; q++)
						{
							if(data[i+p][j+q] != filter[k][(1+p)*3+(1+q)])
								hit = 0;
						}
					}
					if(hit==1) break;		
				}

				if(hit==1)
				{
					mask[i][j] = 1;
					notConverge = 1;
				}
			}
		}
	}

	return notConverge;
}

void secondStage(int** data, int** mask, vector<int*> filter, int width, int height, int count, map<int, int>* countingMap)
{
	bool hit;
	int n = filter.size();
	for(int i=1; i<height+1; i++)
	{
		for(int j=1; j<width+1; j++)
		{
			if(mask[i][j]==1)
			{
				int k=0;
				if(mask[i-1][j] == filter[0][(0)*3+(1)] 
					&& mask[i][j] == filter[0][(1)*3+(1)] 
					&& mask[i+1][j] == filter[0][(2)*3+(1)])
				{
					hit=1;
				}
				else if(mask[i][j-1] == filter[1][(1)*3+(0)]
					&& mask[i][j] == filter[1][(1)*3+(1)]
					&& mask[i][j+1] == filter[1][(1)*3+(2)])
				{
					hit=1;
				}
				else 
				{
					for(k=2; k<n; k++)
					{
						hit = 1;
						for(int p=-1; p<2; p++)
					    {
							for(int q=-1; q<2; q++)
							{
								if(mask[i+p][j+q] != filter[k][(1+p)*3+(1+q)])
									hit = 0;
							}
						}
						if(hit==1) break;
					}
					
				}

				if(hit==1)
				{
					data[i][j] = 0;
					if(k==n-1) 
					{
						if(countingMap->find(i*width+j) == countingMap->end())
						{
							countingMap->insert(make_pair(i*width+j,count));
						}
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	const int width = 480;
	const int height = 480;
	string method;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input.raw output.raw"<<endl;		
		return 0; 
	}

	int** data = new int*[height+2];
	for (int i = 0; i < height+2; i++)
	{
		data[i] = new int[width+2];
	}

	int** mask = new int*[height+2];
	for (int i = 0; i < height+2; i++)
	{
		mask[i] = new int[width+2];
	}

	unsigned char *buf1;
	buf1 = new unsigned char[height* width];
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(buf1, sizeof(unsigned char), height*width, file);
	fclose(file);
	for (int i = 0; i < height+2; i++)
	{
		for (int j = 0; j < width+2; j++)
		{
			if(i==0 || j==0 || i==height+1 || j==width+1)
			{
				data[i][j] = 0;
			}

			else
			{
				data[i][j] = buf1[i*width + j]/255;
			}
			mask[i][j] = 0;

			
		}	
	}


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	vector<int*> S1;
	vector<int*> S2;
	map<int, int>* countingMap = new map<int, int>;
	int* p1 = new int[9];
	int* p2 = new int[9];
	int* p3 = new int[9];
	int* p4 = new int[9];
	int* p5 = new int[9];
	int* p6 = new int[9];
	int* p7 = new int[9];
	int* p8 = new int[9];
	int* p9 = new int[9];
	int* p10 = new int[9];
	int* p11 = new int[9];
	int* p12 = new int[9];
	int* p13 = new int[9];
	int* p14 = new int[9];

	int* p16 = new int[9];
	int* p17 = new int[9];
	int* p18 = new int[9];
	int* p19 = new int[9];
	int* p20 = new int[9];

	int s1[3][3]={{0, 1, 1},
				 {0, 1, 1},
				 {0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p1[i*3+j] = s1[i][j];
	}
	S1.push_back(p1);

	int s2[3][3]={{0, 0, 0},
					{1, 1, 0},
					{1, 1, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p2[i*3+j] = s2[i][j];
	}
	S1.push_back(p2);

	int s3[3][3]={{0, 0, 0},
					{0, 1, 1},
					{0, 1, 1}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p3[i*3+j] = s3[i][j];
	}
	S1.push_back(p3);


	int s4[3][3]={{1, 1, 0},
				{1, 1, 0},
				{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p4[i*3+j] = s4[i][j];
	}
	S1.push_back(p4);

	
	int s5[3][3]={{0, 1, 1},
					{0, 1, 1},
					{0, 1, 1}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p5[i*3+j] = s5[i][j];
	}
	S1.push_back(p5);

	
	int s6[3][3]={{1, 1, 1},
					{1, 1, 1},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p6[i*3+j] = s6[i][j];
	}
	S1.push_back(p6);

	
	int s7[3][3]={{1, 1, 0},
					{1, 1, 0},
					{1, 1, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p7[i*3+j] = s7[i][j];
	}
	S1.push_back(p7);

	
	int s8[3][3]={{0, 0, 0},
					{1, 1, 1},
					{1, 1, 1}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p8[i*3+j] = s8[i][j];
	}
	S1.push_back(p8);

	int s19[3][3]={{0, 0, 0},
					{1, 1, 0},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p19[i*3+j] = s19[i][j];
	}
	S1.push_back(p19);

	int s20[3][3]={{0, 0, 0},
					{0, 1, 1},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p20[i*3+j] = s20[i][j];
	}
	S1.push_back(p20);





	int s9[3][3]={{0, 1, 0},
					 {0, 1, 0},
					 {0, 1, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p9[i*3+j] = s9[i][j];
	}
	S2.push_back(p9);

	int s10[3][3]={{0, 0, 0},
					 {1, 1, 1},
					 {0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p10[i*3+j] = s10[i][j];
	}
	S2.push_back(p10);

	int s11[3][3]={{0, 0, 0},
					{0, 1, 1},
					{0, 1, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p11[i*3+j] = s11[i][j];
	}
	S2.push_back(p11);

	int s12[3][3]={{0, 0, 0},
					{1, 1, 0},
					{0, 1, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p12[i*3+j] = s12[i][j];
	}
	S2.push_back(p12);

	int s13[3][3]={{0, 1, 0},
					{0, 1, 1},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p13[i*3+j] = s13[i][j];
	}
	S2.push_back(p13);

	int s14[3][3]={{0, 1, 0},
					{1, 1, 0},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p14[i*3+j] = s14[i][j];
	}
	S2.push_back(p14);

	int s16[3][3]={{1, 1, 0},
					{1, 1, 0},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p16[i*3+j] = s16[i][j];
	}
	S2.push_back(p16);

	int s17[3][3]={{0, 1, 1},
					{0, 1, 1},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p17[i*3+j] = s17[i][j];
	}
	S2.push_back(p17);

	int s18[3][3]={{0, 0, 0},
					{0, 1, 1},
					{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			p18[i*3+j] = s18[i][j];
	}
	S2.push_back(p18);



	//apply first stage mask to the whole image
	bool notConverge=1;
	int count=0;
		while(true)
		{
			notConverge = firstStage(data, mask, S1, width, height, count, countingMap);
			if(notConverge==0) break;
			secondStage(data, mask, S2, width, height, count, countingMap);
			for (int i = 0; i < height+2; i++)
			{
				for (int j = 0; j < width+2; j++)
					mask[i][j] = 0;
			}
			count = count + 2;
		}

	cout<<"The total number of squares is "<<countingMap->size()<<endl;
	map<int,int> freqHist;
	map<int,int>::iterator it;
	for (it=countingMap->begin(); it!=countingMap->end(); ++it)
	{
		if(freqHist.find(it->second) == freqHist.end())
			freqHist[it->second] = 1;
		else 
			freqHist[it->second]++;
	}

	for (it=freqHist.begin(); it!=freqHist.end(); ++it)
		cout<<"There are "<<it->second<<" sqaures with size "<<it->first<<" exist."<<endl;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			buf1[i*width + j] = data[i+1][j+1]*255;
	}

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(buf1, sizeof(char), height*width, file);
	fclose(file);

	
	return 0;
}