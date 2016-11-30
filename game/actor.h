#ifndef __M_ACTOR_H__
#define __M_ACTOR_H__

#include "graphic.h"
#include "model.h"
#include "g_math.h"
#include "m_array.h"

typedef struct actor_t
{
  /* render information */
  model_mesh* mesh;
  vector3_t position;
  vector3_t scale;
  quaternion_t quat;
  /* parent child cordinate */
  matrix4_t model;
  /* upload render model to gpu */
  matrix4_t render_model;
  /* importing collada file may have wrong cordinate */
  matrix4_t fix_model;
  /* update render information */
  int update;

  /* children */
  array_t* children;
  struct actor_t* parent;
} actor_t;

actor_t* actor_new();
void actor_set_position(actor_t* actor, vector3_t vector);
void actor_set_scale(actor_t* actor, vector3_t vector);
void actor_set_quat(actor_t* actor, quaternion_t quat);
void actor_add_child(actor_t* actor, actor_t* child);
void actor_update(actor_t* actor, matrix4_t trans, int flag);
void actor_free(actor_t* actor);

typedef struct actor_context_t
{
  array_t* actors;
} actor_context_t;

actor_context_t* actor_context_new();
void actor_context_free(actor_context_t* ctx);

#endif
