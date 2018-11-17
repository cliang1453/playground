/*
  CSCI 420 Computer Graphics, USC
  Assignment 1: Height Fields
  C++ starter code

  Student username: <lian455>
*/

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "openGLHeader.h"
#include "glutHeader.h"

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


int mousePos[2]; // x,y coordinate of the mouse position

int leftMouseButton = 0; // 1 if pressed, 0 if not 
int middleMouseButton = 0; // 1 if pressed, 0 if not
int rightMouseButton = 0; // 1 if pressed, 0 if not

typedef enum { POINT123, LINE, TRIANGLE } RENDER_STATE;
RENDER_STATE renderState = POINT123;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROL_STATE;
CONTROL_STATE controlState = ROTATE;

typedef enum { DECREASE, ENLARGE } SIZE_STATE;
SIZE_STATE sizeState = DECREASE;


// state of the world
float landRotate[3] = { 0.0f, 0.0f, 0.0f };
float landTranslate[3] = { 0.0f, 0.0f, 0.0f };
float landScale[3] = { 1.0f, 1.0f, 1.0f };

int windowWidth = 1280;
int windowHeight = 720;
char windowTitle[512] = "CSCI 420 homework I";

//initialize global variables 
ImageIO * heightmapImage;
ImageIO * colorImage;
OpenGLMatrix * matrix;
GLuint vao;
GLuint vbo;
GLuint element_vbo;
BasicPipelineProgram * pipelineProgram;
GLuint program;
float* position;
float* color;
int size_point;
vector<GLuint> indices;
GLboolean animationEffect;
GLboolean usingDifferentColor;
int frameCount;
float r = 9.0f;

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
	if (animationEffect)
		matrix->LookAt(0, 20, -5, 0, 0, -1, 0, 1, 0); // default camera
	else 
		matrix->LookAt(0, 12, -15, 0, 0, -1, 0, 1, 0); // default camera
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

void renderPoint()
{
	GLint first = 0;
	GLsizei numberOfVertices = size_point;
	glDrawArrays(GL_POINTS, first, numberOfVertices);
}

