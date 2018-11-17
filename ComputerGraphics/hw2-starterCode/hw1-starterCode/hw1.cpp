/*
CSCI 420 Computer Graphics, USC
Assignment 2: Roller Coaster
C++ starter code

Student username: <lian455>
*/

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include "openGLHeader.h"
#include "glutHeader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imageIO.h"
#include "openGLMatrix.h"
#include "basicPipelineProgram.h"
#include "texturePipelineProgram.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "glew32d.lib")
#else
#pragma comment(lib, "glew32.lib")
#endif
#endif

#ifdef WIN32
char shaderBasePath[1024] = SHADER_BASE_PATH;
#else
char shaderBasePath[1024] = "../openGLHelper-starterCode";
#endif

using namespace std;
using namespace glm;


int mousePos[2]; // x,y coordinate of the mouse position

int leftMouseButton = 0; // 1 if pressed, 0 if not 
int middleMouseButton = 0; // 1 if pressed, 0 if not
int rightMouseButton = 0; // 1 if pressed, 0 if not

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROL_STATE;
CONTROL_STATE controlState = ROTATE;

// state of the world
float landRotate[3] = { 0.0f, 0.0f, 0.0f };
float landTranslate[3] = { 0.0f, 0.0f, 0.0f };
float landScale[3] = { 1.0f, 1.0f, 1.0f };

int windowWidth = 1280;
int windowHeight = 720;
char windowTitle[512] = "CSCI 420 homework I";

// represents one control point along the spline 
struct Point
{
	double x;
	double y;
	double z;
};

// spline struct 
// contains how many control points the spline has, and an array of control points 
struct Spline
{
	int numControlPoints;
	Point * points;
};

//initialize spline variables 
OpenGLMatrix * matrix;
GLuint left_spline_vao;
GLuint right_spline_vao;
GLuint bar_spline_vao;
GLuint left_spline_vbo;
GLuint right_spline_vbo;
GLuint bar_spline_vbo;
GLuint left_element_vbo;
GLuint right_element_vbo;
GLuint bar_element_vbo;
GLuint left_program;
GLuint right_program;
GLuint bar_program;
BasicPipelineProgram * left_pipelineProgram;
BasicPipelineProgram * right_pipelineProgram;
BasicPipelineProgram * bar_pipelineProgram;
vector<float> position;
vector<float> tangent;
float* leftcolor;
float* rightcolor;
float* leftRail;
float* rightRail;
vector<float> barRail;
vector<float> barcolor;
vector<GLuint> indices_leftRail;
vector<GLuint> indices_rightRail;
vector<GLuint> indices_barRail;
int frameCount;
// the spline array 
Spline * splines;
// total number of splines 
int numSplines;
int size_point;
int size_rail_points;
int size_bar_points;
float s = 0.5;
int* pulse;
bool GRAVITY_MODE = 0;

//initialize ground variables
ImageIO* groundImage;
GLuint groundTexHandle;
GLuint ground_vao;
GLuint ground_vbo;
GLuint ground_program;
TexturePipelineProgram * pipelineProgram2;
vector<float> texCord;
vector<float> groundPos;
int size_ground_point;
int size_text;

//initialize sky variables
ImageIO* skyImage;
GLuint skyTexHandle;
GLuint sky_vao;
GLuint sky_vbo;
GLuint sky_program;
TexturePipelineProgram * pipelineProgram3;
vector<float> texCord2;
vector<float> skyPos;
int size_sky_point;
int size_text2;

//camera parameter
const int sliding_speed = 15;
int current_pos = 0;
const vec3 V_START(0, 1, 0);
const vec3 V_HORIZON(1, 0, 0);
const vec3 V_DOWN(0, -1, 0);
const vec3 V_UP(0, 1, 0);
vec3 B;


// write a screenshot to the specified filename
void saveScreenshot(const char * filename)
{
	unsigned char * screenshotData = new unsigned char[windowWidth * windowHeight * 3];
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, screenshotData);

	ImageIO screenshotImg(windowWidth, windowHeight, 3, screenshotData);

	if (screenshotImg.save(filename, ImageIO::FORMAT_JPEG) == ImageIO::OK)
		cout << "File " << filename << " saved successfully." << endl;
	else cout << "Failed to save file " << filename << '.' << endl;

	delete[] screenshotData;
}

void determineCameraView(const int k, int &pos)
{
	float tan[3] = { tangent[pos], tangent[pos + 1], tangent[pos + 2] };
	vec3 T = make_vec3(tan);
	T = normalize(T);

	vec3 N;
	if (pos == 0)
	{
		// we want a roller coaster that settle on the ground (y axis is the vertical direction)
		vec3 V = normalize(V_START);
		N = V;
		//if the roller coaster is initially direct in y direction
		if (T == V_UP || T == V_DOWN)
		{
			N = V_HORIZON;
		}
	}
	else
	{
		N = cross(B, T);
	}
	N = normalize(N);
	B = cross(T, N);
	B = normalize(B);

	matrix->LookAt(position[pos] + N[0] * 0.05, position[pos + 1] + N[1] * 0.05, position[pos + 2] + N[2] * 0.05,
		position[pos] + T[0] * 5, position[pos + 1] + T[1] * 5, position[pos + 2] + T[2] * 5,
		N[0], N[1], N[2]); // default camera
	pos = pos + k;
}

