#include "scene.h"
#include <stdlib.h>
#include "shader_id.h"
#include "g_math.h"
#include "keycode.h"

static int root_actor_context = 0;
static int light_source_main = 0;

static void setup_kai(scene_t* scene)
{
  scene->kai = kai_new();
  collada_context* ctx = collada_context_new("res/model/kai.dae");
  m_array* models = collada_context_parse(ctx);

  scene->kai->actor = actor_new();
  scene->kai->actor->mesh = m_array_get(models, model_mesh*, 0);
  actor_set_scale(scene->kai->actor, vector3_new(30, 30, 30));
  actor_add_child(scene->root, scene->kai->actor);
  m_array_free(models);
  collada_context_free(ctx);
}

static void setup_kai_shader(scene_t* scene)
{
  light_context_t* light_ctx = m_map_get(scene->material_ctx->id_to_light_ctx, light_context_t*, qpkey(light_source_main));
  shader_3d_default_param_t param =
  {
  	.has_texture = scene->kai->actor->mesh->has_uvs,
  	.has_shadow = 0,
  	.use_model_index = 0,
  	.number_join_ids = scene->kai->actor->mesh->joins_per_vertex,
  	.number_joins = scene->kai->actor->mesh->join_count,
  	.number_direction_lights = light_ctx->direction_lights->len,
  	.number_point_lights = light_ctx->point_lights->len,
  	.number_spot_lights = light_ctx->spot_lights->len
  };
  shader_t* s = shader_3d_default_new(param);
  scene_material_context_add_shader(scene->material_ctx, kai_default_shader, s);
  scene->kai->default_shader_ctx->light_ctx = light_ctx;
  kai_set_shader(scene->kai, s);
}

static void setup_light(scene_t* scene)
{
  light_context_t* main_light = light_context_new();
  {
    direction_light_t dlight = {
      .direction = vector3_new(0, 0, -1),
      .ambient = vector3_new(0.3, 0.3, 0.3),
      .diffuse = vector3_new(0.45, 0.15, 0.15),
      .specular = vector3_new(0, 0, 0)
    };
    m_array_push(main_light->direction_lights, &dlight);
  }
  {
    direction_light_t dlight = {
      .direction = vector3_new(0, 1, 0),
      .ambient = vector3_new(0.001, 0.001, 0.001),
      .diffuse = vector3_new(0.45 * 0.5, 0.15 * 0.5, 0.15 * 0.5),
      .specular = vector3_new(0, 0, 0)
    };
    m_array_push(main_light->direction_lights, &dlight);
  }
  point_light_t plight =
  {
    .position = vector3_new(-0, 0, 200),
    .constant = 1.0,
    .linear = 0.007,
    .quadratic = 0.0002,
    .ambient = vector3_new(0.8, 0.2, 0.2),
    .diffuse = vector3_new(0.6, 0.6, 0.6),
    .specular = vector3_new(1, 1, 1)
  };
  // m_array_push(main_light->point_lights, &plight);
  scene_material_context_add_light_context(scene->material_ctx, light_source_main, main_light);
}

static void setup_camera(scene_t* scene)
{
  scene->project = matrix4_create_perspective(DEG_TO_RAD(45.0f), 800.0f / 600.0f, 1.0f, 10000.0f);
  scene->camera = camera_create(matrix4_create_look_at(
    0, -0.0f, 800.0f,
    0, 0, 0,
    0.0f, 1.0f, 0.0f
  ));
  camera_rotate_around(scene->camera, quaternion_new_angle_axis(DEG_TO_RAD(-45), 1, 0, 0));
  camera_rotate_around(scene->camera, quaternion_new_angle_axis(DEG_TO_RAD(-15), 0, 0, 1));
  camera_translate_by_world(scene->camera, vector3_new(0, 275, 0));
}

scene_t* scene_new()
{
  scene_t* scene = malloc(sizeof(scene_t));
  scene->material_ctx = 0;
  scene->actor_ctxs = m_array_new(sizeof(actor_context_t*));
  scene->id_to_actor_ctx = m_map_new(sizeof(actor_context_t*));
  scene->material_ctx = scene_material_context_new();
  scene->root = actor_new();

  setup_camera(scene);
  /* setup light */
  setup_light(scene);

  /* setup root context

     root context manages all character actors
   */
  actor_context_t* root_ctx = actor_context_new();
  m_array_push(scene->actor_ctxs, &root_ctx);
  m_map_set(scene->id_to_actor_ctx, qpkey(root_actor_context), &root_ctx);
  m_array_push(root_ctx->actors, &scene->root);

  /* setup character */
  /* setup kai */
  setup_kai(scene);

  /* setup shaders */
  /* create kai shader */
  setup_kai_shader(scene);
  return scene;
}

void scene_input_keyboard(scene_t* scene, keyboard_event_t e)
{
  if(e.state == keyboard_event_down)
  {
    if(e.key == game_key_a)
    {
      actor_set_position(scene->kai->actor, vector3_sub(scene->kai->actor->position, vector3_new(30,0,0)));
    }
    else if(e.key == game_key_d)
    {
      actor_set_position(scene->kai->actor, vector3_add(scene->kai->actor->position, vector3_new(30,0,0)));
    }
    else if(e.key == game_key_w)
    {
      actor_set_position(scene->kai->actor, vector3_add(scene->kai->actor->position, vector3_new(0,30,0)));
    }
    else if(e.key == game_key_s)
    {
      actor_set_position(scene->kai->actor, vector3_sub(scene->kai->actor->position, vector3_new(0,30,0)));
    }
  }
}

void scene_input_touch(scene_t* scene, touch_event_t e)
{

}

void scene_update(scene_t* scene, float delta)
{
  actor_update(scene->root, matrix4_identity, 0);
}

static void update_shader_projection_view(scene_t* scene, shader_t* shader)
{
  glUniformMatrix4fv(glGetUniformLocation(shader->id, "proj"), 1, GL_FALSE, scene->project.m);
  glUniformMatrix4fv(glGetUniformLocation(shader->id, "view"), 1, GL_FALSE, scene->camera->view.m);
  glUniform3fv(glGetUniformLocation(shader->id, "view_position"), 1, camera_get_position(scene->camera).v);
}

void scene_render(scene_t* scene)
{
  /* clear screen */
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  int flag;
  //render kai
  /* update lights, texture */
  flag = shader_3d_default_context_use(scene->kai->default_shader_ctx);
  if(flag || scene->camera->update)
  {
    /* update projection view camera */
    update_shader_projection_view(scene, scene->kai->default_shader_ctx->shader);
    scene->camera->update = 0;
  }
  /* render mesh or skeleton mesh */
  kai_render(scene->kai);
}

void scene_free(scene_t* scene)
{
  int i;
  /* fre camera */
  camera_free(scene->camera);
  /* free material context */
  if(scene->material_ctx) scene_material_context_free(scene->material_ctx);
  /* free actors */
  for(i = 0; i < scene->actor_ctxs->len; i++)
  {
    actor_context_free(m_array_get(scene->actor_ctxs, actor_context_t*, i));
  }
  m_array_free(scene->actor_ctxs);
  m_map_free(scene->id_to_actor_ctx);
  /* free character */
  kai_free(scene->kai);
  /* free scene */
  free(scene);
}
