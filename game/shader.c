#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

shader_t* shader_new(char* vertex_source, char* fragment_source)
{
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const GLchar**)&vertex_source, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("compile shader vertex error:\n%s\n", infoLog);
      return 0;
  }
  // Fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const GLchar**)&fragment_source, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      printf("compile shader fragment error:\n%s\n", infoLog);
      return 0;
  }
  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      printf("link shader error:\n%s\n", infoLog);
      return 0;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  shader_t* ret = malloc(sizeof(shader_t));
  ret->id = shaderProgram;
  return ret;
}

static shader_t* current_shader = 0;

int shader_use(shader_t* s)
{
  if(s != current_shader)
  {
    current_shader = s;
    glUseProgram(s->id);
    return 1;
  }
  return 0;
}

void shader_free(shader_t* s)
{
  if(s->id)
  {
    if(s == current_shader) current_shader = 0;
    glDeleteProgram(s->id);
  }
  free(s);
}