void bindProgram()
{
	//Set up Perspective Matrix
	matrix->SetMatrixMode(OpenGLMatrix::Projection);
	float p[16]; // column major 
	matrix->GetMatrix(p);

	//Set up Modelview Matirx
	matrix->SetMatrixMode(OpenGLMatrix::ModelView);
	matrix->LoadIdentity();
	determineCameraView(sliding_speed, current_pos);
	//matrix->LookAt(10, 10, 10, 0, 0, -1, 0, 1, 0);
	matrix->Translate(landTranslate[0], landTranslate[1], landTranslate[2]);
	matrix->Rotate(landRotate[0], 1, 0, 0);
	matrix->Rotate(landRotate[1], 0, 1, 0);
	matrix->Rotate(landRotate[2], 0, 0, 1);
	matrix->Scale(landScale[0], landScale[1], landScale[2]);
	float m[16]; // column major
	matrix->GetMatrix(m);

	//upload perspective and Modelview matrix
	left_pipelineProgram->Bind();
	left_pipelineProgram->SetModelViewMatrix(m);
	left_pipelineProgram->SetProjectionMatrix(p);

	right_pipelineProgram->Bind();
	right_pipelineProgram->SetModelViewMatrix(m);
	right_pipelineProgram->SetProjectionMatrix(p);

	bar_pipelineProgram->Bind();
	bar_pipelineProgram->SetModelViewMatrix(m);
	bar_pipelineProgram->SetProjectionMatrix(p);

	pipelineProgram2->Bind();
	pipelineProgram2->SetModelViewMatrix(m);
	pipelineProgram2->SetProjectionMatrix(p);

	pipelineProgram3->Bind();
	pipelineProgram3->SetModelViewMatrix(m);
	pipelineProgram3->SetProjectionMatrix(p);
}

void renderElementArray(GLuint element_vbo, int numberOfVertices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
	glDrawElements(GL_TRIANGLES, numberOfVertices, GL_UNSIGNED_INT, (void*)0);
}

void renderTriangle(int numberOfVertices)
{
	GLint first = 0;
	glDrawArrays(GL_TRIANGLES, first, numberOfVertices);
}

void setTextureUnit(GLint unit, GLuint program)
{
	glActiveTexture(unit); // select the active texture unit
						   // get a handle to the “textureImage” shader variable
	GLint h_textureImage = glGetUniformLocation(program, "textureImage");
	// deem the shader variable “textureImage” to read from texture unit “unit”
	glUniform1i(h_textureImage, unit - GL_TEXTURE0);
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind pespective matrix and modelview matrix to pipeline program
	bindProgram();

	//render left spline
	left_pipelineProgram->Bind();
	glBindVertexArray(left_spline_vao); //bind the VAO
	renderElementArray(left_element_vbo, indices_leftRail.size());
	glBindVertexArray(0); //unbind the VAO

						  //render right spline
	right_pipelineProgram->Bind();
	glBindVertexArray(right_spline_vao); //bind the VAO
	renderElementArray(right_element_vbo, indices_rightRail.size());
	glBindVertexArray(0); //unbind the VAO

						  //render bar spline
	bar_pipelineProgram->Bind();
	glBindVertexArray(bar_spline_vao); //bind the VAO
	renderElementArray(bar_element_vbo, indices_barRail.size());
	glBindVertexArray(0); //unbind the VAO

						  //render ground
	pipelineProgram2->Bind();
	setTextureUnit(GL_TEXTURE0, ground_program); // it is safe to always use GL_TEXTURE0
	glBindTexture(GL_TEXTURE_2D, groundTexHandle);
	glBindVertexArray(ground_vao); //bind the VAO
	renderTriangle(size_ground_point);
	glBindVertexArray(0); //unbind the VAO

						  //render sky
	pipelineProgram3->Bind();
	setTextureUnit(GL_TEXTURE0, sky_program); // it is safe to always use GL_TEXTURE0
	glBindTexture(GL_TEXTURE_2D, skyTexHandle);
	glBindVertexArray(sky_vao); //bind the VAO
	renderTriangle(size_sky_point);
	glBindVertexArray(0); //unbind the VAO

	glutSwapBuffers();
}

void idleFunc()
{
	frameCount++;
	if (frameCount % 4 == 0 && frameCount % 4 < 800)
	{
		  char filename[10];
		  sprintf(filename, "%03d", frameCount/4);
		  saveScreenshot((std::string(filename) + ".jpg").c_str());
	}
	
	glutPostRedisplay();
}

void reshapeFunc(int w, int h)
{
	float aspect = (float)w / (float)h;
	glViewport(0, 0, w, h);

	// setup perspective matrix...
	matrix->SetMatrixMode(OpenGLMatrix::Projection);
	matrix->LoadIdentity();
	matrix->Perspective(60, 1.0*w / h, 0.01, 1000.0);
	// setup modelview matrix..
	matrix->SetMatrixMode(OpenGLMatrix::ModelView);
}

