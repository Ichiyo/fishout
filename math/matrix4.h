#ifndef _M_MATRIX4_H
#define _M_MATRIX4_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#include <stddef.h>

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include "types.h"
#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const m_matrix4 matrix4_identity;

static __inline__ m_matrix4 matrix4_create( float m00, float m01, float m02, float m03,
                                          float m10, float m11, float m12, float m13,
                                          float m20, float m21, float m22, float m23,
                                          float m30, float m31, float m32, float m33);
static __inline__ m_matrix4 matrix4_create_transpose( float m00, float m01, float m02, float m03,
                                                  float m10, float m11, float m12, float m13,
                                                  float m20, float m21, float m22, float m23,
                                                  float m30, float m31, float m32, float m33);
static __inline__ m_matrix4 matrix4_create_array(float values[16]);
static __inline__ m_matrix4 matrix4_create_array_transpose(float values[16]);
static __inline__ m_matrix4 matrix4_create_rows(m_vector4 row0,
                                              m_vector4 row1,
                                              m_vector4 row2,
                                              m_vector4 row3);
static __inline__ m_matrix4 matrix4_create_columns( m_vector4 column0,
                                                  m_vector4 column1,
                                                  m_vector4 column2,
                                                  m_vector4 column3);
static __inline__ m_matrix4 matrix4_create_quaternion(quaternion quaternion);
static __inline__ m_matrix4 matrix4_create_translation(float tx, float ty, float tz);
static __inline__ m_matrix4 matrix4_create_scale(float sx, float sy, float sz);
static __inline__ m_matrix4 matrix4_create_rotation(float radians, float x, float y, float z);
static __inline__ m_matrix4 matrix4_create_x_rotation(float radians);
static __inline__ m_matrix4 matrix4_create_y_rotation(float radians);
static __inline__ m_matrix4 matrix4_create_z_rotation(float radians);
static __inline__ m_matrix4 matrix4_create_perspective(float fovyRadians, float aspect, float nearZ, float farZ);
static __inline__ m_matrix4 matrix4_create_frustum( float left, float right,
                                                  float bottom, float top,
                                                  float nearZ, float farZ);
static __inline__ m_matrix4 matrix4_create_ortho( float left, float right,
                                                float bottom, float top,
                                                float nearZ, float farZ);
static __inline__ m_matrix4 matrix4_create_look_at( float eyeX, float eyeY, float eyeZ,
                                                  float centerX, float centerY, float centerZ,
                                                  float upX, float upY, float upZ);
