/* **************************
 * CSCI 420
 * Assignment 3 Raytracer
 * Name: Chen Liang
 * *************************
*/

#ifdef WIN32
  #include <windows.h>
#endif

#if defined(WIN32) || defined(linux)
  #include <GL/gl.h>
  #include <GL/glut.h>
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
  #define strcasecmp _stricmp
#endif

#include <imageIO.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace glm;

#define MAX_TRIANGLES 20000
#define MAX_SPHERES 100
#define MAX_LIGHTS 100

char * filename = NULL;

//different display modes
#define MODE_DISPLAY 1
#define MODE_JPEG 2

int mode = MODE_DISPLAY;

//you may want to make these smaller for debugging purposes
//#define WIDTH 640
//#define HEIGHT 480
#define WIDTH 640
#define HEIGHT 480


//the field of view of the camera
#define fov 60.0
#define PI 3.14159265

//the x, y, z normal vector
#define normal_X vec3(1, 0, 0)
#define normal_Y vec3(0, 1, 0)
#define normal_Z vec3(0, 0, 1)

//background color 
#define background vec3(255,255,255)

//define parameters of image plane
const float angle_in_degree = fov * PI / 180.0;
const float aspect_ratio = (float(WIDTH) / float(HEIGHT));
const float x_left = -aspect_ratio * tan(angle_in_degree / 2.0);
const float x_right = aspect_ratio * tan(angle_in_degree / 2.0);
const float y_bottom = -tan(angle_in_degree / 2.0);
const float y_up = tan(angle_in_degree / 2.0);

int depth_max = 0;
bool anti_aliasing = 0;
unsigned char buffer[HEIGHT][WIDTH][3];

struct pixelPoint
{
	vec3 intersection_position;
	vec3 normal;
	int geometry_ID;
	string geometry_name;
	vec2 pixel_position;
	vec3 rgbValue;
	bool is_shadow;
	vec3 baryParam;
};

struct Vertex
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double normal[3];
  double shininess;
};

struct Triangle
{
  Vertex v[3];
};

struct Sphere
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double shininess;
  double radius;
};

struct Light
{
  double position[3];
  double color[3];
};

Triangle triangles[MAX_TRIANGLES];
Sphere spheres[MAX_SPHERES];
Light lights[MAX_LIGHTS];
double ambient_light[3];

int num_triangles = 0;
int num_spheres = 0;
int num_lights = 0;

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);


float check_sphere_intersection(vec3 &ray_ori, vec3 &ray_dir, int i, vec3 &N)
{
	float b = 0, c = 0, tmin = -1, t0, t1;
	vec3 sphere_center(spheres[i].position[0], spheres[i].position[1], spheres[i].position[2]);

	for (int j = 0; j < 3; j++)
	{
		b += 2 * ray_dir[j] * (ray_ori[j] - spheres[i].position[j]);
		c += pow((ray_ori[j] - spheres[i].position[j]), 2);
	}
	c -= pow(spheres[i].radius, 2);

	if (pow(b, 2) - 4.0 * c >= 0)
	{
		t0 = (-b + sqrt(pow(b, 2) - 4.0 * c)) / 2.0;
		t1 = (-b - sqrt(pow(b, 2) - 4.0 * c)) / 2.0;

		if (t0 > 0.001 && t1 > 0.001) tmin = min(t0, t1);
		else if (t0 < 0.001 && t1 < 0.001) return -1;
		else tmin = max(t0, t1); // if inside of the sphere

		N = normalize(ray_ori + tmin * ray_dir - sphere_center);
		return tmin;
	}
	else return -1;
}