void mouseMotionDragFunc(int x, int y)
{
	// mouse has moved and one of the mouse buttons is pressed (dragging)
	// the change in mouse position since the last invocation of this function
	int mousePosDelta[2] = { x - mousePos[0], y - mousePos[1] };

	switch (controlState)
	{
		// translate the landscape
	case TRANSLATE:
		if (leftMouseButton)
		{
			// control x,y translation via the left mouse button
			landTranslate[0] += mousePosDelta[0] * 0.01f;
			landTranslate[1] -= mousePosDelta[1] * 0.01f;
		}
		if (middleMouseButton)
		{
			// control z translation via the middle mouse button
			landTranslate[2] += mousePosDelta[1] * 0.01f;
		}
		break;

		// rotate the landscape
	case ROTATE:
		if (leftMouseButton)
		{
			// control x,y rotation via the left mouse button
			landRotate[0] += mousePosDelta[1];
			landRotate[1] += mousePosDelta[0];
		}
		if (middleMouseButton)
		{
			// control z rotation via the middle mouse button
			landRotate[2] += mousePosDelta[1];
		}
		break;

		// scale the landscape
	case SCALE:
		if (leftMouseButton)
		{
			// control x,y scaling via the left mouse button
			landScale[0] *= 1.0f + mousePosDelta[0] * 0.01f;
			landScale[1] *= 1.0f - mousePosDelta[1] * 0.01f;
		}
		if (middleMouseButton)
		{
			// control z scaling via the middle mouse button
			landScale[2] *= 1.0f - mousePosDelta[1] * 0.01f;
		}
		break;
	}

	// store the new mouse position
	mousePos[0] = x;
	mousePos[1] = y;
}

void mouseMotionFunc(int x, int y)
{
	// mouse has moved
	// store the new mouse position
	mousePos[0] = x;
	mousePos[1] = y;
}

void mouseButtonFunc(int button, int state, int x, int y)
{
	// a mouse button has has been pressed or depressed

	// keep track of the mouse button state, in leftMouseButton, middleMouseButton, rightMouseButton variables
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		leftMouseButton = (state == GLUT_DOWN);
		break;

	case GLUT_MIDDLE_BUTTON:
		middleMouseButton = (state == GLUT_DOWN);
		break;

	case GLUT_RIGHT_BUTTON:
		rightMouseButton = (state == GLUT_DOWN);
		break;
	}

	// keep track of whether CTRL and SHIFT keys are pressed
	switch (glutGetModifiers())
	{
	case GLUT_ACTIVE_CTRL:
		controlState = TRANSLATE;
		break;

	case GLUT_ACTIVE_SHIFT:
		controlState = SCALE;
		break;

		// if CTRL and SHIFT are not pressed, we are in rotate mode
	default:
		controlState = ROTATE;
		break;
	}

	// store the new mouse position
	mousePos[0] = x;
	mousePos[1] = y;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // ESC key
		exit(0); // exit the program
		break;

	case 'x':
		// take a screenshot
		saveScreenshot("screenshot.jpg");
		break;
	}
}

int readTextImage(const char *imageFilename, ImageIO* img)
{
	// read the texture image
	ImageIO::fileFormatType imgFormat;
	ImageIO::errorType err = img->load(imageFilename, &imgFormat);

	if (err != ImageIO::OK)
	{
		printf("Loading texture from %s failed.\n", imageFilename);
		return -1;
	}

	// check that the number of bytes is a multiple of 4
	if (img->getWidth() * img->getBytesPerPixel() % 4)
	{
		printf("Error (%s): The width*numChannels in the loaded image must be a multiple of 4.\n", imageFilename);
		return -1;
	}
}

int initTexture(ImageIO* img, GLuint textureHandle)
{
	// allocate space for an array of pixels
	int width = img->getWidth();
	int height = img->getHeight();
	unsigned char * pixelsRGBA = new unsigned char[4 * width * height]; // we will use 4 bytes per pixel, i.e., RGBA

																		// fill the pixelsRGBA array with the image pixels
	memset(pixelsRGBA, 0, 4 * width * height); // set all bytes to 0
	for (int h = 0; h < height; h++)
		for (int w = 0; w < width; w++)
		{
			// assign some default byte values (for the case where img.getBytesPerPixel() < 4)
			pixelsRGBA[4 * (h * width + w) + 0] = 0; // red
			pixelsRGBA[4 * (h * width + w) + 1] = 0; // green
			pixelsRGBA[4 * (h * width + w) + 2] = 0; // blue
			pixelsRGBA[4 * (h * width + w) + 3] = 255; // alpha channel; fully opaque

													   // set the RGBA channels, based on the loaded image
			int numChannels = img->getBytesPerPixel();
			for (int c = 0; c < numChannels; c++) // only set as many channels as are available in the loaded image; the rest get the default value
				pixelsRGBA[4 * (h * width + w) + c] = img->getPixel(w, h, c);
		}

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// initialize the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsRGBA);

	// generate the mipmaps for this texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	// query support for anisotropic texture filtering
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	printf("Max available anisotropic samples: %f\n", fLargest);
	// set anisotropic texture filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0.5f * fLargest);

	// query for any errors
	GLenum errCode = glGetError();
	if (errCode != 0)
	{
		printf("Texture initialization error. Error code: %d.\n", errCode);
		return -1;
	}

	// de-allocate the pixel array -- it is no longer needed
	delete[] pixelsRGBA;

	return 0;
}

