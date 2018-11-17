#include <iostream>
#include <cstring>
#include "openGLHeader.h"
#include "basicPipelineProgram.h"
using namespace std;

int BasicPipelineProgram::Init(const char * shaderBasePath) 
{
  if (BuildShadersFromFiles(shaderBasePath, "basic.vertexShader.glsl", "basic.fragmentShader.glsl") != 0)
  {
    cout << "Failed to build the pipeline program." << endl;
    return 1;
  }

  cout << "Successfully built the pipeline program." << endl;
  return 0;
}

void BasicPipelineProgram::SetModelViewMatrix(const float * m) 
{
  // pass "m" to the pipeline program, as the modelview matrix
  // students need to implement this
  glUniformMatrix4fv(h_modelViewMatrix, 1, GL_FALSE, m);
}

void BasicPipelineProgram::SetProjectionMatrix(const float * m) 
{
  // pass "m" to the pipeline program, as the projection matrix
  // students need to implement this
  glUniformMatrix4fv(h_projectionMatrix, 1, GL_FALSE, m);
}

int BasicPipelineProgram::SetShaderVariableHandles() 
{
  GLuint h_program= GetProgramHandle();
  // set h_modelViewMatrix and h_projectionMatrix
  h_modelViewMatrix = glGetUniformLocation(h_program,"modelViewMatrix");
  h_projectionMatrix = glGetUniformLocation(h_program,"projectionMatrix");
  // students need to implement this

  return 0;
}

