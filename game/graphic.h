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

#define __gl_h_
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#include <GLES3/gl3.h>
#endif

#endif
