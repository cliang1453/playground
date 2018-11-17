#include <iostream>
#include <cstring>
#include "openGLHeader.h"
#include "texturePipelineProgram.h"
using namespace std;

int TexturePipelineProgram::Init(const char * shaderBasePath) 
{
  if (BuildShadersFromFiles(shaderBasePath, "texture.vertexShader.glsl", "texture.fragmentShader.glsl") != 0)
  {
    cout << "Failed to build the pipeline program." << endl;
    return 1;
  }

  cout << "Successfully built the pipeline program." << endl;
  return 0;
}

void TexturePipelineProgram::SetModelViewMatrix(const float * m) 
{
  // pass "m" to the pipeline program, as the modelview matrix
  // students need to implement this
  glUniformMatrix4fv(h_modelViewMatrix, 1, GL_FALSE, m);
}

void TexturePipelineProgram::SetProjectionMatrix(const float * m) 
{
  // pass "m" to the pipeline program, as the projection matrix
  // students need to implement this
  glUniformMatrix4fv(h_projectionMatrix, 1, GL_FALSE, m);
}

int TexturePipelineProgram::SetShaderVariableHandles() 
{
  GLuint h_program = GetProgramHandle();
  // set h_modelViewMatrix and h_projectionMatrix
  h_modelViewMatrix = glGetUniformLocation(h_program,"modelViewMatrix");
  h_projectionMatrix = glGetUniformLocation(h_program,"projectionMatrix");
  // students need to implement this

  return 0;
}