static __inline__ m_matrix3 matrix4_to_matrix3(m_matrix4 matrix);
static __inline__ m_matrix2 matrix4_to_matrix2(m_matrix4 matrix);
static __inline__ m_vector4 matrix4_get_row(m_matrix4 matrix, int row);
static __inline__ m_vector4 matrix4_get_column(m_matrix4 matrix, int column);
static __inline__ m_matrix4 matrix4_set_row(m_matrix4 matrix, int row, m_vector4 vector);
static __inline__ m_matrix4 matrix4_set_column(m_matrix4 matrix, int column, m_vector4 vector);
static __inline__ m_matrix4 matrix4_transpose(m_matrix4 matrix);
static __inline__ m_matrix4 matrix4_mul(m_matrix4 matrixLeft, m_matrix4 matrixRight);
static __inline__ m_matrix4 matrix4_add(m_matrix4 matrixLeft, m_matrix4 matrixRight);
static __inline__ m_matrix4 matrix4_sub(m_matrix4 matrixLeft, m_matrix4 matrixRight);
static __inline__ m_matrix4 matrix4_translate(m_matrix4 matrix, float tx, float ty, float tz);
static __inline__ m_matrix4 matrix4_translate_vector3(m_matrix4 matrix, m_vector3 translationVector);
static __inline__ m_matrix4 matrix4_translate_vector4(m_matrix4 matrix, m_vector4 translationVector);
static __inline__ m_matrix4 matrix4_scale(m_matrix4 matrix, float sx, float sy, float sz);
static __inline__ m_matrix4 matrix4_scale_vector3(m_matrix4 matrix, m_vector3 scaleVector);
static __inline__ m_matrix4 matrix4_scale_vector4(m_matrix4 matrix, m_vector4 scaleVector);
static __inline__ m_matrix4 matrix4_rotate(m_matrix4 matrix, float radians, float x, float y, float z);
static __inline__ m_matrix4 matrix4_rotate_vector3(m_matrix4 matrix, float radians, m_vector3 axisVector);
static __inline__ m_matrix4 matrix4_rotate_vector4(m_matrix4 matrix, float radians, m_vector4 axisVector);
static __inline__ m_matrix4 matrix4_rotate_x(m_matrix4 matrix, float radians);
static __inline__ m_matrix4 matrix4_rotate_y(m_matrix4 matrix, float radians);
static __inline__ m_matrix4 matrix4_rotate_z(m_matrix4 matrix, float radians);
static __inline__ m_vector3 matrix4_mul_vector3(m_matrix4 matrixLeft, m_vector3 vectorRight);
static __inline__ m_vector3 matrix4_mul_vector3_translation(m_matrix4 matrixLeft, m_vector3 vectorRight);
static __inline__ m_vector3 matrix4_mul_project_vector3(m_matrix4 matrixLeft, m_vector3 vectorRight);
static __inline__ void matrix4_mul_vector3_array(m_matrix4 matrix, m_vector3 *vectors, size_t vectorCount);
static __inline__ void matrix4_mul_vector3_array_translation(m_matrix4 matrix, m_vector3 *vectors, size_t vectorCount);
static __inline__ void matrix4_mul_project_vector3_array(m_matrix4 matrix, m_vector3 *vectors, size_t vectorCount);
static __inline__ m_vector4 matrix4_mul_vector4(m_matrix4 matrixLeft, m_vector4 vectorRight);
static __inline__ void matrix4_mul_vector4_array(m_matrix4 matrix, m_vector4 *vectors, size_t vectorCount);

static __inline__ void matrix4_print(m_matrix4 mat);

m_matrix4 matrix4_invert(m_matrix4 matrix, int *isInvertible);
m_matrix4 matrix4_invert_transpose(m_matrix4 matrix, int *isInvertible);

static __inline__ m_matrix4 matrix4_create( float m00, float m01, float m02, float m03,
                                          float m10, float m11, float m12, float m13,
                                          float m20, float m21, float m22, float m23,
                                          float m30, float m31, float m32, float m33)
{
    m_matrix4 m = { m00, m01, m02, m03,
                  m10, m11, m12, m13,
                  m20, m21, m22, m23,
                  m30, m31, m32, m33 };
    return m;
}

static __inline__ m_matrix4 matrix4_create_transpose( float m00, float m01, float m02, float m03,
                                                  float m10, float m11, float m12, float m13,
                                                  float m20, float m21, float m22, float m23,
                                                  float m30, float m31, float m32, float m33)
{
    m_matrix4 m = { m00, m10, m20, m30,
                  m01, m11, m21, m31,
                  m02, m12, m22, m32,
                  m03, m13, m23, m33 };
    return m;
}

static __inline__ m_matrix4 matrix4_create_array(float values[16])
{
    m_matrix4 m = { values[0], values[1], values[2], values[3],
                  values[4], values[5], values[6], values[7],
                  values[8], values[9], values[10], values[11],
                  values[12], values[13], values[14], values[15] };
    return m;
}

static __inline__ m_matrix4 matrix4_create_array_transpose(float values[16])
{
#if defined(__ARM_NEON__)
    float32x4x4_t m = vld4q_f32(values);
    return *(m_matrix4 *)&m;
#else
    m_matrix4 m = { values[0], values[4], values[8], values[12],
                  values[1], values[5], values[9], values[13],
                  values[2], values[6], values[10], values[14],
                  values[3], values[7], values[11], values[15] };
    return m;
#endif
}