float check_triangle_intersection(pixelPoint& curr, vec3 &ray_ori, vec3 &ray_dir, int i, vec3 &N)
{
	Vertex A = triangles[i].v[0];
	Vertex B = triangles[i].v[1];
	Vertex C = triangles[i].v[2];
	vec3 AB(A.position[0] - B.position[0], A.position[1] - B.position[1], A.position[2] - B.position[2]);
	vec3 AC(A.position[0] - C.position[0], A.position[1] - C.position[1], A.position[2] - C.position[2]);
	vec3 N0 = normalize(cross(AB, AC));

	float d = -(N0[0] * A.position[0] + N0[1] * A.position[1] + N0[2] * A.position[2]);

	float denominator = N0[0] * ray_dir[0] + N0[1] * ray_dir[1] + N0[2] * ray_dir[2];
	float numerator = -(N0[0] * ray_ori[0] + N0[1] * ray_ori[1] + N0[2] * ray_ori[2] + d);
	if (denominator == 0) return -1;
	else if (numerator / denominator <= 0.001) return -1;
	else //check point-in-triangle
	{
		float t = numerator / denominator;
		vec3 I = ray_ori + t*ray_dir;

		//project the triangle to 2D plane to avoid complex calculation
		vec2 A_proj, B_proj, C_proj, I_proj;
		if (dot(N0, normal_X) >= 1 || dot(N0, normal_X) <= -1) //can be project to (y,z) plane
		{
			A_proj = vec2(A.position[1], A.position[2]);
			B_proj = vec2(B.position[1], B.position[2]);
			C_proj = vec2(C.position[1], C.position[2]);
			I_proj = vec2(I[1], I[2]);
		}
		else if(dot(N0, normal_Y) >= 1 || dot(N0, normal_Y) <= -1) //project to (x,z) plane instead
		{
			A_proj = vec2(A.position[0], A.position[2]);
			B_proj = vec2(B.position[0], B.position[2]);
			C_proj = vec2(C.position[0], C.position[2]);
			I_proj = vec2(I[0], I[2]);
		}
		else//project to (x,y) plane instead 
		{
			A_proj = vec2(A.position[0], A.position[1]);
			B_proj = vec2(B.position[0], B.position[1]);
			C_proj = vec2(C.position[0], C.position[1]);
			I_proj = vec2(I[0], I[1]);
		}

		//perform the barycentric test
		float area_ABC = 0.5 * ((B_proj[0] - A_proj[0])*(C_proj[1] - A_proj[1]) - (C_proj[0] - A_proj[0])*(B_proj[1] - A_proj[1]));
		float area_IBC = 0.5 * ((B_proj[0] - I_proj[0])*(C_proj[1] - I_proj[1]) - (C_proj[0] - I_proj[0])*(B_proj[1] - I_proj[1]));
		float area_AIC = 0.5 * ((I_proj[0] - A_proj[0])*(C_proj[1] - A_proj[1]) - (C_proj[0] - A_proj[0])*(I_proj[1] - A_proj[1]));
		float alpha = area_IBC / area_ABC;
		float beta = area_AIC / area_ABC;
		float gamma = 1 - alpha - beta;

		
		//if the intersection point is tested in the triangle, return
		if ((alpha > -0.001 && beta > -0.001 && gamma > -0.001) || (alpha < 0.001 && beta < 0.001 && gamma < 0.001))
		{
			//Calculate new normal using normal interpolation
			vec3 NA(A.normal[0], A.normal[1], A.normal[2]);
			vec3 NB(B.normal[0], B.normal[1], B.normal[2]);
			vec3 NC(C.normal[0], C.normal[1], C.normal[2]);
			N = normalize(alpha * NA + beta * NB + gamma * NC);

			//Store Barrycentric values for future use 
			curr.baryParam = vec3(alpha, beta, gamma);
			return t;
		}
		else return -1;
	}
}

void check_shadow(pixelPoint& curr, vec3 L, vec3 light_pos)
{
	float temp_t;
	vec3 idle;
	for (int i = 0; i < num_spheres; i++)
	{
		temp_t = check_sphere_intersection(curr.intersection_position, L, i, idle);
		if (temp_t == -1) continue;
		else
		{
			//avoid the case it intersect with the same point on it's own
			if (length(temp_t*L) > 0.001 && length(temp_t*L) < length(light_pos - curr.intersection_position) - 0.001)
			{
				curr.is_shadow = true;
				return;
			}
		}
	}

	//vec3 temp_bary = curr.baryParam;
	for (int i = 0; i < num_triangles; i++)
	{
		temp_t = check_triangle_intersection(curr, curr.intersection_position, L, i, idle);
		//curr.baryParam = temp_bary;
		if (temp_t == -1) continue;
		else
		{
			if (length(temp_t*L) > 0.001 && length(temp_t*L) < length(light_pos - curr.intersection_position) - 0.001)
			{
				curr.is_shadow = true;
				return;
			}
		}
	}

	curr.is_shadow = false;
	return;
}

