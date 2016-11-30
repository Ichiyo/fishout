#ifndef __M_GAME_H__
#define __M_GAME_H__

#include "scene.h"

typedef struct game_t
{
  scene_t* scene;
} game_t;

game_t* game_new();
void game_loop(game_t* game);
void game_free(game_t* game);

#endif
