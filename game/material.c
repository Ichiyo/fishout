#include "material.h"
#include "m_string.h"
#include <string.h>
#include <stdlib.h>
#include "io.h"
#include "conv.h"
/*
  light
*/
light_context_t* light_context_new()
{
  light_context_t* ctx = malloc(sizeof(light_context_t));
  ctx->direction_lights = m_array_new(sizeof(direction_light_t));
  ctx->point_lights = m_array_new(sizeof(point_light_t));
  ctx->spot_lights = m_array_new(sizeof(spot_light_t));
  ctx->update = 1;
  return ctx;
}

void light_context_free(light_context_t* ctx)
{
  m_array_free(ctx->direction_lights);
  m_array_free(ctx->point_lights);
  m_array_free(ctx->spot_lights);
  free(ctx);
}

/*
  material
*/
material_t* material_new()
{
  material_t* mat = malloc(sizeof(material_t));
  mat->diffuse = 0;
  mat->specular = 0;
  mat->shininess = 0;
  return mat;
}

void material_set_diffuse(material_t* material, texture_t* diffuse)
{
  if(material->diffuse) texture_free(material->diffuse);
  material->diffuse = diffuse;
  if(material->diffuse) diffuse->ref++;
}

void material_set_specular(material_t* material, texture_t* specular)
{
  if(material->specular) texture_free(material->specular);
  material->specular = specular;
  if(material->specular) specular->ref++;
}

void material_set_shiniess(material_t* material, float shininess)
{
  material->shininess = shininess;
}

void material_free(material_t* material)
{
  if(material->diffuse) texture_free(material->diffuse);
  if(material->specular) texture_free(material->specular);
  free(material);
}

/*
  scene material
*/
scene_material_context_t* scene_material_context_new()
{
  scene_material_context_t* ctx = malloc(sizeof(scene_material_context_t));
  ctx->lights = m_array_new(sizeof(light_context_t*));
  ctx->shaders = m_array_new(sizeof(shader_t*));
  ctx->id_to_shader = m_map_new(sizeof(shader_t*));
  ctx->id_to_light_ctx = m_map_new(sizeof(light_context_t*));
  return ctx;
}

void scene_material_context_add_light_context(scene_material_context_t* ctx, int id, light_context_t* light_ctx)
{
  m_array_push(ctx->lights, &light_ctx);
  m_map_set(ctx->id_to_light_ctx, qpkey(id), &light_ctx);
}

void scene_material_context_add_shader(scene_material_context_t* ctx, int id, shader_t* shader)
{
  m_array_push(ctx->shaders, &shader);
  m_map_set(ctx->id_to_shader, qpkey(id), &shader);
}

void scene_material_context_free(scene_material_context_t* ctx)
{
  int i;
  for(i = 0; i < ctx->shaders->len; i++)
  {
    shader_free(m_array_get(ctx->shaders, shader_t*, i));
  }
  m_array_free(ctx->shaders);
  m_map_free(ctx->id_to_shader);
  for(i = 0; i < ctx->lights->len; i++)
  {
    light_context_free(m_array_get(ctx->lights, light_context_t*, i));
  }
  m_array_free(ctx->lights);
  m_map_free(ctx->id_to_light_ctx);
  free(ctx);
}
