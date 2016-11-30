#ifndef __M_SCENE_H__
#define __M_SCENE_H__

#include "material.h"
#include "kai.h"
#include "actor.h"
#include "m_map.h"
#include "g_math.h"

typedef struct scene_t
{
  kai_t* kai;

  actor_t* root;

  /* camera */
  matrix4_t project;
  matrix4_t view;
  vector3_t view_position;
  int first_view;

  /* array of actor contexts */
  array_t* actor_ctxs;
  map_t* id_to_actor_ctx;
  /* scene material context */
  scene_material_context_t* material_ctx;
} scene_t;

scene_t* scene_new();
void scene_update(scene_t* scene, float delta);
void scene_render(scene_t* scene);
void scene_free(scene_t* scene);

#endif