void initVBO()
{
	glGenBuffers(1, &left_spline_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, left_spline_vbo);
	glBufferData(GL_ARRAY_BUFFER, size_rail_points * sizeof(float) * 3 + size_rail_points * sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_rail_points * sizeof(float) * 3, leftRail);
	glBufferSubData(GL_ARRAY_BUFFER, size_rail_points * sizeof(float) * 3, size_rail_points * sizeof(float) * 4, leftcolor);

	glGenBuffers(1, &right_spline_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, right_spline_vbo);
	glBufferData(GL_ARRAY_BUFFER, size_rail_points * sizeof(float) * 3 + size_rail_points * sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_rail_points * sizeof(float) * 3, rightRail);
	glBufferSubData(GL_ARRAY_BUFFER, size_rail_points * sizeof(float) * 3, size_rail_points * sizeof(float) * 4, rightcolor);

	glGenBuffers(1, &bar_spline_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, bar_spline_vbo);
	glBufferData(GL_ARRAY_BUFFER, size_bar_points * sizeof(float) * 3 + size_bar_points * sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_bar_points * sizeof(float) * 3, barRail.data());
	glBufferSubData(GL_ARRAY_BUFFER, size_bar_points * sizeof(float) * 3, size_bar_points * sizeof(float) * 4, barcolor.data());

	glGenBuffers(1, &ground_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glBufferData(GL_ARRAY_BUFFER, size_ground_point * sizeof(float) * 3 + size_text * sizeof(float) * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_ground_point * sizeof(float) * 3, groundPos.data());
	glBufferSubData(GL_ARRAY_BUFFER, size_ground_point * sizeof(float) * 3, size_text * sizeof(float) * 2, texCord.data());

	glGenBuffers(1, &sky_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sky_vbo);
	glBufferData(GL_ARRAY_BUFFER, size_sky_point * sizeof(float) * 3 + size_text2 * sizeof(float) * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_sky_point * sizeof(float) * 3, skyPos.data());
	glBufferSubData(GL_ARRAY_BUFFER, size_sky_point * sizeof(float) * 3, size_text2 * sizeof(float) * 2, texCord2.data());
}

void initElementVBO()
{
	glGenBuffers(1, &left_element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, left_element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_leftRail.size() * sizeof(unsigned int), &indices_leftRail[0], GL_STATIC_DRAW);

	glGenBuffers(1, &right_element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, right_element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_rightRail.size() * sizeof(unsigned int), &indices_rightRail[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bar_element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bar_element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_barRail.size() * sizeof(unsigned int), &indices_barRail[0], GL_STATIC_DRAW);
}

void initPipe()
{
	// LEFT RAIL INITIALIZATION
	glGenVertexArrays(1, &left_spline_vao);
	glBindVertexArray(left_spline_vao);
	glBindBuffer(GL_ARRAY_BUFFER, left_spline_vbo);

	left_pipelineProgram = new BasicPipelineProgram();
	left_pipelineProgram->Init("../openGLHelper-starterCode");
	left_pipelineProgram->Bind();
	left_program = left_pipelineProgram->GetProgramHandle();
	//bind position spline data shader
	GLuint loc = glGetAttribLocation(left_program, "position");
	glEnableVertexAttribArray(loc);
	const void * offset = (const void*)0;
	GLsizei stride = 0;
	GLboolean normalized = GL_FALSE;
	glVertexAttribPointer(loc, 3, GL_FLOAT, normalized, stride, offset);
	//bind color spline data shader
	loc = glGetAttribLocation(left_program, "color");
	glEnableVertexAttribArray(loc); // enable the “color” attribute
	offset = (const void*)(size_rail_points * sizeof(float) * 3);
	stride = 0;
	normalized = GL_FALSE;
	glVertexAttribPointer(loc, 4, GL_FLOAT, normalized, stride, offset);


	//RIGHT RAIL INITIALIZATION
	glGenVertexArrays(1, &right_spline_vao);
	glBindVertexArray(right_spline_vao);
	glBindBuffer(GL_ARRAY_BUFFER, right_spline_vbo);

	right_pipelineProgram = new BasicPipelineProgram();
	right_pipelineProgram->Init("../openGLHelper-starterCode");
	right_pipelineProgram->Bind();
	right_program = right_pipelineProgram->GetProgramHandle();
	//bind position spline data shader
	loc = glGetAttribLocation(right_program, "position");
	glEnableVertexAttribArray(loc);
	offset = (const void*)0;
	stride = 0;
	normalized = GL_FALSE;
	glVertexAttribPointer(loc, 3, GL_FLOAT, normalized, stride, offset);
	//bind color spline data shader
	loc = glGetAttribLocation(right_program, "color");
	glEnableVertexAttribArray(loc); // enable the “color” attribute
	offset = (const void*)(size_rail_points * sizeof(float) * 3);
	stride = 0;
	normalized = GL_FALSE;
	glVertexAttribPointer(loc, 4, GL_FLOAT, normalized, stride, offset);


	//BAR INITIALIZATION
	glGenVertexArrays(1, &bar_spline_vao);
	glBindVertexArray(bar_spline_vao);
	glBindBuffer(GL_ARRAY_BUFFER, bar_spline_vbo);

	bar_pipelineProgram = new BasicPipelineProgram();
	bar_pipelineProgram->Init("../openGLHelper-starterCode");
	bar_pipelineProgram->Bind();
	bar_program = bar_pipelineProgram->GetProgramHandle();
	//bind position spline data shader
	loc = glGetAttribLocation(bar_program, "position");
	glEnableVertexAttribArray(loc);
	offset = (const void*)0;
	stride = 0;
	normalized = GL_FALSE;
	glVertexAttribPointer(loc, 3, GL_FLOAT, normalized, stride, offset);
	//bind color spline data shader
	loc = glGetAttribLocation(bar_program, "color");
	glEnableVertexAttribArray(loc); // enable the “color” attribute
	offset = (const void*)(size_bar_points * sizeof(float) * 3);
	stride = 0;
	normalized = GL_FALSE;
	glVertexAttribPointer(loc, 4, GL_FLOAT, normalized, stride, offset);


	//GROUND INITIALIZATION
	glGenVertexArrays(1, &ground_vao);
	glBindVertexArray(ground_vao);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);

	pipelineProgram2 = new TexturePipelineProgram();
	pipelineProgram2->Init("../openGLHelper-starterCode");
	pipelineProgram2->Bind();
	ground_program = pipelineProgram2->GetProgramHandle();
	//bind ground position data shader
	loc = glGetAttribLocation(ground_program, "groundPos");
	glEnableVertexAttribArray(loc);
	offset = (const void*)0;
	stride = 0;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, stride, offset);
	//bind texCord data shader
	loc = glGetAttribLocation(ground_program, "texCord");
	glEnableVertexAttribArray(loc);
	offset = (const void*)(sizeof(float) * groundPos.size());
	stride = 0;
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, stride, offset);

	glGenTextures(1, &groundTexHandle);
	readTextImage("./textures/ground_8.jpg", groundImage);
	if (initTexture(groundImage, groundTexHandle) != 0) {
		printf("Error loading the texture image.\n");
		exit(EXIT_FAILURE);
	}


	//SKY INITIALIZATION
	glGenVertexArrays(1, &sky_vao);
	glBindVertexArray(sky_vao);
	glBindBuffer(GL_ARRAY_BUFFER, sky_vbo);

	pipelineProgram3 = new TexturePipelineProgram();
	pipelineProgram3->Init("../openGLHelper-starterCode");
	pipelineProgram3->Bind();
	sky_program = pipelineProgram3->GetProgramHandle();
	//bind ground position data shader
	loc = glGetAttribLocation(sky_program, "groundPos");
	glEnableVertexAttribArray(loc);
	offset = (const void*)0;
	stride = 0;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, stride, offset);
	//bind texCord data shader
	loc = glGetAttribLocation(sky_program, "texCord");
	glEnableVertexAttribArray(loc);
	offset = (const void*)(sizeof(float) * skyPos.size());
	stride = 0;
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, stride, offset);

	glGenTextures(1, &skyTexHandle);
	readTextImage("./textures/sky_14.jpg", skyImage);
	if (initTexture(skyImage, skyTexHandle) != 0) {
		printf("Error loading the texture image.\n");
		exit(EXIT_FAILURE);
	}
}