void calc_coefficient(pixelPoint& curr, float &sh, vec3 &kd, vec3 &ks)
{
	int i = curr.geometry_ID;

	if (curr.geometry_name == "triangle")
	{
		//cout << "Reflect on the floor! " << endl;
		vec3 bary = curr.baryParam;
		Vertex A = triangles[i].v[0], B = triangles[i].v[1], C = triangles[i].v[2];
		sh = bary[0] * A.shininess + bary[1] * B.shininess + bary[2] * C.shininess;

		vec3 A_d(A.color_diffuse[0], A.color_diffuse[1], A.color_diffuse[2]);
		vec3 B_d(B.color_diffuse[0], B.color_diffuse[1], B.color_diffuse[2]);
		vec3 C_d(C.color_diffuse[0], C.color_diffuse[1], C.color_diffuse[2]);
		kd = bary[0] * A_d + bary[1] * B_d + bary[2] * C_d;

		vec3 A_s(A.color_specular[0], A.color_specular[1], A.color_specular[2]);
		vec3 B_s(B.color_specular[0], B.color_specular[1], B.color_specular[2]);
		vec3 C_s(C.color_specular[0], C.color_specular[1], C.color_specular[2]);
		ks = bary[0] * A_s + bary[1] * B_s + bary[2] * C_s;
	}
	else
	{
		sh = spheres[i].shininess;
		kd = vec3(spheres[i].color_diffuse[0], spheres[i].color_diffuse[1], spheres[i].color_diffuse[2]);
		ks = vec3(spheres[i].color_specular[0], spheres[i].color_specular[1], spheres[i].color_specular[2]);
	}
}

void calc_illumination(pixelPoint& curr, vec3 L, vec3 V, vec3 R, float sh, vec3 kd, vec3 ks, int i)
{
	vec3 light_color(lights[i].color[0], lights[i].color[1], lights[i].color[2]);
	float R_V = dot(R, V);
	if (R_V < 0) R_V = 0;
	float L_N = dot(L, curr.normal);
	if (L_N < 0) L_N = 0;
	curr.rgbValue += 255.0f * light_color * (kd * L_N + ks * pow(R_V, sh));
}

vec3 Trace(vec3 ray_ori, vec3 ray_dir, int depth)
{
	if(depth > depth_max)
		return vec3(0,0,0);

	pixelPoint curr;
	//Detect Intersections
	vec3 temp_normal, normal;
	float temp_t, t = 1000000; 
	int sph_ID = -1, tri_ID = -1;

	for (int i = 0; i < num_spheres; i++)
	{
		temp_t = check_sphere_intersection(ray_ori, ray_dir, i, temp_normal);
		if (temp_t != -1 && temp_t < t)
		{
			t = temp_t;
			normal = temp_normal;
			sph_ID = i;
		}
		else continue;
	}

	for (int i = 0; i < num_triangles; i++)
	{
		temp_t = check_triangle_intersection(curr, ray_ori, ray_dir, i, temp_normal);
		if (temp_t != -1 && temp_t < t)
		{
			t = temp_t;
			normal = temp_normal;
			tri_ID = i;
		}
		else continue;
	}

	//If no intersection detected, return the background color
	if (t == 1000000)
	{
		if (depth == 0)
			return background;
		else
			return vec3(0,0,0);
	}

	//If intersection is detected
	//Set the parameters for the current point
	curr.intersection_position = ray_ori + t * ray_dir;
	curr.normal = normal;
	if (tri_ID != -1)
	{
		curr.geometry_name = "triangle";
		curr.geometry_ID = tri_ID;
	}
	else
	{
		curr.geometry_name = "sphere";
		curr.geometry_ID = sph_ID;
	}  
	curr.rgbValue = vec3(0,0,0);
	vec3 V, R, L, kd, ks, R_trace;
	float sh;

	//Calculate the illumination
	for (int i = 0; i < num_lights; i++)
	{
		 //Calculate L, V, N, R
		 vec3 light_pos(lights[i].position[0], lights[i].position[1], lights[i].position[2]);
		 L = normalize(light_pos - curr.intersection_position);
		 V = normalize(ray_ori - curr.intersection_position);
		 R = normalize(2 * dot(L, curr.normal)* curr.normal - L);

			 calc_coefficient(curr, sh, kd, ks);
			 //Check if the intersection is in shadow
			 check_shadow(curr, L, light_pos);

			 //If the intersection is not in shadow, calculate the illumination
			 if (!curr.is_shadow)
				 calc_illumination(curr, L, V, R, sh, kd, ks, i);
	}

	vec3 currColor = curr.rgbValue;

	R_trace = normalize(2 * dot(V, curr.normal)* curr.normal - V);
	ray_ori = curr.intersection_position;

	if (depth_max == 0)
		currColor += Trace(ray_ori, R_trace, depth + 1);
	else
		currColor += ks * Trace(ray_ori, R_trace, depth + 1);

	return currColor;
}

