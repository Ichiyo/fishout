#ifndef __M_MATERIAL_H__
#define __M_MATERIAL_H__

#include "texture.h"
#include "shader.h"
#include "g_math.h"
#include "m_array.h"
#include "m_map.h"
#include "model.h"

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
  dispose scene material to free all lights, shaders ...
*/
typedef struct scene_material_context_t
{
  m_array* lights; // unique ref to light contexts
  map_t* id_to_light_ctx;
  m_array* shaders; // unique ref to shaders
  map_t* id_to_shader;
} scene_material_context_t;

scene_material_context_t* scene_material_context_new();
void scene_material_context_add_light_context(scene_material_context_t* ctx, int id, light_context_t* light_ctx);
void scene_material_context_add_shader(scene_material_context_t* ctx, int id, shader_t* shader);
void scene_material_context_free(scene_material_context_t* ctx);

#endif
