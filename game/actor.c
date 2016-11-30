#include "actor.h"
#include <stdlib.h>

static const vector3_t vector3_one = {1, 1, 1};

actor_t* actor_new()
{
  actor_t* actor = malloc(sizeof(actor_t));
  memset(&actor->position, 0, sizeof(actor->position));
  actor->update = 0;
  actor->scale = vector3_one;
  actor->quat = quaternion_identity;
  actor->model = matrix4_identity;
  actor->render_model = matrix4_identity;
  actor->fix_model = matrix4_identity;
  actor->children = m_array_new(sizeof(actor_t*));
  actor->parent = 0;
  return actor;
}

void actor_update(actor_t* actor, matrix4_t trans, int flag)
{
  int i;
  flag |= actor->update;
  if(flag)
  {
    //update actor render information
    m_matrix4 model = matrix4_identity;
		model = matrix4_translate_vector3(model, actor->position);
    model = matrix4_mul(model, actor->fix_model);
		model = matrix4_mul(model, matrix4_create_quaternion(actor->quat));
		model = matrix4_scale_vector3(model, actor->scale);
    actor->render_model = model;
    actor->render_model = matrix4_mul(trans, actor->render_model);
		model = matrix4_mul(trans, model);
    actor->model = model;

    actor->update = 0;
  }
  /* update children */
  for(i = 0; i < actor->children->len; i++)
  {
    actor_update(m_array_get(actor->children, actor_t*, i), actor->model, flag);
  }
}

void actor_free(actor_t* actor)
{
  m_array_free(actor->children);
  free(actor);
}

/*
  actor context
*/
actor_context_t* actor_context_new()
{
  actor_context_t* ctx = malloc(sizeof(actor_context_t));
  ctx->actors = m_array_new(sizeof(actor_t*));
  return ctx;
}

void actor_context_free(actor_context_t* ctx)
{
  int i;
  for(i = 0; i < ctx->actors->len; i++)
  {
    actor_free(m_array_get(ctx->actors, actor_t*, i));
  }
  m_array_free(ctx->actors);
  free(ctx);
}