//MODIFY THIS FUNCTION
void draw_scene()
{
	float z = -1.0;
	float x[5];
	float y[5];
	vec3 ray_ori(0, 0, 0);
	vector<pixelPoint> imagePoints;


	for (unsigned int i = 0; i < WIDTH; i++)
	{
		imagePoints.clear();
		for (unsigned int j = 0; j < HEIGHT; j++)
		{
			//Check the closet intersection
			pixelPoint curr;
			curr.pixel_position = vec2(i, j);
			int ray_number_per_pixel = 1;

			x[0] = (x_right - x_left) * (float(i + 0.5) / float(WIDTH)) + x_left;
			y[0] = (y_up - y_bottom) * (float(j + 0.5) / float(HEIGHT)) + y_bottom;

			if(anti_aliasing)
			{
				ray_number_per_pixel = 5;
				x[1] = (x_right - x_left) * (float(i + 0.25) / float(WIDTH)) + x_left;
				y[1] = (y_up - y_bottom) * (float(j + 0.75) / float(HEIGHT)) + y_bottom;
				x[2] = (x_right - x_left) * (float(i + 0.75) / float(WIDTH)) + x_left;
				y[2] = (y_up - y_bottom) * (float(j + 0.25) / float(HEIGHT)) + y_bottom;
				x[3] = (x_right - x_left) * (float(i + 0.75) / float(WIDTH)) + x_left;
				y[3] = (y_up - y_bottom) * (float(j + 0.75) / float(HEIGHT)) + y_bottom;
				x[4] = (x_right - x_left) * (float(i + 0.25) / float(WIDTH)) + x_left;
				y[4] = (y_up - y_bottom) * (float(j + 0.25) / float(HEIGHT)) + y_bottom;
			}
			
			vec3 ray_dir;
			vec3 rgb = vec3(0, 0, 0);

			for (int k = 0; k < ray_number_per_pixel; k++)
			{
				ray_dir = normalize(vec3(x[k], y[k], z));
				vec3 temp_trace = Trace(ray_ori, ray_dir, 0);
				if (temp_trace != background) temp_trace += 255.0f * vec3(ambient_light[0], ambient_light[1], ambient_light[2]);
				rgb += temp_trace;
			}



			curr.rgbValue = (float(1.0)/float(ray_number_per_pixel)) * rgb;
			if (curr.rgbValue.x > 255) curr.rgbValue.x = 255;
			if (curr.rgbValue.y > 255) curr.rgbValue.y = 255;
			if (curr.rgbValue.z > 255) curr.rgbValue.z = 255;

			imagePoints.push_back(curr);
		}

		glPointSize(2.0);
		glBegin(GL_POINTS);
		for (unsigned int m = 0; m<HEIGHT; m++)
		{
			plot_pixel(int(imagePoints[m].pixel_position.x), 
				int(imagePoints[m].pixel_position.y),
				int(imagePoints[m].rgbValue.x) % 256,
				int(imagePoints[m].rgbValue.y) % 256, 
				int(imagePoints[m].rgbValue.z) % 256);
		}
		glEnd();
		glFlush();
	}

  printf("Done!\n"); 
  fflush(stdout);
}

void plot_pixel_display(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  glColor3f(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f);
  glVertex2i(x,y);
}

void plot_pixel_jpeg(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  buffer[y][x][0] = r;
  buffer[y][x][1] = g;
  buffer[y][x][2] = b;
}

void plot_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  plot_pixel_display(x,y,r,g,b);
  if(mode == MODE_JPEG)
    plot_pixel_jpeg(x,y,r,g,b);
}