int loadSplines(char * argv)
{
	char * cName = (char *)malloc(128 * sizeof(char));
	FILE * fileList;
	FILE * fileSpline;
	int iType, i = 0, j, iLength;

	// load the track file 
	fileList = fopen(argv, "r");
	if (fileList == NULL)
	{
		printf("can't open file\n");
		exit(1);
	}

	// stores the number of splines in a global variable 
	fscanf(fileList, "%d", &numSplines);

	splines = (Spline*)malloc(numSplines * sizeof(Spline));

	// reads through the spline files 
	for (j = 0; j < numSplines; j++)
	{
		i = 0;
		fscanf(fileList, "%s", cName);
		fileSpline = fopen(cName, "r");

		if (fileSpline == NULL)
		{
			printf("can't open file\n");
			exit(1);
		}

		// gets length for spline file
		fscanf(fileSpline, "%d %d", &iLength, &iType);

		// allocate memory for all the points
		splines[j].points = (Point *)malloc(iLength * sizeof(Point));
		splines[j].numControlPoints = iLength;

		// saves the data to the struct
		while (fscanf(fileSpline, "%lf %lf %lf",
			&splines[j].points[i].x,
			&splines[j].points[i].y,
			&splines[j].points[i].z) != EOF)
		{
			i++;
		}
	}

	free(cName);

	return 0;
}

