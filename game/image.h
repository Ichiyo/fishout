#ifndef __M_IMAGE_H__
#define __M_IMAGE_H__

#include "graphic.h"
#include "platform.h"

#if TARGET_PLATFORM == PLATFORM_LINUX
  #if defined(USE_SDL)
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #endif
#elif TARGET_PLATFORM == PLATFORM_IOS
  #if defined(USE_SDL)
  #include <SDL.h>
  #include <SDL_image.h>
  #endif
#elif TARGET_PLATFORM == PLATFORM_MAC
  #if defined(USE_SDL)
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #endif
#endif

typedef struct image_t
{
  SDL_Surface* surface;
  int type;
} image_t;

image_t* image_new_from_file(char* file);
unsigned int image_get_width(image_t* image);
unsigned int image_get_height(image_t* image);
void* image_get_pixels(image_t* image);
int image_get_type(image_t* image);
void image_free(image_t* image);

#endif
