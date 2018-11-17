Assignment #3: Ray tracing

FULL NAME: Chen Liang


MANDATORY FEATURES
------------------

<Under "Status" please indicate whether it has been implemented and is
functioning correctly.  If not, please explain the current status.>

Command Line Instruction 
------------------------
./program_name input_file.scene output_image.jpg [recursion_depth = 0/1/2/3/4 (default = 0)] [anti_aliasing = 0/1 (default = 0)]

Feature:                                 Status: finish? (yes/no)
-------------------------------------    -------------------------
1) Ray tracing triangles                  yes

2) Ray tracing sphere                     yes

3) Triangle Phong Shading                 yes

4) Sphere Phong Shading                   yes

5) Shadows rays                           yes

6) Still images                           yes
   
7) Extra Credit (up to 20 points)
   1) Using recusive reflection. The reflection term is weighted by ks, which is the specular coefficient.2) Using fixed supersampling for anti-aliasing. For each pixel, I shot 5 rays in total. One of them is shot from center of the pixel. We divide the pixel into 4 grids, the other 4 rays are shot from the center of each grid. 
