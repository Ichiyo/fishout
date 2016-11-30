#include "camera.h"
#include <stdlib.h>

camera_t* camera_create(matrix4_t lookat)
{
  camera_t* camera = malloc(sizeof(camera_t));
  camera->view = lookat;
  camera->update = 1;
  return camera;
}

void camera_translate_by_world(camera_t* camera, vector3_t offset)
{
  camera->view = matrix4_translate_vector3(camera->view, vector3_neg(offset));
  camera->update = 1;
}

void camera_translate_by_direction(camera_t* camera, float length)
{
  camera->view = matrix4_translate_vector3(camera->view, vector3_neg(vector3_mul_scalar(camera_get_direction(camera),length)));
  camera->update = 1;
}

void camera_rotate_self(camera_t* camera, quaternion_t quat)
{
  camera->view = matrix4_mul(matrix4_create_quaternion(quat), camera->view);
  camera->update = 1;
}

void camera_rotate_around(camera_t* camera, quaternion_t quat)
{
  camera->view = matrix4_mul(camera->view, matrix4_create_quaternion(quat));
  camera->update = 1;
}

vector3_t camera_get_position(camera_t* camera)
{
  int i;
  matrix4_t inv = matrix4_invert(camera->view, &i);
  return vector3_new(inv.m[12],inv.m[13],inv.m[14]);
}

vector3_t camera_get_direction(camera_t* camera)
{
  return vector3_normalize(vector3_new(-camera->view.m[2], -camera->view.m[6], -camera->view.m[10]));
}

void camera_free(camera_t* camera)
{
  free(camera);
}
