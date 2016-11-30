#include "image.h"
#include "io.h"

image_t* image_new_from_file(char* file)
{
  SDL_Surface *tempimage = IMG_Load(file);
  if( ! tempimage) return 0;
  image_t* image = malloc(sizeof(image_t));

  SDL_Surface *tempsurface;
  Uint32 rmask, gmask, bmask, amask;

  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
  #else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
  #endif
  tempsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, tempimage->w, tempimage->h, 32, rmask, gmask, bmask, amask);
  SDL_BlitSurface(tempimage, NULL, tempsurface, NULL);
  SDL_FreeSurface(tempimage);

  image->surface = tempsurface;

  /* Extracting color components from a 32-bit color value */
  SDL_PixelFormat *fmt;
  SDL_Surface *surface = image->surface;
  Uint32 temp, pixel;
  Uint8 red, green, blue, alpha;

  fmt = surface->format;
  SDL_LockSurface(surface);
  pixel = *((Uint32*)surface->pixels);
  SDL_UnlockSurface(surface);

  /* Get Red component */
  temp = pixel & fmt->Rmask;  /* Isolate red component */
  temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
  temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
  red = (Uint8)temp;

  /* Get Green component */
  temp = pixel & fmt->Gmask;  /* Isolate green component */
  temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
  temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
  green = (Uint8)temp;

  /* Get Blue component */
  temp = pixel & fmt->Bmask;  /* Isolate blue component */
  temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
  temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
  blue = (Uint8)temp;

  /* Get Alpha component */
  temp = pixel & fmt->Amask;  /* Isolate alpha component */
  temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
  temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
  alpha = (Uint8)temp;

  if(alpha == 0)
  {
    image->type = GL_RGB;
  }
  else
  {
    image->type = GL_RGBA;
  }

  return image;
}

unsigned int image_get_width(image_t* image)
{
  return image->surface->w;
}

unsigned int image_get_height(image_t* image)
{
  return image->surface->h;
}

void* image_get_pixels(image_t* image)
{
  return image->surface->pixels;
}

int image_get_type(image_t* image)
{
  return image->type;
}

void image_free(image_t* image)
{
  SDL_FreeSurface(image->surface);
  free(image);
}
