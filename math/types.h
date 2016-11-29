#ifndef _M_MATH_TYPES_H
#define _M_MATH_TYPES_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__STRICT_ANSI__)
struct _Matrix2
{
  float m[4];
}__attribute__((aligned(16)));
typedef struct _Matrix2 m_matrix2;
#else
union _Matrix2
{
  struct {
    float m00, m01;
    float m10, m11;
  };
  float m2[2][2]__attribute__((aligned(16)));
  float m[4]__attribute__((aligned(16)));
}__attribute__((aligned(16)));
typedef union _Matrix2 m_matrix2;
#endif

#if defined(__STRICT_ANSI__)
struct _Matrix3
{
  float m[9];
};
typedef struct _Matrix3 m_matrix3;
#else
union _Matrix3
{
  struct {
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
  };
  float m[9];
};
typedef union _Matrix3 m_matrix3;
#endif

#if defined(__STRICT_ANSI__)
struct _Matrix4
{
  float m[16] __attribute__((aligned(16)));
} __attribute__((aligned(16)));
typedef struct _Matrix4 m_matrix4;
#else
union _Matrix4
{
  struct {
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
  } __attribute__((aligned(16)));
  float m[16] __attribute__((aligned(16)));
} __attribute__((aligned(16)));
typedef union _Matrix4 m_matrix4;
#endif

#if defined(__STRICT_ANSI__)
struct _Vector2
{
  float m[2] __attribute__((aligned(8)));
} __attribute__((aligned(8)));
typedef struct _Vector2 m_vector2;
#else
union _Vector2
{
  struct { float x, y; } __attribute__((aligned(8)));
  struct { float s, t; } __attribute__((aligned(8)));
  struct { float w, h; } __attribute__((aligned(8)));
  float v[2] __attribute__((aligned(8)));
} __attribute__((aligned(8)));
typedef union _Vector2 m_vector2;
#endif

#if defined(__STRICT_ANSI__)
struct _Vector3
{
  float v[3];
};
typedef struct _Vector3 m_vector3;
#else
union _Vector3
{
  struct {float x, y, z;};
  struct {float r, g, b;};
  struct {float w, h, d;};
  float v[3];
};
typedef union _Vector3 m_vector3;
#endif

#if defined(__STRICT_ANSI__)
struct _Vector4
{
  float v[4] __attribute__((aligned(16)));
} __attribute__((aligned(16)));
typedef struct _Vector4 m_vector4;
#else
union _Vector4
{
  struct {float x, y, z, w;} __attribute__((aligned(16)));
  struct {float r, g, b, a;} __attribute__((aligned(16)));
  float v[4] __attribute__((aligned(16)));
} __attribute__((aligned(16)));
typedef union _Vector4 m_vector4;
#endif

#if defined(__STRICT_ANSI__)
struct _Quaternion
{
  float q[4] __attribute__((aligned(16)));
} __attribute__((aligned(16)));
typedef struct _Quaternion quaternion;
#else
union _Quaternion
{
  struct{m_vector3 v; float s;} __attribute__((aligned(16)));
  struct{float x, y, z, w;} __attribute__((aligned(16)));
  float q[4] __attribute__((aligned(16)));
} __attribute__((aligned(16)));
typedef union _Quaternion quaternion;
#endif

#ifdef __cplusplus
}
#endif

#endif