void generateRailroad()
{
	vec3 Tan;
	vec3 Nor;
	vec3 Bin;
	vec3 Pos;
	vec3 Pos_left;
	vec3 Pos_right;
	float disp = 0.2;
	float alpha = 0.02;
	int diff = 40;
	size_rail_points = size_point * 4;
	leftRail = new float[3 * size_rail_points];
	rightRail = new float[3 * size_rail_points];
	leftcolor = new float[4 * size_rail_points];
	rightcolor = new float[4 * size_rail_points];
	
	int count = 0;
	for (int i = 0; i < size_point * 3; i = i + 3)
	{
		//compute normal, binormal and tangent vector
		float tan[3] = { tangent[i], tangent[i + 1], tangent[i + 2] };
		Tan = make_vec3(tan);
		Tan = normalize(Tan);

		if (i == 0)
		{
			// we want a roller coaster that settle on the ground (y axis is the vertical direction)
			vec3 V = normalize(V_START);
			Nor = V;
			//if the roller coaster is initially direct in y direction
			if (Tan == V_UP || Tan == V_DOWN)
			{
				Nor = V_HORIZON;
			}
		}
		else
			Nor = cross(Bin, Tan);

		Nor = normalize(Nor);
		Bin = cross(Tan, Nor);
		Bin = normalize(Bin);

		//start computing the vertices 
		float pos[3] = { position[i], position[i + 1], position[i + 2] };
		Pos = make_vec3(pos);
		Pos_left = Pos - Bin*disp;
		Pos_right = Pos + Bin*disp;

		vector <vec3> p_left;
		p_left.push_back(Pos_left + alpha*(-Nor + Bin));
		p_left.push_back(Pos_left + alpha*(Nor + Bin));
		p_left.push_back(Pos_left + alpha*(Nor - Bin));
		p_left.push_back(Pos_left + alpha*(-Nor - Bin));
		vector <vec3> p_right;
		p_right.push_back(Pos_right + alpha*(-Nor + Bin));
		p_right.push_back(Pos_right + alpha*(Nor + Bin));
		p_right.push_back(Pos_right + alpha*(Nor - Bin));
		p_right.push_back(Pos_right + alpha*(-Nor - Bin));
		vector <vec3> bar;
		bar.push_back(Pos_left + alpha*(-Nor + Bin));
		bar.push_back(Pos_left + alpha*(Nor + Bin));
		bar.push_back(Pos_right + alpha*(-Nor - Bin));
		bar.push_back(Pos_right + alpha*(Nor - Bin));


		for (int j = 0; j < 4; j++)
		{
			for (int m = 0; m < 3; m++)
			{
				leftRail[i / 3 * 4 * 3 + j * 3 + m] = p_left[j][m];
				rightRail[i / 3 * 4 * 3 + j * 3 + m] = p_right[j][m];

				if ((i / 3) % diff == 0 || (i / 3) % diff == 5)
				{
					barRail.push_back(bar[j][m]);
				}
			}

			if ((i / 3) % diff == 0 || (i / 3) % diff == 5)
			{
				barcolor.push_back(0.0);
				barcolor.push_back(1.0);
				barcolor.push_back(0.0);
				barcolor.push_back(1.0);
			}

			leftcolor[i / 3 * 4 * 4 + j * 4] = 1.0;
			leftcolor[i / 3 * 4 * 4 + j * 4 + 1] = 0.0;
			leftcolor[i / 3 * 4 * 4 + j * 4 + 2] = 0.0;
			leftcolor[i / 3 * 4 * 4 + j * 4 + 3] = 1.0;


			rightcolor[i / 3 * 4 * 4 + j * 4] = 1.0;
			rightcolor[i / 3 * 4 * 4 + j * 4 + 1] = 0.0;
			rightcolor[i / 3 * 4 * 4 + j * 4 + 2] = 0.0;
			rightcolor[i / 3 * 4 * 4 + j * 4 + 3] = 1.0;
		}

		if ((i / 3) % diff == 0 || (i / 3) % diff == 5)
		{
			count++;
		}
	}

	if ((barRail.size()/3) % 4 != 0)
	{
		for (int poptime = 0; poptime < (barRail.size() / 3) % 4; poptime++)
		{
			barRail.pop_back();
			barRail.pop_back();
			barRail.pop_back();
			barcolor.pop_back();
			barcolor.pop_back();
			barcolor.pop_back();
			barcolor.pop_back();
		}
	}
	size_bar_points = barRail.size()/3;
}

void constructSplineElement(vector<GLuint> &indices, int size_x_points, int increment)
{
	if (increment == 4)
	{
		for (int i = 0; i < size_x_points - increment; i = i + increment)
		{
			//0 4 5
			indices.push_back(i + 0);
			indices.push_back(i + 4);
			indices.push_back(i + 5);
			//0 1 5
			indices.push_back(i + 0);
			indices.push_back(i + 1);
			indices.push_back(i + 5);
			//1 5 6
			indices.push_back(i + 1);
			indices.push_back(i + 5);
			indices.push_back(i + 6);
			//1 2 6
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 6);
			//3 7 6
			indices.push_back(i + 3);
			indices.push_back(i + 7);
			indices.push_back(i + 6);
			//3 2 6
			indices.push_back(i + 3);
			indices.push_back(i + 2);
			indices.push_back(i + 6);
			//3 0 4
			indices.push_back(i + 3);
			indices.push_back(i + 0);
			indices.push_back(i + 4);
			//3 7 4
			indices.push_back(i + 3);
			indices.push_back(i + 7);
			indices.push_back(i + 4);
		}
	}
	else
	{
		for (int i = 0; i < size_x_points - increment; i = i + increment)
		{
			//0 2 3
			indices.push_back(i + 0);
			indices.push_back(i + 2);
			indices.push_back(i + 3);
			//0 1 3
			indices.push_back(i + 0);
			indices.push_back(i + 1);
			indices.push_back(i + 3);
			//1 3 7 
			indices.push_back(i + 1);
			indices.push_back(i + 3);
			indices.push_back(i + 7);
			//1 5 7 
			indices.push_back(i + 1);
			indices.push_back(i + 5);
			indices.push_back(i + 7);
			//4 6 7
			indices.push_back(i + 4);
			indices.push_back(i + 6);
			indices.push_back(i + 7);
			//4 5 7 
			indices.push_back(i + 4);
			indices.push_back(i + 5);
			indices.push_back(i + 7);
			//0 2 6 
			indices.push_back(i + 0);
			indices.push_back(i + 2);
			indices.push_back(i + 6);
			//0 3 6
			indices.push_back(i + 0);
			indices.push_back(i + 3);
			indices.push_back(i + 6);
		}
	}
}

