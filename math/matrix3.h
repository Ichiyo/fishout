#ifndef _M_MATRIX3_H
#define _M_MATRIX3_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const m_matrix3 matrix3_identity;
static __inline__ m_matrix3 matrix3_create( float m00, float m01, float m02,
                                          float m10, float m11, float m12,
                                          float m20, float m21, float m22);
static __inline__ m_matrix3 matrix3_create_transpose( float m00, float m01, float m02,
                                                    float m10, float m11, float m12,
                                                    float m20, float m21, float m22);
static __inline__ m_matrix3 matrix3_create_array(float values[9]);
static __inline__ m_matrix3 matrix3_create_array_transpose(float values[9]);
static __inline__ m_matrix3 matrix3_create_rows(m_vector3 row0,
                                              m_vector3 row1,
                                              m_vector3 row2);
static __inline__ m_matrix3 matrix3_create_columns( m_vector3 column0,
                                                  m_vector3 column1,
                                                  m_vector3 column2);
static __inline__ m_matrix3 matrix3_create_quaternion(quaternion quat);
static __inline__ m_matrix3 matrix3_create_scale(float sx, float sy, float sz);
static __inline__ m_matrix3 matrix3_create_rotation(float radians, float x, float y, float z);
static __inline__ m_matrix3 matrix3_create_x_rotation(float radians);
static __inline__ m_matrix3 matrix3_create_y_rotation(float radians);
static __inline__ m_matrix3 matrix3_create_z_rotation(float radians);
static __inline__ m_matrix2 matrix3_get_matrix2(m_matrix3 matrix);
static __inline__ m_vector3 matrix3_get_row(m_matrix3 matrix, int row);
static __inline__ m_vector3 matrix3_get_coulm(m_matrix3 matrix, int column);
static __inline__ m_matrix3 matrix3_set_row(m_matrix3 matrix, int row, m_vector3 vector);
static __inline__ m_matrix3 matrix3_set_column(m_matrix3 matrix, int column, m_vector3 vector);
static __inline__ m_matrix3 matrix3_transpose(m_matrix3 matrix);
static __inline__ m_matrix3 matrix3_mul(m_matrix3 matrixLeft, m_matrix3 matrixRight);
static __inline__ m_matrix3 matrix3_add(m_matrix3 matrixLeft, m_matrix3 matrixRight);
static __inline__ m_matrix3 matrix3_sub(m_matrix3 matrixLeft, m_matrix3 matrixRight);
static __inline__ m_matrix3 matrix3_scale(m_matrix3 matrix, float sx, float sy, float sz);
static __inline__ m_matrix3 matrix3_scale_vector3(m_matrix3 matrix, m_vector3 scaleVector);
static __inline__ m_matrix3 matrix3_scale_vector4(m_matrix3 matrix, m_vector4 scaleVector);
static __inline__ m_matrix3 matrix3_rotate(m_matrix3 matrix, float radians, float x, float y, float z);
static __inline__ m_matrix3 matrix3_rotate_vector3(m_matrix3 matrix, float radians, m_vector3 axisVector);
static __inline__ m_matrix3 matrix3_rotate_vector4(m_matrix3 matrix, float radians, m_vector4 axisVector);
static __inline__ m_matrix3 matrix3_rotate_x(m_matrix3 matrix, float radians);
static __inline__ m_matrix3 matrix3_rotate_y(m_matrix3 matrix, float radians);
static __inline__ m_matrix3 matrix3_rotate_z(m_matrix3 matrix, float radians);
static __inline__ m_vector3 matrix3_mul_vector3(m_matrix3 matrixLeft, m_vector3 vectorRight);
static __inline__ void matrix3_mul_vector3_array(m_matrix3 matrix, m_vector3 *vectors, size_t vectorCount);
m_matrix3 matrix3_invert(m_matrix3 matrix, int *isInvertible);
m_matrix3 matrix3_invert_transpose(m_matrix3 matrix, int *isInvertible);


static __inline__ m_matrix3 matrix3_create( float m00, float m01, float m02,
                                          float m10, float m11, float m12,
                                          float m20, float m21, float m22)
{
    m_matrix3 m = { m00, m01, m02,
                  m10, m11, m12,
                  m20, m21, m22 };
    return m;
}

static __inline__ m_matrix3 matrix3_create_transpose( float m00, float m01, float m02,
                                                    float m10, float m11, float m12,
                                                    float m20, float m21, float m22)
{
    m_matrix3 m = { m00, m10, m20,
                  m01, m11, m21,
                  m02, m12, m22 };
    return m;
}