static __inline__ m_matrix4 matrix4_create_rows(m_vector4 row0,
                                              m_vector4 row1,
                                              m_vector4 row2,
                                              m_vector4 row3)
{
    m_matrix4 m = { row0.v[0], row1.v[0], row2.v[0], row3.v[0],
                  row0.v[1], row1.v[1], row2.v[1], row3.v[1],
                  row0.v[2], row1.v[2], row2.v[2], row3.v[2],
                  row0.v[3], row1.v[3], row2.v[3], row3.v[3] };
    return m;
}

static __inline__ m_matrix4 matrix4_create_columns( m_vector4 column0,
                                                  m_vector4 column1,
                                                  m_vector4 column2,
                                                  m_vector4 column3)
{
#if defined(__ARM_NEON__)
    float32x4x4_t m;
    m.val[0] = vld1q_f32(column0.v);
    m.val[1] = vld1q_f32(column1.v);
    m.val[2] = vld1q_f32(column2.v);
    m.val[3] = vld1q_f32(column3.v);
    return *(m_matrix4 *)&m;
#else
    m_matrix4 m = { column0.v[0], column0.v[1], column0.v[2], column0.v[3],
                  column1.v[0], column1.v[1], column1.v[2], column1.v[3],
                  column2.v[0], column2.v[1], column2.v[2], column2.v[3],
                  column3.v[0], column3.v[1], column3.v[2], column3.v[3] };
    return m;
#endif
}

