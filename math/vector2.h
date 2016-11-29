#ifndef _M_VECTOR2_H
#define _M_VECTOR2_H

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

static __inline__ m_vector2 vector2_new(float x, float y);
static __inline__ m_vector2 vector2_neg(m_vector2 vector);
static __inline__ m_vector2 vector2_add(m_vector2 left, m_vector2 right);
static __inline__ m_vector2 vector2_sub(m_vector2 left, m_vector2 right);
static __inline__ m_vector2 vector2_mul(m_vector2 left, m_vector2 right);
static __inline__ m_vector2 vector2_div(m_vector2 left, m_vector2 right);
static __inline__ m_vector2 vector2_add_scalar(m_vector2 left, float scalar);
static __inline__ m_vector2 vector2_sub_scalar(m_vector2 left, float scalar);
static __inline__ m_vector2 vector2_mul_scalar(m_vector2 left, float scalar);
static __inline__ m_vector2 vector2_div_scalar(m_vector2 left, float scalar);
static __inline__ m_vector2 vector2_normalize(m_vector2 vector);
static __inline__ float   vector2_dot_product(m_vector2 left, m_vector2 right);
static __inline__ float   vector2_length(m_vector2 vector);
static __inline__ float   vector2_distance(m_vector2 left, m_vector2 right);
static __inline__ m_vector2 vector2_lerp(m_vector2 begin, m_vector2 end, float t);

static __inline__ m_vector2 vector2_new(float x, float y)
{
  m_vector2 ret = {x , y};
  return ret;
}

static __inline__ m_vector2 vector2_neg(m_vector2 v)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vneg_f32(*(float32x2_t *)&v);
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {-v.v[0], -v.v[1]};
  return ret;
#endif
}

static __inline__ m_vector2 vector2_add(m_vector2 left, m_vector2 right)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vadd_f32(*(float32x2_t *)&left,
                           *(float32x2_t *)&right);
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] + right.v[0],
    left.v[1] + right.v[1]
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_sub(m_vector2 left, m_vector2 right)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vsub_f32(*(float32x2_t *)&left,
                           *(float32x2_t *)&right);
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] - right.v[0],
    left.v[1] - right.v[1]
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_mul(m_vector2 left, m_vector2 right)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vmul_f32(*(float32x2_t *)&left,
                           *(float32x2_t *)&right);
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] * right.v[0],
    left.v[1] * right.v[1]
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_div(m_vector2 left, m_vector2 right)
{
#if defined(__ARM_NEON__)
  float32x2_t *vLeft = (float32x2_t *)&left;
  float32x2_t *vRight = (float32x2_t *)&right;
  float32x2_t estimate = vrecpe_f32(*vRight);
  estimate = vmul_f32(vrecps_f32(*vRight, estimate), estimate);
  estimate = vmul_f32(vrecps_f32(*vRight, estimate), estimate);
  float32x2_t v = vmul_f32(*vLeft, estimate);
  return *(m_vector2 *)&v;
#else
  m_vector2 ret = {
    left.v[0] / right.v[0],
    left.v[1] / right.v[1]
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_add_scalar(m_vector2 left, float scalar)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vadd_f32(*(float32x2_t *)&left,
                           vdup_n_f32((float32_t)scalar));
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] + scalar,
    left.v[1] + scalar
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_sub_scalar(m_vector2 left, float scalar)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vsub_f32(*(float32x2_t *)&left,
                           vdup_n_f32((float32_t)scalar));
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] - scalar,
    left.v[1] - scalar
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_mul_scalar(m_vector2 left, float scalar)
{
#if defined(__ARM_NEON__)
  float32x2_t ret = vmul_f32(*(float32x2_t *)&left,
                           vdup_n_f32((float32_t)scalar));
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] * scalar,
    left.v[1] * scalar
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_div_scalar(m_vector2 left, float scalar)
{
#if defined(__ARM_NEON__)
  float32x2_t values = vdup_n_f32((float32_t)scalar);
  float32x2_t estimate = vrecpe_f32(values);
  estimate = vmul_f32(vrecps_f32(values, estimate), estimate);
  estimate = vmul_f32(vrecps_f32(values, estimate), estimate);
  float32x2_t ret = vmul_f32(*(float32x2_t *)&left, estimate);
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    left.v[0] / scalar,
    left.v[1] / scalar
  };
  return ret;
#endif
}

static __inline__ m_vector2 vector2_normalize(m_vector2 vector)
{
  float scale = 1.0f / vector2_length(vector);
  m_vector2 v = vector2_mul_scalar(vector, scale);
  return v;
}

static __inline__ float   vector2_dot_product(m_vector2 left, m_vector2 right)
{
#if defined(__ARM_NEON__)
  float32x2_t v = vmul_f32(*(float32x2_t *)&left,
                           *(float32x2_t *)&right);
  v = vpadd_f32(v, v);
  return vget_lane_f32(v, 0);
#else
  return left.v[0] * right.v[0] + left.v[1] * right.v[1];
#endif
}

static __inline__ float   vector2_length(m_vector2 vector)
{
#if defined(__ARM_NEON__)
    float32x2_t v = vmul_f32(*(float32x2_t *)&vector,
                             *(float32x2_t *)&vector);
    v = vpadd_f32(v, v);
    return sqrt(vget_lane_f32(v, 0));
#else
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1]);
#endif
}

static __inline__ float   vector2_distance(m_vector2 left, m_vector2 right)
{
    return vector2_length(vector2_sub(right, left));
}

static __inline__ m_vector2 vector2_lerp(m_vector2 begin, m_vector2 end, float t)
{
#if defined(__ARM_NEON__)
  float32x2_t vDiff = vsub_f32(*(float32x2_t *)&end,
                               *(float32x2_t *)&begin);
  vDiff = vmul_f32(vDiff, vdup_n_f32((float32_t)t));
  float32x2_t ret = vadd_f32(*(float32x2_t *)&begin, vDiff);
  return *(m_vector2 *)&ret;
#else
  m_vector2 ret = {
    begin.v[0] + (end.v[0] - begin.v[0]) * t,
    begin.v[1] + (end.v[1] - begin.v[1]) * t
  };
  return ret;
#endif
}

#ifdef __cplusplus
}
#endif

#endif
