#include "g_math.h"

#ifdef __cplusplus
extern "C" {
#endif

const m_matrix4 matrix4_identity = {1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1};

const quaternion quaternion_identity = {0, 0, 0, 1};

const m_matrix3 matrix3_identity = {1, 0, 0,
                                  0, 1, 0,
                                  0, 0, 1};

m_matrix4 matrix4_invert(m_matrix4 matrix, int *isInvertible)
{
  m_matrix4 inv = matrix4_identity;
  float det;
  int i;

  inv.m[0] =  matrix.m[5]  * matrix.m[10] * matrix.m[15] -
              matrix.m[5]  * matrix.m[11] * matrix.m[14] -
              matrix.m[9]  * matrix.m[6]  * matrix.m[15] +
              matrix.m[9]  * matrix.m[7]  * matrix.m[14] +
              matrix.m[13] * matrix.m[6]  * matrix.m[11] -
              matrix.m[13] * matrix.m[7]  * matrix.m[10];

  inv.m[4] = -matrix.m[4]  * matrix.m[10] * matrix.m[15] +
              matrix.m[4]  * matrix.m[11] * matrix.m[14] +
              matrix.m[8]  * matrix.m[6]  * matrix.m[15] -
              matrix.m[8]  * matrix.m[7]  * matrix.m[14] -
              matrix.m[12] * matrix.m[6]  * matrix.m[11] +
              matrix.m[12] * matrix.m[7]  * matrix.m[10];

  inv.m[8] =  matrix.m[4]  * matrix.m[9] * matrix.m[15] -
              matrix.m[4]  * matrix.m[11] * matrix.m[13] -
              matrix.m[8]  * matrix.m[5] * matrix.m[15] +
              matrix.m[8]  * matrix.m[7] * matrix.m[13] +
              matrix.m[12] * matrix.m[5] * matrix.m[11] -
              matrix.m[12] * matrix.m[7] * matrix.m[9];

  inv.m[12] =  -matrix.m[4]  * matrix.m[9] * matrix.m[14] +
                matrix.m[4]  * matrix.m[10] * matrix.m[13] +
                matrix.m[8]  * matrix.m[5] * matrix.m[14] -
                matrix.m[8]  * matrix.m[6] * matrix.m[13] -
                matrix.m[12] * matrix.m[5] * matrix.m[10] +
                matrix.m[12] * matrix.m[6] * matrix.m[9];

  inv.m[1] = -matrix.m[1]  * matrix.m[10] * matrix.m[15] +
              matrix.m[1]  * matrix.m[11] * matrix.m[14] +
              matrix.m[9]  * matrix.m[2] * matrix.m[15] -
              matrix.m[9]  * matrix.m[3] * matrix.m[14] -
              matrix.m[13] * matrix.m[2] * matrix.m[11] +
              matrix.m[13] * matrix.m[3] * matrix.m[10];

  inv.m[5] =  matrix.m[0]  * matrix.m[10] * matrix.m[15] -
              matrix.m[0]  * matrix.m[11] * matrix.m[14] -
              matrix.m[8]  * matrix.m[2] * matrix.m[15] +
              matrix.m[8]  * matrix.m[3] * matrix.m[14] +
              matrix.m[12] * matrix.m[2] * matrix.m[11] -
              matrix.m[12] * matrix.m[3] * matrix.m[10];

  inv.m[9] = -matrix.m[0]  * matrix.m[9] * matrix.m[15] +
              matrix.m[0]  * matrix.m[11] * matrix.m[13] +
              matrix.m[8]  * matrix.m[1] * matrix.m[15] -
              matrix.m[8]  * matrix.m[3] * matrix.m[13] -
              matrix.m[12] * matrix.m[1] * matrix.m[11] +
              matrix.m[12] * matrix.m[3] * matrix.m[9];

  inv.m[13] = matrix.m[0]  * matrix.m[9] * matrix.m[14] -
              matrix.m[0]  * matrix.m[10] * matrix.m[13] -
              matrix.m[8]  * matrix.m[1] * matrix.m[14] +
              matrix.m[8]  * matrix.m[2] * matrix.m[13] +
              matrix.m[12] * matrix.m[1] * matrix.m[10] -
              matrix.m[12] * matrix.m[2] * matrix.m[9];

  inv.m[2] =  matrix.m[1]  * matrix.m[6] * matrix.m[15] -
              matrix.m[1]  * matrix.m[7] * matrix.m[14] -
              matrix.m[5]  * matrix.m[2] * matrix.m[15] +
              matrix.m[5]  * matrix.m[3] * matrix.m[14] +
              matrix.m[13] * matrix.m[2] * matrix.m[7] -
              matrix.m[13] * matrix.m[3] * matrix.m[6];

  inv.m[6] = -matrix.m[0]  * matrix.m[6] * matrix.m[15] +
              matrix.m[0]  * matrix.m[7] * matrix.m[14] +
              matrix.m[4]  * matrix.m[2] * matrix.m[15] -
              matrix.m[4]  * matrix.m[3] * matrix.m[14] -
              matrix.m[12] * matrix.m[2] * matrix.m[7] +
              matrix.m[12] * matrix.m[3] * matrix.m[6];

  inv.m[10] = matrix.m[0]  * matrix.m[5] * matrix.m[15] -
              matrix.m[0]  * matrix.m[7] * matrix.m[13] -
              matrix.m[4]  * matrix.m[1] * matrix.m[15] +
              matrix.m[4]  * matrix.m[3] * matrix.m[13] +
              matrix.m[12] * matrix.m[1] * matrix.m[7] -
              matrix.m[12] * matrix.m[3] * matrix.m[5];

  inv.m[14] =  -matrix.m[0]  * matrix.m[5] * matrix.m[14] +
                matrix.m[0]  * matrix.m[6] * matrix.m[13] +
                matrix.m[4]  * matrix.m[1] * matrix.m[14] -
                matrix.m[4]  * matrix.m[2] * matrix.m[13] -
                matrix.m[12] * matrix.m[1] * matrix.m[6] +
                matrix.m[12] * matrix.m[2] * matrix.m[5];

  inv.m[3] = -matrix.m[1] * matrix.m[6] * matrix.m[11] +
              matrix.m[1] * matrix.m[7] * matrix.m[10] +
              matrix.m[5] * matrix.m[2] * matrix.m[11] -
              matrix.m[5] * matrix.m[3] * matrix.m[10] -
              matrix.m[9] * matrix.m[2] * matrix.m[7] +
              matrix.m[9] * matrix.m[3] * matrix.m[6];

  inv.m[7] =  matrix.m[0] * matrix.m[6] * matrix.m[11] -
              matrix.m[0] * matrix.m[7] * matrix.m[10] -
              matrix.m[4] * matrix.m[2] * matrix.m[11] +
              matrix.m[4] * matrix.m[3] * matrix.m[10] +
              matrix.m[8] * matrix.m[2] * matrix.m[7] -
              matrix.m[8] * matrix.m[3] * matrix.m[6];

  inv.m[11] =  -matrix.m[0] * matrix.m[5] * matrix.m[11] +
                matrix.m[0] * matrix.m[7] * matrix.m[9] +
                matrix.m[4] * matrix.m[1] * matrix.m[11] -
                matrix.m[4] * matrix.m[3] * matrix.m[9] -
                matrix.m[8] * matrix.m[1] * matrix.m[7] +
                matrix.m[8] * matrix.m[3] * matrix.m[5];

  inv.m[15] = matrix.m[0] * matrix.m[5] * matrix.m[10] -
              matrix.m[0] * matrix.m[6] * matrix.m[9] -
              matrix.m[4] * matrix.m[1] * matrix.m[10] +
              matrix.m[4] * matrix.m[2] * matrix.m[9] +
              matrix.m[8] * matrix.m[1] * matrix.m[6] -
              matrix.m[8] * matrix.m[2] * matrix.m[5];

  det = matrix.m[0] * inv.m[0] + matrix.m[1] * inv.m[4] + matrix.m[2] * inv.m[8] + matrix.m[3] * inv.m[12];

  if (det == 0)
  {
    if(isInvertible)
      *isInvertible = 0;

    return matrix;
  }

  if(isInvertible)
    *isInvertible = 1;

  det = 1.0 / det;

  for (i = 0; i < 16; i++)
      inv.m[i] = inv.m[i] * det;

  return inv;
}

m_matrix4 matrix4_invert_transpose(m_matrix4 matrix, int *isInvertible)
{
  int invertable;
  matrix = matrix4_invert(matrix, &invertable);
  if(isInvertible)
  {
    *isInvertible = invertable;
  }
  if(invertable)
  {
    matrix = matrix4_transpose(matrix);
  }
  return matrix;
}

quaternion quaternion_new_matrix3(m_matrix3 matrix)
{
  m_matrix4 temp = matrix4_identity;
  temp.m[0] = matrix.m[0];
  temp.m[1] = matrix.m[1];
  temp.m[2] = matrix.m[2];
  temp.m[4] = matrix.m[3];
  temp.m[5] = matrix.m[4];
  temp.m[6] = matrix.m[5];
  temp.m[8] = matrix.m[6];
  temp.m[9] = matrix.m[7];
  temp.m[10] = matrix.m[8];
  return quaternion_new_matrix4(temp);
}

quaternion quaternion_new_matrix4(m_matrix4 matrix)
{
  float tr =  matrix.m[0] + matrix.m[5] + matrix.m[10];
  quaternion ret;
  if(tr > 0)
  {
    float s = sqrt(tr + 1.0) * 2;
    ret.q[3] = 0.25 * s;
    ret.q[0] = (matrix.m[9] - matrix.m[6]) / s;
    ret.q[1] = (matrix.m[2] - matrix.m[8]) / s;
    ret.q[2] = (matrix.m[4] - matrix.m[1]) / s;
  }
  else if(matrix.m[0] > matrix.m[5] && matrix.m[0] > matrix.m[10])
  {
    float s = sqrt(1.0 + matrix.m[0] - matrix.m[5] - matrix.m[10]) * 2;
    ret.q[0] = 0.25 * s;
    ret.q[1] = (matrix.m[4] + matrix.m[1]) / s;
    ret.q[2] = (matrix.m[2] + matrix.m[8]) / s;
    ret.q[3] = (matrix.m[9] - matrix.m[6]) / s;
  }
  else if(matrix.m[5] > matrix.m[10])
  {
    float s = sqrt(1.0 + matrix.m[5] - matrix.m[0] - matrix.m[10]) * 2;
    ret.q[0] = (matrix.m[4] + matrix.m[1]) / s;
    ret.q[1] = 0.25 * s;
    ret.q[2] = (matrix.m[9] + matrix.m[6]) / s;
    ret.q[3] = (matrix.m[2] - matrix.m[8]) / s;
  }
  else
  {
    float s = sqrt(1.0 + matrix.m[10] - matrix.m[0] - matrix.m[5]) * 2;
    ret.q[0] = (matrix.m[2] + matrix.m[8]) / s;
    ret.q[1] = (matrix.m[9] + matrix.m[6]) / s;
    ret.q[2] = 0.25 * s;
    ret.q[3] = (matrix.m[4] - matrix.m[1]) / s;
  }
  return ret;
}

float quaternion_to_angle(quaternion quat)
{
  quat = quaternion_normalize(quat);
  float cos_angle = quat.q[3];
  float angle = acos(cos_angle) * 2;

  return angle;
}

m_vector3 quaternion_to_axis(quaternion quat)
{
  m_vector3 ret;
  quat = quaternion_normalize(quat);
  float cos_angle = quat.q[3];
  float s = sqrt(1 - cos_angle * cos_angle);
  if(s < 0.001)
  {
    ret.v[0] = quat.q[0];
    ret.v[1] = quat.q[1];
    ret.v[2] = quat.q[2];
  }
  else
  {
    ret.v[0] = quat.q[0] / s;
    ret.v[1] = quat.q[1] / s;
    ret.v[2] = quat.q[2] / s;
  }
  return ret;
}

m_matrix3 matrix3_invert(m_matrix3 matrix, int *isInvertible)
{
  m_matrix4 temp;
  temp.m[0] = matrix.m[0];
  temp.m[1] = matrix.m[1];
  temp.m[2] = matrix.m[2];
  temp.m[3] = 0;
  temp.m[4] = matrix.m[3];
  temp.m[5] = matrix.m[4];
  temp.m[6] = matrix.m[5];
  temp.m[7] = 0;
  temp.m[8] = matrix.m[6];
  temp.m[9] = matrix.m[7];
  temp.m[10] = matrix.m[8];
  temp.m[11] = 0;
  temp.m[12] = 0;
  temp.m[13] = 0;
  temp.m[14] = 0;
  temp.m[15] = 1;
  int invertable = 0;
  temp = matrix4_invert(temp, &invertable);
  if(isInvertible) *isInvertible = invertable;

  if(invertable)
  {
    return matrix4_to_matrix3(temp);
  }
  else return matrix;
}

m_matrix3 matrix3_invert_transpose(m_matrix3 matrix, int *isInvertible)
{
  m_matrix4 temp;
  temp.m[0] = matrix.m[0];
  temp.m[1] = matrix.m[1];
  temp.m[2] = matrix.m[2];
  temp.m[3] = 0;
  temp.m[4] = matrix.m[3];
  temp.m[5] = matrix.m[4];
  temp.m[6] = matrix.m[5];
  temp.m[7] = 0;
  temp.m[8] = matrix.m[6];
  temp.m[9] = matrix.m[7];
  temp.m[10] = matrix.m[8];
  temp.m[11] = 0;
  temp.m[12] = 0;
  temp.m[13] = 0;
  temp.m[14] = 0;
  temp.m[15] = 1;
  int invertable = 0;
  temp = matrix4_invert_transpose(temp, &invertable);
  if(isInvertible) *isInvertible = invertable;

  if(invertable)
  {
    return matrix4_to_matrix3(temp);
  }
  else return matrix;
}

#ifdef __cplusplus
}
#endif
