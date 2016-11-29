#ifndef _M_GRAPHIC_H
#define _M_GRAPHIC_H

#include "platform.h"

#if TARGET_PLATFORM == PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES

#elif TARGET_PLATFORM == PLATFORM_MAC

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#include <GLES3/gl3.h>
#endif

#endif