void save_jpg()
{
  printf("Saving JPEG file: %s\n", filename);

  ImageIO img(WIDTH, HEIGHT, 3, &buffer[0][0][0]);
  if (img.save(filename, ImageIO::FORMAT_JPEG) != ImageIO::OK)
    printf("Error in Saving\n");
  else 
    printf("File saved Successfully\n");
}

void parse_check(const char *expected, char *found)
{
  if(strcasecmp(expected,found))
  {
    printf("Expected '%s ' found '%s '\n", expected, found);
    printf("Parse error, abnormal abortion\n");
    exit(0);
  }
}

void parse_doubles(FILE* file, const char *check, double p[3])
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check(check,str);
  fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]);
  printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
}

void parse_rad(FILE *file, double *r)
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check("rad:",str);
  fscanf(file,"%lf",r);
  printf("rad: %f\n",*r);
}

void parse_shi(FILE *file, double *shi)
{
  char s[100];
  fscanf(file,"%s",s);
  parse_check("shi:",s);
  fscanf(file,"%lf",shi);
  printf("shi: %f\n",*shi);
}

int loadScene(char *argv)
{
  FILE * file = fopen(argv,"r");
  int number_of_objects;
  char type[50];
  Triangle t;
  Sphere s;
  Light l;
  fscanf(file,"%i", &number_of_objects);

  printf("number of objects: %i\n",number_of_objects);

  parse_doubles(file,"amb:",ambient_light);

  for(int i=0; i<number_of_objects; i++)
  {
    fscanf(file,"%s\n",type);
    printf("%s\n",type);
    if(strcasecmp(type,"triangle")==0)
    {
      printf("found triangle\n");
      for(int j=0;j < 3;j++)
      {
        parse_doubles(file,"pos:",t.v[j].position);
        parse_doubles(file,"nor:",t.v[j].normal);
        parse_doubles(file,"dif:",t.v[j].color_diffuse);
        parse_doubles(file,"spe:",t.v[j].color_specular);
        parse_shi(file,&t.v[j].shininess);
      }

      if(num_triangles == MAX_TRIANGLES)
      {
        printf("too many triangles, you should increase MAX_TRIANGLES!\n");
        exit(0);
      }
      triangles[num_triangles++] = t;
    }
    else if(strcasecmp(type,"sphere")==0)
    {
      printf("found sphere\n");

      parse_doubles(file,"pos:",s.position);
      parse_rad(file,&s.radius);
      parse_doubles(file,"dif:",s.color_diffuse);
      parse_doubles(file,"spe:",s.color_specular);
      parse_shi(file,&s.shininess);

      if(num_spheres == MAX_SPHERES)
      {
        printf("too many spheres, you should increase MAX_SPHERES!\n");
        exit(0);
      }
      spheres[num_spheres++] = s;
    }
    else if(strcasecmp(type,"light")==0)
    {
      printf("found light\n");
      parse_doubles(file,"pos:",l.position);
      parse_doubles(file,"col:",l.color);

      if(num_lights == MAX_LIGHTS)
      {
        printf("too many lights, you should increase MAX_LIGHTS!\n");
        exit(0);
      }
      lights[num_lights++] = l;
    }
    else
    {
      printf("unknown type in scene description:\n%s\n",type);
      exit(0);
    }
  }
  return 0;
}

void display()
{
}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void idle()
{
  //hack to make it only draw once
  static int once=0;
  if(!once)
  {
    draw_scene();
    if(mode == MODE_JPEG)
      save_jpg();
  }
  once=1;
}

int main(int argc, char ** argv)
{
  if ((argc < 2) || (argc > 5))
  {  
    printf ("Usage: %s <input scenefile> [output jpegname] [depth = 0/1/2/3/4...] [anti_aliasing = 0/1]\n", argv[0]);
    exit(0);
  }
  if(argc == 2)
  {
  	mode = MODE_DISPLAY;
  }
  else if(argc >= 3)
  {
    mode = MODE_JPEG;
    filename = argv[2];
    if(argc >= 4)
    {
    	if(atoi(argv[4]) >=0)
    		depth_max = atoi(argv[4]);
    	else
    	{
    		printf ("The input of depth value is invalid. The depth value should be positive.");
    		exit(0);
    	}

    	if(argc == 5)
    	{
    		if (atoi(argv[4]) == 1)
    			anti_aliasing = true;
    		else anti_aliasing = false;
    	}
    }
  }

  glutInit(&argc,argv);
  loadScene(argv[1]);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}

