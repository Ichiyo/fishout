#ifndef __M_SHADER_H__
#define __M_SHADER_H__

#include "graphic.h"

typedef struct shader_t
{
  GLuint id;
} shader_t;

shader_t* shader_new(char* vert, char* frag);
int shader_use(shader_t* s);
void shader_free(shader_t* s);

#endif
