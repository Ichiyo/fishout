#ifndef __M_SCENE_H__
#define __M_SCENE_H__

#include "material.h"
#include "kai.h"
#include "actor.h"

typedef struct scene_t
{
  kai_t* kai;
  /* array of actor contexts */
  array_t* actor_ctxs;
  /* scene material context */
  scene_material_context_t* material_ctx;
} scene_t;

scene_t* scene_new();
void scene_render(scene_t* scene);
void scene_free(scene_t* scene);

#endif
