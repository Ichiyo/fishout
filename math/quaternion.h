#ifndef _M_QUATERNION_H
#define _M_QUATERNION_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#include <stddef.h>

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include "types.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const quaternion quaternion_identity;

static __inline__ quaternion quaternion_new(float x, float y, float z, float w);
static __inline__ quaternion quaternion_new_vector3(m_vector3 vector, float scalar);
static __inline__ quaternion quaternion_new_array(float values[4]);
static __inline__ quaternion quaternion_new_angle_axis(float radians, float x, float y, float z);
static __inline__ quaternion quaternion_new_angle_vector3_axis(float radians, m_vector3 axisVector);
static __inline__ quaternion quaternion_add(quaternion quaternionLeft, quaternion quaternionRight);
static __inline__ quaternion quaternion_sub(quaternion quaternionLeft, quaternion quaternionRight);
static __inline__ quaternion quaternion_mul(quaternion quaternionLeft, quaternion quaternionRight);
static __inline__ float quaternion_length(quaternion quat);
static __inline__ quaternion quaternion_conjugate(quaternion quat);
static __inline__ quaternion quaternion_invert(quaternion quat);
static __inline__ quaternion quaternion_normalize(quaternion quat);
static __inline__ m_vector3 quaternion_rotate_vector3(quaternion quat, m_vector3 vector);
static __inline__ m_vector4 quaternion_rotate_vector4(quaternion quat, m_vector4 vector);
static __inline__ quaternion quaternion_new_euler_angle(float pitch, float roll, float yaw);
static __inline__ quaternion quaternion_new_vector3_euler_angle(m_vector3 vector);
static __inline__ quaternion quaternion_lerp(quaternion from, quaternion to, float time);
quaternion quaternion_new_matrix3(m_matrix3 matrix);
quaternion quaternion_new_matrix4(m_matrix4 matrix);
float quaternion_to_angle(quaternion quat);
m_vector3 quaternion_to_axis(quaternion quat);

static __inline__ quaternion quaternion_new(float x, float y, float z, float w)
{
    quaternion q = { x, y, z, w };
    return q;
}

static __inline__ quaternion quaternion_new_vector3(m_vector3 vector, float scalar)
{
    quaternion q = { vector.v[0], vector.v[1], vector.v[2], scalar };
    return q;
}

static __inline__ quaternion quaternion_new_array(float values[4])
{
    quaternion q = { values[0], values[1], values[2], values[3] };
    return q;
}

static __inline__ quaternion quaternion_new_angle_axis(float radians, float x, float y, float z)
{
    float halfAngle = radians * 0.5f;
    float scale = sinf(halfAngle);
    quaternion q = { scale * x, scale * y, scale * z, cosf(halfAngle) };
    return q;
}

static __inline__ quaternion quaternion_new_angle_vector3_axis(float radians, m_vector3 axisVector)
{
    return quaternion_new_angle_axis(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
}

static __inline__ quaternion quaternion_add(quaternion quaternionLeft, quaternion quaternionRight)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vaddq_f32(*(float32x4_t *)&quaternionLeft,
                              *(float32x4_t *)&quaternionRight);
    return *(quaternion *)&v;
#else
    quaternion q = {  quaternionLeft.q[0] + quaternionRight.q[0],
                      quaternionLeft.q[1] + quaternionRight.q[1],
                      quaternionLeft.q[2] + quaternionRight.q[2],
                      quaternionLeft.q[3] + quaternionRight.q[3] };
    return q;
#endif
}

static __inline__ quaternion quaternion_sub(quaternion quaternionLeft, quaternion quaternionRight)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vsubq_f32(*(float32x4_t *)&quaternionLeft,
                              *(float32x4_t *)&quaternionRight);
    return *(quaternion *)&v;
#else
    quaternion q = {  quaternionLeft.q[0] - quaternionRight.q[0],
                      quaternionLeft.q[1] - quaternionRight.q[1],
                      quaternionLeft.q[2] - quaternionRight.q[2],
                      quaternionLeft.q[3] - quaternionRight.q[3] };
    return q;
#endif
}

static __inline__ quaternion quaternion_mul(quaternion quaternionLeft, quaternion quaternionRight)
{
    quaternion q = {  quaternionLeft.q[3] * quaternionRight.q[0] +
                      quaternionLeft.q[0] * quaternionRight.q[3] +
                      quaternionLeft.q[1] * quaternionRight.q[2] -
                      quaternionLeft.q[2] * quaternionRight.q[1],

                      quaternionLeft.q[3] * quaternionRight.q[1] +
                      quaternionLeft.q[1] * quaternionRight.q[3] +
                      quaternionLeft.q[2] * quaternionRight.q[0] -
                      quaternionLeft.q[0] * quaternionRight.q[2],

                      quaternionLeft.q[3] * quaternionRight.q[2] +
                      quaternionLeft.q[2] * quaternionRight.q[3] +
                      quaternionLeft.q[0] * quaternionRight.q[1] -
                      quaternionLeft.q[1] * quaternionRight.q[0],

                      quaternionLeft.q[3] * quaternionRight.q[3] -
                      quaternionLeft.q[0] * quaternionRight.q[0] -
                      quaternionLeft.q[1] * quaternionRight.q[1] -
                      quaternionLeft.q[2] * quaternionRight.q[2] };
    return q;
}

static __inline__ float quaternion_length(quaternion quat)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)&quat,
                              *(float32x4_t *)&quat);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return sqrt(vget_lane_f32(v2, 0));
