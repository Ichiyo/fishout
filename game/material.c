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
  shader default 3d
*/
shader_3d_default_context_t* shader_3d_default_context_new()
{
  shader_3d_default_context_t* ctx = malloc(sizeof(shader_3d_default_context_t));
  ctx->light_ctx = 0;
  ctx->shader = 0;
  ctx->material = 0;
  return ctx;
}

void shader_3d_default_context_use(shader_3d_default_context_t* ctx)
{
  int flag = shader_use(ctx->shader);
  /* setup light uniforms */
  flag != ctx->light_ctx->update;
  if(flag)
  {
    if(ctx->light_ctx->direction_lights->len)
    {
      int total_size = ctx->light_ctx->direction_lights->len * sizeof(direction_light_t);
      GLuint address = glGetUniformLocation(ctx->shader->id, "dirLights");
      glUniform1fv(address, total_size, ctx->light_ctx->direction_lights->data);
    }
    if(ctx->light_ctx->point_lights->len)
    {
      int total_size = ctx->light_ctx->point_lights->len * sizeof(point_light_t);
      GLuint address = glGetUniformLocation(ctx->shader->id, "pointLights");
      glUniform1fv(address, total_size, ctx->light_ctx->point_lights->data);
    }
    if(ctx->light_ctx->spot_lights->len)
    {
      int total_size = ctx->light_ctx->spot_lights->len * sizeof(spot_light_t);
      GLuint address = glGetUniformLocation(ctx->shader->id, "spotLights");
      glUniform1fv(address, total_size, ctx->light_ctx->point_lights->data);
    }
    ctx->light_ctx->update = 0;

    glUniform1i(glGetUniformLocation(ctx->shader->id, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(ctx->shader->id, "material.specular"), 1);
  }
  /* setup texture uniform */
  if(ctx->material->diffuse) texture_bind(ctx->material->diffuse, 0);
  if(ctx->material->specular) texture_bind(ctx->material->specular, 1);
}

void shader_3d_default_context_free(shader_3d_default_context_t* ctx)
{
  if(ctx->material) material_free(ctx->material);
  free(ctx);
}

shader_t* shader_3d_default_new(shader_3d_default_param_t param)
{
  m_string* vert = io_file_to_string("res/shaders/shader_3d.vert");
	m_string* frag = io_file_to_string("res/shaders/shader_3d.frag");

  char s1[] = "HAS_TEXTURE _";
  memcpy(s1 + sizeof(s1) - 2, (param.has_texture ? "1" : "0"), 1);
	m_string_replace_char_array(vert, "HAS_TEXTURE _", s1);
  m_string_replace_char_array(frag, "HAS_TEXTURE _", s1);

  char s2[] = "HAS_SHADOW _";
  memcpy(s2 + sizeof(s2) - 2, (param.has_shadow ? "1" : "0"), 1);
	m_string_replace_char_array(vert, "HAS_SHADOW _", s2);
  m_string_replace_char_array(frag, "HAS_SHADOW _", s2);

  char s3[] = "USE_MODEL_INDEX _";
  memcpy(s3 + sizeof(s3) - 2, (param.use_model_index ? "1" : "0"), 1);
  m_string_replace_char_array(vert, "USE_MODEL_INDEX _", s3);

  char s4[] = "NUMBER_JOIN_IDS __";
  snprintf(s4 + sizeof(s4) - 3, sizeof(s4), "%02d", param.number_join_ids);
	m_string_replace_char_array(vert, "NUMBER_JOIN_IDS __", s4);

  char s5[] = "NUMBER_JOINS __";
  snprintf(s5 + sizeof(s5) - 3, sizeof(s5), "%02d", param.number_joins);
	m_string_replace_char_array(vert, "NUMBER_JOINS __", s5);

  char s6[] = "NR_DIRECTION_LIGHTS __";
  snprintf(s6 + sizeof(s6) - 3, sizeof(s6), "%02d", param.number_direction_lights);
	m_string_replace_char_array(frag, "NR_DIRECTION_LIGHTS __", s6);

  char s7[] = "NR_POINT_LIGHTS __";
  snprintf(s7 + sizeof(s7) - 3, sizeof(s7), "%02d", param.number_point_lights);
	m_string_replace_char_array(frag, "NR_POINT_LIGHTS __", s7);

  char s8[] = "NR_SPOT_LIGHTS __";
  snprintf(s8 + sizeof(s8) - 3, sizeof(s8), "%02d", param.number_spot_lights);
	m_string_replace_char_array(frag, "NR_SPOT_LIGHTS __", s8);

	shader_t* s = shader_new(vert->str, frag->str);

	m_string_free(vert);
	m_string_free(frag);

  return s;
}

/*
  scene material
*/
scene_material_context_t* scene_material_context_new()
{
  scene_material_context_t* ctx = malloc(sizeof(scene_material_context_t));
  ctx->lights = m_array_new(sizeof(light_context_t*));
  ctx->shaders = m_array_new(sizeof(shader_t*));
  return ctx;
}

void scene_material_context_free(scene_material_context_t* ctx)
{
  int i;
  for(i = 0; i < ctx->shaders->len; i++)
  {
    shader_free(m_array_get(ctx->shaders, shader_t*, i));
  }
  m_array_free(ctx->shaders);
  for(i = 0; i < ctx->lights->len; i++)
  {
    light_context_free(m_array_get(ctx->lights, light_context_t*, i));
  }
  m_array_free(ctx->lights);
  free(ctx);
}
