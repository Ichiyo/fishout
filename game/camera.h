#ifndef __M_CAMERA_H__
#define __M_CAMERA_H__

#include "g_math.h"

typedef struct camera_t
{
  matrix4_t view;
  int update;
} camera_t;

camera_t* camera_create(matrix4_t lookat);
void camera_translate_by_world(camera_t* camera, vector3_t offset);
void camera_translate_by_direction(camera_t* camera, float length);
void camera_rotate_self(camera_t* camera, quaternion_t quat);
void camera_rotate_around(camera_t* camera, quaternion_t quat);
vector3_t camera_get_position(camera_t* camera);
vector3_t camera_get_direction(camera_t* camera);
void camera_free(camera_t* camera);

#endif
