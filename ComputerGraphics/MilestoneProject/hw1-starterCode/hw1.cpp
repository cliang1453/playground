/*
CSCI 420 Computer Graphics, USC
Assignment 1: Height Fields
C++ starter code

Student username: <type your USC username here>
*/

#include <iostream>
#include <cstring>
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

int windowWidth = 1280;
int windowHeight = 720;
char windowTitle[512] = "CSCI 420 milstone 1";



//define object vertices -- a quad vertices -- glTriangles
/* vertices of the quad (will form two triangles;
rendered via GL_TRIANGLES) */


float position[3][3] =
{ { 0.0, 0.0, -1.0 },{ 1.0, 0.0, -1.0 },{ 0.0, 1.0, -1.0 } };

float color[6][4] =
{ { 1.0, 0.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0, 1.0 } };
//black, red, green, blue, yellow, magenta

int axis = 2;
OpenGLMatrix * matrix;
GLuint buffer;
BasicPipelineProgram * pipelineProgram;
GLuint vao;
GLuint program;



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

void bindProgram()
{
	//ProjectionView
	matrix->SetMatrixMode(OpenGLMatrix::Projection);
	GLint h_projectionMatrix = glGetUniformLocation(program, "projectionMatrix");
	float p[16]; // column major 
	matrix->GetMatrix(p);
	glUniformMatrix4fv(h_projectionMatrix, 1, GL_FALSE, p);

	//ModelView
	matrix->SetMatrixMode(OpenGLMatrix::ModelView);
	matrix->LoadIdentity();
	matrix->LookAt(0, 0, 3.7314634396, 0, 0, -1, 0, 1, 0); // default camera
	GLint h_modelViewMatrix = glGetUniformLocation(program, "modelViewMatrix");
	float m[16]; // column major
	matrix->GetMatrix(m);
	glUniformMatrix4fv(h_modelViewMatrix, 1, GL_FALSE, m);
}

void renderQuad()
{
	GLint first = 0;
	GLsizei numberOfVertices = 3;
	glDrawArrays(GL_TRIANGLES, first, numberOfVertices);
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the depth buffer bit and color buffer bit
	bindProgram();//set modelview and projectionview

	pipelineProgram->Bind(); //bind the pipeline program (activate)
	glBindVertexArray(vao); //bind the VAO
	renderQuad(); //render
	glBindVertexArray(0); //unbind the VAO

	glutSwapBuffers(); //Swapping the double buffering
}

void idleFunc()
{
	// for example, here, you can save the screenshots to disk (to make the animation)
	saveScreenshot("triangle.jpg");
	// make the screen update 
	glutPostRedisplay();
}

//set projection and viewport, preserve aspect ratio 
void reshapeFunc(int w, int h)
{
	float aspect = (float)w / (float)h;
	glViewport(0, 0, w, h); 

	// setup perspective matrix...
	matrix->SetMatrixMode(OpenGLMatrix::Projection);
	matrix->LoadIdentity();
	matrix->Perspective(45, 1.0*w/h, 0.01, 10.0);

	matrix->SetMatrixMode(OpenGLMatrix::ModelView);
}

void mouseButtonFunc(int btn, int state, int x, int y)
{
	if ((btn == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		axis = 0;
	if ((btn == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN))
		axis = 1;
	if ((btn == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
		axis = 2;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
		exit(0);
}

void initVBO()
{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position) + sizeof(color),
		NULL, GL_STATIC_DRAW); // init buffer’s size, but don’t assign any
							   // data to it
							   // upload position data
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(position), position);
	// upload color data
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(position),
		sizeof(color), color);
}

void initPipelineProgram()
{
	// initialize shader pipeline program (next lecture)
	pipelineProgram->Init("../openGLHelper-starterCode");

}

void initScene(int argc, char *argv[])
{
	cout << "initial scene" << endl;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	matrix = new OpenGLMatrix();

	//create VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	//create VBO, and allocate model space inside VBO, and upload the data inside VBO
	initVBO();

	//create pipeline program
	pipelineProgram = new BasicPipelineProgram();
	initPipelineProgram();
	//create pipeline program handle
	program = pipelineProgram->GetProgramHandle();
	
	//allocate shader space inside pipeline program
	GLuint loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(loc); // enable the “position” attribute
	const void * offset = (const void*)0;
	GLsizei stride = 0;
	GLboolean normalized = GL_FALSE;
	glVertexAttribPointer(loc, 3, GL_FLOAT, normalized, stride, offset);
	loc = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(loc); // enable the “color” attribute
	offset = (const void*) sizeof(position);
	stride = 0;
	normalized = GL_FALSE;
	glVertexAttribPointer(loc, 4, GL_FLOAT, normalized, stride, offset);
	
	// unbind the VAO
	glBindVertexArray(0); 
}

int main(int argc, char *argv[])
{
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

	// tells glut to use a particular display function to redraw -- window must be drawn
	glutDisplayFunc(displayFunc);
	// perform animation inside idleFunc
	glutIdleFunc(idleFunc);
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


	return(0);
}