#else
    return sqrt(quat.q[0] * quat.q[0] +
                quat.q[1] * quat.q[1] +
                quat.q[2] * quat.q[2] +
                quat.q[3] * quat.q[3]);
#endif
}

static __inline__ quaternion quaternion_conjugate(quaternion quat)
{
#if defined(__ARM_NEON__)
    float32x4_t *q = (float32x4_t *)&quat;

    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32x4_t mask = vdupq_n_u32(signBit);
    mask = vsetq_lane_u32(zeroBit, mask, 3);
    *q = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(*q), mask));

    return *(quaternion *)q;
#else
    quaternion q = { -quat.q[0], -quat.q[1], -quat.q[2], quat.q[3] };
    return q;
#endif
}

static __inline__ quaternion quaternion_invert(quaternion quat)
{
#if defined(__ARM_NEON__)
    float32x4_t *q = (float32x4_t *)&quat;
    float32x4_t v = vmulq_f32(*q, *q);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    float32_t scale = 1.0f / vget_lane_f32(v2, 0);
    v = vmulq_f32(*q, vdupq_n_f32(scale));

    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32x4_t mask = vdupq_n_u32(signBit);
    mask = vsetq_lane_u32(zeroBit, mask, 3);
    v = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v), mask));

    return *(quaternion *)&v;
#else
    float scale = 1.0f / (quat.q[0] * quat.q[0] +
                          quat.q[1] * quat.q[1] +
                          quat.q[2] * quat.q[2] +
                          quat.q[3] * quat.q[3]);
    quaternion q = { -quat.q[0] * scale, -quat.q[1] * scale, -quat.q[2] * scale, quat.q[3] * scale };
    return q;
#endif
}

static __inline__ quaternion quaternion_normalize(quaternion quat)
{
    float scale = 1.0f / quaternion_length(quat);
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)&quat,
                              vdupq_n_f32((float32_t)scale));
    return *(quaternion *)&v;
#else
    quaternion q = { quat.q[0] * scale, quat.q[1] * scale, quat.q[2] * scale, quat.q[3] * scale };
    return q;
#endif
}

static __inline__ m_vector3 quaternion_rotate_vector3(quaternion quat, m_vector3 vector)
{
    quaternion rotatedQuaternion = quaternion_new(vector.v[0], vector.v[1], vector.v[2], 0.0f);
    rotatedQuaternion = quaternion_mul(quaternion_mul(quat, rotatedQuaternion), quaternion_invert(quat));

    return vector3_new(rotatedQuaternion.q[0], rotatedQuaternion.q[1], rotatedQuaternion.q[2]);
}

static __inline__ m_vector4 quaternion_rotate_vector4(quaternion quat, m_vector4 vector)
{
    quaternion rotatedQuaternion = quaternion_new(vector.v[0], vector.v[1], vector.v[2], 0.0f);
    rotatedQuaternion = quaternion_mul(quaternion_mul(quat, rotatedQuaternion), quaternion_invert(quat));

    return vector4_new(rotatedQuaternion.q[0], rotatedQuaternion.q[1], rotatedQuaternion.q[2], vector.v[3]);
}

static __inline__ quaternion quaternion_new_euler_angle(float pitch, float roll, float yaw)
{
	quaternion q;
	float t0 = cos(yaw * 0.5f);
	float t1 = sin(yaw * 0.5f);
	float t2 = cos(roll * 0.5f);
	float t3 = sin(roll * 0.5f);
	float t4 = cos(pitch * 0.5f);
	float t5 = sin(pitch * 0.5f);

	q.q[3] = t0 * t2 * t4 + t1 * t3 * t5;
	q.q[0] = t0 * t3 * t4 - t1 * t2 * t5;
	q.q[1] = t0 * t2 * t5 + t1 * t3 * t4;
	q.q[2] = t1 * t2 * t4 - t0 * t3 * t5;
	return q;
}

static __inline__ quaternion quaternion_new_vector3_euler_angle(m_vector3 vector)
{
  float pitch = vector.v[0];
  float roll = vector.v[2];
  float yaw = vector.v[1];
  quaternion q;
	float t0 = cos(yaw * 0.5f);
	float t1 = sin(yaw * 0.5f);
	float t2 = cos(roll * 0.5f);
	float t3 = sin(roll * 0.5f);
	float t4 = cos(pitch * 0.5f);
	float t5 = sin(pitch * 0.5f);

	q.q[3] = t0 * t2 * t4 + t1 * t3 * t5;
	q.q[0] = t0 * t3 * t4 - t1 * t2 * t5;
	q.q[1] = t0 * t2 * t5 + t1 * t3 * t4;
	q.q[2] = t1 * t2 * t4 - t0 * t3 * t5;
	return q;
}

static __inline__ quaternion quaternion_lerp(quaternion from, quaternion to, float t)
{
  #if defined(__ARM_NEON__)
      float32x4_t vDiff = vsubq_f32(*(float32x4_t *)&to,
                                    *(float32x4_t *)&from);
      vDiff = vmulq_f32(vDiff, vdupq_n_f32((float32_t)t));
      float32x4_t v = vaddq_f32(*(float32x4_t *)&from, vDiff);
      return *(quaternion *)&v;
  #else
      quaternion q = { from.q[0] + ((to.q[0] - from.q[0]) * t),
                    from.q[1] + ((to.q[1] - from.q[1]) * t),
                    from.q[2] + ((to.q[2] - from.q[2]) * t),
                    from.q[3] + ((to.q[3] - from.q[3]) * t) };
      return q;
  #endif
}

#ifdef __cplusplus
}
#endif

#endif