vector<vec3> findPT(float u, mat4x3& CMat, mat4x4& MMat)
{
	float U[4] = { pow(u,3), pow(u,2), u, 1 };
	float U_prime[4] = { 3 * pow(u,2), 2 * u, 1, 0 };
	vec4 UMat = glm::make_vec4(U);
	vec4 U_primeMat = glm::make_vec4(U_prime);
	vec3 P = CMat * MMat * UMat;
	vec3 Tan = CMat * MMat * U_primeMat;
	vector<vec3> PT; 
	PT.push_back(P);
	PT.push_back(Tan);
	return PT;
}

void subdivision(float u0, float u1, float maxLength, mat4x3& CMat, mat4x4& MMat)
{
	float umid = (u0 + u1) / 2;
	vector<vec3> PT1 = findPT(u0, CMat, MMat);
	vec3 P1 = PT1[0];
	vector<vec3> PT2 = findPT(u1, CMat, MMat);
	vec3 P2 = PT2[0];
	vec3 dist = P2 - P1; 

	if (length(dist) > maxLength)
	{
		subdivision(u0, umid, maxLength, CMat, MMat);
		subdivision(umid, u1, maxLength, CMat, MMat);
	}
	else
	{
		vec3 P = PT1[0];
		vec3 Tan = PT1[1]; 

		position.push_back(P[0]);
		position.push_back(P[1]);
		position.push_back(P[2]);

		tangent.push_back(Tan[0]);
		tangent.push_back(Tan[1]);
		tangent.push_back(Tan[2]);
		return;
	}
}

void initSplines(Spline* splines)
{
	float u;
	int numIter;
	float M[16] = { -s, 2.0f - s, s - 2.0f, s,
		2.0f*s, s - 3.0f, 3.0f - 2 * s, -s,
		-s, 0, s, 0,
		0, 1.0f, 0, 0 };
	mat4x4 MMat = glm::make_mat4(M);

	for (int k = 0; k < numSplines; k++)
	{
		numIter = splines[k].numControlPoints - 3;
		
		float H_max = 0;
		if (GRAVITY_MODE==1)
		{
			H_max = splines[k].points[1].y;
			for (int j = 1; j < splines[k].numControlPoints - 1; j++)
				if (splines[k].points[j].y > H_max) H_max = splines[k].points[j].y;
		}

		for (int j = 0; j < numIter; j++)
		{
			Point p1 = splines[k].points[j];
			Point p2 = splines[k].points[j + 1];
			Point p3 = splines[k].points[j + 2];
			Point p4 = splines[k].points[j + 3];
			float C[12] = { p1.x, p1.y, p1.z,
				p2.x, p2.y, p2.z,
				p3.x, p3.y, p3.z,
				p4.x, p4.y, p4.z };
			mat4x3 CMat = glm::make_mat4x3(C);

			if (GRAVITY_MODE==1)
			{
				float u = 0; 
				float H_current = 0;
				float dp_norm = 0;
				float g_const = 9.81;
				float delta_t = 0.00025;
				int times = 0;
				while (u < 1)
				{
					vector<vec3> PT = findPT(u, CMat, MMat);
					vec3 P = PT[0];
					vec3 Tan = PT[1];

					position.push_back(P[0]);
					position.push_back(P[1]);
					position.push_back(P[2]);

					tangent.push_back(Tan[0]);
					tangent.push_back(Tan[1]);
					tangent.push_back(Tan[2]);

					H_current = P[1];
					dp_norm = length(Tan);
					if (H_max - H_current == 0) u = u + 0.001;
					else u = u + delta_t * pow(2 * g_const*(H_max - H_current), 0.5) / dp_norm;
					times++;
				}
			}
			else
			{
				for (int i = 0; i < 1000; i++)
					subdivision(float(i) / 1000.0, float(i + 1) / 1000.0, 0.006, CMat, MMat);
			}
		}
	}
	size_point = position.size()/3;
	generateRailroad();
	constructSplineElement(indices_leftRail, size_rail_points, 4);
	constructSplineElement(indices_rightRail, size_rail_points, 4);
	constructSplineElement(indices_barRail, size_bar_points, 8);
}

void readTextCord()
{
	size_text = 6;

	//(0,0)
	texCord.push_back(0.0f); texCord.push_back(0.0f);
	//(1,1)
	texCord.push_back(1.0f); texCord.push_back(1.0f);
	//(1,0)
	texCord.push_back(1.0f); texCord.push_back(0.0f);

	//(0,0)
	texCord.push_back(0.0f); texCord.push_back(0.0f);
	//(1,1)
	texCord.push_back(1.0f); texCord.push_back(1.0f);
	//(0,1)
	texCord.push_back(0.0f); texCord.push_back(1.0f);
}

