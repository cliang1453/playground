# CSCI420 Homework Assignment #1
# Date: Feb 17, 2017
# Name: Chen Liang
# ID: 7314634396
# email: lian455@usc.edu
#
# Compiled on Microsoft Visual Studio Community 2015, Version 14.0.25431.01 Update 3
# OpenGL Version: 4.5.0 NVIDIA 369.00
# OpenGL Renderer: GeForce GTX 960M/PCIe/SSE2
# Shading Language Version: 4.50 NVIDIA

========================================================================================
After enter the directory of hw1, compile and run with:

./hw1 [path/to/heightmap] [path/to/colormap] [Special Effect = y/n]

a. If using height and color information only from the height map (support both grayscale and RGBcolor): 
./hw1 [path/to/heightmap]

b. If using height and color information only from the height map (support both grayscale and RGBcolor) and special animation effect: 
./hw1 [path/to/heightmap] [n] [y]

c. If using height information from the height map and color information from the color map (support both grayscale and RGBcolor):
./hw1 [path/to/heightmap] [path/to/colormap]

d. If using height information from the height map and color information from the color map (support both grayscale and RGBcolor) and special animation effect:
./hw1 [path/to/heightmap] [path/to/colormap] [y]

========================================================================================
For this animation:

0-50 frames are compiled and ran with: 
./hw1 ./heightmap/SantaMonicaMountains-512.jpg

51-100 frames are compiled and ran with: 
./hw1 ./heightmap/SantaMonicaMountains-512.jpg ./heightmap/3.jpg

101-150 frames are compiled and ran with:
./hw1 ./heightmap/SantaMonicaMountains-512.jpg ./heightmap/1.jpg

151-300 frames are compiled and ran with: 
./hw1 ./heightmap/SantaMonicaMountains-512.jpg ./heightmap/5.jpg y

========================================================================================
Extra Credit:

a. Use element arrays and glDrawElements.
b. Support color (ImageIO::getBytesPerPixel == 3) in input images.
c. Color the vertices based on color values taken from another image of equal size. 
d. Support smooth gradients as per the core requirements.