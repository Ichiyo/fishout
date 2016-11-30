#ifndef __M_TEXTURE_H__
#define __M_TEXTURE_H__

#include "image.h"

typedef struct texture_t
{
  GLuint id;
  unsigned int width;
  unsigned int height;
  int ref;
} texture_t;

texture_t* texture_new_from_file(char* file);
texture_t* texture_new_from_image(image_t* image);
void texture_bind(texture_t* tex, int id);
void texture_free(texture_t* tex);
void texture_remove_unused();
void texture_free_cache();

#endif