void readTextCord2()
{
	size_text2 = 36;
	texCord2.push_back(0.000059f); texCord2.push_back(1.0f - 0.000004f);
	texCord2.push_back(0.000103f); texCord2.push_back(1.0f - 0.336048f),
		texCord2.push_back(0.335973f); texCord2.push_back(1.0f - 0.335903f),
		texCord2.push_back(1.000023f); texCord2.push_back(1.0f - 0.000013f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f),
		texCord2.push_back(0.999958f); texCord2.push_back(1.0f - 0.336064f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f),
		texCord2.push_back(0.336024f); texCord2.push_back(1.0f - 0.671877f),
		texCord2.push_back(0.667969f); texCord2.push_back(1.0f - 0.671889f),
		texCord2.push_back(1.000023f); texCord2.push_back(1.0f - 0.000013f),
		texCord2.push_back(0.668104f); texCord2.push_back(1.0f - 0.000013f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f),
		texCord2.push_back(0.000059f); texCord2.push_back(1.0f - 0.000004f),
		texCord2.push_back(0.335973f); texCord2.push_back(1.0f - 0.335903f),
		texCord2.push_back(0.336098f); texCord2.push_back(1.0f - 0.000071f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f),
		texCord2.push_back(0.335973f); texCord2.push_back(1.0f - 0.335903f),
		texCord2.push_back(0.336024f); texCord2.push_back(1.0f - 0.671877f),
		texCord2.push_back(1.000004f); texCord2.push_back(1.0f - 0.671847f),
		texCord2.push_back(0.999958f); texCord2.push_back(1.0f - 0.336064f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f),
		texCord2.push_back(0.668104f); texCord2.push_back(1.0f - 0.000013f),
		texCord2.push_back(0.335973f); texCord2.push_back(1.0f - 0.335903f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f),
		texCord2.push_back(0.335973f); texCord2.push_back(1.0f - 0.335903f),
		texCord2.push_back(0.668104f); texCord2.push_back(1.0f - 0.000013f),
		texCord2.push_back(0.336098f); texCord2.push_back(1.0f - 0.000071f),
		texCord2.push_back(0.000103f); texCord2.push_back(1.0f - 0.336048f),
		texCord2.push_back(0.000004f); texCord2.push_back(1.0f - 0.671870f),
		texCord2.push_back(0.336024f); texCord2.push_back(1.0f - 0.671877f),
		texCord2.push_back(0.000103f); texCord2.push_back(1.0f - 0.336048f),
		texCord2.push_back(0.336024f); texCord2.push_back(1.0f - 0.671877f),
		texCord2.push_back(0.335973f); texCord2.push_back(1.0f - 0.335903f),
		texCord2.push_back(0.667969f); texCord2.push_back(1.0f - 0.671889f),
		texCord2.push_back(1.000004f); texCord2.push_back(1.0f - 0.671847f),
		texCord2.push_back(0.667979f); texCord2.push_back(1.0f - 0.335851f);
}

void readGroundPos()
{
	size_ground_point = 6;
	groundPos.push_back(-40.0f);   groundPos.push_back(-20.0f);  groundPos.push_back(-40.0f);
	groundPos.push_back(40.0f);  groundPos.push_back(-20.0f);  groundPos.push_back(40.0f);
	groundPos.push_back(40.0f);  groundPos.push_back(-20.0f); groundPos.push_back(-40.0f);
	//secondTriangle
	groundPos.push_back(-40.0f);  groundPos.push_back(-20.0f); groundPos.push_back(-40.0f);
	groundPos.push_back(40.0f);  groundPos.push_back(-20.0f); groundPos.push_back(40.0f);
	groundPos.push_back(-40.0f);  groundPos.push_back(-20.0f); groundPos.push_back(40.0f);
}

void readSkyPos()
{
	size_sky_point = 36;
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(-40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(-40.0f); skyPos.push_back(40.0f); skyPos.push_back(40.0f);
	skyPos.push_back(40.0f); skyPos.push_back(-40.0f); skyPos.push_back(40.0f);
}

void initScene(int argc, char *argv[])
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	matrix = new OpenGLMatrix();
	groundImage = new ImageIO();
	skyImage = new ImageIO();

	//initialize spline
	initSplines(splines);
	//initiate ground texture and ground position array, element array 
	readTextCord();
	readGroundPos();
	//initiate sky texture and sky position array
	readTextCord2();
	readSkyPos();
	//initialize vao and vbo
	initVBO();
	initElementVBO();
	initPipe();

	frameCount = 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: %s <trackfile>\n", argv[0]);
		exit(0);
	}

	cout << "Initializing GLUT..." << endl;
	glutInit(&argc, argv);

	cout << "Initializing OpenGL..." << endl;

#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
#else
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
#endif
	glEnable(GL_DEPTH_TEST);

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(windowTitle);

	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
	cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// load the splines from the provided filename
	loadSplines(argv[1]);
	if (argc > 2)
	{
		GRAVITY_MODE = atoi(argv[2]);
	}
	printf("Loaded %d spline(s).\n", numSplines);
	for (int i = 0; i<numSplines; i++)
		printf("Num control points in spline %d: %d.\n", i, splines[i].numControlPoints);

	// tells glut to use a particular display function to redraw -- window must be drawn
	glutDisplayFunc(displayFunc);
	// perform animation inside idleFunc
	glutIdleFunc(idleFunc);
	// callback for mouse drags
	glutMotionFunc(mouseMotionDragFunc);
	// callback for idle mouse movement
	glutPassiveMotionFunc(mouseMotionFunc);
	// callback for mouse button changes
	glutMouseFunc(mouseButtonFunc);
	// callback for resizing the window
	glutReshapeFunc(reshapeFunc);
	// callback for pressing the keys on the keyboard
	glutKeyboardFunc(keyboardFunc);

	// init glew
#ifdef __APPLE__
	// nothing is needed on Apple
#else
	// Windows, Linux
	GLint result = glewInit();
	if (result != GLEW_OK)
	{
		cout << "error: " << glewGetErrorString(result) << endl;
		exit(EXIT_FAILURE);
	}
#endif

	// do initialization
	initScene(argc, argv);

	// sink forever into the glut loop
	glutMainLoop();
}


