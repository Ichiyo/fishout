#ifndef _M_VECTOR3_H
#define _M_VECTOR3_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include "types.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

static __inline__ m_vector3 vector3_new(float x, float y, float z);
static __inline__ m_vector3 vector3_neg(m_vector3 vector);
static __inline__ m_vector3 vector3_add(m_vector3 left, m_vector3 right);
static __inline__ m_vector3 vector3_sub(m_vector3 left, m_vector3 right);
static __inline__ m_vector3 vector3_mul(m_vector3 left, m_vector3 right);
static __inline__ m_vector3 vector3_div(m_vector3 left, m_vector3 right);
static __inline__ m_vector3 vector3_add_scalar(m_vector3 left, float scalar);
static __inline__ m_vector3 vector3_sub_scalar(m_vector3 left, float scalar);
static __inline__ m_vector3 vector3_mul_scalar(m_vector3 left, float scalar);
static __inline__ m_vector3 vector3_div_scalar(m_vector3 left, float scalar);
static __inline__ m_vector3 vector3_normalize(m_vector3 vector);
static __inline__ float   vector3_dot_product(m_vector3 left, m_vector3 right);
static __inline__ float   vector3_length(m_vector3 vector);
static __inline__ float   vector3_distance(m_vector3 left, m_vector3 right);
static __inline__ m_vector3 vector3_lerp(m_vector3 begin, m_vector3 end, float t);
static __inline__ m_vector3 vector3_cross_product(m_vector3 left, m_vector3 right);
static __inline__ m_vector3 vector3_project(m_vector3 vectorToProject, m_vector3 projectionVector);

static __inline__ m_vector3 vector3_new(float x, float y, float z)
{
  m_vector3 ret = {x, y, z}; 
  return ret;
}

static __inline__ m_vector3 vector3_neg(m_vector3 vector)
{
  m_vector3 ret = {-vector.v[0], -vector.v[1], -vector.v[2]};
  return ret;
}

static __inline__ m_vector3 vector3_add(m_vector3 left, m_vector3 right)
{
  m_vector3 ret = {
    left.v[0] + right.v[0],
    left.v[1] + right.v[1],
    left.v[2] + right.v[2]
  };
  return ret;
}

static __inline__ m_vector3 vector3_sub(m_vector3 left, m_vector3 right)
{
  m_vector3 ret = {
    left.v[0] - right.v[0],
    left.v[1] - right.v[1],
    left.v[2] - right.v[2]
  };
  return ret;
}

static __inline__ m_vector3 vector3_mul(m_vector3 left, m_vector3 right)
{
  m_vector3 ret = {
    left.v[0] * right.v[0],
    left.v[1] * right.v[1],
    left.v[2] * right.v[2]
  };
  return ret;
}

static __inline__ m_vector3 vector3_div(m_vector3 left, m_vector3 right)
{
  m_vector3 ret = {
    left.v[0] / right.v[0],
    left.v[1] / right.v[1],
    left.v[2] / right.v[2]
  };
  return ret;
}

static __inline__ m_vector3 vector3_add_scalar(m_vector3 left, float scalar)
{
  m_vector3 ret = {
    left.v[0] + scalar,
    left.v[1] + scalar,
    left.v[2] + scalar
  };
  return ret;
}

static __inline__ m_vector3 vector3_sub_scalar(m_vector3 left, float scalar)
{
  m_vector3 ret = {
    left.v[0] - scalar,
    left.v[1] - scalar,
    left.v[2] - scalar
  };
  return ret;
}

static __inline__ m_vector3 vector3_mul_scalar(m_vector3 left, float scalar)
{
  m_vector3 ret = {
    left.v[0] * scalar,
    left.v[1] * scalar,
    left.v[2] * scalar
  };
  return ret;
}

static __inline__ m_vector3 vector3_div_scalar(m_vector3 left, float scalar)
{
  m_vector3 ret = {
    left.v[0] / scalar,
    left.v[1] / scalar,
    left.v[2] / scalar
  };
  return ret;
}

static __inline__ m_vector3 vector3_normalize(m_vector3 vector)
{
  float scale = 1.0f / vector3_length(vector);
  m_vector3 v = vector3_mul_scalar(vector, scale);
  return v;
}

static __inline__ float   vector3_dot_product(m_vector3 left, m_vector3 right)
{
  return left.v[0] * right.v[0] + left.v[1] * right.v[1] + left.v[2] * right.v[2];
}

static __inline__ float   vector3_length(m_vector3 vector)
{
  return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
}

static __inline__ float   vector3_distance(m_vector3 left, m_vector3 right)
{
  return vector3_length(vector3_sub(right, left));
}

static __inline__ m_vector3 vector3_lerp(m_vector3 begin, m_vector3 end, float t)
{
  m_vector3 ret = {
    begin.v[0] + (end.v[0] - begin.v[0]) * t,
    begin.v[1] + (end.v[1] - begin.v[1]) * t,
    begin.v[2] + (end.v[2] - begin.v[2]) * t
  };
  return ret;
}

static __inline__ m_vector3 vector3_cross_product(m_vector3 left, m_vector3 right)
{
  m_vector3 v = { left.v[1] * right.v[2] - left.v[2] * right.v[1],
                left.v[2] * right.v[0] - left.v[0] * right.v[2],
                left.v[0] * right.v[1] - left.v[1] * right.v[0] };
  return v;
}

static __inline__ m_vector3 vector3_project(m_vector3 vectorToProject, m_vector3 projectionVector)
{
    float scale = vector3_dot_product(projectionVector, vectorToProject) / vector3_dot_product(projectionVector, projectionVector);
    m_vector3 v = vector3_mul_scalar(projectionVector, scale);
    return v;
}

#ifdef __cplusplus
}
#endif

#endif
