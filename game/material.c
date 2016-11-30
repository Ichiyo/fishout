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
  shader default 3d
*/
shader_3d_default_context_t* shader_3d_default_context_new()
{
  shader_3d_default_context_t* ctx = malloc(sizeof(shader_3d_default_context_t));
  ctx->light_ctx = 0;
  ctx->shader = 0;
  ctx->material = material_new();
  return ctx;
}

void shader_3d_default_context_config_shader_mesh(shader_3d_default_context_t* ctx, model_mesh_t* mesh)
{
  shader_t* shader = ctx->shader;
  int one_vertex_size = mesh->has_uvs ? (3 + 3 + 2 + 2 * mesh->joins_per_vertex) : (3 + 3 + 2 * mesh->joins_per_vertex);
  shader_use(shader);

  glBindVertexArray(mesh->gl_data->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->gl_data->vbo);
  GLint posAttrib = glGetAttribLocation(shader->id, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, one_vertex_size * sizeof(GLfloat), 0);

  GLint norAttrib = glGetAttribLocation(shader->id, "normal");
  glEnableVertexAttribArray(norAttrib);
  glVertexAttribPointer(norAttrib, 3, GL_FLOAT, GL_FALSE, one_vertex_size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  if(mesh->has_uvs)
  {
    GLint texAttrib = glGetAttribLocation(shader->id, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, one_vertex_size * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  }

  int offset = mesh->has_uvs ? 8 : 6;
  for(int i = 0; i < mesh->joins_per_vertex; i++)
  {
    int_to_chars(id, i);
    string_t* join_attr_name = m_string_new();
    m_string_reserve(join_attr_name, 9);
    m_string_cat_char_array(join_attr_name, "joinIds_");
    m_string_cat_char_array(join_attr_name, id);
    string_t* weight_attr_name = m_string_new();
    m_string_reserve(weight_attr_name, 9);
    m_string_cat_char_array(weight_attr_name, "weights_");
    m_string_cat_char_array(weight_attr_name, id);

    GLint joinIdAttrib = glGetAttribLocation(shader->id, join_attr_name->str);
    glEnableVertexAttribArray(joinIdAttrib);
    glVertexAttribPointer(joinIdAttrib, 1, GL_FLOAT, GL_FALSE, one_vertex_size * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
    offset++;
    GLint weightAttrib = glGetAttribLocation(shader->id, weight_attr_name->str);
    glEnableVertexAttribArray(weightAttrib);
    glVertexAttribPointer(weightAttrib, 1, GL_FLOAT, GL_FALSE, one_vertex_size * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
    offset++;

    m_string_free(join_attr_name);
    m_string_free(weight_attr_name);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

int shader_3d_default_context_use(shader_3d_default_context_t* ctx)
{
  int flag = shader_use(ctx->shader);
  int reuse_shader = flag;
  /* setup light uniforms */
  flag |= ctx->light_ctx->update;
  if(flag)
  {
    if(ctx->light_ctx->direction_lights->len)
    {
      direction_light_t d = m_array_get(ctx->light_ctx->direction_lights, direction_light_t, 0);
      GLuint address = glGetUniformLocation(ctx->shader->id, "dirLights[0].direction");
      glUniform3fv(address, 1, d.direction.v);
      address = glGetUniformLocation(ctx->shader->id, "dirLights[0].ambient");
      glUniform3fv(address, 1, d.ambient.v);
      address = glGetUniformLocation(ctx->shader->id, "dirLights[0].diffuse");
      glUniform3fv(address, 1, d.diffuse.v);
      address = glGetUniformLocation(ctx->shader->id, "dirLights[0].specular");
      glUniform3fv(address, 1, d.specular.v);
    }
    if(ctx->light_ctx->point_lights->len)
    {
      point_light_t p = m_array_get(ctx->light_ctx->point_lights, point_light_t, 0);
      GLuint address = glGetUniformLocation(ctx->shader->id, "pointLights[0].position");
      glUniform3fv(address, 1, p.position.v);
      address = glGetUniformLocation(ctx->shader->id, "pointLights[0].constant");
      glUniform1fv(address, 1, &p.constant);
      address = glGetUniformLocation(ctx->shader->id, "pointLights[0].linear");
      glUniform1fv(address, 1, &p.linear);
      address = glGetUniformLocation(ctx->shader->id, "pointLights[0].quadratic");
      glUniform1fv(address, 1, &p.quadratic);
      address = glGetUniformLocation(ctx->shader->id, "pointLights[0].ambient");
      glUniform3fv(address, 1, p.ambient.v);
      address = glGetUniformLocation(ctx->shader->id, "pointLights[0].diffuse");
      glUniform3fv(address, 1, p.diffuse.v);
      address = glGetUniformLocation(ctx->shader->id, "pointLights[0].specular");
      glUniform3fv(address, 1, p.specular.v);
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
    glUniform1fv(glGetUniformLocation(ctx->shader->id, "material.shininess"), 1, &ctx->material->shininess);
  }
  /* setup texture uniform */
  if(ctx->material->diffuse) texture_bind(ctx->material->diffuse, 0);
  if(ctx->material->specular) texture_bind(ctx->material->specular, 1);

  return reuse_shader;
}

void shader_3d_default_context_free(shader_3d_default_context_t* ctx)
{
  material_free(ctx->material);
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
