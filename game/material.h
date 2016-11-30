#ifndef __M_MATERIAL_H__
#define __M_MATERIAL_H__

#include "texture.h"
#include "shader.h"
#include "g_math.h"
#include "m_array.h"

typedef struct direction_light_t
{
  vector3_t direction;
  vector3_t ambient;
  vector3_t diffuse;
  vector3_t specular;
} direction_light_t;

typedef struct point_light_t
{
  vector3_t position;
  float constant;
  float linear;
  float quadratic;
  vector3_t ambient;
  vector3_t diffuse;
  vector3_t specular;
} point_light_t;

typedef struct spot_light_t
{
  vector3_t position;
  vector3_t direction;
  float cut_off;
  float outer_cut_off;
  float constant;
  float linear;
  float quadratic;
  vector3_t ambient;
  vector3_t diffuse;
  vector3_t specular;
} spot_light_t;

typedef struct light_context_t
{
  m_array* direction_lights;
  m_array* point_lights;
  m_array* spot_lights;
  int update;
} light_context_t;

light_context_t* light_context_new();
void light_context_free(light_context_t* ctx);

typedef struct material_t
{
  texture_t* diffuse;
  texture_t* specular;
  float shininess;
} material_t;

material_t* material_new();
void material_set_diffuse(material_t* material, texture_t* diffuse);
void material_set_specular(material_t* material, texture_t* specular);
void material_set_shiniess(material_t* material, float shininess);
void material_free(material_t* material);

/*
  shader types
*/
typedef struct shader_3d_default_context_t
{
  light_context_t* light_ctx; //weak light refernece
  material_t* material; // unique ref to material
  shader_t* shader; // weak shader reference
} shader_3d_default_context_t;

shader_3d_default_context_t* shader_3d_default_context_new();
void shader_3d_default_context_use(shader_3d_default_context_t* ctx);
void shader_3d_default_context_free(shader_3d_default_context_t* ctx);
typedef struct shader_3d_default_param_t
{
  int has_texture;
  int has_shadow;
  int use_model_index;
  int number_join_ids;
  int number_joins;
  int number_direction_lights;
  int number_point_lights;
  int number_spot_lights;
} shader_3d_default_param_t;
shader_t* shader_3d_default_new(shader_3d_default_param_t param);

/*
  dispose scene material to free all lights, shaders ...
*/
typedef struct scene_material_context_t
{
  m_array* lights; // unique ref to lights
  m_array* shaders; // unique ref to shaders
} scene_material_context_t;

scene_material_context_t* scene_material_context_new();
void scene_material_context_free(scene_material_context_t* ctx);

#endif