static __inline__ m_matrix4 matrix4_create_quaternion(quaternion quat)
{
    quat = quaternion_normalize(quat);

    float x = quat.q[0];
    float y = quat.q[1];
    float z = quat.q[2];
    float w = quat.q[3];

    float _2x = x + x;
    float _2y = y + y;
    float _2z = z + z;
    float _2w = w + w;

    m_matrix4 m = { 1.0f - _2y * y - _2z * z,
                  _2x * y + _2w * z,
                  _2x * z - _2w * y,
                  0.0f,
                  _2x * y - _2w * z,
                  1.0f - _2x * x - _2z * z,
                  _2y * z + _2w * x,
                  0.0f,
                  _2x * z + _2w * y,
                  _2y * z - _2w * x,
                  1.0f - _2x * x - _2y * y,
                  0.0f,
                  0.0f,
                  0.0f,
                  0.0f,
                  1.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_translation(float tx, float ty, float tz)
{
    m_matrix4 m = matrix4_identity;
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

static __inline__ m_matrix4 matrix4_create_scale(float sx, float sy, float sz)
{
    m_matrix4 m = matrix4_identity;
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    return m;
}

static __inline__ m_matrix4 matrix4_create_rotation(float radians, float x, float y, float z)
{
    m_vector3 v = vector3_normalize(vector3_new(x, y, z));
    float cos = cosf(radians);
    float cosp = 1.0f - cos;
    float sin = sinf(radians);

    m_matrix4 m = { cos + cosp * v.v[0] * v.v[0],
                     cosp * v.v[0] * v.v[1] + v.v[2] * sin,
                     cosp * v.v[0] * v.v[2] - v.v[1] * sin,
                     0.0f,
                     cosp * v.v[0] * v.v[1] - v.v[2] * sin,
                     cos + cosp * v.v[1] * v.v[1],
                     cosp * v.v[1] * v.v[2] + v.v[0] * sin,
                     0.0f,
                     cosp * v.v[0] * v.v[2] + v.v[1] * sin,
                     cosp * v.v[1] * v.v[2] - v.v[0] * sin,
                     cos + cosp * v.v[2] * v.v[2],
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_x_rotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);

    m_matrix4 m = { 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, cos, sin, 0.0f,
                  0.0f, -sin, cos, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_y_rotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);

    m_matrix4 m = { cos, 0.0f, -sin, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  sin, 0.0f, cos, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_z_rotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);

    m_matrix4 m = { cos, sin, 0.0f, 0.0f,
                  -sin, cos, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_perspective(float fovyRadians, float aspect, float nearZ, float farZ)
{
    float cotan = 1.0f / tanf(fovyRadians / 2.0f);

    m_matrix4 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
                  0.0f, cotan, 0.0f, 0.0f,
                  0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
                  0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_frustum( float left, float right,
                                                  float bottom, float top,
                                                  float nearZ, float farZ)
{
    float ral = right + left;
    float rsl = right - left;
    float tsb = top - bottom;
    float tab = top + bottom;
    float fan = farZ + nearZ;
    float fsn = farZ - nearZ;

    m_matrix4 m = { 2.0f * nearZ / rsl, 0.0f, 0.0f, 0.0f,
                  0.0f, 2.0f * nearZ / tsb, 0.0f, 0.0f,
                  ral / rsl, tab / tsb, -fan / fsn, -1.0f,
                  0.0f, 0.0f, (-2.0f * farZ * nearZ) / fsn, 0.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_ortho( float left, float right,
                                                float bottom, float top,
                                                float nearZ, float farZ)
{
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = farZ + nearZ;
    float fsn = farZ - nearZ;

    m_matrix4 m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
                  0.0f, 2.0f / tsb, 0.0f, 0.0f,
                  0.0f, 0.0f, -2.0f / fsn, 0.0f,
                  -ral / rsl, -tab / tsb, -fan / fsn, 1.0f };

    return m;
}

static __inline__ m_matrix4 matrix4_create_look_at( float eyeX, float eyeY, float eyeZ,
                                                  float centerX, float centerY, float centerZ,
                                                  float upX, float upY, float upZ)
{
    m_vector3 ev = { eyeX, eyeY, eyeZ };
    m_vector3 cv = { centerX, centerY, centerZ };
    m_vector3 uv = { upX, upY, upZ };
    m_vector3 n = vector3_normalize(vector3_add(ev, vector3_neg(cv)));
    m_vector3 u = vector3_normalize(vector3_cross_product(uv, n));
    m_vector3 v = vector3_cross_product(n, u);

    m_matrix4 m = { u.v[0], v.v[0], n.v[0], 0.0f,
                     u.v[1], v.v[1], n.v[1], 0.0f,
                     u.v[2], v.v[2], n.v[2], 0.0f,
                     vector3_dot_product(vector3_neg(u), ev),
                     vector3_dot_product(vector3_neg(v), ev),
                     vector3_dot_product(vector3_neg(n), ev),
                     1.0f };

    return m;
}

static __inline__ m_matrix3 matrix4_to_matrix3(m_matrix4 matrix)
{
    m_matrix3 m = { matrix.m[0], matrix.m[1], matrix.m[2],
                  matrix.m[4], matrix.m[5], matrix.m[6],
                  matrix.m[8], matrix.m[9], matrix.m[10] };
    return m;
}

static __inline__ m_matrix2 matrix4_to_matrix2(m_matrix4 matrix)
{
    m_matrix2 m = { matrix.m[0], matrix.m[1],
                  matrix.m[4], matrix.m[5] };
    return m;
}

static __inline__ m_vector4 matrix4_get_row(m_matrix4 matrix, int row)
{
    m_vector4 v = { matrix.m[row], matrix.m[4 + row], matrix.m[8 + row], matrix.m[12 + row] };
    return v;
}

static __inline__ m_vector4 matrix4_get_column(m_matrix4 matrix, int column)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vld1q_f32(&(matrix.m[column * 4]));
    return *(m_vector4 *)&v;
#else
    m_vector4 v = { matrix.m[column * 4 + 0], matrix.m[column * 4 + 1], matrix.m[column * 4 + 2], matrix.m[column * 4 + 3] };
    return v;
#endif
}

static __inline__ m_matrix4 matrix4_set_row(m_matrix4 matrix, int row, m_vector4 vector)
{
    matrix.m[row] = vector.v[0];
    matrix.m[row + 4] = vector.v[1];
    matrix.m[row + 8] = vector.v[2];
    matrix.m[row + 12] = vector.v[3];

    return matrix;
}

static __inline__ m_matrix4 matrix4_set_column(m_matrix4 matrix, int column, m_vector4 vector)
{
#if defined(__ARM_NEON__)
    float *dst = &(matrix.m[column * 4]);
    vst1q_f32(dst, vld1q_f32(vector.v));
    return matrix;
#else
    matrix.m[column * 4 + 0] = vector.v[0];
    matrix.m[column * 4 + 1] = vector.v[1];
    matrix.m[column * 4 + 2] = vector.v[2];
    matrix.m[column * 4 + 3] = vector.v[3];

    return matrix;
#endif
}

static __inline__ m_matrix4 matrix4_transpose(m_matrix4 matrix)
{
#if defined(__ARM_NEON__)
    float32x4x4_t m = vld4q_f32(matrix.m);
    return *(m_matrix4 *)&m;
#else
    m_matrix4 m = { matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12],
                  matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13],
                  matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14],
                  matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15] };
    return m;
#endif
}

static __inline__ m_matrix4 matrix4_mul(m_matrix4 matrixLeft, m_matrix4 matrixRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;

    m.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
    m.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 0));
    m.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[2], 0));
    m.val[3] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[3], 0));

    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 1));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[3], 1));

    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 2));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[3], 2));

    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[0], 3));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[1], 3));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[2], 3));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[3], 3));

    return *(m_matrix4 *)&m;