static __inline__ m_matrix3 matrix3_create_array(float values[9])
{
    m_matrix3 m = { values[0], values[1], values[2],
                  values[3], values[4], values[5],
                  values[6], values[7], values[8] };
    return m;
}

static __inline__ m_matrix3 matrix3_create_array_transpose(float values[9])
{
    m_matrix3 m = { values[0], values[3], values[6],
                  values[1], values[4], values[7],
                  values[2], values[5], values[8] };
    return m;
}

static __inline__ m_matrix3 matrix3_create_rows(m_vector3 row0,
                                              m_vector3 row1,
                                              m_vector3 row2)
{
    m_matrix3 m = { row0.v[0], row1.v[0], row2.v[0],
                  row0.v[1], row1.v[1], row2.v[1],
                  row0.v[2], row1.v[2], row2.v[2] };
    return m;
}

static __inline__ m_matrix3 matrix3_create_columns( m_vector3 column0,
                                                  m_vector3 column1,
                                                  m_vector3 column2)
{
    m_matrix3 m = { column0.v[0], column0.v[1], column0.v[2],
                  column1.v[0], column1.v[1], column1.v[2],
                  column2.v[0], column2.v[1], column2.v[2] };
    return m;
}

static __inline__ m_matrix3 matrix3_create_quaternion(quaternion quat)
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

    m_matrix3 m = {   1.0f - _2y * y - _2z * z,
                    _2x * y + _2w * z,
                    _2x * z - _2w * y,

                    _2x * y - _2w * z,
                    1.0f - _2x * x - _2z * z,
                    _2y * z + _2w * x,

                    _2x * z + _2w * y,
                    _2y * z - _2w * x,
                    1.0f - _2x * x - _2y * y };

    return m;
}

static __inline__ m_matrix3 matrix3_create_scale(float sx, float sy, float sz)
{
    m_matrix3 m = matrix3_identity;
    m.m[0] = sx;
    m.m[4] = sy;
    m.m[8] = sz;
    return m;
}

static __inline__ m_matrix3 matrix3_create_rotation(float radians, float x, float y, float z)
{
    m_vector3 v = vector3_normalize(vector3_new(x, y, z));
    float cos = cosf(radians);
    float cosp = 1.0f - cos;
    float sin = sinf(radians);

    m_matrix3 m = {    cos + cosp * v.v[0] * v.v[0],
                     cosp * v.v[0] * v.v[1] + v.v[2] * sin,
                     cosp * v.v[0] * v.v[2] - v.v[1] * sin,

                     cosp * v.v[0] * v.v[1] - v.v[2] * sin,
                     cos + cosp * v.v[1] * v.v[1],
                     cosp * v.v[1] * v.v[2] + v.v[0] * sin,

                     cosp * v.v[0] * v.v[2] + v.v[1] * sin,
                     cosp * v.v[1] * v.v[2] - v.v[0] * sin,
                     cos + cosp * v.v[2] * v.v[2] };

    return m;
}

static __inline__ m_matrix3 matrix3_create_x_rotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);

    m_matrix3 m = {    1.0f, 0.0f, 0.0f,
                     0.0f, cos, sin,
                     0.0f, -sin, cos };

    return m;
}

static __inline__ m_matrix3 matrix3_create_y_rotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);

    m_matrix3 m = { cos, 0.0f, -sin,
                  0.0f, 1.0f, 0.0f,
                  sin, 0.0f, cos };

    return m;
}

static __inline__ m_matrix3 matrix3_create_z_rotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);

    m_matrix3 m = { cos, sin, 0.0f,
                  -sin, cos, 0.0f,
                  0.0f, 0.0f, 1.0f };

    return m;
}

static __inline__ m_matrix2 matrix3_get_matrix2(m_matrix3 matrix)
{
    m_matrix2 m = { matrix.m[0], matrix.m[1],
                  matrix.m[3], matrix.m[4] };
    return m;
}

static __inline__ m_vector3 matrix3_get_row(m_matrix3 matrix, int row)
{
    m_vector3 v = { matrix.m[row], matrix.m[3 + row], matrix.m[6 + row] };
    return v;
}

static __inline__ m_vector3 matrix3_get_coulm(m_matrix3 matrix, int column)
{
    m_vector3 v = { matrix.m[column * 3 + 0], matrix.m[column * 3 + 1], matrix.m[column * 3 + 2] };
    return v;
}

