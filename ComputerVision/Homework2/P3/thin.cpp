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

bool firstStage(int** data, int** mask, vector<int*> filter, int width, int height)
{
	bool hit;
	bool notConverge = 0;
	int n = filter.size();
	for(int i=1; i<height+1; i++)
	{
		for(int j=1; j<width+1; j++)
		{
			if(data[i][j]==1) // hit a 1
			{
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

void secondStage(int** data, int** mask, vector<int*> filter, int width, int height)
{
	bool hit;
	int n = filter.size();
	for(int i=1; i<height+1; i++)
	{
		for(int j=1; j<width+1; j++)
		{
			if(mask[i][j]==1) // hit a 1
			{
				for(int k=0; k<n; k++)
				{
					hit = 1; 
					if(k==28)
					{
						if(mask[i][j-1] == filter[28][(1)*3+(0)] 
							&& mask[i][j] == filter[28][(1)*3+(1)] 
							&& mask[i+1][j-1] == filter[28][(2)*3+(0)] 
							&& mask[i+1][j] == filter[28][(2)*3+(1)])
						{
							break;
						}
					}
					else 
					{
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
				if(hit==0)
				{
					data[i][j] = 0;
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
				data[i][j] = 0;
			else
				data[i][j] = buf1[i*width + j]/255;
			mask[i][j] = 0;
		}	
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// Write image data (filename specified by second argument) from image data matrix
	vector<int*> T1;
	vector<int*> T2;
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
	int* p15 = new int[9];
	int* p16 = new int[9];
	int* p17 = new int[9];
	int* p18 = new int[9];
	int* p19 = new int[9];
	int* p20 = new int[9];
	int* p21 = new int[9];
	int* p22 = new int[9];
	int* p23 = new int[9];
	int* p24 = new int[9];
	int* p25 = new int[9];
	int* p26 = new int[9];
	int* p27 = new int[9];
	int* p28 = new int[9];
	int* p29 = new int[9];
	int* p30 = new int[9];
	int* p31 = new int[9];
	int* p32 = new int[9];
	int* p33 = new int[9];
	int* p34 = new int[9];
	int* p35 = new int[9];
	int* p36 = new int[9];
	int* p37 = new int[9];
	int* p38 = new int[9];
	int* p39 = new int[9];
	int* p40 = new int[9];
	int* p41 = new int[9];
	int* p42 = new int[9];
	int* p43 = new int[9];
	int* p44 = new int[9];
	int* p45 = new int[9];
	int* p46 = new int[9];
	int* p47 = new int[9];
	int* p48 = new int[9];
	int* p49 = new int[9];
	int* p50 = new int[9];
	int* p51 = new int[9];
	int* p52 = new int[9];
	int* p53 = new int[9];
	int* p54 = new int[9];
	int* p55 = new int[9];
	int* p56 = new int[9];
	int* p57 = new int[9];
	int* p58 = new int[9];
	int* p59 = new int[9];
	int* p60 = new int[9];
	int* p61 = new int[9];
	int* p62 = new int[9];
	int* p63 = new int[9];
	int* p64 = new int[9];
	int* p65 = new int[9];
	int* p66 = new int[9];
	int* p67 = new int[9];
	int* p68 = new int[9];
	int* p69 = new int[9];
	int* p70 = new int[9];
	int* p71 = new int[9];
	int* p72 = new int[9];
	int* p73 = new int[9];
	int* p74 = new int[9];
	int* p75 = new int[9];
	int* p76 = new int[9];
	int* p77 = new int[9];
	int* p78 = new int[9];
	int* p79 = new int[9];
	int* p80 = new int[9];
	int* p81 = new int[9];
	int* p82 = new int[9];
	int* p83 = new int[9];
	int* p84 = new int[9];
	int* p85 = new int[9];
	int* p86 = new int[9];
	int* p87 = new int[9];
	int* p88 = new int[9];
	int* p89 = new int[9];
	int* p90 = new int[9];
	int* p91 = new int[9];
	int* p92 = new int[9];
	int* p93 = new int[9];
	int* p94 = new int[9];
	int* p95 = new int[9];
	int* p96 = new int[9];
	int* p97 = new int[9];
	int* p98 = new int[9];
	int* p99 = new int[9];
	int* p100 = new int[9];
	int* p101 = new int[9];
	int* p102 = new int[9];
	int* p103 = new int[9];

	//===============================5

	int s1[3][3]={{0, 1, 1},
				 {0, 1, 1},
				 {0, 0, 0}};

	int s2[3][3]={{0, 0, 0},
					{1, 1, 0},
					{1, 1, 0}};

	int s3[3][3]={{0, 0, 0},
					{0, 1, 1},
					{0, 1, 1}};

	int s4[3][3]={{1, 1, 0},
				{1, 1, 0},
				{0, 0, 0}};

	//==========================================8

	int s5[3][3]={{0, 1, 1},
					{0, 1, 1},
					{0, 1, 1}};

	int s6[3][3]={{1, 1, 1},
					{1, 1, 1},
					{0, 0, 0}};
	
	int s7[3][3]={{1, 1, 0},
					{1, 1, 0},
					{1, 1, 0}};

	int s8[3][3]={{0, 0, 0},
					{1, 1, 1},
					{1, 1, 1}};

	//================================9
	int s9[3][3]={{1, 1, 1},
				 {0, 1, 1},
				 {0, 1, 1}};

	int s10[3][3]={{0, 1, 1},
				 {0, 1, 1},
				 {1, 1, 1}};

	int s11[3][3]={{1, 1, 1},
				 {1, 1, 1},
				 {1, 0, 0}};

	int s12[3][3]={{1, 1, 1},
				 {1, 1, 1},
				 {0, 0, 1}};

	int s13[3][3]={{1, 1, 1},
				 {1, 1, 0},
				 {1, 1, 0}};

	int s14[3][3]={{1, 1, 0},
				 {1, 1, 0},
				 {1, 1, 1}};

	int s15[3][3]={{1, 0, 0},
				 {1, 1, 1},
				 {1, 1, 1}};

	int s16[3][3]={{0, 0, 1},
				 {1, 1, 1},
				 {1, 1, 1}};

	//======================================10

	int s17[3][3]={{1, 0, 1},
				 {1, 1, 1},
				 {1, 1, 1}};

	int s18[3][3]={{1, 1, 1},
				 {0, 1, 1},
				 {1, 1, 1}};

	int s19[3][3]={{1, 1, 1},
				 {1, 1, 1},
				 {1, 0, 1}};

	int s20[3][3]={{1, 1, 1},
				 {1, 1, 0},
				 {1, 1, 1}};

	//================================4

	int s21[3][3]={{0, 1, 0},
				 {0, 1, 1},
				 {0, 0, 0}};

	int s22[3][3]={{0, 0, 0},
					{1, 1, 0},
					{0, 1, 0}};

	int s23[3][3]={{0, 0, 0},
					{0, 1, 1},
					{0, 1, 0}};

	int s24[3][3]={{0, 1, 0},
				{1, 1, 0},
				{0, 0, 0}};

	//======================================4
	int s25[3][3]={{0, 0, 1},
				 {0, 1, 1},
				 {0, 0, 1}};

	int s26[3][3]={{1, 1, 1},
					{0, 1, 0},
					{0, 0, 0}};

	int s27[3][3]={{1, 0, 0},
					{1, 1, 0},
					{1, 0, 0}};

	int s28[3][3]={{0, 0, 0},
				{0, 1, 0},
				{1, 1, 1}};

	//====================================5
	int s29[3][3]={{1, 1, 0},
				 {0, 1, 1},
				 {0, 0, 0}};

	int s30[3][3]={{0, 1, 0},
					{0, 1, 1},
					{0, 0, 1}};

	int s31[3][3]={{0, 1, 1},
					{1, 1, 0},
					{0, 0, 0}};

	int s32[3][3]={{0, 0, 1},
				{0, 1, 1},
				{0, 1, 0}};

	//=============================6
	int s33[3][3]={{1, 1, 0},
					{0, 1, 1},
					{0, 0, 1}};

	int s34[3][3]={{0, 1, 1},
				{1, 1, 0},
				{1, 0, 0}};
	//===================================6

	int s35[3][3]={{0, 0, 1},
					{0, 1, 1},
					{0, 1, 1}};

	int s36[3][3]={{0, 1, 1},
					{0, 1, 1},
					{0, 0, 1}};
	
	int s37[3][3]={{1, 1, 1},
					{0, 1, 1},
					{0, 0, 0}};

	int s38[3][3]={{1, 1, 1},
					{1, 1, 0},
					{0, 0, 0}};
	
	int s39[3][3]={{1, 0, 0},
					{1, 1, 0},
					{1, 1, 0}};

	int s40[3][3]={{1, 1, 0},
					{1, 1, 0},
					{1, 0, 0}};
	
	int s41[3][3]={{0, 0, 0},
					{0, 1, 1},
					{1, 1, 1}};

	int s42[3][3]={{0, 0, 0},
					{1, 1, 0},
					{1, 1, 1}};

	//================================7
	int s43[3][3]={{1, 1, 1},
					{0, 1, 1},
					{0, 0, 1}};

	int s44[3][3]={{1, 1, 1},
				{1, 1, 0},
				{1, 0, 0}};

	int s45[3][3]={{1, 0, 0},
				{1, 1, 0},
				{1, 1, 1}};

	int s46[3][3]={{0, 0, 1},
					{0, 1, 1},
					{1, 1, 1}};
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			p1[i*3+j] = s1[i][j];
			T1.push_back(p1);
			p2[i*3+j] = s2[i][j];
			T1.push_back(p2);
			p3[i*3+j] = s3[i][j];
			T1.push_back(p3);
			p4[i*3+j] = s4[i][j];
			T1.push_back(p4);
			p5[i*3+j] = s5[i][j];
			T1.push_back(p5);
			p6[i*3+j] = s6[i][j];
			T1.push_back(p6);
			p7[i*3+j] = s7[i][j];
			T1.push_back(p7);
			p8[i*3+j] = s8[i][j];
			T1.push_back(p8);
			p9[i*3+j] = s9[i][j];
			T1.push_back(p9);
			p10[i*3+j] = s10[i][j];
			T1.push_back(p10);
			p11[i*3+j] = s11[i][j];
			T1.push_back(p11);
			p12[i*3+j] = s12[i][j];
			T1.push_back(p12);
			p13[i*3+j] = s13[i][j];
			T1.push_back(p13);
			p14[i*3+j] = s14[i][j];
			T1.push_back(p14);
			p15[i*3+j] = s15[i][j];
			T1.push_back(p15);
			p16[i*3+j] = s16[i][j];
			T1.push_back(p16);
			p17[i*3+j] = s17[i][j];
			T1.push_back(p17);
			p18[i*3+j] = s18[i][j];
			T1.push_back(p18);
			p19[i*3+j] = s19[i][j];
			T1.push_back(p19);
			p20[i*3+j] = s20[i][j];
			T1.push_back(p20);
			p21[i*3+j] = s21[i][j];
			T1.push_back(p21);
			p22[i*3+j] = s22[i][j];
			T1.push_back(p22);
			p23[i*3+j] = s23[i][j];
			T1.push_back(p23);
			p24[i*3+j] = s24[i][j];
			T1.push_back(p24);
			p25[i*3+j] = s25[i][j];
			T1.push_back(p25);
			p26[i*3+j] = s26[i][j];
			T1.push_back(p26);
			p27[i*3+j] = s27[i][j];
			T1.push_back(p27);
			p28[i*3+j] = s28[i][j];
			T1.push_back(p28);
			p29[i*3+j] = s29[i][j];
			T1.push_back(p29);
			p30[i*3+j] = s30[i][j];
			T1.push_back(p30);
			p31[i*3+j] = s31[i][j];
			T1.push_back(p31);
			p32[i*3+j] = s32[i][j];
			T1.push_back(p32);
			p33[i*3+j] = s33[i][j];
			T1.push_back(p33);
			p34[i*3+j] = s34[i][j];
			T1.push_back(p34);
			p35[i*3+j] = s35[i][j];
			T1.push_back(p35);
			p36[i*3+j] = s36[i][j];
			T1.push_back(p36);
			p37[i*3+j] = s37[i][j];
			T1.push_back(p37);
			p38[i*3+j] = s38[i][j];
			T1.push_back(p38);
			p39[i*3+j] = s39[i][j];
			T1.push_back(p39);
			p40[i*3+j] = s40[i][j];
			T1.push_back(p40);
			p41[i*3+j] = s41[i][j];
			T1.push_back(p41);
			p42[i*3+j] = s42[i][j];
			T1.push_back(p42);
			p43[i*3+j] = s43[i][j];
			T1.push_back(p43);
			p44[i*3+j] = s44[i][j];
			T1.push_back(p44);
			p45[i*3+j] = s45[i][j];
			T1.push_back(p45);
			p46[i*3+j] = s46[i][j];
			T1.push_back(p46);
		}	
	}

	//===============================================
	int s47[3][3]={{0, 0, 1},
				 {0, 1, 0},
				 {0, 0, 0}};

	int s48[3][3]={{1, 0, 0},
				 {0, 1, 0},
				 {0, 0, 0}};
	
	int s49[3][3]={{0, 0, 0},
				 {0, 1, 0},
				 {0, 1, 0}};

	int s50[3][3]={{0, 0, 0},
				 {0, 1, 1},
				 {0, 0, 0}};

	int s51[3][3]={{0, 0, 1},
				 {0, 1, 1},
				 {0, 0, 0}};

	int s52[3][3]={{0, 1, 1},
				 {0, 1, 0},
				 {0, 0, 0}};

	int s53[3][3]={{1, 1, 0},
				 {0, 1, 0},
				 {0, 0, 0}};

	int s54[3][3]={{1, 0, 0},
				 {1, 1, 0},
				 {0, 0, 0}};

	int s55[3][3]={{0, 0, 0},
				 {0, 1, 1},
				 {0, 0, 1}};

	int s56[3][3]={{0, 0, 0},
				 {1, 1, 0},
				 {1, 0, 0}};

	int s57[3][3]={{0, 0, 0},
				 {0, 1, 0},
				 {1, 1, 0}};

	int s58[3][3]={{0, 0, 0},
				 {0, 1, 0},
				 {0, 1, 1}};

	//========================4 connected offset
	int s59[3][3]={{0, 1, 1},
				 {1, 1, 0},
				 {0, 0, 0}};

	int s60[3][3]={{1, 1, 0},
				 {0, 1, 1},
				 {0, 0, 0}};

	int s61[3][3]={{0, 1, 0},
				 {0, 1, 1},
				 {0, 0, 1}};

	int s62[3][3]={{0, 0, 1},
				 {0, 1, 1},
				 {0, 1, 0}};

	//=======================spur corner cluster
	
	int s63[3][3]={{0, 1, 1},
				 {0, 1, 0},
				 {1, 0, 0}};

	int s64[3][3]={{0, 0, 1},
				 {0, 1, 1},
				 {1, 0, 0}};

	int s65[3][3]={{0, 1, 1},
				 {0, 1, 1},
				 {1, 0, 0}};

	int s66[3][3]={{0, 0, 1},
				 {1, 1, 0},
				 {1, 0, 0}};

	int s67[3][3]={{0, 0, 1},
				 {0, 1, 0},
				 {1, 1, 0}};

	int s68[3][3]={{0, 0, 1},
				 {1, 1, 0},
				 {1, 1, 0}};

	int s69[3][3]={{1, 1, 0},
				 {0, 1, 0},
				 {0, 0, 1}};

	int s70[3][3]={{1, 0, 0},
				 {1, 1, 0},
				 {0, 0, 1}};

	int s71[3][3]={{1, 1, 0},
				 {1, 1, 0},
				 {0, 0, 1}};

	int s72[3][3]={{1, 0, 0},
				 {0, 1, 1},
				 {0, 0, 1}};

	int s73[3][3]={{1, 0, 0},
				 {0, 1, 0},
				 {0, 1, 1}};

	int s74[3][3]={{1, 0, 0},
				 {0, 1, 1},
				 {0, 1, 1}};

	//==============================Corner Cluster
	int s75[3][3]={{0, 0, 0},
				 {1, 1, 0},
				 {1, 1, 0}};
	//for 75, if the corner pattern hit, then it is hit 

	//==============================Tee Branch
	int s76[3][3]={{1, 1, 0},
				 {1, 1, 1},
				 {0, 0, 0}};

	int s77[3][3]={{0, 1, 0},
				 {1, 1, 1},
				 {0, 0, 0}};

	int s78[3][3]={{0, 1, 0},
				 {1, 1, 1},
				 {1, 0, 0}};

	int s79[3][3]={{1, 1, 0},
				 {1, 1, 1},
				 {1, 0, 0}};

	int s80[3][3]={{0, 1, 1},
				 {1, 1, 1},
				 {0, 0, 0}};

	int s81[3][3]={{0, 1, 0},
				 {1, 1, 1},
				 {0, 0, 1}};

	int s82[3][3]={{0, 1, 1},
				 {1, 1, 1},
				 {0, 0, 1}};

	//
	int s83[3][3]={{0, 0, 0},
				 {1, 1, 1},
				 {0, 1, 0}};

	int s84[3][3]={{0, 0, 1},
				 {1, 1, 1},
				 {0, 1, 0}};

	int s85[3][3]={{0, 0, 0},
				 {1, 1, 1},
				 {0, 1, 1}};

	int s86[3][3]={{0, 0, 1},
				 {1, 1, 1},
				 {0, 1, 1}};

	int s87[3][3]={{1, 0, 0},
				 {1, 1, 1},
				 {0, 1, 0}};

	int s88[3][3]={{0, 0, 0},
				 {1, 1, 1},
				 {1, 1, 0}};

	int s89[3][3]={{1, 0, 0},
				 {1, 1, 1},
				 {1, 1, 0}};

	//
	int s90[3][3]={{0, 1, 0},
				 {1, 1, 0},
				 {0, 1, 0}};
	
	int s91[3][3]={{1, 1, 0},
				 {1, 1, 0},
				 {0, 1, 0}};

	int s92[3][3]={{0, 1, 1},
				 {1, 1, 0},
				 {0, 1, 0}};
	
	int s93[3][3]={{1, 1, 1},
				 {1, 1, 0},
				 {0, 1, 0}};
	
	int s94[3][3]={{0, 1, 0},
				 {1, 1, 0},
				 {1, 1, 0}};

	int s95[3][3]={{0, 1, 0},
				 {1, 1, 0},
				 {0, 1, 1}};
	
	int s96[3][3]={{0, 1, 0},
				 {1, 1, 0},
				 {1, 1, 1}};

	//
	int s97[3][3]={{0, 1, 0},
				 {0, 1, 1},
				 {0, 1, 0}};

	int s98[3][3]={{0, 1, 0},
				 {0, 1, 1},
				 {1, 1, 0}};

	int s99[3][3]={{0, 1, 0},
				 {0, 1, 1},
				 {0, 1, 1}};

	int s100[3][3]={{0, 1, 0},
				 {0, 1, 1},
				 {1, 1, 1}};

	int s101[3][3]={{1, 1, 0},
				 {0, 1, 1},
				 {0, 1, 0}};

	int s102[3][3]={{0, 1, 1},
				 {0, 1, 1},
				 {0, 1, 0}};

	int s103[3][3]={{1, 1, 1},
				 {0, 1, 1},
				 {0, 1, 0}};


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			p47[i*3+j] = s47[i][j];
			T2.push_back(p47);
			p48[i*3+j] = s48[i][j];
			T2.push_back(p48);
			p49[i*3+j] = s49[i][j];
			T2.push_back(p49);
			p50[i*3+j] = s50[i][j];
			T2.push_back(p50);
			p51[i*3+j] = s51[i][j];
			T2.push_back(p51);
			p52[i*3+j] = s52[i][j];
			T2.push_back(p52);
			p53[i*3+j] = s53[i][j];
			T2.push_back(p53);
			p54[i*3+j] = s54[i][j];
			T2.push_back(p54);
			p55[i*3+j] = s55[i][j];
			T2.push_back(p55);
			p56[i*3+j] = s56[i][j];
			T2.push_back(p56);
			p57[i*3+j] = s57[i][j];
			T2.push_back(p57);
			p58[i*3+j] = s58[i][j];
			T2.push_back(p58);
			p59[i*3+j] = s59[i][j];
			T2.push_back(p59);
			p60[i*3+j] = s60[i][j];
			T2.push_back(p60);
			p61[i*3+j] = s61[i][j];
			T2.push_back(p61);
			p62[i*3+j] = s62[i][j];
			T2.push_back(p62);
			p63[i*3+j] = s63[i][j];
			T2.push_back(p63);
			p64[i*3+j] = s64[i][j];
			T2.push_back(p64);
			p65[i*3+j] = s65[i][j];
			T2.push_back(p65);
			p66[i*3+j] = s66[i][j];
			T2.push_back(p66);
			p67[i*3+j] = s67[i][j];
			T2.push_back(p67);
			p68[i*3+j] = s68[i][j];
			T2.push_back(p68);
			p69[i*3+j] = s69[i][j];
			T2.push_back(p69);
			p70[i*3+j] = s70[i][j];
			T2.push_back(p70);
			p71[i*3+j] = s71[i][j];
			T2.push_back(p71);
			p72[i*3+j] = s72[i][j];
			T2.push_back(p72);
			p73[i*3+j] = s73[i][j];
			T2.push_back(p73);
			p74[i*3+j] = s74[i][j];
			T2.push_back(p74);
			p75[i*3+j] = s75[i][j];
			T2.push_back(p75);
			p76[i*3+j] = s76[i][j];
			T2.push_back(p76);
			p77[i*3+j] = s77[i][j];
			T2.push_back(p77);
			p78[i*3+j] = s78[i][j];
			T2.push_back(p78);
			p79[i*3+j] = s79[i][j];
			T2.push_back(p79);
			p80[i*3+j] = s80[i][j];
			T2.push_back(p80);
			p81[i*3+j] = s81[i][j];
			T2.push_back(p81);
			p82[i*3+j] = s82[i][j];
			T2.push_back(p82);
			p83[i*3+j] = s83[i][j];
			T2.push_back(p83);
			p84[i*3+j] = s84[i][j];
			T2.push_back(p84);
			p85[i*3+j] = s85[i][j];
			T2.push_back(p85);
			p86[i*3+j] = s86[i][j];
			T2.push_back(p86);
			p87[i*3+j] = s87[i][j];
			T2.push_back(p87);
			p88[i*3+j] = s88[i][j];
			T2.push_back(p88);
			p89[i*3+j] = s89[i][j];
			T2.push_back(p89);
			p90[i*3+j] = s90[i][j];
			T2.push_back(p90);
			p91[i*3+j] = s91[i][j];
			T2.push_back(p91);
			p92[i*3+j] = s92[i][j];
			T2.push_back(p92);
			p93[i*3+j] = s93[i][j];
			T2.push_back(p93);
			p94[i*3+j] = s94[i][j];
			T2.push_back(p94);
			p95[i*3+j] = s95[i][j];
			T2.push_back(p95);
			p96[i*3+j] = s96[i][j];
			T2.push_back(p96);
			p97[i*3+j] = s97[i][j];
			T2.push_back(p97);
			p98[i*3+j] = s98[i][j];
			T2.push_back(p98);
			p99[i*3+j] = s99[i][j];
			T2.push_back(p99);
			p100[i*3+j] = s100[i][j];
			T2.push_back(p100);
			p101[i*3+j] = s101[i][j];
			T2.push_back(p101);
			p102[i*3+j] = s102[i][j];
			T2.push_back(p102);
			p103[i*3+j] = s103[i][j];
			T2.push_back(p103);
		}	
	}

	//================================================
	bool notConverge=1;
	while(true)
	{
		notConverge = firstStage(data, mask, T1, width, height);
		if(notConverge==0) break;
		secondStage(data, mask, T2, width, height);
		for (int i = 0; i < height+2; i++)
		{
			for (int j = 0; j < width+2; j++)
				mask[i][j] = 0;
		}
	}

    //=============================================================
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