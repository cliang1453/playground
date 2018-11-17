Subject 	: CSCI420 - Computer Graphics 
Assignment 2: Simulating a Roller Coaster
Author		: < Chen Liang >
USC ID 		: < 7314634396 >

Description: In this assignment, we use Catmull-Rom splines along with OpenGL texture mapping to create a roller coaster simulation.

Compilation Instruction:
======================
1. Compiled on Microsoft Visual Studio Community 2015, Version 14.0.25431.01 Update 3

2. Compile and run with:
./hw1 track.txt [GRAVITY_MODE = 1/0]

-------------------------------
	argv[2]  |      Mode
-------------------------------
	   1     |  GRAVITY_MODE
-------------------------------
	   0   	 | SUBDIVISION_MODE
-------------------------------

Core Credit Features: (Answer these Questions with Y/N; you can also insert comments as appropriate)
======================

1. Uses OpenGL core profile, version 3.2 or higher - Y

2. Completed all Levels:
  Level 1 : - Y
  level 2 : - Y
  Level 3 : - Y
  Level 4 : - Y
  Level 5 : - Y

3. Used Catmull-Rom Splines to render the Track - Y

4. Rendered a Rail Cross Section - Y

5. Rendered the camera at a reasonable speed in a continuous path/orientation - Y

6. Run at interactive frame rate (>15fps at 1280 x 720) - Y

7. Understandably written, well commented code - Y

8. Attached an Animation folder containing not more than 1000 screenshots - Y

9. Attached this ReadMe File - Y

Extra Credit Features: (Answer these Questions with Y/N; you can also insert comments as appropriate)
======================

1. Render a T-shaped rail cross section - Y

2. Render a Double Rail - Y

3. Made the track circular and closed it with C1 continuity -

4. Added OpenGl lighting -

5. Any Additional Scene Elements? (list them here)

6. Generate track from several sequences of splines - Y

7. Draw splines using recursive subdivision - Y

8. Modify velocity with which the camera moves - Y

9. Create tracks that mimic a real world coaster - 

10. Render environment in a better manner - Y

Additional Features: (Please document any additional features you may have implemented other than the ones described above)
1. 
2.

Open-Ended Problems: (Please document approaches to any open-ended problems that you have tackled)
1. 
2.

Keyboard/Mouse controls: (Please document Keyboard/Mouse controls if any)
1.
2.

Names of the .cpp files you made changes to:
1. hw1.cpp
2. texturePipelineProgram.cpp

Comments : (If any)
Since I generate tracks from several sequences of splines, those splines may cross each other. At the end of each track, the camera would jump from the ending point to the beginning point the next spline to create a smooth transition. 