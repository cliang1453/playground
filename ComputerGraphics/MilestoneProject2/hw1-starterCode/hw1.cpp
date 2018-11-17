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

//initialize global variables 
OpenGLMatrix * matrix;
GLuint vao;
GLuint vbo;
GLuint element_vbo;
BasicPipelineProgram * pipelineProgram;
GLuint program;
float* position;
float* color;
int frameCount;
// the spline array 
Spline * splines;
// total number of splines 
int numSplines;
int size_point;
float s = 0.5; 
vector<GLuint> indices; 


// write a screenshot to the specified filename
void saveScreenshot(const char * filename)
{
  unsigned char * screenshotData = new unsigned char[windowWidth * windowHeight * 3];
  glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, screenshotData);

  ImageIO screenshotImg(windowWidth, windowHeight, 3, screenshotData);

  if (screenshotImg.save(filename, ImageIO::FORMAT_JPEG) == ImageIO::OK)
    cout << "File " << filename << " saved successfully." << endl;
  else cout << "Failed to save file " << filename << '.' << endl;

  delete [] screenshotData;
}

void bindProgram()
{
	//Upload Perspective Matrix
	matrix->SetMatrixMode(OpenGLMatrix::Projection);
	GLint h_projectionMatrix = glGetUniformLocation(program, "projectionMatrix");
	float p[16]; // column major 
	matrix->GetMatrix(p);
	glUniformMatrix4fv(h_projectionMatrix, 1, GL_FALSE, p);

	//Set up Modelview Matirx
	matrix->SetMatrixMode(OpenGLMatrix::ModelView);
	matrix->LoadIdentity();
	matrix->LookAt(0, 0, 3, 0, 0, -1, 0, 1, 0); // default camera
	matrix->Translate(landTranslate[0], landTranslate[1], landTranslate[2]);
	matrix->Rotate(landRotate[0], 1, 0, 0);
	matrix->Rotate(landRotate[1], 0, 1, 0);
	matrix->Rotate(landRotate[2], 0, 0, 1);
	matrix->Scale(landScale[0], landScale[1],landScale[2]);
	//Upload Modelview Matrix
	GLint h_modelViewMatrix = glGetUniformLocation(program, "modelViewMatrix");
	float m[16]; // column major
	matrix->GetMatrix(m);
	glUniformMatrix4fv(h_modelViewMatrix, 1, GL_FALSE, m);
}

void renderLine()
{
	GLint first = 0;
	GLsizei numberOfVertices = size_point;
	glDrawArrays(GL_POINTS, first, numberOfVertices);
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind pespective matrix and modelview matrix to pipeline program
	bindProgram();
	pipelineProgram->Bind();

	glBindVertexArray(vao); //bind the VAO
	renderLine();
	glBindVertexArray(0); //unbind the VAO
	glutSwapBuffers();
}

void idleFunc()
{
	// for example, here, you can save the screenshots to disk (to make the animation)
	// if ( frameCount % 5==0 && frameCount / 5 < 300)
	// {
	// 	char filename[10];
	// 	sprintf(filename, "%03d", frameCount/5);
	// 	saveScreenshot((std::string(filename) + ".jpg").c_str());
	// }

	frameCount++;
	glutPostRedisplay();
}

void reshapeFunc(int w, int h)
{
	float aspect = (float)w / (float)h;
	glViewport(0, 0, w, h);

	// setup perspective matrix...
	matrix->SetMatrixMode(OpenGLMatrix::Projection);
	matrix->LoadIdentity();
	matrix->Perspective(45, 1.0*w / h, 0.01, 1000.0);
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

void initVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void initVBO()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size_point * sizeof(float) * 3 + size_point * sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_point * sizeof(float) * 3, position);
	glBufferSubData(GL_ARRAY_BUFFER, size_point * sizeof(float) * 3, size_point * sizeof(float) * 4, color);
}

void initPipe()
{
	pipelineProgram = new BasicPipelineProgram();
	pipelineProgram->Init("../openGLHelper-starterCode");
	program = pipelineProgram->GetProgramHandle();

	//bind position data shader
	GLuint loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(loc); 
	const void * offset = (const void*)0;
	GLsizei stride = 0;
	GLboolean normalized = GL_FALSE;
	glVertexAttribPointer(loc, 3, GL_FLOAT, normalized, stride, offset);

	//bind color data shader
	 loc = glGetAttribLocation(program, "color");
	 glEnableVertexAttribArray(loc); // enable the “color” attribute
	 offset = (const void*)(size_point * sizeof(float) * 3);
	 stride = 0;
	 normalized = GL_FALSE;
	 glVertexAttribPointer(loc, 4, GL_FLOAT, normalized, stride, offset);
}

