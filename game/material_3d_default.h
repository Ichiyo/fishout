#ifndef __M_MATERIAL_3D_DEFAULT_H__
#define __M_MATERIAL_3D_DEFAULT_H__

#include "material.h"

typedef struct shader_3d_default_context_t
{
  light_context_t* light_ctx; //weak light refernece
  material_t* material; // unique ref to material
  shader_t* shader; // weak shader reference
} shader_3d_default_context_t;

shader_3d_default_context_t* shader_3d_default_context_new();
void shader_3d_default_context_config_shader_mesh(shader_3d_default_context_t* ctx, model_mesh_t* mesh);
int shader_3d_default_context_use(shader_3d_default_context_t* ctx);
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

#endif