static __inline__ m_matrix3 matrix3_set_row(m_matrix3 matrix, int row, m_vector3 vector)
{
    matrix.m[row] = vector.v[0];
    matrix.m[row + 3] = vector.v[1];
    matrix.m[row + 6] = vector.v[2];

    return matrix;
}

static __inline__ m_matrix3 matrix3_set_column(m_matrix3 matrix, int column, m_vector3 vector)
{
    matrix.m[column * 3 + 0] = vector.v[0];
    matrix.m[column * 3 + 1] = vector.v[1];
    matrix.m[column * 3 + 2] = vector.v[2];

    return matrix;
}

static __inline__ m_matrix3 matrix3_transpose(m_matrix3 matrix)
{
    m_matrix3 m = { matrix.m[0], matrix.m[3], matrix.m[6],
                  matrix.m[1], matrix.m[4], matrix.m[7],
                  matrix.m[2], matrix.m[5], matrix.m[8] };
    return m;
}

static __inline__ m_matrix3 matrix3_mul(m_matrix3 matrixLeft, m_matrix3 matrixRight)
{
#if defined(__ARM_NEON__)
    m_matrix3 m;
    float32x4x3_t iMatrixLeft;
    float32x4x3_t iMatrixRight;
    float32x4x3_t mm;

    memcpy(&(iMatrixLeft.val[0]), (char *)&(matrixLeft.m[0]), 16); // 0 1 2 3
    memcpy(&(iMatrixLeft.val[1]), (char *)&(matrixLeft.m[3]), 16); // 3 4 5 6
    memcpy(&(iMatrixLeft.val[2]), (char *)&(matrixLeft.m[5]), 16); // 5 6 7 8

    memcpy(&(iMatrixRight.val[0]), (char *)&(matrixRight.m[0]), 16);
    memcpy(&(iMatrixRight.val[1]), (char *)&(matrixRight.m[3]), 16);
    memcpy(&(iMatrixRight.val[2]), (char *)&(matrixRight.m[5]), 16);

    iMatrixLeft.val[2] = vextq_f32(iMatrixLeft.val[2], iMatrixLeft.val[2], 1); // 6 7 8 x

    mm.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
    mm.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 3));
    mm.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 3));

    mm.val[0] = vmlaq_n_f32(mm.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
    mm.val[1] = vmlaq_n_f32(mm.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
    mm.val[2] = vmlaq_n_f32(mm.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 2));

    mm.val[0] = vmlaq_n_f32(mm.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
    mm.val[1] = vmlaq_n_f32(mm.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
    mm.val[2] = vmlaq_n_f32(mm.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 3));

    memcpy(&m.m[0], (char *)&(mm.val[0]), 16);
    memcpy(&m.m[3], (char *)&(mm.val[1]), 16);
    float32x2_t vlow = vget_low_f32(mm.val[2]);
    memcpy(&m.m[6], (char *)&vlow, 8);
    m.m[8] = vgetq_lane_f32(mm.val[2], 2);

    return m;
#else
    m_matrix3 m;

    m.m[0] = matrixLeft.m[0] * matrixRight.m[0] + matrixLeft.m[3] * matrixRight.m[1] + matrixLeft.m[6] * matrixRight.m[2];
    m.m[3] = matrixLeft.m[0] * matrixRight.m[3] + matrixLeft.m[3] * matrixRight.m[4] + matrixLeft.m[6] * matrixRight.m[5];
    m.m[6] = matrixLeft.m[0] * matrixRight.m[6] + matrixLeft.m[3] * matrixRight.m[7] + matrixLeft.m[6] * matrixRight.m[8];

    m.m[1] = matrixLeft.m[1] * matrixRight.m[0] + matrixLeft.m[4] * matrixRight.m[1] + matrixLeft.m[7] * matrixRight.m[2];
    m.m[4] = matrixLeft.m[1] * matrixRight.m[3] + matrixLeft.m[4] * matrixRight.m[4] + matrixLeft.m[7] * matrixRight.m[5];
    m.m[7] = matrixLeft.m[1] * matrixRight.m[6] + matrixLeft.m[4] * matrixRight.m[7] + matrixLeft.m[7] * matrixRight.m[8];

    m.m[2] = matrixLeft.m[2] * matrixRight.m[0] + matrixLeft.m[5] * matrixRight.m[1] + matrixLeft.m[8] * matrixRight.m[2];
    m.m[5] = matrixLeft.m[2] * matrixRight.m[3] + matrixLeft.m[5] * matrixRight.m[4] + matrixLeft.m[8] * matrixRight.m[5];
    m.m[8] = matrixLeft.m[2] * matrixRight.m[6] + matrixLeft.m[5] * matrixRight.m[7] + matrixLeft.m[8] * matrixRight.m[8];

    return m;
#endif
}

static __inline__ m_matrix3 matrix3_add(m_matrix3 matrixLeft, m_matrix3 matrixRight)
{
    m_matrix3 m;

    m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
    m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
    m.m[2] = matrixLeft.m[2] + matrixRight.m[2];

    m.m[3] = matrixLeft.m[3] + matrixRight.m[3];
    m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
    m.m[5] = matrixLeft.m[5] + matrixRight.m[5];

    m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
    m.m[7] = matrixLeft.m[7] + matrixRight.m[7];
    m.m[8] = matrixLeft.m[8] + matrixRight.m[8];

    return m;
}

static __inline__ m_matrix3 matrix3_sub(m_matrix3 matrixLeft, m_matrix3 matrixRight)
{
    m_matrix3 m;

    m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
    m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
    m.m[2] = matrixLeft.m[2] - matrixRight.m[2];

    m.m[3] = matrixLeft.m[3] - matrixRight.m[3];
    m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
    m.m[5] = matrixLeft.m[5] - matrixRight.m[5];

    m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
    m.m[7] = matrixLeft.m[7] - matrixRight.m[7];
    m.m[8] = matrixLeft.m[8] - matrixRight.m[8];

    return m;
}

static __inline__ m_matrix3 matrix3_scale(m_matrix3 matrix, float sx, float sy, float sz)
{
    m_matrix3 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx,
                  matrix.m[3] * sy, matrix.m[4] * sy, matrix.m[5] * sy,
                  matrix.m[6] * sz, matrix.m[7] * sz, matrix.m[8] * sz };
    return m;
}