int loadSplines(char * argv) 
{
  char * cName = (char *) malloc(128 * sizeof(char));
  FILE * fileList;
  FILE * fileSpline;
  int iType, i = 0, j, iLength;

  // load the track file 
  fileList = fopen(argv, "r");
  if (fileList == NULL) 
  {
    printf ("can't open file\n");
    exit(1);
  }
  
  // stores the number of splines in a global variable 
  fscanf(fileList, "%d", &numSplines);

  splines = (Spline*) malloc(numSplines * sizeof(Spline));

  // reads through the spline files 
  for (j = 0; j < numSplines; j++) 
  {
    i = 0;
    fscanf(fileList, "%s", cName);
    fileSpline = fopen(cName, "r");

    if (fileSpline == NULL) 
    {
      printf ("can't open file\n");
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

int initTexture(const char * imageFilename, GLuint textureHandle)
{
  // read the texture image
  ImageIO img;
  ImageIO::fileFormatType imgFormat;
  ImageIO::errorType err = img.load(imageFilename, &imgFormat);

  if (err != ImageIO::OK) 
  {
    printf("Loading texture from %s failed.\n", imageFilename);
    return -1;
  }

  // check that the number of bytes is a multiple of 4
  if (img.getWidth() * img.getBytesPerPixel() % 4) 
  {
    printf("Error (%s): The width*numChannels in the loaded image must be a multiple of 4.\n", imageFilename);
    return -1;
  }

  // allocate space for an array of pixels
  int width = img.getWidth();
  int height = img.getHeight();
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
      int numChannels = img.getBytesPerPixel();
      for (int c = 0; c < numChannels; c++) // only set as many channels as are available in the loaded image; the rest get the default value
        pixelsRGBA[4 * (h * width + w) + c] = img.getPixel(w, h, c);
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
  delete [] pixelsRGBA;

  return 0;
}

void initSplines(Spline* splines)
{
  float u;
  int numIter;
  size_point = 0;
  
  for (int k = 0; k < numSplines; k++)
  {
	  numIter = splines[k].numControlPoints - 3;
	  size_point = size_point + numIter * 1000;
  }
  position = new float[3 * size_point];
  color = new float[4 * size_point];

  float M[16] = { -s, 2.0f - s, s - 2.0f, s,
			2.0f*s, s - 3.0f, 3.0f - 2*s, -s,
			-s, 0, s, 0,
			0, 1.0f, 0, 0 };
  mat4x4 MMat = glm::make_mat4(M);

  for (int k = 0; k < numSplines; k++)
  {
	  numIter = splines[k].numControlPoints - 3;
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

		  for (int i = 0; i < 1000; i++)
		  {
			  u = float(i) / 1000.0;
			  float U[4] = { pow(u,3), pow(u,2), u, 1 };
			  vec4 UMat = glm::make_vec4(U);
			  vec3 P = CMat * MMat * UMat;

			  position[k * 1000 * numIter * 3 + 1000 * j * 3 + i * 3] = P[0];
			  position[k * 1000 * numIter * 3 + 1000 * j * 3 + i * 3 + 1] = P[1];
			  position[k * 1000 * numIter * 3 + 1000 * j * 3 + i * 3 + 2] = P[2];

			  color[k * 1000 * numIter * 4 + 1000 * j * 4 + i * 4] = 1.0;
			  color[k * 1000 * numIter * 4 + 1000 * j * 4 + i * 4 + 1] = 0.0;
			  color[k * 1000 * numIter * 4 + 1000 * j * 4 + i * 4 + 2] = 0.0;
			  color[k * 1000 * numIter * 4 + 1000 * j * 4 + i * 4 + 3] = 1.0;
		  }
	  }
  } 
}

void initScene(int argc, char *argv[])
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	matrix = new OpenGLMatrix();

	initSplines(splines);
	initVAO();
	initVBO();
	initPipe();
	frameCount = 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf ("usage: %s <trackfile>\n", argv[0]);
    exit(0);
  }

  cout << "Initializing GLUT..." << endl;
  glutInit(&argc,argv);

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

  printf("Loaded %d spline(s).\n", numSplines);
  for(int i=0; i<numSplines; i++)
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