#else
    m_matrix4 m;

    m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
	m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
	m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
	m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];

	m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
	m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
	m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
	m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];

	m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
	m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
	m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
	m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];

	m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
	m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
	m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
	m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];

    return m;
#endif
}

static __inline__ m_matrix4 matrix4_add(m_matrix4 matrixLeft, m_matrix4 matrixRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;

    m.val[0] = vaddq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
    m.val[1] = vaddq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
    m.val[2] = vaddq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
    m.val[3] = vaddq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);

    return *(m_matrix4 *)&m;
#else
    m_matrix4 m;

    m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
    m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
    m.m[2] = matrixLeft.m[2] + matrixRight.m[2];
    m.m[3] = matrixLeft.m[3] + matrixRight.m[3];

    m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
    m.m[5] = matrixLeft.m[5] + matrixRight.m[5];
    m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
    m.m[7] = matrixLeft.m[7] + matrixRight.m[7];

    m.m[8] = matrixLeft.m[8] + matrixRight.m[8];
    m.m[9] = matrixLeft.m[9] + matrixRight.m[9];
    m.m[10] = matrixLeft.m[10] + matrixRight.m[10];
    m.m[11] = matrixLeft.m[11] + matrixRight.m[11];

    m.m[12] = matrixLeft.m[12] + matrixRight.m[12];
    m.m[13] = matrixLeft.m[13] + matrixRight.m[13];
    m.m[14] = matrixLeft.m[14] + matrixRight.m[14];
    m.m[15] = matrixLeft.m[15] + matrixRight.m[15];

    return m;
#endif
}

static __inline__ m_matrix4 matrix4_sub(m_matrix4 matrixLeft, m_matrix4 matrixRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;

    m.val[0] = vsubq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
    m.val[1] = vsubq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
    m.val[2] = vsubq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
    m.val[3] = vsubq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);

    return *(m_matrix4 *)&m;
#else
    m_matrix4 m;

    m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
    m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
    m.m[2] = matrixLeft.m[2] - matrixRight.m[2];
    m.m[3] = matrixLeft.m[3] - matrixRight.m[3];

    m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
    m.m[5] = matrixLeft.m[5] - matrixRight.m[5];
    m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
    m.m[7] = matrixLeft.m[7] - matrixRight.m[7];

    m.m[8] = matrixLeft.m[8] - matrixRight.m[8];
    m.m[9] = matrixLeft.m[9] - matrixRight.m[9];
    m.m[10] = matrixLeft.m[10] - matrixRight.m[10];
    m.m[11] = matrixLeft.m[11] - matrixRight.m[11];

    m.m[12] = matrixLeft.m[12] - matrixRight.m[12];
    m.m[13] = matrixLeft.m[13] - matrixRight.m[13];
    m.m[14] = matrixLeft.m[14] - matrixRight.m[14];
    m.m[15] = matrixLeft.m[15] - matrixRight.m[15];

    return m;
