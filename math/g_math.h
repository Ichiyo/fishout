#ifndef _M_MATH_H
#define _M_MATH_H

#include "types.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "matrix4.h"
#include "quaternion.h"
#include <math.h>

typedef m_vector3 vector3_t;
typedef m_vector2 vector2_t;
typedef m_vector4 vector4_t;
typedef m_matrix4 matrix4_t;
typedef m_matrix3 matrix3_t;
typedef quaternion quaternion_t;

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define DEG_TO_RAD(X) (X * M_PI / 180)
#define RAD_TO_DEG(X) (X * 180 / M_PI)

#ifdef __cplusplus
}
#endif

#endif
