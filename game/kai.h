#ifndef __M_KAI_H__
#define __M_KAI_H__

#include "model.h"
#include "shader.h"
#include "texture.h"
#include "g_math.h"
#include "actor.h"
#include "material.h"

typedef struct kai_t
{
  actor_t* actor;
  shader_3d_default_context_t* default_shader_ctx;
} kai_t;

kai_t* kai_new();
void kai_set_shader(kai_t* kai, shader_t* shader);
void kai_render(kai_t* kai);
void kai_free(kai_t* k);

#endif