#endif
}

static __inline__ m_matrix4 matrix4_translate(m_matrix4 matrix, float tx, float ty, float tz)
{
    m_matrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
                  matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
                  matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
                  matrix.m[0] * tx + matrix.m[4] * ty + matrix.m[8] * tz + matrix.m[12],
                  matrix.m[1] * tx + matrix.m[5] * ty + matrix.m[9] * tz + matrix.m[13],
                  matrix.m[2] * tx + matrix.m[6] * ty + matrix.m[10] * tz + matrix.m[14],
                  matrix.m[15] };
    return m;
}

static __inline__ m_matrix4 matrix4_translate_vector3(m_matrix4 matrix, m_vector3 translationVector)
{
    m_matrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
                  matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
                  matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
                  matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
                  matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
                  matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
                  matrix.m[15] };
    return m;
}

static __inline__ m_matrix4 matrix4_translate_vector4(m_matrix4 matrix, m_vector4 translationVector)
{
    m_matrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
                  matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
                  matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
                  matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
                  matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
                  matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
                  matrix.m[15] };
    return m;
}

static __inline__ m_matrix4 matrix4_scale(m_matrix4 matrix, float sx, float sy, float sz)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrix;
    float32x4x4_t m;

    m.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)sx);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)sy);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)sz);
    m.val[3] = iMatrix.val[3];

    return *(m_matrix4 *)&m;
#else
    m_matrix4 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx, matrix.m[3] * sx,
                  matrix.m[4] * sy, matrix.m[5] * sy, matrix.m[6] * sy, matrix.m[7] * sy,
                  matrix.m[8] * sz, matrix.m[9] * sz, matrix.m[10] * sz, matrix.m[11] * sz,
                  matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
    return m;
#endif
}

static __inline__ m_matrix4 matrix4_scale_vector3(m_matrix4 matrix, m_vector3 scaleVector)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrix;
    float32x4x4_t m;

    m.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)scaleVector.v[0]);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)scaleVector.v[1]);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)scaleVector.v[2]);
    m.val[3] = iMatrix.val[3];

    return *(m_matrix4 *)&m;
#else
    m_matrix4 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
                  matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
                  matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
                  matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
    return m;
#endif
}

static __inline__ m_matrix4 matrix4_scale_vector4(m_matrix4 matrix, m_vector4 scaleVector)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrix;
    float32x4x4_t m;

    m.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)scaleVector.v[0]);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)scaleVector.v[1]);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)scaleVector.v[2]);
    m.val[3] = iMatrix.val[3];

    return *(m_matrix4 *)&m;
#else
    m_matrix4 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
                  matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
                  matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
                  matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
    return m;
#endif
}

static __inline__ m_matrix4 matrix4_rotate(m_matrix4 matrix, float radians, float x, float y, float z)
{
    m_matrix4 rm = matrix4_create_rotation(radians, x, y, z);
    return matrix4_mul(matrix, rm);
}

static __inline__ m_matrix4 matrix4_rotate_vector3(m_matrix4 matrix, float radians, m_vector3 axisVector)
{
    m_matrix4 rm = matrix4_create_rotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
    return matrix4_mul(matrix, rm);
}

static __inline__ m_matrix4 matrix4_rotate_vector4(m_matrix4 matrix, float radians, m_vector4 axisVector)
{
    m_matrix4 rm = matrix4_create_rotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
    return matrix4_mul(matrix, rm);
}

static __inline__ m_matrix4 matrix4_rotate_x(m_matrix4 matrix, float radians)
{
    m_matrix4 rm = matrix4_create_x_rotation(radians);
    return matrix4_mul(matrix, rm);
}

static __inline__ m_matrix4 matrix4_rotate_y(m_matrix4 matrix, float radians)
{
    m_matrix4 rm = matrix4_create_y_rotation(radians);
    return matrix4_mul(matrix, rm);
}

