# EE569 Homework Assignment #2
# Date: Feb 26, 2017
# Name: Chen Liang
# ID: 7314-6343-96
# email: lian455@usc.edu
#
# Compiled on linux, Ubuntu (64 bit) 14.04, Compiled using g++
# Define compiler and linker

=======================================================================================
Problem I

(a)

Using Visual Studio 2015 Community with OpenCV 2.8. Using only basic Mat::mat multiplication, inversion and division operations. Compile and run with input arguments:
[input.raw] [output.raw] [method = A/B]
-------------------------------
method |		name
-------------------------------
  A    |  Line Streching
-------------------------------
  B    |  Triangle Transformation
-------------------------------

(b)

Using Visual Studio 2015 Community with OpenCV 2.8. Using only basic Mat::mat multiplication, inversion and division operations. Compile and run with input arguments:
[Hillary.raw] [Trump.raw] [piece.raw] [Trump_output.raw] [Hillary_output.raw]

(c)

Using Visual Studio 2015 Community with OpenCV 2.8. Using only basic Mat::mat multiplication, inversion and division operations. Compile and run with input arguments:
[field.raw] [text.raw] [output.raw] [text_width] [text_height]


=======================================================================================
Problem II

(a)

g++ -g dithering.cpp -o dithering
./dithering input.raw output.raw [method = I/A] [matrixSize = 2/4/8] [colorLevel = 2/4]
-------------------------------
method |		name
-------------------------------
  I    |  Bayer index matrices
-------------------------------
  A    |  A4 dithering matrix
-------------------------------

-------------------------------
color level |		name
-------------------------------
	2		|  0, 255
-------------------------------
	4		|  0, 75, 180, 255
-------------------------------

(b)

g++ -g diffusing.cpp -o diffusing
./diffusing input.raw output.raw [method = F/JJN/S]
-------------------------------
	method	|		name
-------------------------------
	F		|  Floyd-Steinberg’s error
-------------------------------
	JJN		|  Jarvis, Judice, and Ninke (JJN)
-------------------------------
	S		|  Stucki
-------------------------------


=======================================================================================
Problem III

(a) 

g++ -g shrink.cpp -o shrink
./shrink squares.raw output.raw
The program would print as follows:

The total number of squares is 24
There are 4 sqaures with size 1 exist.
There are 1 sqaures with size 4 exist.
There are 2 sqaures with size 6 exist.
There are 9 sqaures with size 8 exist.
There are 1 sqaures with size 12 exist.
There are 1 sqaures with size 14 exist.
There are 6 sqaures with size 18 exist.

(b)

(1) Thinning
g++ -g thin.cpp -o thin
./thin letterE.raw output.raw

(2) Skeletonizing
g++ -g skel.cpp -o skel
./skel letterE.raw output.raw

(c)

(1) Hole Counting
g++ -g countHole.cpp -o countHole
./countHole board.raw
The program would print as follows:

The total number of hole is 9 

(2) Pattern Counting
g++ -g countPattern.cpp -o countPattern
./countPattern board.raw
The program would print as follows:

The number of Sqaure is 8
The number of Circle is 7
The total number of object is 15