void renderLine()
{
	GLint first = 0;
	GLsizei numberOfVertices = size_point;
	//glDrawArrays(GL_LINES, first, numberOfVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void renderTriangle()
{
	GLint first = 0;
	GLsizei numberOfVertices = size_point;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind pespective matrix and modelview matrix to pipeline program
	bindProgram();
	pipelineProgram->Bind();

	glBindVertexArray(vao); //bind the VAO
	switch (renderState) //begin render in 3 difference cases
	{
	case POINT123:
		renderPoint();
	break;
	case LINE:
		renderLine();
	break;
	case TRIANGLE:
		renderTriangle();
	break;
	}
	glBindVertexArray(0); //unbind the VAO
	glutSwapBuffers();
}

void idleFunc()
{
	if (animationEffect)
	{
		// make animation
		float theta;
		//change state when the radius is less than minimum value or is larger than maximum value
		switch (sizeState) 
		{
		case DECREASE:
			if (r <= 0.0f)
			{
				sizeState = ENLARGE;
				theta = 0;
			}
			break;
		case ENLARGE:
			if (r >= 9.0f)
			{
				sizeState = DECREASE;
				theta = 0;
			}
			break;
		}

		//begin render in 3 difference cases
		switch (sizeState) 
		{
		case DECREASE:
			r = r - 0.03f;
			theta = frameCount* 2.0f * 3.14159f / 180.0f;
			landTranslate[0] = r*sin(theta);
			landTranslate[2] = r*cos(theta);
			landScale[0] = landScale[0] * 0.987;
			landScale[1] = landScale[1] * 0.987;
			landScale[2] = landScale[2] * 0.987;
			landRotate[1] = landRotate[1] + 3;
			break;
		case ENLARGE:
			r = r + 0.03f;
			theta = frameCount* 2.0f * 3.14159f / 180.0f;
			landTranslate[0] = r*sin(theta);
			landTranslate[2] = r*cos(theta);
			landScale[0] = landScale[0] * 1.013;
			landScale[1] = landScale[1] * 1.013;
			landScale[2] = landScale[2] * 1.013;
			landRotate[1] = landRotate[1] + 3;
			break;
		}

		//make the screen update 
		if (frameCount % 90 < 30)
			renderState = POINT123;
		else if (frameCount % 90 < 60)
			renderState = LINE;
		else
			renderState = TRIANGLE;
	}
	else // just the simple rotation change without special animation effect
		landRotate[1] = landRotate[1] + 1;
	

	// for example, here, you can save the screenshots to disk (to make the animation)
	if ( frameCount % 5==0 && frameCount / 5 < 300)
	{
		char filename[10];
		sprintf(filename, "%03d", frameCount/5);
		saveScreenshot((std::string(filename) + ".jpg").c_str());
	}

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

    case ' ':
      cout << "You pressed the spacebar." << endl;
	  switch (renderState){
	  case POINT123:
		  renderState = LINE;
	  break;
	  case LINE: 
		  renderState = TRIANGLE;
	  break;
	  case TRIANGLE: 
		  renderState= POINT123;
	  break;
	  }
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

//initiate VBO for ploting element array
void initElementVBO()
{
	glGenBuffers(1, &element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
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

void readImage(ImageIO * heightmapImage, ImageIO * colorImage)
{
	int w = heightmapImage->getWidth(); 
	int h = heightmapImage->getHeight();
	size_point = w*h;
	
	//allocate position and color data
	position = new float[3*w*h];
	color = new float[4*w*h];

	for (int i = 0; i < w; i = i++)
	{
		for (int j = 0; j < h; j++)
		{
			//read in position information from the heightmap image
			int z0 = heightmapImage->getPixel(i, j, 0);
			position[i*h * 3 + j * 3] = float(i - w / 2) / 20;
			position[i*h * 3 + j * 3 + 1] = float(z0) / 50;
			position[i*h * 3 + j * 3 + 2] = float(-j + w / 2) / 20;

			//read in color information from the colormap image
			if (usingDifferentColor)
			{
				int z0 = colorImage->getPixel(i, j, 0);
				if (colorImage->getBytesPerPixel() == 1) //color map is grayscale image
				{
					color[i*h * 4 + j * 4] = float(z0) / 255;
					color[i*h * 4 + j * 4 + 1] = 0.0;
					color[i*h * 4 + j * 4 + 2] = 0.0;
					color[i*h * 4 + j * 4 + 3] = 1.0;
				}
				else if (colorImage->getBytesPerPixel() == 3) //color map is RGB image
				{
					int z1 = colorImage->getPixel(i, j, 1);
					int z2 = colorImage->getPixel(i, j, 2);
					color[i*h * 4 + j * 4] = float(z0) / 255;
					color[i*h * 4 + j * 4 + 1] = float(z1) / 255;
					color[i*h * 4 + j * 4 + 2] = float(z2) / 255;
					color[i*h * 4 + j * 4 + 3] = 1.0;
				}
				else if (colorImage->getBytesPerPixel() == 4) //color map is RGBA image
				{
					int z1 = colorImage->getPixel(i, j, 1);
					int z2 = colorImage->getPixel(i, j, 2);
					int z3 = colorImage->getPixel(i, j, 3);
					color[i*h * 4 + j * 4] = float(z0) / 255;
					color[i*h * 4 + j * 4 + 1] = float(z1) / 255;
					color[i*h * 4 + j * 4 + 2] = float(z2) / 255;
					color[i*h * 4 + j * 4 + 3] = float(z3) / 255;
				}
				else
				{
					cout << "Error reading image channels." << endl;
					exit(EXIT_FAILURE);
				}
			}
			else //read in color information from heightmap image
			{
				int z0 = heightmapImage->getPixel(i, j, 0);
				if (heightmapImage->getBytesPerPixel() == 1) // heightmap is grayscale image 
				{
					color[i*h * 4 + j * 4] = float(z0) / 255;
					color[i*h * 4 + j * 4 + 1] = 0.0;
					color[i*h * 4 + j * 4 + 2] = 0.0;
					color[i*h * 4 + j * 4 + 3] = 1.0;
				}
				else if (heightmapImage->getBytesPerPixel() == 3) // heightmap is RGB color image 
				{

					int z1 = heightmapImage->getPixel(i, j, 1);
					int z2 = heightmapImage->getPixel(i, j, 2);
					color[i*h * 4 + j * 4] = float(z0) / 255;
					color[i*h * 4 + j * 4 + 1] = float(z1) / 255;
					color[i*h * 4 + j * 4 + 2] = float(z2) / 255;
					color[i*h * 4 + j * 4 + 3] = 1.0;
				}
				else if (heightmapImage->getBytesPerPixel() == 4) // heightmap is RGBA color image
				{
					int z1 = heightmapImage->getPixel(i, j, 1);
					int z2 = heightmapImage->getPixel(i, j, 2);
					int z3 = heightmapImage->getPixel(i, j, 3);
					color[i*h * 4 + j * 4] = float(z0) / 255;
					color[i*h * 4 + j * 4 + 1] = float(z1) / 255;
					color[i*h * 4 + j * 4 + 2] = float(z2) / 255;
					color[i*h * 4 + j * 4 + 3] = float(z3) / 255;
				}
				else
				{
					cout << "Error reading image channels." << endl;
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}

//construct element array
void constructElement(ImageIO * heightmapImage)
{
	int w = heightmapImage->getWidth();
	int h = heightmapImage->getHeight();
	for (int i = 0; i < w-1; i++)
	{
		for (int j = 0; j < h-1; j++)
		{
			indices.push_back(i*h + j);
			indices.push_back(i*h + j + w + 1);
			indices.push_back(i*h + j + 1);
			indices.push_back(i*h + j);
			indices.push_back(i*h + j + w + 1);
			indices.push_back(i*h + j + w); 
		}
	}
}

void initScene(int argc, char *argv[])
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	matrix = new OpenGLMatrix();

  // load the image from a jpeg disk file to main memory
  heightmapImage = new ImageIO();
  if (heightmapImage->loadJPEG(argv[1]) != ImageIO::OK)
  {
    cout << "Error reading image " << argv[1] << "." << endl;
    exit(EXIT_FAILURE);
  }

  usingDifferentColor = 0;
  animationEffect = 0;

  if (argc >= 3)
  {
	  //read in color map information
	  string arg2(argv[2]);
	  if (arg2 == "n")
		  usingDifferentColor = 0;
	  else
	  {
		  usingDifferentColor = 1;
		  colorImage = new ImageIO();
		  if (colorImage->loadJPEG(argv[2]) != ImageIO::OK)
		  {
			  cout << "Error reading color map image " << argv[2] << "." << endl;
			  exit(EXIT_FAILURE);
		  }
	  }

	  if (argc >= 4)
	  {
		  //read in animation effect boolean variable
		  string arg3(argv[3]);
		  if (arg3 == "n")
			  animationEffect = 0;
		  else if (arg3 == "y")
			  animationEffect = 1;
		  else
		  {
			  cout << "Invalid choise of animation effect. " << argv[3] << "." << endl;
			  exit(EXIT_FAILURE);
		  }
	  }
  }

  //read in height and color information
  readImage(heightmapImage, colorImage);
  //construct element array
  constructElement(heightmapImage);
  //initialize vao
  initVAO();
  //initialize vbo
  initVBO();
  //initialize element array vbo
  initElementVBO();
  //initialize pipeline program
  initPipe();
  //initalize framecount
  frameCount = 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cout << "The arguments are incorrect." << endl;
    cout << "usage: ./hw1 <heightmap file(can be color/grayscale)> <texture file>" << endl;
    exit(EXIT_FAILURE);
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