static __inline__ m_matrix4 matrix4_rotate_z(m_matrix4 matrix, float radians)
{
    m_matrix4 rm = matrix4_create_z_rotation(radians);
    return matrix4_mul(matrix, rm);
}

static __inline__ m_vector3 matrix4_mul_vector3(m_matrix4 matrixLeft, m_vector3 vectorRight)
{
    m_vector4 v4 = matrix4_mul_vector4(matrixLeft, vector4_new(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 0.0f));
    return vector3_new(v4.v[0], v4.v[1], v4.v[2]);
}

static __inline__ m_vector3 matrix4_mul_vector3_translation(m_matrix4 matrixLeft, m_vector3 vectorRight)
{
    m_vector4 v4 = matrix4_mul_vector4(matrixLeft, vector4_new(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
    return vector3_new(v4.v[0], v4.v[1], v4.v[2]);
}

static __inline__ m_vector3 matrix4_mul_project_vector3(m_matrix4 matrixLeft, m_vector3 vectorRight)
{
    m_vector4 v4 = matrix4_mul_vector4(matrixLeft, vector4_new(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
    return vector3_mul_scalar(vector3_new(v4.v[0], v4.v[1], v4.v[2]), 1.0f / v4.v[3]);
}

static __inline__ void matrix4_mul_vector3_array(m_matrix4 matrix, m_vector3 *vectors, size_t vectorCount)
{
    int i;
    for (i=0; i < vectorCount; i++)
        vectors[i] = matrix4_mul_vector3(matrix, vectors[i]);
}

static __inline__ void matrix4_mul_vector3_array_translation(m_matrix4 matrix, m_vector3 *vectors, size_t vectorCount)
{
    int i;
    for (i=0; i < vectorCount; i++)
        vectors[i] = matrix4_mul_vector3_translation(matrix, vectors[i]);
}

static __inline__ void matrix4_mul_project_vector3_array(m_matrix4 matrix, m_vector3 *vectors, size_t vectorCount)
{
    int i;
    for (i=0; i < vectorCount; i++)
        vectors[i] = matrix4_mul_project_vector3(matrix, vectors[i]);
}

static __inline__ m_vector4 matrix4_mul_vector4(m_matrix4 matrixLeft, m_vector4 vectorRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrixLeft;
    float32x4_t v;

    iMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)vectorRight.v[0]);
    iMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)vectorRight.v[1]);
    iMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)vectorRight.v[2]);
    iMatrix.val[3] = vmulq_n_f32(iMatrix.val[3], (float32_t)vectorRight.v[3]);

    iMatrix.val[0] = vaddq_f32(iMatrix.val[0], iMatrix.val[1]);
    iMatrix.val[2] = vaddq_f32(iMatrix.val[2], iMatrix.val[3]);

    v = vaddq_f32(iMatrix.val[0], iMatrix.val[2]);

    return *(m_vector4 *)&v;
#else
    m_vector4 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] + matrixLeft.m[12] * vectorRight.v[3],
                  matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[9] * vectorRight.v[2] + matrixLeft.m[13] * vectorRight.v[3],
                  matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[6] * vectorRight.v[1] + matrixLeft.m[10] * vectorRight.v[2] + matrixLeft.m[14] * vectorRight.v[3],
                  matrixLeft.m[3] * vectorRight.v[0] + matrixLeft.m[7] * vectorRight.v[1] + matrixLeft.m[11] * vectorRight.v[2] + matrixLeft.m[15] * vectorRight.v[3] };
    return v;
#endif
}

static __inline__ void matrix4_mul_vector4_array(m_matrix4 matrix, m_vector4 *vectors, size_t vectorCount)
{
    int i;
    for (i=0; i < vectorCount; i++)
        vectors[i] = matrix4_mul_vector4(matrix, vectors[i]);
}

static __inline__ void matrix4_print(m_matrix4 mat)
{
  for(int i = 0; i < 16; i += 4)
  {
    printf("%f %f %f %f\n",
    mat.m[i+0],
    mat.m[i+1],
    mat.m[i+2],
    mat.m[i+3]);
  }
}

#ifdef __cplusplus
}
#endif

#endif
