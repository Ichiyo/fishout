#ifndef __M_KAI_H__
#define __M_KAI_H__

#include "model.h"
#include "shader.h"
#include "texture.h"
#include "g_math.h"
#include "actor.h"

typedef struct kai_t
{
  actor_t* actor;
} kai_t;

kai_t* kai_new();
void kai_free(kai_t* k);

#endif