static __inline__ m_matrix3 matrix3_scale_vector3(m_matrix3 matrix, m_vector3 scaleVector)
{
    m_matrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
                  matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
                  matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
    return m;
}

static __inline__ m_matrix3 matrix3_scale_vector4(m_matrix3 matrix, m_vector4 scaleVector)
{
    m_matrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
                  matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
                  matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
    return m;
}

static __inline__ m_matrix3 matrix3_rotate(m_matrix3 matrix, float radians, float x, float y, float z)
{
    m_matrix3 rm = matrix3_create_rotation(radians, x, y, z);
    return matrix3_mul(matrix, rm);
}

static __inline__ m_matrix3 matrix3_rotate_vector3(m_matrix3 matrix, float radians, m_vector3 axisVector)
{
    m_matrix3 rm = matrix3_create_rotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
    return matrix3_mul(matrix, rm);
}

static __inline__ m_matrix3 matrix3_rotate_vector4(m_matrix3 matrix, float radians, m_vector4 axisVector)
{
    m_matrix3 rm = matrix3_create_rotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
    return matrix3_mul(matrix, rm);
}

static __inline__ m_matrix3 matrix3_rotate_x(m_matrix3 matrix, float radians)
{
    m_matrix3 rm = matrix3_create_x_rotation(radians);
    return matrix3_mul(matrix, rm);
}

static __inline__ m_matrix3 matrix3_rotate_y(m_matrix3 matrix, float radians)
{
    m_matrix3 rm = matrix3_create_y_rotation(radians);
    return matrix3_mul(matrix, rm);
}

static __inline__ m_matrix3 matrix3_rotate_z(m_matrix3 matrix, float radians)
{
    m_matrix3 rm = matrix3_create_z_rotation(radians);
    return matrix3_mul(matrix, rm);
}

static __inline__ m_vector3 matrix3_mul_vector3(m_matrix3 matrixLeft, m_vector3 vectorRight)
{
    m_vector3 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[3] * vectorRight.v[1] + matrixLeft.m[6] * vectorRight.v[2],
                  matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[7] * vectorRight.v[2],
                  matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] };
    return v;
}

static __inline__ void matrix3_mul_vector3_array(m_matrix3 matrix, m_vector3 *vectors, size_t vectorCount)
{
    int i;
    for (i=0; i < vectorCount; i++)
        vectors[i] = matrix3_mul_vector3(matrix, vectors[i]);
}


#ifdef __cplusplus
}
#endif

#endif
