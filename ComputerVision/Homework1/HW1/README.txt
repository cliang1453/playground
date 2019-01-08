# EE569 Homework Assignment #1
# Date: Feb 4, 2017
# Name: Chen Liang
# ID: 7314634396
# email: lian455@usc.edu
#
# Compiled on linux, Ubuntu (64 bit) 14.04, Compiled using g++
# Define compiler and linker

Problem I 
a)	g++ -g resizing.cpp -o resizing
	./resizing [input_image] [resize200_image] [resize400_image] [resize600_image] [Bytesperpixel = 1] [size = 300]
b)	g++ -g mirroring.cpp -o mirroring
	./mirroring [input_image] [mirrored_image] [Bytesperpixel = 1] [size = 300]
c)	g++ -g compositing.cpp -o compositing
	./compositing [dog_image] [beach_image] [composite_image] [threshould = 30] [Bytesperpixel = 1]
d)  g++ -g cmycolorspace.cpp -o cmycolorspace
	./cmycolorspace [input_image] [c] [m] [y] [Bytesperpixel = 1] [width] [height]
e)	g++ -g hslcolorspace.cpp -o hslcolorspace
	./hslcolorspace [input_image] [h] [s] [l] [Bytesperpixel = 1] [width] [height]
f)	g++ -g sepia.cpp -o sepia
	./sepia [input_image] [intermeida_image] [sepiafiltered_image] [Bytesperpixel = 3][width][height]
g)	g++ -g screenblending.cpp -o screenblending
	./screenblending [top_layer] [bottom_layer] [blended_image] [Bytesperpixel = 3][width][height]


Problem II 
a)	g++ -g histEnhance.cpp -o histEnhance
	./histEnhance [input_image] [equalized_image] [method = A/B] [width] [height]
	-----------------------------
	  Method |                  
	-----------------------------
	     A   | Linear Stretching  
	-----------------------------
	     B   |   CDF Mapping
	-----------------------------
b)  g++ -g colorEqual.cpp -o colorEqual
	./colorEqual [input_image] [equalized_image] [method = A/B] [preserve_saturation_hue = 1/0] [Bytesperpixel = 1] [width] [height]
	-----------------------------
	  Method |                  
	-----------------------------
	     A   | Linear Stretching  
	-----------------------------
	     B   |   CDF Mapping
	-----------------------------
c)	g++ -g histTrans.cpp -o histTrans
	./histTrans [input_image] [equalized_image] [Bytesperpixel = 1] [width] [height]


Problem III
a)	 g++ -g mixnoise.cpp -o mixnoise
	./mixnoise [noise_image] [noise_free_image] [filtered_image] [method = A/B/C/D/E/F/G/N] [filter_size = 3/5/7] [sigma = 1/2/3] [BytesPerPixel = 1] [Width = 512] [Height = 512]
	-----------------------------
	  Method | Filter (G = Gaussian Filter; M = Median Filter)                 
	-----------------------------
	     A   |        G
	-----------------------------
	     B   |  	  M
	-----------------------------
	     C   |       M+G
	-----------------------------
	     D   |   	 G+M
	-----------------------------
	     E   | 		M+G+G  
	-----------------------------
	     F   |   	G+M+M
	-----------------------------
	     G   | 	   M+M+G+G  
	-----------------------------
	     N   | 	   no filter
	-----------------------------
b) 	g++ -g nlm.cpp nlm
	./nlm [noise_image] [noise_free_image] [filtered_image] [filter_size = 3/5/7] [search_size = 21/41/61] [filter_dergree(h) = 40/60/80] [Gaussian std(a) = 1/2/3] [BytesPerPixel = 1] [Width = 512] [Height = 512]
c) Open BM3D_Main.m file in MATLAB 2015b and tuning the value of sigma and include the noisy image in the directory, then directly run in the command window.
	